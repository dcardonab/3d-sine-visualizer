// File defining mathematical constants

#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>
#include <string>

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897932385;

// Utility Functions
inline float degrees_to_radians(float degrees) {
    return degrees * PI / 180.0;
}


// Random Fuctions

// Newer versions of C++ include a random number generator
// in the <random> header.
inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

        // // Older versions of C++ did not have a random number generator.
        // // This logic can be implemented in scenarios where the <random>
        // // library does not work, meaning scenarios where the random
        // // function above won't work either.
        // inline float random_float() {
        //     // Returns a random real in [0,1]
        //     // RAND_MAX is defined in the std library
        //     return rand() / (RAND_MAX + 1.0);
        // }
        
inline float random_float(float min, float max) {
    // Returns a random real between [min, max]
    return min + (max-min)*random_float();
}

// Clamp Function
// Return x in range of min and max if outside range.
inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline char* strToCharArray(std::string str) {
	char* charArray = new char[str.length() + 1];
	return strcpy(charArray, str.c_str());
}

#endif