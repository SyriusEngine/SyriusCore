#include "../../../include/SyriusCore/Context/VertexDescription.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    VertexAttribute::VertexAttribute(const std::string &name, SR_SCALAR_TYPE type, uint8 size, uint8 elementCount):
    name(name),
    type(type),
    size(size),
    elementCount(elementCount) {
        SR_CORE_PRECONDITION(getScalarComponentCount(type) < 5, "Vertex attribute cannot have more than 4 components (%i)", getScalarComponentCount(type));
        SR_CORE_PRECONDITION(getScalarComponentCount(type) > 0, "Vertex attribute cannot have less than 1 component (%i)", getScalarComponentCount(type));
        SR_CORE_PRECONDITION(size > 0, "Vertex attribute size cannot be 0");
        SR_CORE_PRECONDITION(elementCount > 0, "Vertex attribute element count cannot be 0");
        SR_CORE_PRECONDITION(size % elementCount == 0, "Vertex attribute size must be divisible by element count (%i)", elementCount);
        SR_CORE_PRECONDITION(!name.empty(), "Vertex attribute name cannot be empty")

    }


    VertexDescription::VertexDescription()
    : m_Stride(0),
    m_AttributeCount(0){

    }

    VertexDescription::~VertexDescription() = default;

    void VertexDescription::addAttribute(const std::string &name, SR_SCALAR_TYPE dataType) {
        uint8 elementCount = getScalarComponentCount(dataType);
        uint8 size = getScalarSize(dataType);

        m_Attributes.emplace_back(name, dataType, size, elementCount);
        m_Stride += size;
        m_AttributeCount++;

    }

    uint32 VertexDescription::getStride() const {
        return m_Stride;
    }

    const std::vector<VertexAttribute> &VertexDescription::getAttributes() const {
        return m_Attributes;
    }

    uint32 VertexDescription::getAttributeCount() const {
        return m_AttributeCount;
    }

}