#pragma once

#include "snek-config.h"

namespace snek
{
    class Application final
    {
    public:
        Application();
        Application(const Application&) = delete;
        Application(Application&&) = delete;
        ~Application();

        int Run();

    private:
        struct Impl;
        mutable Impl* m_Impl;
    };
}