#pragma once

#include "snek-config.h"

namespace snek
{
    class Application
    {
    public:
        Application(int argc, char** argv);

    private:
        Application(const Application&);

    public:
        ~Application();

        int Run();

    private:
        struct Impl;
        mutable Impl* m_Impl;
    };
}