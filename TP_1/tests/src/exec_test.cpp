#include "exec.hpp"
#include "gtest/gtest.h"

namespace {
  TEST(ExecTest, ReturnString) {
    EXPECT_EQ("Olá mundo", exec("echo -n \"Olá mundo\""));
    EXPECT_EQ("daniel    2994  0.0  0.0  22960  5632 pts/2    Ss   07:09   0:00 bash\n",
      exec("grep bash maquina.1.log"));
  }
}  // namespace
