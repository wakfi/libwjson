#ifndef LWJ_TEST_HELPERS
#define LWJ_TEST_HELPERS

#include <string>
#include <cstdio>
#include <cstdlib>

typedef unsigned long size_t;
typedef unsigned char byte;
typedef unsigned int uint;

//----------------------------------------------------------------------
// Helper functions for testing
//----------------------------------------------------------------------

/** Read file into memory. Derived from https://www.cplusplus.com/reference/cstdio/fread/ */
inline byte* slurp(const std::string& path, long int& len) {
  FILE* pFile;
  byte* buf;
  size_t result;

  // open file
  pFile = fopen(path.c_str(), "r");
  if(pFile == NULL) {fputs(("File error: Could not open '" + path + '\'').c_str(), stderr); return nullptr;}
  // obtain file size
  fseek(pFile, 0, SEEK_END);
  len = ftell(pFile);
  rewind(pFile);
  // allocate memory
  buf = (byte*)malloc(sizeof(byte)*len);
  if(buf == NULL) {fputs("Memory allocation error", stderr); fclose(pFile); return nullptr;}
  // copy the file into the buffer:
  result = fread(buf, 1, len, pFile);
  if(result != len) {fputs("Reading error", stderr);if(!result){free(buf); fclose(pFile); return nullptr;}}
  /* the whole file is now loaded in the memory buffer */
  // cleanup
  fclose(pFile);

  return buf;
}

template<typename T>
inline void unslurp(T*& p)
{
  if(p) free(p);
  p = nullptr;
}

void readbuf(byte* p, size_t start, size_t length, byte out[])
{
  p += start;
  for(size_t i = 0; i < length; ++i)
  {
    out[i] = *(p + i);
  }
}

void readbuf(byte* p, size_t length, byte out[])
{
  readbuf(p, 0, length, out);
}

/* ASSERT_ARRAYEQ DEFINITION START */
#define ASSERT_ARRAYEQ(arr1, arr2, len)\
for(unsigned long i = 0; i < len; ++i) {ASSERT_EQ(arr1[i], arr2[i]);}
/* END ASSERT_ARRAYEQ */

#endif // ifndef LWJ_TEST_HELPERS
