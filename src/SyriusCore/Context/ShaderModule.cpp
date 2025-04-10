#include "../../../include/SyriusCore/Context/ShaderModule.hpp"
#include "../Utils/DebugMacros.hpp"

#include <fstream>

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

    std::string ShaderModule::readFile(const fs::path &fileName) {
        checkFile(fileName);

        checkFile(fileName);

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
            SR_LOG_WARNING("ShaderModule", "Failed to open file: {}", fileName.string());
        }
        return code;
    }

    std::string ShaderModule::readFileBinary(const fs::path &fileName) {
        checkFile(fileName);

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
            SR_LOG_WARNING("ShaderModule", "Failed to open file: {}", fileName.string());
        }

        return byteCode;
    }

}