#pragma once

#ifdef __clang__
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-function"
#elifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
