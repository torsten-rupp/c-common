#define CTEST_MAIN
#define CTEST_SEGFAULT

#include "ctest/ctest.h"

#include "common/global.h"

#include "common/progressinfo.h"

CTEST(progressInfo,init_done)
{
  ProgressInfo progressInfo;

  ProgressInfo_init(&progressInfo,
                    NULL,
                    0, 0, 50,
                    NULL, NULL,
                    NULL, NULL,
                    NULL, NULL,
                    NULL
                   );
  ProgressInfo_done(&progressInfo);

  size_t initStep = 0;
  size_t doneStep = 0;
  ProgressInfo_init(&progressInfo,
                    NULL,                      // no parent
                    0,                         // no filter window
                    0,                         // no report time
                    100,                       // 100 steps
                    CALLBACK_LAMBDA_(void, (const char *, uint64, void *), { initStep++; }, NULL),
                    CALLBACK_LAMBDA_(void, (size_t, void *), { doneStep++; }, NULL),
                    CALLBACK_LAMBDA_(void, (uint64, ulong, ulong, void *), { }, NULL),
                    "test progress %d", 1
                   );
  ASSERT_EQUAL(1, initStep);
  ASSERT_EQUAL(0, doneStep);

  ProgressInfo_done(&progressInfo);
  ASSERT_EQUAL(1, doneStep);
}

CTEST(progressInfo,step)
{
  ProgressInfo progressInfo;

  ProgressInfo_init(&progressInfo,
                    NULL,
                    0, 0, 50,
                    NULL, NULL,
                    NULL, NULL,
                    NULL, NULL,
                    NULL
                   );
  ProgressInfo_done(&progressInfo);

  size_t infoStep = 0;
  ProgressInfo_init(&progressInfo,
                    NULL,                      // no parent
                    0,                         // no filter window
                    0,                         // no report time
                    100,                       // 100 steps
                    CALLBACK_LAMBDA_(void, (const char *, uint64, void *), { }, NULL),
                    CALLBACK_LAMBDA_(void, (size_t, void *), { }, NULL),
                    CALLBACK_LAMBDA_(void, (uint64, size_t, size_t, void *), { infoStep++; }, NULL),
                    "test progress %d", 1
                   );

  ASSERT_EQUAL(0, infoStep);
  ProgressInfo_step(&progressInfo);
  ASSERT_EQUAL(1, infoStep);
  ProgressInfo_step(&progressInfo);
  ASSERT_EQUAL(2, infoStep);

  ProgressInfo_done(&progressInfo);
}

CTEST(progressInfo,reset)
{
  ProgressInfo progressInfo;

  size_t initStep = 0;
  size_t doneStep = 0;
  size_t infoStep = 0;
  ProgressInfo_init(&progressInfo,
                    NULL,                      // no parent
                    0,                         // no filter window
                    0,                         // no report time
                    100,                       // 100 steps
                    CALLBACK_LAMBDA_(void, (const char *, uint64, void *), { initStep++; }, NULL),
                    CALLBACK_LAMBDA_(void, (size_t, void *), { doneStep++; }, NULL),
                    CALLBACK_LAMBDA_(void, (uint64, size_t, size_t, void *), { infoStep++; }, NULL),
                    "test progress %d", 1
                   );

  ASSERT_EQUAL(1, initStep);

  ASSERT_EQUAL(0, infoStep);
  ProgressInfo_step(&progressInfo);
  ASSERT_EQUAL(1, infoStep);

  ProgressInfo_reset(&progressInfo, 100);
  ASSERT_EQUAL(2, initStep);

  ProgressInfo_done(&progressInfo);
}

int main(int argc, const char *argv[])
{
  return ctest_main(argc, argv);
}
