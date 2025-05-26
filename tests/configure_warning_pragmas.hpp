#pragma once

// To be placed after all includes
// gtest throws a lot of warnings

#ifdef __clang__
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
