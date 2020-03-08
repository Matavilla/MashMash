#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "Bitmap.h"
#include "Tracing.h"

int main(int argc, const char** argv) {
    std::unordered_map<std::string, std::string> cmdLineParams;

    for(int i=0; i<argc; i++) {
        std::string key(argv[i]);

        if(key.size() > 0 && key[0]=='-') {
            if(i != argc-1) {
                cmdLineParams[key] = argv[i+1];
                i++;
            } else { 
                cmdLineParams[key] = "";
            }
        }
    }

    std::string outFilePath = "zout.bmp";
    if(cmdLineParams.find("-out") != cmdLineParams.end())
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
    Pixel image[w * h]; 
    for (int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            image[y * w + x]  = TraceRay(s, Coord(0, 0, 1), Coord(x, y, 0));
        }
    }


    SaveBMP(outFilePath.c_str(), image, w, h);

    std::cout << "end." << std::endl;
    return 0;
}
