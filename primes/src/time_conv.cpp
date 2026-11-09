#include <time_conv.h>
#include <cstdio>
#include <type_aliases.h>

const char *suffixes[] = { "d", "h", "min", "s", "ms", "µs" };
int subValsTo[] = {3, 3, 3, 4, 4, 5};

u64 ticks[] = {
    (u64)1000 * 1000 * 60 * 60 * 24,       // d
    (u64)1000 * 1000 * 60 * 60,            // h
    1000 * 1000 * 60,                           // m
    1000 * 1000,                                // s
    1000,                                       // ms
    1
};

// convertToDurationString
// ARGS:
//  - std::chrono duration millisecond ticks
void convertToDurationString(uint64_t duration)
{
    u32 index = 0, _subValsTo = 5;

    for (; index <= 5; index++)
    {
        if(duration > ticks[index]){ break; }
    }

    if( index != 6){
        _subValsTo = subValsTo[index];
    }

    u32 a = 0;
    
    for (; index <= _subValsTo; index++){

        a = (duration / ticks[index]);

        printf("%ld %s ", a, suffixes[index]);
        duration -= ticks[index] * a;
    }
}