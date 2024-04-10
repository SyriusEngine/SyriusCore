#pragma once

#include "../TestEnvironment.hpp"
#include "../TestUtils.hpp"
#include "../ShaderSource.hpp"

struct SSBOPositionData{
    glm::vec4 positionData[3];
    glm::vec4 offset = glm::vec4(0.0f);
    glm::bvec4 readPositionFromBuffer = glm::bvec4(false);
};

class ShaderStorageBufferTest : public testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

protected:
    ResourceView<VertexLayout> m_Layout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<VertexArray> m_VertexArray;
    ResourceView<Shader> m_Shader;
};