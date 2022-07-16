#pragma once

#include "../../../../include/SyriusCore/Window/Context/Context.hpp"

namespace Syrius{

    class GlContext: public Context{
    public:

        ~GlContext() override;

    protected:
        GlContext();
    };

}
