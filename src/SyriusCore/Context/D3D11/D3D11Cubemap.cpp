#include "D3D11CubeMap.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11CubeMap::D3D11CubeMap(const ResourceView<CubeMapLayout>& desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    CubeMap(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
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
        auto& faces = desc->getFaces();
        D3D11_SUBRESOURCE_DATA subresources[6];
        for (uint32 i = 0; i < 6; ++i) {
            subresources[i].pSysMem = faces[i]->getData();
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


    D3D11CubeMap::~D3D11CubeMap() {
        if (m_TextureView) {
            m_TextureView->Release();
        }
        if (m_Texture) {
            m_Texture->Release();
        }
    }

    void D3D11CubeMap::bind() {
        // D3D11 doesn't have a bind function
    }

    void D3D11CubeMap::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
    }

    void D3D11CubeMap::copyFrom(const ResourceView<Texture2D> &other, SR_CUBEMAP_FACE destinationFace) {

    }

    void D3D11CubeMap::copyFrom(const ResourceView<ColorAttachment> &other, SR_CUBEMAP_FACE destinationFace) {

    }

    uint64 D3D11CubeMap::getIdentifier() const {
        return reinterpret_cast<uint64>(m_TextureView);
    }

}

#endif
