#include "D3D11Texture2D.hpp"
#include "D3D11ColorAttachment.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11Texture2D::D3D11Texture2D(const Texture2DDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    Texture2D(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        if (!m_DeviceLimits->texture2DFormatSupported(desc.format)) {
            SR_LOG_ERROR("D3D11Texture2D", "Supplied texture format ({}) is not supported by the device", desc.format);
            return;
        }
        createResources(desc.data);
    }

    D3D11Texture2D::D3D11Texture2D(const Texture2DImageDesc &desc, const UP<DeviceLimits>& deviceLimits, ID3D11Device *device, ID3D11DeviceContext *context):
    Texture2D(desc, deviceLimits),
    m_Device(device),
    m_Context(context),
    m_Texture(nullptr),
    m_TextureView(nullptr){
        if (!m_DeviceLimits->texture2DFormatSupported(desc.image->getFormat())) {
            SR_LOG_ERROR("D3D11Texture2D", "Supplied texture format ({}) is not supported by the device", desc.image->getFormat());
            return;
        }
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

    void D3D11Texture2D::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot ({}) is greater than the device number of texture slots ({})", slot, m_DeviceLimits->getMaxTextureSlots());

        m_Context->PSSetShaderResources(slot, 1, &m_TextureView);
    }


    void D3D11Texture2D::setData(const void *data, u32 x, u32 y, u32 width, u32 height) {
        SR_PRECONDITION(data != nullptr, "[Texture2D]: Data is nullptr ({})", data)
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[Texture2D]: Update on texture object requested, which has been created with SR_BUFFER_USAGE_STATIC flag!");
        SR_PRECONDITION(x + width <= m_Width, "[Texture2D]: Width ({}) exceeds the texture width ({})", width, m_Width);
        SR_PRECONDITION(y + height <= m_Height, "[Texture2D]: Height ({}) exceeds the texture height ({})", height, m_Height);

        // TODO: Test this code because dont know if it works
        auto channelCount = getTextureChannelCount(m_Format);
        auto bytesPerPixel = channelCount * sizeof(UByte);
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

    void D3D11Texture2D::copyFrom(const ResourceView<Texture2D> &other) {
        SR_PRECONDITION(m_Width == other->getWidth(), "[D3D11Texture2D]: Width of the source texture ({}) does not match the width of the destination texture ({})", other->getWidth(), m_Width);
        SR_PRECONDITION(m_Height == other->getHeight(), "[D3D11Texture2D]: Height of the source texture ({}) does not match the height of the destination texture ({})", other->getHeight(), m_Height);
        SR_PRECONDITION(m_Format == other->getFormat(), "[D3D11Texture2D]: Format of the source texture ({}) does not match the format of the destination texture ({})", other->getFormat(), m_Format);
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[D3D11Texture2D]: Copy on texture object requested, which has been created with SR_BUFFER_USAGE_STATIC flag!");

        auto srcBuffer = dynamic_cast<D3D11Texture2D*>(other.get());
        SR_ASSERT(srcBuffer, "[D3D11Texture2D]: Copy from buffer object requested, which is invalid!");
        m_Context->CopyResource(m_Texture, srcBuffer->getTexture());
    }

    void D3D11Texture2D::copyFrom(const ResourceView<ColorAttachment> &other) {
        SR_PRECONDITION(m_Width == other->getWidth(), "[D3D11Texture2D]: Width of the source texture ({}) does not match the width of the destination texture ({})", other->getWidth(), m_Width);
        SR_PRECONDITION(m_Height == other->getHeight(), "[D3D11Texture2D]: Height of the source texture ({}) does not match the height of the destination texture ({})", other->getHeight(), m_Height);
        SR_PRECONDITION(m_Format == other->getFormat(), "[D3D11Texture2D]: Format of the source texture ({}) does not match the format of the destination texture ({})", other->getFormat(), m_Format);
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[D3D11Texture2D]: Copy on texture object requested, which has been created with SR_BUFFER_USAGE_STATIC flag!");

        auto srcBuffer = dynamic_cast<D3D11ColorAttachment*>(other.get());
        SR_ASSERT(srcBuffer, "[D3D11Texture2D]: Copy from buffer object requested, which is invalid!");
        m_Context->CopyResource(m_Texture, srcBuffer->getTexture());
    }

    UP<Image> D3D11Texture2D::getData() {
        // we use a staging texture to copy the data back to the CPU as a texture cannot be mapped
        auto channelCount = getTextureChannelCount(m_Format);

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

        if (!map.pData) {
            SR_LOG_WARNING("D3D11Texture2D", "Failed to map staging texture");
            stagingTexture->Release();
            return nullptr;
        }

        auto* data = static_cast<u8*>(map.pData); // TODO: Data is not always u8

        ImageUI8Desc imgDesc;
        imgDesc.width = desc.Width;
        imgDesc.height = desc.Height;
        // TODO: The image will always be UI8, this will not work correctly for other formats
        switch (channelCount) {
            case 1: imgDesc.format = SR_TEXTURE_R_UI8; break;
            case 2: imgDesc.format = SR_TEXTURE_RG_UI8; break;
            case 3: imgDesc.format = SR_TEXTURE_RGB_UI8; break;
            case 4: imgDesc.format = SR_TEXTURE_RGBA_UI8; break;
            default: SR_LOG_THROW("D3D11ColorAttachment", "Invalid channel count {}", channelCount);
        }
        imgDesc.data = nullptr;
        auto img = createImage(imgDesc);

        auto imgData = reinterpret_cast<u8*>(img->getData());
        size_t rowPitch = map.RowPitch / sizeof(u8);
        // remove added padding data (if any), this is added by D3D11 to properly align the data
        for (u32 y = 0; y < desc.Height; ++y) {
            for (u32 x = 0; x < desc.Width; ++x) {
                for (u32 c = 0; c < channelCount; ++c) {
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

    u64 D3D11Texture2D::getIdentifier() const {
        return reinterpret_cast<u64>(m_TextureView);
    }

    void D3D11Texture2D::createResources(const void *data) {
        if (m_Usage == SR_BUFFER_USAGE_DYNAMIC){
            SR_LOG_WARNING("D3D11Texture2D", "Dynamic textures with mipmapping are not supported in D3D11. Changing usage to default");
            m_Usage = SR_BUFFER_USAGE_DEFAULT;
        }
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
            SR_LOG_WARNING("D3D11Texture", "Supplied texture format has 3 channels which can behave weird D3D11");
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
