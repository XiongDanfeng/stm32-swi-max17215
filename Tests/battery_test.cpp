#include "executor.h"
#include <gtest/gtest.h>

static int counter = 0;

void test_cb(void *ctx)
{
    counter++;
}

TEST(ExecutorTest, SingleCallback)
{
    Executor_Init();

    bool ok = Executor_Post(100, test_cb, nullptr);
    EXPECT_TRUE(ok);

    Executor_IRQHandler();

    EXPECT_EQ(counter, 1);
}