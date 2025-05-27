#include "D3D11Buffer.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    D3D11Buffer::D3D11Buffer(const std::string &name, const UINT bindFlags, const UINT miscFlags, const Size size, const Size count,
        const void *data, const SR_BUFFER_USAGE usage, ID3D11Device *device, ID3D11DeviceContext *context):
    m_Device(device),
    m_Context(context),
    m_Name(name),
    m_Size(size),
    m_Count(count),
    m_Usage(usage),
    m_BindFlags(bindFlags){
        SR_PRECONDITION(size > 0, "Size is 0");
        SR_PRECONDITION(count > 0, "Count is 0");
        SR_PRECONDITION(device != nullptr, "Device is null")
        SR_PRECONDITION(context != nullptr, "Context is null")

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_Size;
        bufferDesc.Usage = getD3d11BufferType(m_Usage);
        bufferDesc.BindFlags = m_BindFlags;
        if (m_Usage == SR_BUFFER_USAGE_DYNAMIC){
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else{
            bufferDesc.CPUAccessFlags = 0;
        }
        bufferDesc.MiscFlags = miscFlags;
        bufferDesc.StructureByteStride = m_Size / m_Count;

        UP<D3D11_SUBRESOURCE_DATA> subResourceData = nullptr;
        if (data != nullptr) {
            subResourceData = createUP<D3D11_SUBRESOURCE_DATA>();
            subResourceData->pSysMem = data;
            subResourceData->SysMemPitch = 0;
            subResourceData->SysMemSlicePitch = 0;
        }
        SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&bufferDesc, subResourceData.get(), &m_Buffer));

        SR_POSTCONDITION(m_Buffer != nullptr, "[{}]: Buffer creation failed!", m_Name);
    }

    D3D11Buffer::~D3D11Buffer() {
        release();
    }

    void D3D11Buffer::release() const {
        if (m_Buffer) {
            m_Buffer->Release();
        }
    }

    void D3D11Buffer::setData(const void *data, const Size size) const {
        SR_PRECONDITION(m_Buffer != nullptr, "[{}]: Buffer creation failed!", m_Name);
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[{}]: Update on buffer object requested, which was created with {} usage flag!", m_Name, m_Usage);

        // 1. Check Sizes
        if (size > m_Size) {
            SR_LOG_WARNING(m_Name, "Data size {} exceeds the current buffer size {}. Data will be partially written!", size, m_Size);
        }
        const Size copySize = std::min(m_Size, size);

        // 2. Map the buffer
        D3D11_MAPPED_SUBRESOURCE map  = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map));
        if (!map.pData) {
            SR_LOG_WARNING("D3D11VertexBuffer", "Failed to map buffer");
            return;
        }

        // 3. Copy data
        memcpy(map.pData, data, copySize);

        // 4. Unmap the buffer
        m_Context->Unmap(m_Buffer, 0);
    }

    void D3D11Buffer::copyFrom(const D3D11Buffer &other) const {
        SR_PRECONDITION(m_Buffer != nullptr, "[{}]: Buffer creation failed!", m_Name);
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[{}]: Update on buffer object requested, which was created with {} usage flag!", m_Name, m_Usage);

        if (m_Size < other.m_Size) {
            SR_LOG_WARNING(m_Name, "Copy from buffer object requested, which exceeds the current buffer size ({} > {}). No data will be copied!", other.m_Size, m_Size);
            return;
        }
        m_Context->CopyResource(m_Buffer, other.m_Buffer);
    }

    UP<UByte[]> D3D11Buffer::getData() const {
        SR_PRECONDITION(m_Buffer != nullptr, "[{}]: Buffer creation failed!", m_Name);

        auto data = createUP<UByte[]>(m_Size);

        /*
        * Reading from a ID3D11Buffer is not directly supported.
        * The only way to read from a buffer is to copy the data to a staging buffer and then read from the staging buffer.
        */
        ID3D11Buffer* stagingBuffer = nullptr;
        D3D11_BUFFER_DESC desc = { 0 };
        m_Buffer->GetDesc(&desc);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;
        SR_CORE_D3D11_CALL(m_Device->CreateBuffer(&desc, nullptr, &stagingBuffer));

        // Copy the contents of our buffer to the staging buffer
        m_Context->CopyResource(stagingBuffer, m_Buffer);

        // and copy the data
        D3D11_MAPPED_SUBRESOURCE map = { nullptr };
        SR_CORE_D3D11_CALL(m_Context->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &map));

        if (!map.pData) {
            SR_LOG_WARNING(m_Name, "Failed to map buffer");
            return std::move(data);
        }

        memcpy(data.get(), map.pData, m_Size);
        m_Context->Unmap(stagingBuffer, 0);

        // Release the staging buffer
        stagingBuffer->Release();

        return std::move(data);
    }

    ID3D11Buffer *D3D11Buffer::getBufferID() const {
        return m_Buffer;
    }

    Size D3D11Buffer::getSize() const {
        return m_Size;
    }

    Size D3D11Buffer::getCount() const {
        return m_Count;
    }

    SR_BUFFER_USAGE D3D11Buffer::getUsage() const {
        return m_Usage;
    }

    const std::string &D3D11Buffer::getName() const {
        return m_Name;
    }

    UINT D3D11Buffer::getBindFlags() const {
        return m_BindFlags;
    }
}

#endif
