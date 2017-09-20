#include "exec.hpp"
#include "gtest/gtest.h"
#include <cstring>

namespace {
    TEST(ExecTest, ReturnString) {
        EXPECT_EQ("Olá mundo", exec("echo -n \"Olá mundo\""));
        EXPECT_STRNE(NULL, strstr(exec("grep \"Linha 1:\" maquina.*.log").c_str(),
            "Linha 1: Lorem ipsum dolor sit amet, consectetur adipiscing elit."));
    }
}  // namespace
