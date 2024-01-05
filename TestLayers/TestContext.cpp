#include "TestContext.hpp"

TestContext::TestContext(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                         EasyIni::Configuration &config) : Layer(context, window, config) {
    printContextInfo(m_Context);

}

TestContext::~TestContext() {

}

void TestContext::onUpdate() {

}

void TestContext::onEvent(const Event &event) {
    printEventInfo(event);
}
