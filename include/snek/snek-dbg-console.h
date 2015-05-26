#pragma once

#include <snek-pch.h>
#include "snek-config.h"

/* console only required on Win32 debug */
#if defined(SN_DEBUG) && (SN_PLATFORM == SN_WIN)
#   define SN_ALLOC_DEBUG_CONSOLE() sn_alloc_dbg_console()

#include <cstdio>
#include <io.h>
#include <fcntl.h>

/* creates a console in Win32 debug builds */
static void sn_alloc_dbg_console(void)
{
    int h_crt;
    FILE *f;
    HANDLE handle;

    /* Allocate console for process and check for failure */
    if (AllocConsole() == 0)
    {
        return;
    }

    /* stdout file descriptor */
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    h_crt = _open_osfhandle((long)handle, _O_TEXT);
    f = _fdopen(h_crt, "w");
    setvbuf(f, NULL, _IONBF, 1);
    /* cout */
    *stdout = *f;

    /* stderr file descriptor */
    handle = GetStdHandle(STD_ERROR_HANDLE);
    h_crt = _open_osfhandle((long)handle, _O_TEXT);
    f = _fdopen(h_crt, "w");
    setvbuf(f, NULL, _IONBF, 1);
    /* clog, cerr */
    *stderr = *f;

    /* stdin file descriptor */
    handle = GetStdHandle(STD_INPUT_HANDLE);
    h_crt = _open_osfhandle((long)handle, _O_TEXT);
    f = _fdopen(h_crt, "r");
    setvbuf(f, NULL, _IONBF, 128);
    /* cin */
    *stdin = *f;
}

#else
#   define SN_ALLOC_DEBUG_CONSOLE()
#endif