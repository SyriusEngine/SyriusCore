#pragma once

#include "../TestEnvironment.hpp"
#include "../TestUtils.hpp"
#include "../ShaderSource.hpp"

struct SSBOPositionData{
    glm::vec4 positionData[3];
    glm::vec4 offset = glm::vec4(0.0f);
    glm::bvec4 readPositionFromBuffer = glm::bvec4(false);
};

struct SSBOColorData{
    glm::vec4 colorData = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
    glm::ivec4 increment = glm::ivec4(0);
    glm::bvec4 readColorFromBuffer = glm::bvec4(false);
};

class ShaderStorageBufferTest : public testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

protected:
    ResourceView<VertexLayout> m_Layout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<VertexArray> m_VSSBOVertexArray;
    ResourceView<Shader> m_VSSBOShader;
    ResourceView<VertexArray> m_FSSBOVertexArray;
    ResourceView<Shader> m_FSSBOShader;
};