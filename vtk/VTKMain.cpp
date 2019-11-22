#include <iostream>
#include <chrono>
#include <vector>
#include <dirent.h>

#include "loadVTK.h"
#include "parseArgs.h"
#include "helper.h"
#include "writeFile.h"

#include "ospcommon/math/vec.h"
#include "ospcommon/math/box.h"

// This is used for writing to pidx file
// #include "writePIDX.h"


int main(int argc, const char** argv)
{
    // Parse arguments loading a list of dataset
    Args args;
    parseArgs(argc, argv, args);

    std::cout << "Files Dir: " << args.timeStepPaths[0] << std::endl;

    // Read all files dir
    std::vector<timesteps> files;
    for (const auto &dir : args.timeStepPaths) {
        DIR *dp = opendir(dir.c_str());
        if (!dp) {
            throw std::runtime_error("failed to open directory: " + dir);
        }
        for (dirent *e = readdir(dp); e; e = readdir(dp)) {
            if(std::string(e ->d_name).length() > 3){
                const std::string vtkFile = dir + "/" + std::string(e->d_name);
                std::string ext = getFileExt(std::string(e->d_name));
                timesteps t(std::stoi(ext), vtkFile);
                files.push_back(t);
            }
        }
    }
    // Sort time steps 
    std::sort(files.begin(), files.end(), sort_timestep());

    std::vector<std::vector<float>> volumes;
    ospcommon::math::vec3i dims;

    // only load 2 time steps for testing now 
    int count = args.numTimesteps;
    int numFiles = 0;
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    for(auto t : files){
        if(numFiles < count){
            std::cout << "file dir " << t.fileDir << " time step = " << t.timeStep << std::endl;
            // load all volumes
            std::vector<float> v;
            loadVTK(t.fileDir, v, dims);
            if(numFiles % 5 != 0){
                // For others time step in between, we save differents
                compute_deltas(volumes.back(), v);
            }
            volumes.push_back(v);
            numFiles++;  
        }
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "It took " << time_span.count() << " seconds loading " << count << " time step." << std::endl;

    ospcommon::math::vec3i writeDims(dims.x, dims.y, dims.z - 2);
    for(int i = 0; i < volumes.size(); i++){
        std::string file = "/home/mengjiao/Desktop/data/compression/test" + std::to_string(i) + ".raw";
        writeRaw(volumes[i], file, writeDims);
    }
    return 0;
}
