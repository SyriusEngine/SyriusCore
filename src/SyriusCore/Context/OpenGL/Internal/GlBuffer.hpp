#pragma once

#include "../GlUtils.hpp"

namespace Syrius {

    class GlBuffer {
    public:
        GlBuffer(const std::string& name, GLenum target, Size size, Size count, const void* data, SR_BUFFER_USAGE usage);

        ~GlBuffer();

        void release();

        void bind() const;

        void bindShaderResource(u32 slot) const;

        void setData(const void* data, Size size);

        void copyFrom(const GlBuffer& other);

        [[nodiscard]] UP<UByte[]> getData() const;

        [[nodiscard]] GLuint getBufferID() const;

        [[nodiscard]] Size getSize() const;

        [[nodiscard]] Size getCount() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] GLenum getTarget() const;

    private:
        const std::string m_Name;
        const Size m_Size = 0;
        const Size m_Count = 0;
        const SR_BUFFER_USAGE m_Usage = SR_BUFFER_USAGE_DEFAULT;
        GLuint m_BufferID = 0;
        GLenum m_Target = 0;

    };

}
