#pragma once

#ifndef ERASTO_SIEVE
#define ERASTO_SIEVE

#include <type_aliases.h>
#include <cmath>

u32 findIndexForFirstFullyDivisible(u64 from, u64 divisor);
u64 sieveSegment(u64 from);
u64 customSieveSegment(u64 from, u64 segmentSize);

#endif