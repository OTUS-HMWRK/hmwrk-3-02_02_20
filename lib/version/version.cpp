#include "cmake_values.h"
#include "version.h"

namespace test{
    int getVersion(){
        return PROJECT_VERSION_PATCH;
    }
}