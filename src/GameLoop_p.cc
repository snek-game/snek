#include <snek-pch.h>

#include "GameLoop_p.h"

#include <string>
#include <sstream>

#include <snek/snek-config.h>
#include <snek/Point.h>
#include <snek/Snake.h>
#include <snek/Texture.h>

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

#if defined(SN_DEBUG)
/* console io */
#   include <iostream>
using std::clog;
using std::endl;
#endif

#include "Constants_p.h"

typedef snek::Grid<GRID_WIDTH, GRID_HEIGHT> GridType;

struct Game_Data
{
    int Score;
    int Lives;
    snek::Point Food;
    snek::Snake Snek;
    GridType Grid;

    bool IsAtTitleScreen;
};
static Game_Data Data;

/* generate a random coordinate on the grid */
template <size_t W = GridType::RowsCount, size_t H = GridType::ColumnsCount>
static snek::Point RandomCoord();

/* generate new food position and ensure that the tile is previously empty */
static void GenerateNewFoodPosition();

/* find the offset caused by moving in a particular direction */
static snek::Point OffsetOf(snek::GridState state);

/* update a position given an offset direction */
template <size_t W = GridType::RowsCount, size_t H = GridType::ColumnsCount>
static void UpdatePosition(snek::Point& position, snek::Point offset);

/* reset game */
static void Reset(Application_LocalImpl& impl);

/* hard reset game */
static void HardReset(Application_LocalImpl& impl);

/* get executable path */
static boost::filesystem::path GetExecutingPath(Application_LocalImpl& impl);

/* convert wide to narrow string */
static std::string Wide2Narrow(const std::wstring& wstr);

/* get path relative to executable */
static std::string GetExecutingPath(Application_LocalImpl& impl, const boost::filesystem::path& rel_path);

/* update score texture */
static void UpdateScore(Application_LocalImpl& impl);

/* update lives texture */
static void UpdateLives(Application_LocalImpl& impl);

/* FPS formula */
static int FPSFormula();

struct Res_Data
{
    TTF_Font* Font;
    snek::Texture* TitleTexture;
    snek::Texture* ScoreTexture;
    snek::Texture* LivesTexture;
};
static Res_Data Resources;

#define IS_DIRECTION(dir) (int(dir) && (((dir) >= ::snek::GridState::WEST) && ((dir) <= ::snek::GridState::EAST)))
#define ARE_OPPOSITE(dir1, dir2) (!((int(dir1)) + (int(dir2))))

namespace snek
{
    void GameBegin(Application_LocalImpl& impl)
    {
        Resources.Font = TTF_OpenFont(GetExecutingPath(impl, "res/" FONT_FILE ).c_str(), FONT_SIZE);
        Resources.TitleTexture = new Texture();
        Resources.TitleTexture->LoadTextureFromPath(GetExecutingPath(impl, "res/titlescreen.png").c_str(), impl.Renderer);
        Resources.ScoreTexture = new Texture();
        Resources.LivesTexture = new Texture();
        UpdateScore(impl);
        UpdateLives(impl);
        HardReset(impl);
    }

    void GameLoop(Application_LocalImpl& impl, const Duration& dt)
    {
        static Point game_offset((WINDOW_WIDTH - (GRID_WIDTH * GRID_SIZE)) / 2, (WINDOW_HEIGHT - (GRID_HEIGHT * GRID_SIZE)) / 2 + 7);

        /* for ease of reference */
        SDL_Window* window = impl.Window;
        SDL_Renderer* renderer = impl.Renderer;

        #if defined(SN_DEBUG)
        clog << "Virtual FPS: " << 1 / dt.S() << endl;
        #endif
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255);
        SDL_RenderClear(renderer);

        if (Data.IsAtTitleScreen)
        {
            Resources.TitleTexture->Render(impl.Renderer, 0, 0);
        }
        else
        {
            /* render blue filled rect to highlight playing field */
            SDL_Rect fill_rect = { game_offset.x, game_offset.y, GRID_WIDTH * GRID_SIZE, GRID_HEIGHT * GRID_SIZE };
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderFillRect(renderer, &fill_rect);

            /* render black filled rect to represent food */
            SDL_Rect food = { Data.Food.x * GRID_SIZE + game_offset.x, Data.Food.y * GRID_SIZE + game_offset.y, GRID_SIZE, GRID_SIZE };
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(renderer, &food);

            /* draw in green rectangles wherever snake segments occupy grid */
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            for (int i = 0; i < GRID_WIDTH; i++)
            {
                for (int j = 0; j < GRID_HEIGHT; j++)
                {
                    if (IS_DIRECTION(Data.Grid(i, j)))
                    {
                        SDL_Rect snek_segment = { i * GRID_SIZE + game_offset.x, j * GRID_SIZE + game_offset.y, GRID_SIZE, GRID_SIZE };
                        SDL_RenderFillRect(renderer, &snek_segment);
                    }
                }
            }

            /* render scores and lives */
            Resources.LivesTexture->Render(impl.Renderer, game_offset.x, 10);
            Resources.ScoreTexture->Render(impl.Renderer, WINDOW_WIDTH - Resources.ScoreTexture->GetWidth() - game_offset.x, 10);

            /* get the current direction and find its offset */
            GridState current_direction = Data.Grid(Data.Snek.Head);
            Point offset = OffsetOf(current_direction);

            /* update head based on current direction */
            UpdatePosition<GRID_WIDTH, GRID_HEIGHT>(Data.Snek.Head, offset);

            /* This indicates that the snake has bitten itself */
            if (IS_DIRECTION(Data.Grid(Data.Snek.Head)))
            {
                /* subtract one life */
                --Data.Lives;
                if (Data.Lives < 0)
                {
                    /* game over if player runs out of lives */
                    Data.IsAtTitleScreen = true;
                    HardReset(impl);
                    return;
                }
                else
                {
                    /* reset game if player still has lives */
                    Reset(impl);
                    UpdateLives(impl);
                    return;
                }
            }

            /* this indicates that the food has been eaten */
            if (Data.Snek.Head == Data.Food)
            {
                /* Respawn the food */
                GenerateNewFoodPosition();

                /* Increment score and update texture */
                Data.Score += 1;
                UpdateScore(impl);
            }
            else
            {
                /* snake's tail keeps moving */
                GridState& tail = Data.Grid(Data.Snek.Tail);
                Point tail_offset = OffsetOf(Data.Grid(Data.Snek.Tail));
                tail = GridState::EMPTY;
                UpdatePosition<GRID_WIDTH, GRID_HEIGHT>(Data.Snek.Tail, tail_offset);
            }

            /* set fps based on formula */
            impl.SetFPS(FPSFormula());

            /* set head to move forward */
            Data.Grid(Data.Snek.Head) = current_direction;
        }

        /* update screen */
        SDL_RenderPresent(renderer);
    }

    void EventLoop(Application_LocalImpl& impl, SDL_Event& e)
    {
        /* ignore on quit */
        if (e.type == SDL_QUIT)
        {
            return;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            /* start game */
            if (Data.IsAtTitleScreen)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_0:
                case SDLK_o:
                    Data.IsAtTitleScreen = false;
                    break;
                }
            }
            else
            {
                /* set direction of snake */
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                case SDLK_w:
                    if (!ARE_OPPOSITE(Data.Grid(Data.Snek.Head), snek::GridState::NORTH)) { Data.Grid(Data.Snek.Head) = snek::GridState::NORTH; }
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (!ARE_OPPOSITE(Data.Grid(Data.Snek.Head), snek::GridState::SOUTH)) { Data.Grid(Data.Snek.Head) = snek::GridState::SOUTH; }
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (!ARE_OPPOSITE(Data.Grid(Data.Snek.Head), snek::GridState::WEST)) { Data.Grid(Data.Snek.Head) = snek::GridState::WEST; }
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (!ARE_OPPOSITE(Data.Grid(Data.Snek.Head), snek::GridState::EAST)) { Data.Grid(Data.Snek.Head) = snek::GridState::EAST; }
                    break;
                case SDLK_c:
                    HardReset(impl);
                    break;
                }
            }            
        }
    }

    void GameEnd(Application_LocalImpl& impl)
    {
        TTF_CloseFont(Resources.Font);
        delete Resources.TitleTexture;
        delete Resources.ScoreTexture;
        delete Resources.LivesTexture;
    }
}

template <size_t W, size_t H>
static snek::Point RandomCoord()
{
    static boost::random::random_device random_device;
    static boost::random::uniform_int_distribution<> w_distr(0, W - static_cast<size_t>(1));
    static boost::random::uniform_int_distribution<> h_distr(0, H - static_cast<size_t>(1));

    /* use uniform distribution to get random point */
    return snek::Point(w_distr(random_device), h_distr(random_device));
}

static void GenerateNewFoodPosition()
{
    snek::Point point;
    do
    {
        point = RandomCoord<GRID_WIDTH, GRID_HEIGHT>();
    } while (!Data.Grid.IsEmpty(point));
    Data.Food = point;
}

static snek::Point OffsetOf(snek::GridState state)
{
    snek::Point offset(0, 0);
    switch (state)
    {
    case snek::GridState::EAST:
        ++offset.x;
        break;
    case snek::GridState::WEST:
        --offset.x;
        break;
    case snek::GridState::NORTH:
        --offset.y;
        break;
    case snek::GridState::SOUTH:
        ++offset.y;
        break;
    default:
        SN_ASSERT(!"'state' does not denote a direction");
    }
    return offset;
}

template <size_t W, size_t H>
static void UpdatePosition(snek::Point& position, snek::Point offset)
{
    if (offset.x < 0 && position.x == 0) { position.x = int(W) - 1; }
    else if (offset.x > 0 && position.x == (int(W) - 1)) { position.x = 0; }
    else { position.x += offset.x; }

    if (offset.y < 0 && position.y == 0) { position.y = int(H) - 1; }
    else if (offset.y > 0 && position.y == (int(H) - 1)) { position.y = 0; }
    else { position.y += offset.y; }
}

static void Reset(Application_LocalImpl& impl)
{
    Data.Grid.Reset();
    Data.Snek.Reset();
    GenerateNewFoodPosition();
    Data.Grid(Data.Snek.Head) = snek::GridState::EAST;
    Data.Grid(Data.Food) = snek::GridState::FOOD;
}

static void HardReset(Application_LocalImpl& impl)
{
    Reset(impl);
    Data.Score = 0;
    Data.Lives = MAX_LIVES;
    impl.SetFPS(0);
    Data.IsAtTitleScreen = true;
    UpdateScore(impl);
    UpdateLives(impl);
}

static boost::filesystem::path GetExecutingPath(Application_LocalImpl& impl)
{
    static boost::filesystem::path p = boost::filesystem::path(impl.argv[0]).parent_path();
    return p;
}

static std::string Wide2Narrow(const std::wstring& wstr)
{
    return boost::locale::conv::utf_to_utf<char>(wstr);
}

static std::string GetExecutingPath(Application_LocalImpl& impl, const boost::filesystem::path& rel_path)
{
    return Wide2Narrow(std::wstring((GetExecutingPath(impl) / rel_path).c_str()));
}

static void UpdateScore(Application_LocalImpl& impl)
{
    static const SDL_Color text_color{ 0xFF, 0xFF, 0xFF };
    std::stringstream str;
    str << "Score: " << Data.Score;
    Resources.ScoreTexture->LoadText(str.str(), impl.Renderer, Resources.Font, text_color);
}

static void UpdateLives(Application_LocalImpl& impl)
{
    static const SDL_Color text_color{ 0xFF, 0xFF, 0xFF };
    std::stringstream str;
    str << "Lives: " << Data.Lives;
    Resources.LivesTexture->LoadText(str.str(), impl.Renderer, Resources.Font, text_color);
}

static int FPSFormula()
{
    int val = (TARGET_MIN_FPS + Data.Score - 20 * (MAX_LIVES - Data.Lives));
    return ((val < TARGET_MIN_FPS) ? (TARGET_MIN_FPS) : (val));
}