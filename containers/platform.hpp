#pragma once

#ifdef _MSC_VER
#if _MSC_VER >= 1929
#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define NO_UNIQUE_ADDRESS /* [[no_unique_address]] */
#endif
#else
#define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
