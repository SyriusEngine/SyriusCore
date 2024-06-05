#include "D3D11Texture2D.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    D3D11Texture2D::D3D11Texture2D(const Texture2DDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    Texture2D(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        createResources(desc.data);
    }

    D3D11Texture2D::D3D11Texture2D(const Texture2DImageDesc &desc, const Resource<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    Texture2D(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        createResources(desc.image->getData());
    }

    D3D11Texture2D::~D3D11Texture2D() {
        if (m_TextureView) {
            m_TextureView->Release();
        }
        if (m_Texture) {
            m_Texture->Release();
        }

    }

    void D3D11Texture2D::bind() {
        // D3D11 doesn't have a bind function
    }

    void D3D11Texture2D::bindShaderResource(uint32_t slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
    }


    void D3D11Texture2D::setData(const void *data, uint32 x, uint32 y, uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(data != nullptr, "[Texture2D]: Data is nullptr (%p)", data)
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[Texture2D]: Update on texture object (%p) requested, which has been created with SR_BUFFER_USAGE_STATIC flag!", this);
        SR_CORE_PRECONDITION(x + width <= m_Width, "[Texture2D]: Width (%i) exceeds the texture width (%i)", width, m_Width);
        SR_CORE_PRECONDITION(y + height <= m_Height, "[Texture2D]: Height (%i) exceeds the texture height (%i)", height, m_Height);

        // TODO: Test this code because dont know if it works
        auto channelCount = getTextureChannelCount(m_Format);
        auto bytesPerPixel = channelCount * sizeof(ubyte);
        D3D11_BOX box = { 0 };
        box.left = x;
        box.right = x + width;
        box.top = y;
        box.bottom = y + height;
        box.front = 0;
        box.back = 1;

        m_Context->UpdateSubresource(m_Texture, 0, &box, data, width * bytesPerPixel, 0);
        m_Context->GenerateMips(m_TextureView);
    }

    Resource<Image> D3D11Texture2D::getData() {
        // we use a staging texture to copy the data back to the CPU as a texture cannot be mapped
        ID3D11Texture2D* stagingTexture = nullptr;
        D3D11_TEXTURE2D_DESC desc = { 0 };
        m_Texture->GetDesc(&desc);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&desc, nullptr, &stagingTexture));

        m_Context->CopyResource(stagingTexture, m_Texture);

        D3D11_MAPPED_SUBRESOURCE map = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &map));
        if (map.pData == nullptr){
            SR_CORE_THROW("[D3D11Texture2D]: Failed to map staging texture (%p)", stagingTexture);
        }

        ImageUI8Desc imgDesc;
        imgDesc.width = desc.Width;
        imgDesc.height = desc.Height;
        imgDesc.format = m_Format;
        imgDesc.data = static_cast<ubyte*>(map.pData);
        auto img = createImage(imgDesc);
        m_Context->Unmap(stagingTexture, 0);
        stagingTexture->Release();
        return std::move(img);
    }

    uint64 D3D11Texture2D::getIdentifier() const {
        return reinterpret_cast<uint64>(m_TextureView);
    }

    void D3D11Texture2D::createResources(const void *data) {
        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 0; // Enable auto mipmap generation
        textureDesc.ArraySize = 1;
        textureDesc.Format = getD3d11TextureFormat(m_Format);
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        textureDesc.Usage = getD3d11BufferType(m_Usage);
        if (m_Usage != SR_BUFFER_USAGE_STATIC){
            textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            textureDesc.CPUAccessFlags = 0;
        }
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        auto dataType = getTextureDataType(m_Format);
        auto channelCount = getTextureChannelCount(m_Format);

        if (channelCount == 3){
            SR_CORE_WARNING("Supplied texture format has 3 channels which can behave weird D3D11");
        }

        SR_CORE_D3D11_CALL(m_Device->CreateTexture2D(&textureDesc, nullptr, &m_Texture));

        if (data != nullptr){
            auto memPitch = m_Width * (getTypeSize(dataType) * channelCount);
            /*
             * When enabling MIPmapping in D3D11, the initial data for the texture should be the texture itself
             * and all other mip levels. But we want the GPU to generate them for us. The CreateTexture2D function
             * takes in a pointer to a D3D11_SUBRESOURCE_DATA structure which can be used to specify the initial data
             * but when specifying mip levels, the initial data is an array. Thus supply the first mip level and let
             * the GPU generate the rest. This can be done by UpdateSubresource function.
             */
            m_Context->UpdateSubresource(m_Texture, 0, nullptr, data, memPitch, 0);
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {  };
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureViewDesc.Texture2D.MipLevels = -1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        SR_CORE_D3D11_CALL(m_Device->CreateShaderResourceView(m_Texture, &textureViewDesc, &m_TextureView));

        m_Context->GenerateMips(m_TextureView);
    }
}

#endif
