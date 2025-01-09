#include "../../../include/SyriusCore/Context/ShaderModule.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ShaderModule::ShaderModule(const ShaderModuleDesc& desc):
    m_ShaderType(desc.shaderType),
    m_EntryPoint(desc.entryPoint){

    }

    ShaderModule::ShaderModule(const ShaderModuleFileDesc &desc):
    m_ShaderType(desc.shaderType),
    m_EntryPoint(desc.entryPoint){

    }

    ShaderModule::~ShaderModule() = default;

    SR_SHADER_TYPE ShaderModule::getShaderModuleType() const {
        return m_ShaderType;
    }

    const std::string &ShaderModule::getEntryPoint() const {
        return m_EntryPoint;
    }

    std::string ShaderModule::readFile(const std::string &fileName) {
        SR_LOG_THROW_IF_FALSE(std::filesystem::exists(fileName), "ShaderModule", "File does not exist: %s", fileName.c_str());

        std::string code;
        std::string line;
        std::ifstream file(fileName);
        if (file.is_open()){
            while (std::getline(file, line)) {
                if (line.empty()) {
                    continue;
                }
                else{
                    code += line + "\n";
                }
            }
        }
        else{
            SR_LOG_WARNING("ShaderModule", "Failed to open file: %s", fileName.c_str());
        }
        return code;
    }

    std::string ShaderModule::readFileBinary(const std::string &fileName) {
        SR_LOG_THROW_IF_FALSE(std::filesystem::exists(fileName), "ShaderModule", "File does not exist: %s", fileName.c_str());

        std::string byteCode;
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);
        if (file.is_open()){
            size_t fileSize = (size_t)file.tellg();
            byteCode.resize(fileSize);
            file.seekg(0);
            file.read(byteCode.data(), fileSize);
            file.close();
        }
        else{
            SR_LOG_WARNING("ShaderModule", "Failed to open file: %s", fileName.c_str());
        }

        return byteCode;
    }

}