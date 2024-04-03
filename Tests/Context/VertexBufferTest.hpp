#pragma once

#include "../TestEnvironment.hpp"
#include "../TestUtils.hpp"

class VertexBufferTest : public testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

protected:
    ResourceView<VertexLayout> m_Layout;
};