#pragma once

#include <gtest/gtest.h>
#include <SyriusUtils/SyriusUtils.hpp>

#if defined(SR_PLATFORM_WIN64)

class D3D11UtilsTest: public testing::Test{
protected:
    void SetUp() override;

    void TearDown() override;
};

#endif