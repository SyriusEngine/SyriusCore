#pragma once

#include <array>
#include "Utils.hpp"
#include "../Image/Image.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    class SR_CORE_API CubeMapLayout{
    public:
        CubeMapLayout() = delete;

        CubeMapLayout(u32 width, u32 height, SR_TEXTURE_FORMAT format);

        ~CubeMapLayout();

        void addFace(SR_CUBEMAP_FACE faceID, UP<Image>& image);

        void addFaceFromImage(SR_CUBEMAP_FACE faceID, const ImageFileDesc& desc);

        void addFaceFromFile(SR_CUBEMAP_FACE faceID, const fs::path& file);

        [[nodiscard]] u32 getWidth() const;

        [[nodiscard]] u32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

        [[nodiscard]] const UP<Image>& getFace(SR_CUBEMAP_FACE faceID) const;

        [[nodiscard]] const std::array<UP<Image>, 6>& getFaces() const;

    protected:
        const u32 m_Width;
        const u32 m_Height;
        const SR_TEXTURE_FORMAT m_Format;

        std::array<UP<Image>, 6> m_Faces;
    };
}
