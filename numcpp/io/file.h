#ifndef FILE_H
#define FILE_H

//#include "../core.h"
#include <fstream>
#include <iostream>
#include <string>

namespace numcpp
{


template<class T=double>
Vector<T> fromfile(std::string filename, long count=-1, std::string sep="")
{
  if(sep.empty())
  {
    std::ifstream file(filename, std::ios::in|std::ios::binary|std::ios::ate);
    std::ifstream::pos_type sizebytes;
    size_t size;

    if(count == -1)
    {
      sizebytes = file.tellg();
      size = sizebytes / sizeof(T);
    } else
    {
      sizebytes = count*sizeof(T);
      size = count;
    }

    Vector<T> x({size});

    file.seekg (0, std::ios::beg);
    file.read (x.data(), sizebytes);
    file.close();

    return x;
  }
}


template<class T, int D>
void tofile(Array<T, D> x, std::string filename, std::string sep="")
{
  if(sep.empty())
  {
    std::ofstream file(filename, std::ios::out|std::ios::binary|std::ios::trunc);
    file.write((char*) x.data(), x.size()*sizeof(T));
    file.close();
  } else
  {
    std::ofstream file(filename, std::ios::out|std::ios::trunc);
    for(size_t i=0; i< x.size(); i++)
      file << x[i] << sep;
    file.close();
  }
}

}

#endif // FILE_H
