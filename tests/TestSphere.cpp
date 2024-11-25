#include <gtest/gtest.h>
#include "../src/rayscene/Scene.hpp"
#include "../src/rayscene/Sphere.hpp"
#include "../src/rayscene/Camera.hpp"

TEST(SceneRenderTest, GeneratesNonMonochromeImage) {
    // Setup basic scene
    Scene scene;
    scene.add(new Sphere(1.0));
    scene.addLight(new Light(Vector3(2, 2, -1)));
    
    // Create and render image
    Image image(100, 100);
    Camera camera;
    camera.render(image, scene);
    
    // Check pixel variation
    Color firstPixel = image.getPixel(0, 0);
    bool hasVariation = false;
    
    for(unsigned int y = 0; y < image.height && !hasVariation; y += 10) {
        for(unsigned int x = 0; x < image.width && !hasVariation; x += 10) {
            Color pixel = image.getPixel(x, y);
            if (std::abs(pixel.r - firstPixel.r) > 0.1 || 
                std::abs(pixel.g - firstPixel.g) > 0.1 || 
                std::abs(pixel.b - firstPixel.b) > 0.1) {
                hasVariation = true;
            }
        }
    }
    
    std::string filename = "test_output.png";
    image.writeFile(filename);
    EXPECT_TRUE(hasVariation) << "L'image générée est monochrome";
}