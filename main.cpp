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

    //int sceneId = 0;
    //if(cmdLineParams.find("-scene") != cmdLineParams.end())
        //sceneId = atoi(cmdLineParams["-scene"].c_str());

    //uint32_t color = 0;
    //if(sceneId == 1)
        //color = RED;
    //else if(sceneId == 2)
        //color = RED | GREEN;
    //else if(sceneId == 3)
        //color = BLUE;
  
    Scene s;
    Pixel image[s.w * s.h]; 

#pragma omp parallel for
    for (int y = 0; y < s.h; y++) {
        for (int x = 0; x < s.w; x++) {
            image[y * s.w + x]  = s.TraceRay(Coord(0, 0, 0), s.ToRealCoord(x, y), 1, inf);
        }
    }


    SaveBMP(outFilePath.c_str(), image, s.w, s.h);

    std::cout << "end." << std::endl;
    return 0;
}
