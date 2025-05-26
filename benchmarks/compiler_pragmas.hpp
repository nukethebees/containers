#pragma once

#ifdef __clang__
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
