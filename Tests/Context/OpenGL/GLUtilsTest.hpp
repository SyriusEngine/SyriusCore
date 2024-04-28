#pragma once

#include <gtest/gtest.h>
#include "../../../include/SyriusCore/Utils/PlatformDetection.hpp"

class GLUtilsTest: public testing::Test{
protected:
    void SetUp() override;

    void TearDown() override;
};