#include "utils.hpp"
#include <cmath>

float utils::maxFloat(std::vector<float> floats){
    float max = -INFINITY;
    for(int i=0; i<int(floats.size()); i++){
        if(floats[i]>max) max=floats[i];
    }
    return max;
}
