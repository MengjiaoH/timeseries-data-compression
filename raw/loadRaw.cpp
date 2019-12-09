#include "loadRaw.h"

void loadRaw(std::string file, std::vector<float> &volume, ospcommon::math::vec3i &dims)
{
    std::ifstream input(file.c_str(), std::ios::binary);

    if(!input) {
      std::cout << "Cannot open file!" << std::endl;
    }

    //    int size = volume.dim.x * volume.dim.y * volume.dim.z;
   
    // find file size
    input.seekg(0, input.end); 
    int filesize = input.tellg();              
    input.seekg(0, input.beg);
    
    std::cout << "file size " << filesize << std::endl;

    // Here is the size of data
    volume.resize(filesize / sizeof(float));

    input.read(reinterpret_cast<char*>(volume.data()), volume.size()*sizeof(float));

    // find the data range
    // float minimum = *(std::min_element(volume.begin(), volume.end()));
    // float maximum = *(std::max_element(volume.begin(), volume.end()));
    
    //volume.range = ospcommon::math::vec2f(minimum, maximum);

    //std::cout << "data range " << minimum << " " << maximum << std::endl;
}