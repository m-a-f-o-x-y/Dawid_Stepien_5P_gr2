#pragma once

#ifndef PRIME_DICT
#define PRIME_DICT

#include <type_aliases.h>
#include <fstream>

// Dictionary constants
extern u32 dictPointer; 
extern u32 MAXdictPointer;
extern u64 *dictionary;

u32 findLastLesserInSorted(u64 *list, u32 first, u32 last, u32 what);
bool readDictionaryFile();

#endif