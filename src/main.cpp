/**
 * Main.cpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */
#include "Framebuffer.hpp"


#include "MyModel.hpp"
#include "MyPseudocolor.hpp"
#include "MyZebra.hpp"
#include "MyWaveform.hpp"
#include "MyHistogram.hpp"
#include "MySkinsmooth.hpp"

#include <memory>

const std::string APP_WARNING_INFO = "please select application: 1:3DModel, 2:pseudocolor, 3:Zebra, 4:Waveform 5:Histogram, 6:Skinsmooth \n  \
                                      Usage: ./opengl-cmake-cs.exe <app_index>";

int main(int argc, const char* argv[]) {
    std::unique_ptr<Application> app;
    if (argc != 2) {
        std::cerr << APP_WARNING_INFO << std::endl;
        std::unique_ptr<Application> app;
        app = std::make_unique<MyModel>();
        app->run();
        return 1;
    }

    int app_index = std::stoi(argv[1]);
    switch (app_index) {
        case 1:
            app = std::make_unique<MyModel>(); // 3D Model
            break;
        case 2:
            app = std::make_unique<MyPseudocolor>(); // pseudocolor
            break;
        case 3:
            app = std::make_unique<MyZebra>(); // Zebra
            break;
        case 4:
            app = std::make_unique<MyWaveform>(); // Luma Waveform   // TODO: RGB Waveform
            break;
        case 5:
            app = std::make_unique<MyHistogram>(); // TODO: Histogram
            break;
        case 6:
            app = std::make_unique<MySkinsmooth>(); // Skinsmooth
            break;
        default:
            std::cerr << "Unrecognized app index: " << app_index << std::endl;
            app = std::make_unique<MyModel>();
            return 1;
    }
    app->run();
    return 0;
}
