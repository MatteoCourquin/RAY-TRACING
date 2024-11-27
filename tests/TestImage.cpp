#include <gtest/gtest.h>
#include "../src/rayscene/SceneLoader.hpp"
#include <fstream>
#include <sstream>
#include <openssl/md5.h>
#include <iomanip>

std::string calculateMD5(const std::string &filename)
{
  // ? Open file without conversion
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open())
  {
    return "";
  }

  // ? Init MD5 context
  MD5_CTX md5Context;
  MD5_Init(&md5Context);

  // ? Read by 16Ko blocks
  char buf[1024 * 16];
  while (file.good())
  {
    file.read(buf, sizeof(buf));
    MD5_Update(&md5Context, buf, file.gcount());
  }

  // ? Finalize MD5 in 16Ko block
  unsigned char result[MD5_DIGEST_LENGTH];
  MD5_Final(result, &md5Context);

  // ? Convert to string
  std::stringstream ss;
  for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
  {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
  }

  return ss.str();
}

TEST(SceneRenderTest, LoadAndRenderTwoSpheresScene)
{
  auto [scene, camera, image] = SceneLoader::Load("../scenes/iso-sphere-on-plane.json");

  const std::string referenceImagePath = "../reference/reference_image.png";

  auto begin = std::chrono::high_resolution_clock::now();
  camera->render(*image, *scene);

  std::string generatedImagePath = "test_render.png";
  image->writeFile(generatedImagePath);
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

  std::cout << "Done." << std::endl;
  std::printf("Total time: %.3f seconds.\n", elapsed.count() * 1e-9);

  // std::string referenceHash = calculateMD5(referenceImagePath);
  std::string referenceHash = "4a33ea1966836758798c83d8de2395fb";
  std::string generatedHash = calculateMD5(generatedImagePath);

  EXPECT_EQ(generatedHash, referenceHash)
      << "L'image générée est différente de l'image de référence.\n"
      << "Hash de référence: " << referenceHash << "\n"
      << "Hash généré: " << generatedHash;

  delete scene;
  delete camera;
  delete image;
}
