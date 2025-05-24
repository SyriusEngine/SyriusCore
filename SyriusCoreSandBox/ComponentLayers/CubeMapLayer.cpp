#include "CubeMapLayer.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

CubeMapLayer::CubeMapLayer(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    auto shader = container->getComponent<ShaderComponent>();
    shader->setShader("CubeMap", "CubeMap");

    auto cubeMapLayout = m_Context->createCubeMapLayout(2048, 2048, SR_TEXTURE_RGBA_UI8);
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_BACK, "./Resources/Textures/Skybox1/back.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_FRONT, "./Resources/Textures/Skybox1/front.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_LEFT, "./Resources/Textures/Skybox1/left.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_RIGHT, "./Resources/Textures/Skybox1/right.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_TOP, "./Resources/Textures/Skybox1/top.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_BOTTOM, "./Resources/Textures/Skybox1/bottom.jpg");

    m_CubeMap = m_Context->createCubeMap(cubeMapLayout);
    m_CubeMap->bindShaderResource(0);
}

void CubeMapLayer::onUpdate(Duration deltaTime) {
    m_CubeMap->bindShaderResource(0);

}


