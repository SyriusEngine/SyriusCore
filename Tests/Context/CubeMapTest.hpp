#pragma once

#include "../TestEnvironment.hpp"
#include "../TestUtils.hpp"

class CubeMapTest: public testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;

    static ResourceView<CubeMap> createCubeMap();
};