#pragma once

#include "TestInclude.hpp"

static float s_TriangleVertices[] = {
    // X      Y     Z     R     G       B
    -0.5f,  -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
    0.5f,   -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
    0.0f,   0.5f,   0.0f, 0.0f, 0.0f, 1.0f
};

static uint32 s_TriangleIndices[] = {
    0, 1, 2
};