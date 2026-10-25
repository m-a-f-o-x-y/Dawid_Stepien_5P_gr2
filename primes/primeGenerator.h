#ifndef primeGenerator_general
#define primeGenerator_general

typedef unsigned int uint;
typedef unsigned long long ulong;

std::fstream output_file;

inline int piX(uint x);
inline uint fitChunkSize(uint a);

// Prime seeking methods
inline bool GrowingListMethod(unsigned int a, std::vector<uint>& calc_data);
inline bool EasyMetWithSkipping(unsigned int a);

void primesChunk(uint start, uint chunkSize);


#endif // primeGenerator_general
