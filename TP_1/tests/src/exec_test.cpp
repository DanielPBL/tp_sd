#include "exec.hpp"
#include "gtest/gtest.h"

namespace {
  TEST(ExecTest, ReturnString) {
    EXPECT_EQ("Olá mundo", exec("echo -n \"Olá mundo\""));
  }
}  // namespace
