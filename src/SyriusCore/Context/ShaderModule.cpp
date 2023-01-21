#include "../../../include/SyriusCore/Context/ShaderModule.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    ShaderModule::ShaderModule(const ShaderModuleDesc& desc)
    : m_ShaderType(desc.shaderType),
    m_EntryPoint(desc.entryPoint){

    }

    ShaderModule::~ShaderModule() {

    }

    SR_SHADER_TYPE ShaderModule::getShaderModuleType() const {
        return m_ShaderType;
    }

    std::string ShaderModule::readFile(const std::string &fileName) {
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
            SR_CORE_WARNING("Failed to open file: " + fileName);
        }
        return code;
    }

    std::string ShaderModule::readFileBinary(const std::string &fileName) {
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
            SR_CORE_WARNING("Failed to open file: " + fileName);
        }

        return byteCode;
    }

    const std::string &ShaderModule::getEntryPoint() const {
        return m_EntryPoint;
    }
}