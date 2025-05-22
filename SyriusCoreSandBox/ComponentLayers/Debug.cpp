#include "Debug.hpp"

Debug::Debug(const UP<SyriusWindow> &window, const ResourceView<Context> &context):
IComponent(window, context){
}

void Debug::onImGui(ImGuiWindowData &windowData) {
    imGuiBegin(windowData, "Debug");

    ImGui::ColorPicker3("Background Color", m_Context->getDefaultFrameBuffer()->getColorAttachment(0)->getClearColor());

    ImGui::Columns(2, "Memory Allocation Tracker");
    ImGui::Separator();
    ImGui::Text("Total Allocated"); ImGui::NextColumn();
    ImGui::Text("%lu bytes", MemoryProfiler::getAllocatedMemory()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("Total Freed"); ImGui::NextColumn();
    ImGui::Text("%lu bytes",  MemoryProfiler::getDeallocatedMemory()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("Usage"); ImGui::NextColumn();
    ImGui::Text("%lu bytes",  MemoryProfiler::getCurrentUsage()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Columns(1);

    imGuiEnd(windowData);
}
