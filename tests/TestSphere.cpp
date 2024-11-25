#include <gtest/gtest.h>
#include "../src/rayscene/SceneLoader.hpp"

TEST(SceneRenderTest, LoadAndRenderTwoSpheresScene) {
    // auto [scene, camera, image] = SceneLoader::Load("../scenes/empty.json");
    auto [scene, camera, image] = SceneLoader::Load("../scenes/two-spheres-on-plane.json");
    
    ASSERT_NE(scene, nullptr);
    ASSERT_NE(camera, nullptr);
    ASSERT_NE(image, nullptr);
    
    camera->render(*image, *scene);
    
    Color topLeft = image->getPixel(0, 0);
    Color center = image->getPixel(image->width/2, image->height/2);
    Color bottomRight = image->getPixel(image->width-1, image->height-1);
    
    bool hasColorVariation = 
        (std::abs(topLeft.r - center.r) > 0.1) ||
        (std::abs(topLeft.g - center.g) > 0.1) ||
        (std::abs(topLeft.b - center.b) > 0.1) ||
        (std::abs(center.r - bottomRight.r) > 0.1) ||
        (std::abs(center.g - bottomRight.g) > 0.1) ||
        (std::abs(center.b - bottomRight.b) > 0.1);
    
    EXPECT_TRUE(hasColorVariation) << "L'image devrait avoir des variations de couleur";
    
    std::string filename = "test_render.png";
    image->writeFile(filename);
    
    delete scene;
    delete camera;
    delete image;
}