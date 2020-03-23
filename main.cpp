#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <omp.h>

#include "Bitmap.h"
#include "Tracing.h"

int main(int argc, const char** argv) {
    std::unordered_map<std::string, std::string> cmdLineParams;

    for (int i=0; i<argc; i++) {
        std::string key(argv[i]);

        if (key.size() > 0 && key[0]=='-') {
            if (i != argc-1) {
                cmdLineParams[key] = argv[i+1];
                i++;
            } else { 
                cmdLineParams[key] = "";
            }
        }
    }

    std::string outFilePath = "zout.bmp";
    if (cmdLineParams.find("-out") != cmdLineParams.end())
        outFilePath = cmdLineParams["-out"];

    int numThreads = 1;
    if (cmdLineParams.find("-threads") != cmdLineParams.end())
        numThreads = stoi(cmdLineParams["-threads"]);
    omp_set_num_threads(numThreads);

    bool flag = false;
    if (cmdLineParams.find("-simd") != cmdLineParams.end() && cmdLineParams["-simd"] == "1")
        flag = true;
    

    Scene s;
    Pixel image[s.w * s.h];

    if (flag) {
#pragma omp parallel for
        for (int y = 0; y < s.h; y++) {
#pragma omp simd
            for (int x = 0; x < s.w; x++) {
                image[y * s.w + x]  = s.TraceRay(Coord(0, 0, 0), s.ToRealCoord(x, y), 1, inf);
            }
        }
    } else {
#pragma omp parallel for
        for (int y = 0; y < s.h; y++) {
            for (int x = 0; x < s.w; x++) {
                image[y * s.w + x]  = s.TraceRay(Coord(0, 0, 0), s.ToRealCoord(x, y), 1, inf);
            }
        }
    }
    SaveBMP(outFilePath.c_str(), image, s.w, s.h);

    std::cout << "end." << std::endl;
    return 0;
}
