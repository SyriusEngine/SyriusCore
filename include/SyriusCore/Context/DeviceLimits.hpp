#pragma once

#include <unordered_map>
#include "Utils.hpp"

namespace Syrius{

    /**
     * @brief This class is used to get information about the selected graphics device.
     *        These values can be dependent on both the graphics API and the graphics device.
     */
    class SR_CORE_API DeviceLimits{
    public:
        DeviceLimits() = default;

        virtual ~DeviceLimits() = default;

        /**
         * @brief Returns the name of the graphics API which should normally be equivalent to the SR_API enum.
         * @return The name of the graphics API as a string
         */
        [[nodiscard]] virtual std::string getAPIVersion() const = 0;

        /**
         * @brief Returns the name of the graphics device. This is usually the name of the GPU.
         * @return The name of the graphics device as a string
         */
        [[nodiscard]] virtual std::string getDeviceName() const = 0;

        /**
         * @brief Returns the name of the graphics device vendor. This is usually the name of the GPU vendor (NVIDIA, AMD, Intel, etc.).
         * @return
         */
        [[nodiscard]] virtual std::string getDeviceVendor() const = 0;

        /**
         * @brief Returns the version of the shading language used by the graphics device (GLSL, HLSL, etc.).
         * @return
         */
        [[nodiscard]] virtual std::string getShadingLanguageVersion() const = 0;

        /**
         * @brief Retrieves the maximum width of a framebuffer in pixels.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 16384.
         * @return u32 containing the maximum width of a framebuffer in pixels
         */
        [[nodiscard]] virtual u32 getMaxFramebufferWidth() const = 0;

        /**
         * @brief Retrieves the maximum height of a framebuffer in pixels.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 16384.
         * @return u32 containing the maximum height of a framebuffer in pixels
         */
        [[nodiscard]] virtual u32 getMaxFramebufferHeight() const = 0;

        /**
         * @brief Retrieves the maximum number of color attachments that can be bound to a framebuffer.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 8.
         * @return u32 containing the maximum number of color attachments that can be bound to a framebuffer
         */
        [[nodiscard]] virtual u32 getMaxFramebufferColorAttachments() const = 0;

        /**
         * @brief Retrieves the maximum width of a viewport in pixels.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 16384.
         * @return u32 containing the maximum width of a viewport in pixels
         */
        [[nodiscard]] virtual u32 getMaxViewportWidth() const = 0;

        /**
         * @brief Retrieves the maximum height of a viewport in pixels.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 16384.
         * @return u32 containing the maximum height of a viewport in pixels
         */
        [[nodiscard]] virtual u32 getMaxViewportHeight() const = 0;

        /**
         * @brief Retrieves the maximum number of texture slots that can be bound to a shader.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 16.
         * @return u32 containing the maximum number of texture slots that can be bound to a shader
         */
        [[nodiscard]] virtual u32 getMaxTextureSlots() const = 0;

        /**
         * @brief Retrieves the maximum size of a texture in pixels.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 16384.
         * @return u32 containing the maximum size of a texture in pixels
         */
        [[nodiscard]] virtual u32 getMaxTextureSize() const = 0;

        /**
         * @brief Retrieves the maximum number of shader storage buffer slots that can be bound to a shader.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 8.
         * @return u32 containing the maximum number of shader storage buffer slots that can be bound to a shader
         */
        [[nodiscard]] virtual u32 getMaxSSSBOSlots() const = 0;

        /**
         * @brief Retrieves the maximum size of a constant buffer in bytes.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 65536.
         * @return u32 containing the maximum size of a constant buffer in bytes
         */
        [[nodiscard]] virtual u32 getMaxConstantBufferSize() const = 0;

        /**
         * @brief Retrieves the maximum number of constant buffer slots that can be bound to a shader.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 14.
         * @return u32 containing the maximum number of constant buffer slots that can be bound to a shader
         */
        [[nodiscard]] virtual u32 getMaxConstantBufferSlots() const = 0;

        /**
         * @brief Retrieves the maximum number of bits in the depth buffer. On current hardware, this value is nearly
         *        always 32.
         * @return u32 containing the maximum number of bits in the depth buffer
         */
        [[nodiscard]] virtual u32 getMaxDepthBufferBits() const = 0;

        /**
         * @brief Retrieves the maximum number of attributes that a single vertex can have.
         *        The supported graphics APIs by SyriusCore guarantee that this value is at least 16.
         * @return u32 containing the maximum number of vertex attributes.
         */
        [[nodiscard]] virtual u32 getMaxVertexAttributes() const = 0;

        /**
         * @brief Retrieves the maximum number of vertices that can be drawn in a single draw call. This value is equal
         *        to the number of vertices that can be stored in a single vertex buffer.
         * @return u64 containing the maximum number of vertices that can be drawn in a single draw call.
         */
        [[nodiscard]] virtual u64 getMaxVertexCount() const = 0; // maximum number of vertices for a draw call

        /**
         * @brief Retrieves the maximum number of indices that can be drawn in a single draw call. This value is equal
         *       to the number of indices that can be stored in a single index buffer.
         * @return u64 containing the maximum number of indices that can be drawn in a single draw call.
         */
        [[nodiscard]] virtual u64 getMaxIndexCount() const = 0; // maximum number of indices for a draw call

        /**
         * @brief The maximum index value that can be used in a draw call. This value can be different from the maximum
         *        number of indices that can be drawn in a single draw call.
         */
        [[nodiscard]] virtual u64 getMaxIndex() const = 0; // largest index value for a draw call

        /**
         * @brief Returns whether or not the graphics device supports double buffering.
         * @return true if the graphics device supports double buffering, false otherwise
         */
        [[nodiscard]] virtual bool isDoubleBuffered() const = 0;

        /**
         * @brief Returns whether or not the graphics device supports the specified texture format.
         * @param format The SR_TEXTURE_FORMAT format to check for support
         */
         [[nodiscard]] virtual bool texture2DFormatSupported(SR_TEXTURE_FORMAT format) = 0;

    protected:
        std::unordered_map<SR_TEXTURE_FORMAT, bool> m_SupportedTexture2DFormats;

    };

}