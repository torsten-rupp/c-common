#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"

#include "common/tinymemory.h"

CTEST(tinyMemory,init_done)
{
  byte memory[1024];
  TinyMemory *tinyMemory = TinyMemory_init(memory,sizeof(memory));
  TinyMemory_done(tinyMemory);
}

CTEST(tinyMemory,alloc_free)
{
  byte memory[1024];
  TinyMemory *tinyMemory = TinyMemory_init(memory,sizeof(memory));

  void *p = TinyMemory_alloc(tinyMemory,100);
  ASSERT_NOT_NULL(p);
  p = TinyMemory_realloc(tinyMemory,p,200);
  TinyMemory_free(tinyMemory,p);
  TinyMemory_done(tinyMemory);
}

CTEST(tinyMemory,realloc)
{
  byte memory[1024];
  TinyMemory *tinyMemory = TinyMemory_init(memory,sizeof(memory));
  void *p = TinyMemory_alloc(tinyMemory,100);
  ASSERT_NOT_NULL(p);
  TinyMemory_free(tinyMemory,p);
  TinyMemory_done(tinyMemory);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
