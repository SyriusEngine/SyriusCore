#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API VertexAttribute{
        std::string name;
        SR_SCALAR_TYPE type;
        uint8 size;
        uint8 elementCount;

        VertexAttribute(const std::string& name, SR_SCALAR_TYPE type, uint8 size, uint8 elementCount);
    };

    class SR_CORE_API VertexLayout{
    public:
        explicit VertexLayout(const Resource<DeviceLimits>& deviceLimits);

        ~VertexLayout();

        void addAttribute(const std::string& name, SR_SCALAR_TYPE dataType);

        bool hasAttribute(const std::string& name) const;

        const VertexAttribute& operator[](const std::string& name) const;

        VertexAttribute& operator[](const std::string& name);

        const VertexAttribute& operator[](uint32 index) const;

        VertexAttribute& operator[](uint32 index);

        [[nodiscard]] uint32 getStride() const;

        [[nodiscard]] const std::vector<VertexAttribute>& getAttributes() const;

        [[nodiscard]] uint32 getAttributeCount() const;

    private:
        const Resource<DeviceLimits>& m_DeviceLimits;

        uint32 m_Stride;
        uint32 m_AttributeCount;
        std::vector<VertexAttribute> m_Attributes;

    };
}

