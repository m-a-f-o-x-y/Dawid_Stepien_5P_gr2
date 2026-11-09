#include <main.h>
#include <omp.h>

// Flags
bool simulatenousCPU = false;

// Data
u64 maxNumber = 1e10;
u64 currentNumber = 7;

static u64 primeCounter = 3;

const u32 physicalCoresNumber = std::thread::hardware_concurrency();
const u32 threadListSize = physicalCoresNumber * 4;

std::thread *threads = new std::thread[threadListSize];

int main()
{
  printf("[ PrimeSeek2 ]\n[ Version 1 ]\nPhysical cores: %u\n", physicalCoresNumber);

  readDictionaryFile();

  auto start = std::chrono::high_resolution_clock::now();

  // Exact computation
  if (simulatenousCPU)
  {
    while (currentNumber < maxNumber)
    {
      #pragma omp parallel for 
      for (u32 i = 0; i < threadListSize; i++)
      {

        threads[i] = std::thread(
            [i]()
            {
              primeCounter += sieveSegment(currentNumber);
            });
        currentNumber += DOUBLEsubSieveLength;

        threads[i].detach();
      }
    }
  }
  else
  {
    for (; currentNumber < maxNumber; currentNumber += DOUBLEsubSieveLength)
    {
      primeCounter += sieveSegment(currentNumber);
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