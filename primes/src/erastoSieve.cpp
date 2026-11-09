#include <erastoSieve.h>
#include <primeDictionary.h>
#include <main.h>

u32 findIndexForFirstFullyDivisible(u64 from, u64 divisor)
{
  u64 mod = from % divisor;
  u64 _from;

  if (mod == 0)
    return 0;

  _from = from;
  _from += (divisor - mod); // _next_state + inverse of modulus

  if (!(_from & 1))
    _from += divisor;

  return (_from - from) >> 1;
}

u64 sieveSegment(u64 from)
{
  
  if (!(from & 1))
  {
    from++;
  }

  std::bitset<subSieveLength> *sieve = new std::bitset<subSieveLength>();

  u64 sqrtOfUpperNum = std::sqrt(from + DOUBLEsubSieveLength);
  u32 divisiorID = 0;
  u32 lastDivisorID = findLastLesserInSorted(dictionary, divisiorID, dictPointer, sqrtOfUpperNum); // search from the divisor
  u32 it = 0;

  u64 divisor;
  u64 divisorSquare;

  // Actual loop!
  // Note:
  //      D - divisor
  for (; divisiorID <= lastDivisorID; divisiorID++) // iterate thru divisors
  {
    divisor = dictionary[divisiorID];
    divisorSquare = divisor * divisor;

    // elimination of redundant checks before D²...
    if (divisorSquare < from)
    {
      it = findIndexForFirstFullyDivisible(from, divisor); // find first value that is divisible by some D
    }
    else
    {
      it = (divisorSquare - from) >> 1;
    }

    for (; it < subSieveLength; it += divisor)
    {
      sieve->set(it);
    }
  }

  u64 totalFound = (subSieveLength - sieve->count()); // all zeros

  delete sieve;

  return totalFound;
};