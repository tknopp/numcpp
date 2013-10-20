#include "file.h"
#include "../core.h"

namespace numcpp
{

size_t filesize(std::string filename)
{
    std::ifstream in(filename, std::ifstream::in | std::ifstream::binary);
    in.seekg(0, std::ifstream::end);
    return in.tellg();
}

}
