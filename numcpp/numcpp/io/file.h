#ifndef NUMCPP_FILE_H
#define NUMCPP_FILE_H

#include <fstream>
#include <iostream>
#include <string>
#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup io
@{
*/

template<class T=double>
Array<T> fromfile(std::string filename, long count=-1, std::string sep="")
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

    Array<T> x(size);

    file.seekg (0, std::ios::beg);
    file.read ((char*)x.data(), sizebytes);
    file.close();

    return x;
  }
}

template<class T=double>
Array<T> fromfile(std::ifstream& file, long count)
{
  std::ifstream::pos_type sizebytes;
  size_t size;

  sizebytes = count*sizeof(T);
  size = count;

  Array<T> x(size);

  file.read ((char*)x.data(), sizebytes);

  return x;
}

size_t filesize(std::string filename);

template<class T>
void tofile(Array<T> x, std::string filename, std::string sep="")
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

inline void tofile(DynTypeArray& x, std::ofstream& file)
{
   file.write((char*) x.data(), x.size()*x.elemSize());
}

template<class T>
Array<T> loadMatrix(std::string filename, size_t nrCols, Array<size_t> rowIndices=Array<size_t>(),
                    ptrdiff_t rowIncrement=-1)
{
  size_t sizeBytes = filesize(filename);
  size_t size = sizeBytes / sizeof(T);

  if(rowIndices.size() == 0 && rowIncrement <= 0)
  {
    size_t nrRows = size / nrCols;
    return reshape(fromfile<T>(filename, size), nrRows,nrCols);
  } else {

    if(rowIndices.size() == 0)
      rowIndices = range(0, size / rowIncrement);
    if(rowIncrement <= 0)
      rowIncrement = nrCols;

    auto nrRows = rowIndices.size();

    Array<T> matrix = zeros(nrRows, nrCols);

    std::ifstream file(filename, std::ios::in|std::ios::binary|std::ios::ate);

    for(size_t l=0; l< nrRows;l++)
    {
      file.seekg(rowIndices(l)*rowIncrement*sizeof(T), std::ios::beg);
      file.read( (char*)(matrix.data()+nrCols*l), nrCols*sizeof(T));
    }
    file.close();

    return matrix;
  }
}

/*

import numpy as np
import scipy as sp
import logging
from fileSize import *


def loadMatrix(filename, dtype=np.double, nrCols=1, rowIndices=(), rowIncrement=-1 ):
  fd = open(filename,'rb')
  sizeBytes = fileSize(fd)
  itemsize = np.dtype(dtype).itemsize
  size = sizeBytes / itemsize

  if len(rowIndices) == 0 and rowIncrement <= 0:
    nrRows = size / nrCols
    matrix = np.fromfile(fd, dtype=dtype, count=size).reshape((nrRows,nrCols))
  else:

    if len(rowIndices) == 0:
      rowIndices = range(0, size / rowIncrement)
    if rowIncrement <= 0:
      rowIncrement = nrCols

    nrRows = len(rowIndices)

    matrix = np.zeros((nrRows, nrCols), dtype=dtype)
    for l in xrange(nrRows):
      fd.seek(rowIndices[l]*itemsize*rowIncrement)
      matrix[l,:] = np.fromfile(fd, dtype=dtype, count=nrCols)

  fd.close()
  return matrix

if __name__ == "__main__":
  import doctest
  doctest.testmod()



*/

/*! @} */

}

#endif
