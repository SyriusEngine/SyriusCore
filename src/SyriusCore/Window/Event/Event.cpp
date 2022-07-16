#include "../../../../include/SyriusCore/Window/Event/Event.hpp"

namespace Syrius {

    Event::Event(SR_EVENT_CLASS_TYPE classType, SR_EVENT_TYPE type)
            : m_EventClass(classType),
              type(type){

    }

    Event::~Event() {

    }
}