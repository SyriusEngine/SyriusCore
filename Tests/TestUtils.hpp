#pragma once

#include "TestInclude.hpp"

static std::vector<float> s_TriangleVertices = {
    // X      Y     Z     R     G       B
    -0.5f,  -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
    0.5f,   -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
    0.0f,   0.5f,   0.0f, 0.0f, 0.0f, 1.0f
};

static std::vector<float> s_TriangleVertices2 = {
    // X      Y     Z     R     G       B
    -1.0f,  -1.0f,  0.0f, 1.0f, 0.0f, 0.0f,
    1.0f,   -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
    0.0f,   1.0f,   0.0f, 0.0f, 0.0f, 1.0f
};

static std::vector<uint32> s_TriangleIndices = {
    0, 1, 2
};

static std::vector<uint32> s_TriangleIndices2 = {
    2, 1, 0
};

static std::vector<float> s_RectangleVertices = {
    // X      Y     Z     R     G       B
    -0.5f,  -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
    0.5f,   -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
    0.5f,   0.5f,   0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,   0.0f, 1.0f, 1.0f, 0.0f
};

static std::vector<uint32> s_RectangleIndices = {
    0, 1, 2,
    2, 3, 0
};

static std::vector<uint8> s_RedGreen = {
    255, 0, 0, 255,     0, 255, 0, 255,
    0, 255, 0, 255,     255, 0, 0, 255
};

static std::vector<uint8> s_RedBlue = {
    255, 0, 0, 255,     0, 0, 255, 255,
    0, 0, 255, 255,     255, 0, 0, 255
};

static std::vector<uint8> s_Magenta = {
    255, 0, 255, 255,
};

static std::vector<uint8> s_Turquoise = {
    0, 255, 255, 255,
};

static std::vector<uint8> s_RedGreenBlue = {
    255, 0, 0, 255,     0, 255, 0, 255,     0, 0, 255, 255,
    0, 255, 0, 255,     0, 0, 255, 255,     255, 0, 0, 255,
    0, 0, 255, 255,     255, 0, 0, 255,     0, 255, 0, 255
};