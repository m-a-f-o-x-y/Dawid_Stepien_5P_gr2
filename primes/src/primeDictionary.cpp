#include <primeDictionary.h>

u32 dictPointer = 0;
u32 MAXdictPointer = 100000;
u64 *dictionary = new u64[MAXdictPointer];

u32 findLastLesserInSorted(u64 *list, u32 first, u32 last, u32 what)
{
  u32 mid;
  last--;

  while (first <= last)
  {
    mid = (first + last) >> 1;

    if (what > list[mid])
    {
      first = mid + 1;
    }
    else
    {
      last = mid - 1;
    }
  }

  return first;
}

bool readDictionaryFile()
{
  
  std::fstream dictFile;
  std::string buf;
  dictFile.open("prime_dict.txt", std::fstream::in);

  if (dictFile.good())
  {
    while (MAXdictPointer > dictPointer && !dictFile.eof())
    {
      getline(dictFile, buf, ' ');

      if (dictPointer < MAXdictPointer)
      {
        dictionary[dictPointer++] = std::stoll(buf);
      }
    }
  }
  else {
    return false;
  }
  return true;
}