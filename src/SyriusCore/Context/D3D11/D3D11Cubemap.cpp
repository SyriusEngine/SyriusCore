#include "D3D11Cubemap.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Cubemap::D3D11Cubemap(const CubemapDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context):
    Cubemap(desc),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        std::vector<CubemapFaceDesc> faces = {
                desc.right,
                desc.left,
                desc.top,
                desc.bottom,
                desc.front,
                desc.back
        };

        createResources(faces);
    }

    D3D11Cubemap::D3D11Cubemap(const CubemapImageDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context):
    Cubemap(desc),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        std::vector<CubemapFaceDesc> faces = {
                {desc.right->getWidth(), desc.right->getHeight(), desc.right->getFormat(), desc.right->getData()},
                {desc.left->getWidth(), desc.left->getHeight(), desc.left->getFormat(), desc.left->getData()},
                {desc.top->getWidth(), desc.top->getHeight(), desc.top->getFormat(), desc.top->getData()},
                {desc.bottom->getWidth(), desc.bottom->getHeight(), desc.bottom->getFormat(), desc.bottom->getData()},
                {desc.front->getWidth(), desc.front->getHeight(), desc.front->getFormat(), desc.front->getData()},
                {desc.back->getWidth(), desc.back->getHeight(), desc.back->getFormat(), desc.back->getData()}
        };

        createResources(faces);
    }

    D3D11Cubemap::~D3D11Cubemap() {
        if (m_TextureView) {
            m_TextureView->Release();
        }
        if (m_Texture) {
            m_Texture->Release();
        }
    }

    void D3D11Cubemap::bind(uint32 slot) {
        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
    }

    uint64 D3D11Cubemap::getIdentifier() const {
        return reinterpret_cast<uint64>(m_Texture);
    }

    void D3D11Cubemap::createResources(const std::vector<CubemapFaceDesc> &faces) {
        SR_CORE_PRECONDITION(faces.size() == 6, "Cubemap must have 6 faces but has %i faces", faces.size());

        D3D11_TEXTURE2D_DESC textureDesc;
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 6;
        textureDesc.Format = getD3d11TextureFormat(m_Format);
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        uint32 typeSize = getTypeSize(getTextureDataType(m_Format)) * getTextureChannelCount(m_Format);
        D3D11_SUBRESOURCE_DATA subresources[6];
        for (uint32 i = 0; i < 6; ++i) {
            subresources[i].pSysMem = faces[i].data;
            subresources[i].SysMemPitch = faces[i].width * typeSize;
            subresources[i].SysMemSlicePitch = 0;
        }

        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, subresources, &m_Texture));

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
        viewDesc.Format = textureDesc.Format;
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        viewDesc.TextureCube.MipLevels = 1;
        viewDesc.TextureCube.MostDetailedMip = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Texture, &viewDesc, &m_TextureView));
    }
}

#endif
