#pragma once

#include "../TestInclude.hpp"

class VertexLayoutTest : public testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

protected:
    Resource<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;
};