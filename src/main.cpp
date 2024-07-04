/**
 * Main.cpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */
#include "Framebuffer.hpp"

#include "MyGaussianblur.hpp"
#include "MyHistogram.hpp"
#include "MyModel.hpp"
#include "MyPseudocolor.hpp"
#include "MySkinsmooth.hpp"
#include "MyVectorscope.hpp"
#include "MyWaveform.hpp"
#include "MyZebra.hpp"

#include <memory>

const std::string APP_WARNING_INFO =
    "please select application: 1:3DModel, 2:pseudocolor, 3:Zebra, 4:Waveform 5:Histogram, 6:Skinsmooth \n  \
                                      Usage: ./opengl-cmake-cs.exe <app_index>";

#define ENUMNAME(n) \
    case n:         \
        return #n
const char* print_enum_name(int n) {
    switch (n) {
        ENUMNAME(CS_NONE);
        ENUMNAME(CS_3DMODEL);
        ENUMNAME(CS_PSEUDOCOLOR);
        ENUMNAME(CS_ZEBRA);
        ENUMNAME(CS_WAVEFORM);
        ENUMNAME(CS_HISTOGRAM);
        ENUMNAME(CS_VECTORSCOPE);
        ENUMNAME(CS_SKINSMOOTH);
        ENUMNAME(CS_GAUSSIANBLUR);
        default:
            return "unknow enum";
    }
}

/**
 * Main function for running the selected application.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Exit code.
 */
int main(int argc, const char** argv) {
    int selectedAppIndex = CS_NONE;
    std::unique_ptr<Application> selectedApp;

    if (argc != 2) {
        std::cerr << APP_WARNING_INFO << std::endl;
        selectedAppIndex = CS_3DMODEL;
    } else {
        selectedAppIndex = static_cast<int>(std::stoi(argv[1]));
    }

    while (true) {
        switch (selectedAppIndex) {
            case CS_3DMODEL:
                selectedApp = std::make_unique<MyModel>();
                break;
            case CS_PSEUDOCOLOR:
                selectedApp = std::make_unique<MyPseudocolor>();
                break;
            case CS_ZEBRA:
                selectedApp = std::make_unique<MyZebra>();
                break;
            case CS_WAVEFORM:
                selectedApp = std::make_unique<MyWaveform>();
                break;
            case CS_HISTOGRAM:
                selectedApp = std::make_unique<MyHistogram>();
                break;
            case CS_VECTORSCOPE:
                selectedApp = std::make_unique<MyVectorscope>();
                break;
            case CS_SKINSMOOTH:
                selectedApp = std::make_unique<MySkinsmooth>();
                break;
            case CS_GAUSSIANBLUR:
                selectedApp = std::make_unique<MyGaussianblur>();
                break;
            default:
                selectedApp = std::make_unique<MyModel>();
                break;
        }

        selectedApp->run();

        if (selectedAppIndex != static_cast<int>(selectedApp->funcIndex)) {
            selectedAppIndex = static_cast<int>(selectedApp->funcIndex);
            selectedApp.reset();
        } else {
            break;
        }
    }

    return 0;
}
