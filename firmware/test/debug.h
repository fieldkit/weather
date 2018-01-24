#ifndef FK_DEBUG_H_INCLUDED
#define FK_DEBUG_H_INCLUDED

#include <cstdarg>

void debugf(const char *f, ...) __attribute__((format(printf, 1, 2)));

void debugfln(const char *f, ...) __attribute__((format(printf, 1, 2)));

void debugfpln(const char *prefix, const char *f, ...) __attribute__((format(printf, 2, 3)));

#endif

