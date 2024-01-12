#include "D3D11Cubemap.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Cubemap::D3D11Cubemap(const CubemapDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context):
    Cubemap(desc),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        const void* faces[6] = {
                desc.faces[0],
                desc.faces[1],
                desc.faces[2],
                desc.faces[3],
                desc.faces[4],
                desc.faces[5]
        };
        createResources(faces);
    }

    D3D11Cubemap::D3D11Cubemap(const CubemapImageDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context):
    Cubemap(desc),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        const void* faces[6] = {
                desc.faces[0]->getData(),
                desc.faces[1]->getData(),
                desc.faces[2]->getData(),
                desc.faces[3]->getData(),
                desc.faces[4]->getData(),
                desc.faces[5]->getData()
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

    void D3D11Cubemap::createResources(const void* faces[6]) {
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
            subresources[i].pSysMem = faces[i];
            subresources[i].SysMemPitch = m_Width * typeSize;
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
