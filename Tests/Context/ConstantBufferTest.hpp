#pragma once

#include "../TestEnvironment.hpp"

struct Vec4{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
};

struct Matrix4x4{
    Vec4 rows[4] = { 0 };
};

struct CBData{
    Matrix4x4 view = { 0 };
    Vec4 position = { 0 };
};

class ConstantBufferTest : public testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

protected:
    CBData m_Data;
};
