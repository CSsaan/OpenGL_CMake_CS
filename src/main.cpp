/**
 * Main.cpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#include "MyModel.hpp"
#include "MyPseudocolor.hpp"
#include "MyZebra.hpp"

#include <memory>

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::cerr << "please select application: 1:3DModel, 2:pseudocolor, 3:Zebra" << std::endl;
    std::cerr << "Usage: ./opengl-cmake-cs.exe <app_index>" << std::endl;
    return 1;
  }
  int app_index = std::stoi(argv[1]);
  std::unique_ptr<Application> app;
  switch (app_index) {
    case 1:
      app = std::make_unique<MyModel>();   // 3D Model
      break;
    case 2:
      app = std::make_unique<MyPseudocolor>();  // pseudocolor
      break;
    case 3:
      app = std::make_unique<MyZebra>();        // Zebra
      break;
    default:
      std::cerr << "Unrecognized app index: " << app_index << std::endl;
      std::cerr << "please select application: 0:3DModel, 1:pseudocolor, 2:Zebra" << std::endl;
      std::cerr << "Usage: ./opengl-cmake-cs.exe <app_index>" << std::endl;
      return 1;
  }
  app->run();
  return 0;
}

