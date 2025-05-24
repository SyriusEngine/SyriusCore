#include "SamplerComponent.hpp"

SamplerComponent::SamplerComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);
}

void SamplerComponent::onUpdate(Duration deltaTime) {
    m_Sampler->bindShaderResource(0);
}

void SamplerComponent::onImGui(ImGuiWindowData &windowData) {
    imGuiBegin(windowData, "Sampler");

    static std::vector<std::string> filterModes = {"SR_TEXTURE_FILTER_LINEAR", "SR_TEXTURE_FILTER_POINT"};
    static int minFilterIndex = 0;
    static int magFilterIndex = 0;
    static std::vector<std::string> wrapModes = {"SR_TEXTURE_WRAP_REPEAT", "SR_TEXTURE_WRAP_MIRROR_REPEAT",
                                                 "SR_TEXTURE_WRAP_CLAMP_EDGE", "SR_TEXTURE_WRAP_CLAMP_BORDER"};
    static int wrapUModeIndex = 0;
    static int wrapVModeIndex = 0;
    static int wrapWModeIndex = 0;
    ImGui::Text("Min Filter Mode: %s", filterToString(m_Sampler->getMinFilter()).c_str());
    ImGui::Text("Mag Filter Mode: %s", filterToString(m_Sampler->getMagFilter()).c_str());
    ImGui::Text("Wrap U Mode: %s", wrapModeToString(m_Sampler->getWrapU()).c_str());
    ImGui::Text("Wrap V Mode: %s", wrapModeToString(m_Sampler->getWrapV()).c_str());
    ImGui::Text("Wrap W Mode: %s", wrapModeToString(m_Sampler->getWrapW()).c_str());
    if (ImGui::BeginCombo("Min Filter", filterModes[minFilterIndex].c_str())){
        for(int i = 0; i < filterModes.size(); i++){
            bool isSelected = (minFilterIndex == i);
            if (ImGui::Selectable(filterModes[i].c_str(), isSelected)){
                minFilterIndex = i;
                m_Sampler->setFilter(static_cast<SR_TEXTURE_FILTER>(minFilterIndex + 1), m_Sampler->getMagFilter());
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Mag Filter", filterModes[magFilterIndex].c_str())){
        for(int i = 0; i < filterModes.size(); i++){
            bool isSelected = (magFilterIndex == i);
            if (ImGui::Selectable(filterModes[i].c_str(), isSelected)){
                magFilterIndex = i;
                m_Sampler->setFilter(m_Sampler->getMinFilter(), static_cast<SR_TEXTURE_FILTER>(magFilterIndex + 1));
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Wrap U", wrapModes[wrapUModeIndex].c_str())){
        for(int i = 0; i < wrapModes.size(); i++){
            bool isSelected = (wrapUModeIndex == i);
            if (ImGui::Selectable(wrapModes[i].c_str(), isSelected)){
                wrapUModeIndex = i;
                m_Sampler->setWrap(static_cast<SR_TEXTURE_WRAP>(wrapUModeIndex + 1), m_Sampler->getWrapV(), m_Sampler->getWrapW());
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Wrap V", wrapModes[wrapVModeIndex].c_str())){
        for(int i = 0; i < wrapModes.size(); i++){
            bool isSelected = (wrapVModeIndex == i);
            if (ImGui::Selectable(wrapModes[i].c_str(), isSelected)){
                wrapVModeIndex = i;
                m_Sampler->setWrap(m_Sampler->getWrapU(), static_cast<SR_TEXTURE_WRAP>(wrapVModeIndex + 1), m_Sampler->getWrapW());
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Wrap W", wrapModes[wrapWModeIndex].c_str())){
        for(int i = 0; i < wrapModes.size(); i++){
            bool isSelected = (wrapWModeIndex == i);
            if (ImGui::Selectable(wrapModes[i].c_str(), isSelected)){
                wrapWModeIndex = i;
                m_Sampler->setWrap(m_Sampler->getWrapU(), m_Sampler->getWrapV(), static_cast<SR_TEXTURE_WRAP>(wrapWModeIndex + 1));
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    auto borderColorPtr = m_Sampler->getBorderColor();
    float borderColor[4] = {borderColorPtr[0], borderColorPtr[1], borderColorPtr[2], borderColorPtr[3]};
    if (ImGui::ColorPicker3("Border Color", borderColor)){
        m_Sampler->setBorderColor(borderColor);
    }

    imGuiEnd(windowData);
}


