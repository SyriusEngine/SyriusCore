#pragma once

#include "../TestEnvironment.hpp"
#include "../TestUtils.hpp"

class FrameBufferTest: public testing::Test{
protected:
    void SetUp() override;

    void TearDown() override;

    static ResourceView<FrameBuffer> createFrameBuffer1CA();

    static ResourceView<FrameBuffer> createFrameBuffer2CA();
};