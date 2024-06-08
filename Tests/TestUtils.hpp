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

static std::vector<float> s_RectangleVertices2 = {
        // X      Y     Z     R     G       B
        -1.0f,  -1.0f,  0.0f, 1.0f, 0.0f, 0.0f,
        1.0f,   -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
        1.0f,   1.0f,   0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,   0.0f, 1.0f, 1.0f, 0.0f
};

static std::vector<uint32> s_RectangleIndices = {
    0, 1, 2,
    2, 3, 0
};

static std::vector<uint8> s_RedGreen = {
    255, 0, 0, 255,     0, 255, 0, 255,
    0, 255, 0, 255,     255, 0, 0, 255
};

static std::vector<uint8> s_RedGreenBlue = {
    255, 0, 0, 255,     0, 255, 0, 255,     0, 0, 255, 255,
    0, 255, 0, 255,     0, 0, 255, 255,     255, 0, 0, 255,
    0, 0, 255, 255,     255, 0, 0, 255,     0, 255, 0, 255
};

inline void createRedGreenCheckerboard(std::vector<ubyte>& data, uint32 width){
    data.resize(width * width * 4);
    int32 sw = 0;
    for (size_t i = 0; i < data.size(); i += 4) {
        if (sw % 2 == 0) {
            data[i] = 255;
            data[i + 1] = 0;
            data[i + 2] = 0;
            data[i + 3] = 255;
        } else {
            data[i] = 0;
            data[i + 1] = 255;
            data[i + 2] = 0;
            data[i + 3] = 255;
        }
        sw++;
    }
}