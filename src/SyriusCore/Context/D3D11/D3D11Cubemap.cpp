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

    Resource<Image> D3D11CubeMap::getData(SR_CUBEMAP_FACE destinationFace) {
        // we use a staging texture to copy the data back to the CPU as a cubeMap cannot be mapped
        auto channelCount = getTextureChannelCount(m_Format);

        ID3D11Texture2D* stagingTexture = nullptr;
        D3D11_TEXTURE2D_DESC desc = { 0 };
        m_Texture->GetDesc(&desc);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&desc, nullptr, &stagingTexture));

        m_Context->CopySubresourceRegion(
            stagingTexture, 0, 0, 0, 0,
            m_Texture, D3D11CalcSubresource(0, (UINT) destinationFace, 1), nullptr);

        D3D11_MAPPED_SUBRESOURCE map = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &map));
        if (map.pData == nullptr){
            SR_CORE_THROW("[D3D11Texture2D]: Failed to map staging texture (%p)", stagingTexture);
        }
        auto* data = static_cast<uint8*>(map.pData); // TODO: Data is not always uint8

        ImageUI8Desc imgDesc;
        imgDesc.width = desc.Width;
        imgDesc.height = desc.Height;
        // TODO: The image will always be UI8, this will not work correctly for other formats
        switch (channelCount) {
            case 1: imgDesc.format = SR_TEXTURE_R_UI8; break;
            case 2: imgDesc.format = SR_TEXTURE_RG_UI8; break;
            case 3: imgDesc.format = SR_TEXTURE_RGB_UI8; break;
            case 4: imgDesc.format = SR_TEXTURE_RGBA_UI8; break;
            default: SR_CORE_THROW("[D3D11ColorAttachment]: Invalid channel count %i", channelCount);
        }
        imgDesc.data = nullptr;
        auto img = createImage(imgDesc);

        auto imgData = reinterpret_cast<uint8*>(img->getData());
        size_t rowPitch = map.RowPitch / sizeof(uint8);
        // remove added padding data (if any), this is added by D3D11 to properly align the data
        for (uint32 y = 0; y < desc.Height; ++y) {
            for (uint32 x = 0; x < desc.Width; ++x) {
                for (uint32 c = 0; c < channelCount; ++c) {
                    size_t srcIndex = (y * rowPitch) + (x * channelCount) + c;
                    size_t destIndex = (y * desc.Width * channelCount) + (x * channelCount) + c;
                    imgData[destIndex] = data[srcIndex];
                }
            }
        }

        m_Context->Unmap(stagingTexture, 0);
        stagingTexture->Release();
        return std::move(img);
    }

    uint64 D3D11CubeMap::getIdentifier() const {
        return reinterpret_cast<uint64>(m_TextureView);
    }

}

#endif
