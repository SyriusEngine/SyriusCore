#include "SyriusCoreDev.hpp"


SyriusCoreDev::SyriusCoreDev(const std::string &iniFile, const std::string& windowTitle):
m_Config(iniFile){
    ContextDesc cDesc;
    cDesc.api = static_cast<SR_SUPPORTED_API>(m_Config["Context"]["API"].getOrDefault<uint32>(SR_API_OPENGL));
    cDesc.enableDepthTest = m_Config["Context"]["EnableDepthTest"].getOrDefault<bool>(false);

    std::string windowName = windowTitle;
    if (cDesc.api == SR_API_OPENGL){
        windowName += " (OpenGL)";
    }
    else if (cDesc.api == SR_API_VULKAN){
        windowName += " (Vulkan)";
    }
    else if (cDesc.api == SR_API_D3D11){
        windowName += " (DirectX 11)";
    }
    WindowDesc wDesc;
    wDesc.title = windowName;
    wDesc.width = m_Config["Window"]["Width"].getOrDefault<uint32>(1280);
    wDesc.height = m_Config["Window"]["Height"].getOrDefault<uint32>(720);

    m_Window = createWindow(wDesc);

    m_Context = m_Window->createContext(cDesc);
    auto vsync = m_Config["Context"]["VerticalSync"].getOrDefault<bool>(true);
    if (vsync){
        m_Context->setVerticalSynchronisation(true);
    }
    printContextInfo(m_Context);
}

SyriusCoreDev::~SyriusCoreDev() {
    m_Layers.clear();
    m_Config.save();
    m_Window->destroyContext();
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
        switch (event.type) {
            case SR_EVENT_WINDOW_CLOSED:
                m_Window->close();
                break;
            case SR_EVENT_WINDOW_RESIZED:
                m_Context->onResize(event.windowWidth, event.windowHeight);
                break;
            default:
                break;
        }
        for (const auto& layer: m_Layers) {
            layer->onEvent(event);
        }
    }
}
