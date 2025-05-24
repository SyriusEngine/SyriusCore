#include "SandBox.hpp"
#include "Utils.hpp"

#include "ComponentLayers/Debug.hpp"
#include "ComponentLayers/VertexDraw.hpp"

void AppMessageHandler(const Message& msg) {
    if (msg.message.find("131185") == std::string::npos) {
        fmt::print("[{}:{}:{}] [{}] [{}]: {}\n",
            msg.file, msg.function, msg.line,
            msg.source, msg.severity, msg.message);
    }
}

Sandbox::Sandbox(const std::string& iniFile, const std::vector<std::string_view> &args):
m_Config(iniFile){
    setupWindow();
    setupContext();
    setupImGui();

    m_ComponentContainer = createUP<ComponentContainer>(m_Window, m_Context);

    // Loop over the requested components
    for (Size i = 2; i < args.size(); i++) {
        m_ComponentContainer->createComponent(args[i]);
    }
}

Sandbox::~Sandbox() {
    m_Context->terminateImGui();
    m_Config.save();
}

void Sandbox::run() {
    TimePoint lastFrameTime = getTime();
    while (m_Window->isOpen()){
        // 1. Process Events
        processEvents();

        // 2. Update components
        m_Context->clear();
        m_Context->beginRenderPass();
        TimePoint currentTime = getTime();
        const Duration deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        m_ComponentContainer->onUpdate(deltaTime);
        m_Context->endRenderPass();

        // 3. Draw ImGui
        m_Context->beginRenderPass();
        m_Context->onImGuiBegin();
        m_ComponentContainer->onImGui();
        m_Context->onImGuiEnd();
        m_Context->endRenderPass();

        // 4. Present final frame
        m_Context->swapBuffers();
    }
}

void Sandbox::processEvents() const {
    m_Window->pollEvents();
    while (m_Window->hasEvent()){
        auto event = m_Window->getNextEvent();
        if (m_PrintEventInfo){
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

        m_ComponentContainer->onEvent(event);
    }
}

void Sandbox::setupWindow() {
    WindowDesc windowDesc;
    windowDesc.width = m_Config["Window"]["Width"].getOrDefault<u32>(1280);
    windowDesc.height = m_Config["Window"]["Height"].getOrDefault<u32>(720);
    windowDesc.title = "SyriusCore Sandbox";
    m_PrintEventInfo = m_Config["Window"]["PrintEventInfo"].getOrDefault<bool>(false);
    // windowDesc.fullscreen = m_Config["Window"]["Fullscreen"].getOrDefault<bool>(false);
    m_Window = createWindow(windowDesc);
}

void Sandbox::setupContext() {
    ContextDesc contextDesc;
    contextDesc.api = static_cast<SR_SUPPORTED_API>(m_Config["Context"]["API"].getOrDefault<u32>(SR_API_OPENGL));
    contextDesc.enableDepthTest = m_Config["Context"]["EnableDepthTest"].getOrDefault<bool>(false);
    const bool vsync = m_Config["Context"]["VerticalSync"].getOrDefault<bool>(true);
    const bool logContextInfo = m_Config["Context"]["PrintContextInfo"].getOrDefault<bool>(false);
    m_Context = m_Window->createContext(contextDesc);
    if (vsync){
        m_Context->setVerticalSynchronisation(true);
    }
    if (logContextInfo){
        printContextInfo(m_Context);
    }

    const bool ignore131185 = m_Config["Context"]["Ignore131185"].getOrDefault<bool>(false);
    if (ignore131185) {
        Logger::setDebugCallback(AppMessageHandler);
    }
}

void Sandbox::setupImGui() {
    ImGuiDesc imGuiDesc;
    imGuiDesc.useDocking = m_Config["ImGui"]["ImGuiUseDocking"].getOrDefault<bool>(false);
    imGuiDesc.style = static_cast<SR_IMGUI_STYLE>(m_Config["ImGui"]["Style"].getOrDefault<u32>(SR_IMGUI_STYLE_DEFAULT));
    m_Context->initImGui(imGuiDesc);
}