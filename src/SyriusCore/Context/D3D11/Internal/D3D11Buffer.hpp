#pragma once

#include "../D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius {

    class D3D11Buffer {
    public:
        D3D11Buffer(const std::string& name, UINT bindFlags, Size size, const void* data, SR_BUFFER_USAGE usage,
            ID3D11Device *device, ID3D11DeviceContext *context);

        ~D3D11Buffer();

        void release() const;

        void setData(const void* data, Size size) const;

        void copyFrom(const D3D11Buffer& other) const;

        [[nodiscard]] UP<UByte[]> getData() const;

        [[nodiscard]] ID3D11Buffer* getBufferID() const;

        [[nodiscard]] Size getSize() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] UINT getBindFlags() const;

    private:
        ID3D11Device* m_Device = nullptr;
        ID3D11DeviceContext* m_Context = nullptr;
        ID3D11Buffer* m_Buffer = nullptr;

        const std::string m_Name;
        const Size m_Size = 0;
        const SR_BUFFER_USAGE m_Usage = SR_BUFFER_USAGE_DEFAULT;
        const UINT m_BindFlags;
    };

}

#endif