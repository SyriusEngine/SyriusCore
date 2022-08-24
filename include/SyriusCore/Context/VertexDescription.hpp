#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_API VertexAttribute{
        std::string m_Name;
        SR_SCALAR_TYPE m_Type;
        uint8 m_Size;
        uint8 m_ElementCount;

        VertexAttribute(const std::string& name, SR_SCALAR_TYPE type, uint8 size, uint8 elementCount);
    };

    class SR_API VertexDescription{
    public:
        VertexDescription();

        ~VertexDescription();

        void addAttribute(const std::string& name, SR_SCALAR_TYPE dataType);

        [[nodiscard]] uint32 getStride() const;

        [[nodiscard]] const std::vector<VertexAttribute>& getAttributes() const;

    public:
        uint32 m_Stride;
        std::vector<VertexAttribute> m_Attributes;

    };
}

