#pragma once

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>

#include "ospcommon/math/vec.h"


void loadRaw(std::string file, std::vector<float> &volume, ospcommon::math::vec3i &dims);


