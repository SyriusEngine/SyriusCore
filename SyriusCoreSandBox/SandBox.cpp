#include "SandBox.hpp"
#include "Utils.hpp"

#include "Components/Debug.hpp"

Sandbox::Sandbox(const std::string& iniFile, const std::vector<std::string_view> &args):
m_Config(iniFile){
    setupWindow();
    setupContext();
    setupImGui();
    setupFactories();

    // Loop over the requested components
    for (Size i = 2; i < args.size(); i++) {
        createComponent(args[i]);
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
        TimePoint currentTime = getTime();
        const Duration deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        for (const auto& component: m_Components){
            component->onUpdate(deltaTime);
        }

        // 3. Draw ImGui
        ImGuiWindowData data;
        m_Context->getDefaultFrameBuffer()->bind();
        m_Context->onImGuiBegin();
        for (const auto& component: m_Components){
            component->onImGui(data);
        }
        m_Context->onImGuiEnd();


        m_Context->swapBuffers();
    }
}

void Sandbox::processEvents() {
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

        for (const auto& component: m_Components) {
            component->onEvent(event);
        }
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
}

void Sandbox::setupImGui() {
    ImGuiDesc imGuiDesc;
    imGuiDesc.useDocking = m_Config["ImGui"]["ImGuiUseDocking"].getOrDefault<bool>(false);
    imGuiDesc.style = static_cast<SR_IMGUI_STYLE>(m_Config["ImGui"]["Style"].getOrDefault<u32>(SR_IMGUI_STYLE_DEFAULT));
    m_Context->initImGui(imGuiDesc);
}

void Sandbox::setupFactories() {
    addFactory<DebugComponentFactory>();
}


void Sandbox::createComponent(const std::string_view name) {
    for (const auto& [provides, factory]: m_ComponentFactories) {
        if (provides == name) {
            // Found a factory that provides the requested component
            // First check if that factory also needs components
            const auto& requires = factory->requires();
            for (const auto& require: requires) {
                if (m_ComponentsCreated.find(require) == m_ComponentsCreated.end()) {
                    // Required component not created yet, so create it
                    createComponent(require);
                }
            }

            // Create the component
            auto component = factory->createComponent(m_Window, m_Context);
            m_Components.push_back(component);
            m_ComponentsCreated.insert(provides);
        }
    }
}






