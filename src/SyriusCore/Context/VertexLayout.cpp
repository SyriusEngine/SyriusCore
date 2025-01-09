#include "../../../include/SyriusCore/Context/VertexLayout.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    VertexAttribute::VertexAttribute(const std::string &name, SR_SCALAR_TYPE type, u8 size, u8 elementCount):
    name(name),
    type(type),
    size(size),
    elementCount(elementCount) {
        SR_PRECONDITION(getScalarComponentCount(type) < 5, "Vertex attribute cannot have more than 4 components (%i)", getScalarComponentCount(type));
        SR_PRECONDITION(getScalarComponentCount(type) > 0, "Vertex attribute cannot have less than 1 component (%i)", getScalarComponentCount(type));
        SR_PRECONDITION(size > 0, "Vertex attribute size cannot be 0");
        SR_PRECONDITION(elementCount > 0, "Vertex attribute element count cannot be 0");
        SR_PRECONDITION(size % elementCount == 0, "Vertex attribute size must be divisible by element count (%i)", elementCount);
        SR_PRECONDITION(!name.empty(), "Vertex attribute name cannot be empty")

    }


    VertexLayout::VertexLayout(const UP<DeviceLimits>& deviceLimits):
    m_DeviceLimits(deviceLimits),
    m_Stride(0),
    m_AttributeCount(0){

    }

    VertexLayout::~VertexLayout() = default;

    void VertexLayout::addAttribute(const std::string &name, SR_SCALAR_TYPE dataType) {
        SR_PRECONDITION(m_AttributeCount < m_DeviceLimits->getMaxVertexAttributes(), "Vertex attribute count cannot exceed %i", m_DeviceLimits->getMaxVertexAttributes());
        SR_PRECONDITION(!hasAttribute(name), "Vertex attribute with name %s already exists", name.c_str());


        u8 elementCount = getScalarComponentCount(dataType);
        u8 size = getTypeSize(dataType);

        m_Attributes.emplace_back(name, dataType, size, elementCount);
        m_Stride += size;
        m_AttributeCount++;

    }

    bool VertexLayout::hasAttribute(const std::string &name) const {
        for(const auto& attribute : m_Attributes){
            if(attribute.name == name){
                return true;
            }
        }
        return false;
    }

    const VertexAttribute &VertexLayout::operator[](const std::string &name) const {
        SR_PRECONDITION(hasAttribute(name), "Vertex attribute with name %s does not exist", name.c_str());

        for (const auto &attribute : m_Attributes) {
            if (attribute.name == name) {
                return attribute;
            }
        }
        return m_Attributes[0];
    }

    VertexAttribute &VertexLayout::operator[](const std::string &name) {
        SR_PRECONDITION(hasAttribute(name), "Vertex attribute with name %s does not exist", name.c_str());

        for (auto &attribute : m_Attributes) {
            if (attribute.name == name) {
                return attribute;
            }
        }
        return m_Attributes[0];
    }

    const VertexAttribute &VertexLayout::operator[](u32 index) const {
        SR_PRECONDITION(index < m_AttributeCount, "Vertex attribute index %i is out of bounds", index);

        return m_Attributes[index];
    }

    VertexAttribute &VertexLayout::operator[](u32 index) {
        SR_PRECONDITION(index < m_AttributeCount, "Vertex attribute index %i is out of bounds", index);

        return m_Attributes[index];
    }

    u32 VertexLayout::getStride() const {
        return m_Stride;
    }

    const std::vector<VertexAttribute> &VertexLayout::getAttributes() const {
        return m_Attributes;
    }

    u32 VertexLayout::getAttributeCount() const {
        return m_AttributeCount;
    }

}