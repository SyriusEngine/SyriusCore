#include "SyriusCoreDev.hpp"


SyriusCoreDev::SyriusCoreDev(const std::string &iniFile):
m_Config(iniFile){
    WindowDesc wDesc;
    wDesc.title = m_Config["Window"]["Title"].getOrDefault("SyriusCoreDev");
    wDesc.width = m_Config["Window"]["Width"].getOrDefault<uint32>(1280);
    wDesc.height = m_Config["Window"]["Height"].getOrDefault<uint32>(720);

    m_Window = createWindow(wDesc);

    ContextDesc cDesc;
    cDesc.api = static_cast<SR_SUPPORTED_API>(m_Config["Context"]["API"].getOrDefault<uint32>(SR_API_OPENGL));
    cDesc.enableDepthTest = m_Config["Context"]["EnableDepthTest"].getOrDefault<bool>(true);

    m_Context = m_Window->createContext(cDesc);
    auto vsync = m_Config["Context"]["VerticalSync"].getOrDefault<bool>(true);
    if (vsync){
        m_Context->setVerticalSynchronisation(true);
    }

    if (m_Config["Context"]["PrintContextInfo"].getOrDefault<bool>(true)){
        printContextInfo(m_Context);
    }
}

SyriusCoreDev::~SyriusCoreDev() {
    for (auto& layer: m_Layers){
        layer->onDetach();
    }
    m_Config.save();
}

void SyriusCoreDev::run() {
    while (m_Window->isOpen()){
       processEvents();

       for (const auto& layer: m_Layers){
           layer->onUpdate();
       }

        m_Context->swapBuffers();
    }
}

void SyriusCoreDev::processEvents() {
    m_Window->pollEvents();
    while (m_Window->hasEvent()){
        auto event = m_Window->getEvent();
        if (m_Config["Window"]["PrintEventInfo"].getOrDefault<bool>(false)){
            printEventInfo(event);
        }
        if (event.type == SR_EVENT_WINDOW_CLOSED){
            m_Window->close();
        }

        for (const auto& layer: m_Layers) {
            layer->onEvent(event);
        }
    }
}
