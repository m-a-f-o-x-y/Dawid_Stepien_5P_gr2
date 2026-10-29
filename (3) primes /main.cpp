#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <cmath>
#include <format>

#include <thread>
#include <mutex>

#include <chrono>

#define subSieveLen 50000

//
// typedefs
typedef unsigned int uint;
typedef uint64_t ulong;

//
// definitions
std::fstream output_file, primes_dict;
inline ulong piX(ulong x);
inline ulong fitChunkSize(ulong a);
bool (*method)(ulong);
bool verifyPrime(ulong a);
void writeBuffer();

//
// Tweaks
static bool
ReturnPrimes = false, // tweaking it on, reduces the speed of execution
FromScratch = false, // use dictionary
WithVerification = false,
PreSortBuffer = false,
BinaryOutput = false,
Sieving = true;

//
// calc variables
static ulong maxNumber = 1e9, numbersDone = 0;

static const ulong bufferSize = 1e8;
static ulong chunkSize = 1e6;

static ulong maxPrime; // value of biggest prime asm
static ulong maxPrimeSquared;
static uint outputFileNumber = 1;
static ulong targetFound = bufferSize; // special variable that monitors vectors

ulong number = 5, found = 3;

static std::chrono::time_point<std::chrono::high_resolution_clock> buffer_start, buffer_end;

//
// Containers
static std::vector<ulong> primes = {}, dictionary = {};
static std::vector<std::thread> threads; // array for threads

//
// Special mutex for synchronous append
static std::mutex primesMutex;

inline ulong
piX(ulong x)
{
    return (2.3 * x) / std::log(x); // std::log(x) => ln, not log10!
}

// Function that seeks primes using previously generated primes
inline bool
is_prime(ulong a)
{
    if (a % 2 == 0) { return false; }
    if (a % 3 == 0) { return false; }
    if (a % 5 == 0) { return false; }

    ulong prime_index = 3;
    ulong divisor = dictionary[prime_index];

    // do it instead doing costly square-rooting for every single
    // divisor in case of checking number that
    // in the end turn out to be prime
    while (divisor*divisor <= a)
    {
        if (a % divisor == 0) return false;
        divisor = dictionary[++prime_index];

    }
    return true;
}

inline bool
is_prime_simpler(ulong a)
{
    // Split deliberately to reduce instructions
    if (a % 2 == 0){ return false; }
    if (a % 3 == 0){ return false; }
    if (a % 5 == 0){ return false; }

    for (int div = 7; div*div <= a; div += 2)
    {
        if (div % 5 == 0 || div % 3 == 0) continue;
        if (a % div == 0) return false;
    }

    return true;
}

// Returns index!
inline ulong firstOfMult(ulong A, ulong mul)
{
    if (A % mul == 0) return 0;
    ulong index = 0;

    ulong a = A;

    A += (mul - (A % mul));

    if (A % 2 == 0) A += mul;

    return (A - a)/2;

}

std::pair<ulong*, ulong> erastoSieve(ulong _begin, ulong chunkSize)
{
    if (_begin % 2 == 0)
    {
        _begin++;
    }
    ulong vector_size = piX(_begin+(chunkSize/2)) - piX(_begin); // reserve instead of constantly reallocating
    ulong* primes_buf;
    ulong foundCount = 0;

    uint subSieveNum = chunkSize/(2*subSieveLen) + (chunkSize % (2*subSieveLen) != 0);

    auto *sieve = new std::bitset<subSieveLen>[subSieveNum]; // 1 -> composite, 0 -> prime

    if (ReturnPrimes)
    {
       primes_buf = new ulong[vector_size];
    }

    uint divisor_id = 1;
    ulong divisor = 3, divisorSquare = 9;
    uint subSieve = 0;
    ulong i = 0;

    while (divisorSquare <= (_begin + chunkSize) )
    {
        i = firstOfMult(_begin, divisor);

        for (; i < chunkSize/2;) // seek first of mult
        {
            for (; i < (subSieve+1)*subSieveLen; i += divisor)
            {
                 sieve[subSieve++].set(i % subSieveLen);
            }
        }

        divisor = dictionary[++divisor_id];
        divisorSquare = divisor*divisor;
        subSieve = 0;
    }

    for (uint i = 0; i < subSieveNum; i++)
    {
        for (uint j = 0; j < subSieveLen; j++){
            if (!sieve[i][j])
            {
                foundCount++;
                if (ReturnPrimes)
                {
                    primes_buf[foundCount++] = _begin + ( 2 * i );
                }

            }
        }
    }

    delete[] sieve;
    return std::make_pair(primes_buf, foundCount);
}

bool lessThan(ulong a)
{
    return a > maxNumber;
}

void
primesChunk(ulong start, ulong chunkSize)
{
    std::pair<ulong*, ulong> primes_buf;

    if (Sieving)
    {
        primes_buf = erastoSieve(start, chunkSize);
    }
    else
    {
        for (ulong end = start + chunkSize; start < end; start += 2)
        {
            if (start > maxNumber)
            {
                break;
            }
            if (method(start))
            {
                if (ReturnPrimes)
                {
                    //primes_buf.first->push_back(start); // found prime place to prime buffer
                }

            }
        }
    }

    primesMutex.lock();
    found += primes_buf.second;


    if (ReturnPrimes)
    {
        for (uint i = 0; i < primes_buf.second; i++)
        {
            if (WithVerification){
                if ( !verifyPrime(primes_buf.first[i]) )
                {
                    std::cout << primes_buf.first[i] << " is not a prime!\n";
                }
            }

            primes.push_back( primes_buf.first[i] );

            if (found >= targetFound)
            {
                numbersDone = number;
                writeBuffer();

                buffer_end = std::chrono::high_resolution_clock::now();
                auto buffer_duration = (std::chrono::duration_cast<std::chrono::milliseconds > (buffer_end - buffer_start).count() / 1000.0);
                buffer_start = buffer_end;

                targetFound += bufferSize;
                std::cout << std::format("[ PROGRESS ] ( {} / {} primes/min ) Total primes: {} [{}%]\n",
                                         buffer_duration,
                                         (60 / buffer_duration) * bufferSize,
                                         found,
                                         numbersDone * 100/ maxNumber );
            }
        }
    }

    //delete primes_buf.first;
    primesMutex.unlock();
}

void
writeBuffer()
{
    //
    // File handling
    if (ReturnPrimes)
    {
        if (output_file.bad())
        {
            std::cerr << "[ ERROR ] No reach of primes file\n";
        }

        output_file.open(std::format("Q:\\primes\\primesChunk_{}", outputFileNumber), std::ios::out | std::ios::binary);

        if (PreSortBuffer)
        {
           std::sort(primes.begin(), primes.end());
        }


        uint8_t byteBuffer[8] = {};
        for (ulong i : primes)
        {
            if (i > maxNumber) break;

            if (BinaryOutput)
            {
                byteBuffer[0] = (i >> 56) & 0xFF;
                byteBuffer[1] = (i >> 48) & 0xFF;
                byteBuffer[2] = (i >> 40) & 0xFF;
                byteBuffer[3] = (i >> 32) & 0xFF;
                byteBuffer[4] = (i >> 24) & 0xFF;
                byteBuffer[5] = (i >> 16) & 0xFF;
                byteBuffer[6] = (i >> 8) & 0xFF;
                byteBuffer[7] = i & 0xFF;

                output_file.write(reinterpret_cast<char *>(&byteBuffer), 8);
            }
            else
            {
                output_file << i << "\r\n";
            }

        }

        output_file.close();
        outputFileNumber++;
    }
    primes.clear();
}

bool verifyPrime(ulong a)
{
    return is_prime(a);
}

int main(int argc, char* argv[])
{

    if (argc > 1)
    {
        maxNumber = std::atoll(argv[1]);
    }
    

    // Performance tweaks
    std::cin.tie(nullptr);

    std::fstream logoFile;
    std::string buf;
    logoFile.open("./logoFile");

    while (!logoFile.eof())
    {
        getline(logoFile, buf);
        std::cout << buf << "\n";
    }

    std::cout << "\n";


    // Prime dictionary handling
    if (!FromScratch)
    {
        method = is_prime;

        primes_dict.open("./prime_dict.txt", std::ios::in);
        std::string numberStr;
        ulong _number;

        while (!primes_dict.eof())
        {
            getline(primes_dict, numberStr, ' ');
            _number = std::stoi(numberStr);
            dictionary.push_back(_number);
        }
        std::cout << "[ INFO ] " << dictionary.size() << "-primes dictionary loaded\n";
        maxPrime = dictionary[dictionary.size() - 1];
        maxPrimeSquared = maxPrime * maxPrime;

        primes = dictionary;
        number = maxPrime + 2; // next even number
        numbersDone = number;

        if (found > dictionary.size()) // add uncounted entries
        {
            found += dictionary.size();
        }
        else
        {
            found = std::distance(dictionary.begin(),
                                  std::find_if(dictionary.begin(), dictionary.end(), lessThan));
        }

        for (; number % chunkSize != 0; number++ )
        {
                if (is_prime(number))
                {
                    found++;
                    primes.push_back(number);
                }
        }
    }
    else
    {
        std::cout << "No prime dictionary\n";
        method = is_prime_simpler;
        primes = {2, 3, 5};
    }

    //
    // Chrono timestamps
    auto start = std::chrono::high_resolution_clock::now();

    //
    // Actual loop
    if (ReturnPrimes)
    {
        primes.reserve(bufferSize);
        threads.reserve(75);
    }

    bool working = true;
    buffer_start = std::chrono::high_resolution_clock::now();

    while ( working )   // notion!!
    {
        for (int i = 0; i < 75; i++)
        {
            if ( (number < maxNumber) )
            {
                    threads.emplace_back(std::thread(primesChunk,
                                                 number,
                                                 chunkSize));
                    number += chunkSize;
            }
            else
            {
                // halt the algorithm
                working = false;
                break;
            }
        }

        for (uint i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }

        threads.clear();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "\npi( " << maxNumber << " ) = " << found << " " << "[ in "
              << (std::chrono::duration_cast<std::chrono::milliseconds > (end - start).count() / 1000.0)
              << "s ]\n";

    if (ReturnPrimes)
    {
        writeBuffer();
    }

    return 0;
}
