#include "D3D11VertexArray.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11VertexArray::D3D11VertexArray(const VertexArrayDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : VertexArray(desc),
    m_Device(device),
    m_Context(context),
    m_InputLayout(nullptr),
    m_D3d11DrawMode(getD3d11DrawMode(desc.m_DrawMode)){
        std::vector<D3D11_INPUT_ELEMENT_DESC> elements;

        uint32 index = 0;
        uint32 offset = 0;
        for (const auto& attr: m_VertexBuffer->getVertexDescription()->getAttributes()){
            D3D11_INPUT_ELEMENT_DESC element;
            element.SemanticName = attr.m_Name.c_str();
            element.SemanticIndex = 0;
            element.Format = getD3d11ScalarType(attr.m_Type);
            element.InputSlot = 0;
            element.AlignedByteOffset = offset;
            element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            element.InstanceDataStepRate = 0;

            elements.push_back(element);
            offset += attr.m_Size;
            index++;
        }

        auto vertexShaderBlob = reinterpret_cast<ID3DBlob*>(desc.m_VertexShader->getIdentifier());
        SR_CORE_D3D11_CALL(m_Device->CreateInputLayout(
                &elements[0],
                elements.size(),
                vertexShaderBlob->GetBufferPointer(),
                vertexShaderBlob->GetBufferSize(),
                &m_InputLayout
                ));
    }

    D3D11VertexArray::~D3D11VertexArray() {
        if (m_InputLayout) {
            m_InputLayout->Release();
        }

    }

    void D3D11VertexArray::bind() {
        m_VertexBuffer->bind();
        m_Context->IASetInputLayout(m_InputLayout);
    }

    void D3D11VertexArray::unbind() {

    }

    void D3D11VertexArray::drawBuffers() {
        m_VertexBuffer->bind();
        m_Context->IASetInputLayout(m_InputLayout);
        m_Context->IASetPrimitiveTopology(m_D3d11DrawMode);

        m_Context->Draw(m_VertexBuffer->getCount(), 0);

        SR_CORE_DXGI_GET_MESSAGES()
    }

    void D3D11VertexArray::drawBuffersInstanced(uint32 instanceCount) {
        m_VertexBuffer->bind();
        m_Context->IASetInputLayout(m_InputLayout);
        m_Context->IASetPrimitiveTopology(m_D3d11DrawMode);

        m_Context->DrawInstanced(m_VertexBuffer->getCount(), instanceCount, 0, 0);

    }

    void D3D11VertexArray::setDrawMode(SR_DRAW_TYPE drawMode) {
        m_DrawMode = drawMode;
        m_D3d11DrawMode = getD3d11DrawMode(drawMode);
    }

    D3D11VertexArrayIndexed::D3D11VertexArrayIndexed(const VertexArrayDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : D3D11VertexArray(desc, device, context){

    }

    D3D11VertexArrayIndexed::~D3D11VertexArrayIndexed() {

    }

    void D3D11VertexArrayIndexed::bind() {
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
        m_Context->IASetInputLayout(m_InputLayout);
    }

    void D3D11VertexArrayIndexed::drawBuffers() {
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
        m_Context->IASetInputLayout(m_InputLayout);
        m_Context->IASetPrimitiveTopology(m_D3d11DrawMode);

        m_Context->DrawIndexed(m_IndexBuffer->getCount(), 0, 0);
    }

    void D3D11VertexArrayIndexed::drawBuffersInstanced(uint32 instanceCount) {
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
        m_Context->IASetInputLayout(m_InputLayout);
        m_Context->IASetPrimitiveTopology(m_D3d11DrawMode);

        m_Context->DrawIndexedInstanced(m_IndexBuffer->getCount(), instanceCount, 0, 0, 0);

        SR_CORE_DXGI_GET_MESSAGES()
    }
}

#endif
