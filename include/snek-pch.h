#include "snek/snek-config.h"

#if (SN_PLATFORM == SN_WIN) && !defined(SN_NO_WIN_HEADERS)
#   if !defined(SN_NO_WIN32_LEAN) && !defined(WIN32_LEAN_AND_MEAN)
#      define WIN32_LEAN_AND_MEAN
#   endif
#   if !defined(SN_NO_WIN32_LEAN) && !defined(SN_NO_WIN32_LEAN_EXTRA) && defined(_MSC_VER) && !defined(VC_EXTRALEAN)
#      define VC_EXTRALEAN
#   endif
#   include <windows.h>
#endif