#include <main.h>
#include <unordered_map>
#include <omp.h>
#include <functional>
#include <erastoSieve.h>

// Flags
bool simulatenousCPU = false;
bool usingExternalDictionary = true;

// Data
u64 maxNumber = 1e9; // 1e9 is default
u64 currentNumber = 7;

std::mutex primeCounterMutex;
static u64 primeCounter = 3;

const u32 physicalCoresNumber = std::thread::hardware_concurrency();

// Specialized for 'x^y' number formats
u64 extractNumberFromArgument(const char *argument)
{
  u32 preEnum = 1, postEnum = 1;
  const char *eOccurence;
  char *preEnumStr, *postEnumStr;
  size_t eOccurenceIndex = -1;

  u32 argumentLen = strlen(argument);

  eOccurence = strpbrk(argument, "^");     // find the 'e' if specified
  eOccurenceIndex = eOccurence - argument; // calculate index of 'e'

  // Allocate space for string buffers;
  if (eOccurenceIndex != -1 && eOccurence != 0)
  {
    preEnumStr = new char[eOccurenceIndex];
  }
  
  postEnumStr = new char[argumentLen - eOccurenceIndex];

  // split into two separate strings
  strncpy(preEnumStr, argument, eOccurenceIndex);
  argument += eOccurenceIndex + 1; // shift to second part
  strncpy(postEnumStr, argument, strlen(argument)); // extract the second part from the rest

  // conversion to integers
  preEnum = std::stoll(preEnumStr);
  postEnum = std::stoll(postEnumStr);

  delete[] preEnumStr, postEnumStr, eOccurence; // purge dynamic variables 

  return pow(preEnum, postEnum);
}

int main(int argc, char *argv[])
{
  u32 argument_it = 1;

  std::unordered_map<std::string, std::function<void()>> optionList =
      {
          {"-S", []()
           { simulatenousCPU = true; }},
          {"-r", [argv, argument_it]()
           { maxNumber = extractNumberFromArgument(argv[argument_it + 2]); }},
  };

  // read arguments from command line
  if (argc > 1)
  {
    for (; argument_it < argc; argument_it++)
    {
      try
      {
        // When the option is specified
        auto it = optionList[argv[argument_it]];
        it();
      }
      catch (std::bad_function_call) // i.e. if not specified
      {
        printf("Operation not specified: \"%s\"\n", argv[argument_it]);
      }
    }
  }

  printf("--- PrimeSeek2 ---\n|  Physical cores: %u\n|  Multithreaded: %s\n|  Number range: %llu - %llu\n"
    , physicalCoresNumber, (simulatenousCPU ? "true" : "false"), currentNumber, maxNumber);

  readDictionaryFile();

  auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for reduction(+ : primeCounter) if (simulatenousCPU)
  for (u64 i = 7; i < maxNumber; i += DOUBLEsubSieveLength)
  {
    if (i + DOUBLEsubSieveLength > maxNumber){
      primeCounter += customSieveSegment(i, ((maxNumber - i) >> 1) );
    }
    else{
      primeCounter += sieveSegment(i);
      continue;
    }
  }

  // Duration calculation
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>((end - start)).count();

  printf("\n%lld primes | ", primeCounter);
  convertToDurationString(duration);
  printf("\n");

  delete[] dictionary;
}