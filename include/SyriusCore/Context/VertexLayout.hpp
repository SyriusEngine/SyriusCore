#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"
#include <vector>

namespace Syrius{

    struct SR_CORE_API VertexAttribute{
        std::string name;
        SR_SCALAR_TYPE type;
        u8 size;
        u8 elementCount;

        VertexAttribute(const std::string& name, SR_SCALAR_TYPE type, u8 size, u8 elementCount);
    };

    class SR_CORE_API VertexLayout{
    public:
        explicit VertexLayout(const UP<DeviceLimits>& deviceLimits);

        ~VertexLayout();

        void addAttribute(const std::string& name, SR_SCALAR_TYPE dataType);

        bool hasAttribute(const std::string& name) const;

        const VertexAttribute& operator[](const std::string& name) const;

        VertexAttribute& operator[](const std::string& name);

        const VertexAttribute& operator[](u32 index) const;

        VertexAttribute& operator[](u32 index);

        [[nodiscard]] u32 getStride() const;

        [[nodiscard]] const std::vector<VertexAttribute>& getAttributes() const;

        [[nodiscard]] u32 getAttributeCount() const;

    private:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Stride;
        u32 m_AttributeCount;
        std::vector<VertexAttribute> m_Attributes;

    };
}

