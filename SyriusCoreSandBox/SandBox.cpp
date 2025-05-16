#include "SandBox.hpp"

Sandbox::Sandbox(const std::string& iniFile, const std::vector<std::string_view> &args):
m_Config(iniFile){

}

Sandbox::~Sandbox() {
    m_Config.save();
}

void Sandbox::run() {

}



