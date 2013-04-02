#ifndef NUMCPP_HDF5_H
#define NUMCPP_HDF5_H

//#include "../core.h"
#include <fstream>
#include <iostream>
#include <string>
#include <hdf5.h>

namespace numcpp
{

/*!
@file

@addtogroup io
@{
*/

inline hid_t h5create(std::string filename, unsigned flags = H5F_ACC_TRUNC,
             hid_t create_plist=H5P_DEFAULT, hid_t access_plist=H5P_DEFAULT )
{
  hid_t file = H5Fcreate(filename.c_str(), flags, create_plist, access_plist);
  return file;
}

inline hid_t h5open(std::string filename, unsigned flags = H5F_ACC_RDWR, hid_t access_plist=H5P_DEFAULT )
{
  hid_t file = H5Fopen(filename.c_str(), flags, access_plist);
  return file;
}

inline void h5close(hid_t file)
{
    H5Fclose(file);
}

template<class T, int D>
void h5write(const Array<T,D>& x, std::string filename, std::string location)
{
  hid_t file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  //hid_t file = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

  h5write(x, file, location);

  H5Fclose(file);
}

template<class T> hid_t hdf5_mem_type_id();
template<> inline hid_t hdf5_mem_type_id<double>() { return H5T_NATIVE_DOUBLE; }
template<> inline hid_t hdf5_mem_type_id<float>() { return H5T_NATIVE_FLOAT; }
template<> inline hid_t hdf5_mem_type_id<int>() { return H5T_NATIVE_INT; }

template<class T, int D>
void h5write(const Array<T,D>& x, hid_t file, std::string location)
{
  hid_t dataset, dataspace;
  hsize_t dimsf[D];
  herr_t status;

  /* write response */
  std::copy(x.shape().begin(), x.shape().end(), dimsf);

  dataspace = H5Screate_simple(D, dimsf, NULL);
  dataset = H5Dcreate(file, location.c_str(), hdf5_mem_type_id<T>(), dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  status = H5Dwrite(dataset, hdf5_mem_type_id<T>(), H5S_ALL, H5S_ALL, H5P_DEFAULT, x.data() );

  H5Sclose(dataspace);
  H5Dclose(dataset);
}


/*template<class T, int D>
Array<T,D> hdf5read(std::string filename, std::string location)
{
  hid_t file = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  Array<T,D> x = hdf5read<T,D>(file, location);
  H5Fclose(file);

  return x;
}*/

template<class T, int D>
Array<T,D> h5read(std::string filename, std::string location)
{
  hid_t dataset, dataspace;
  hsize_t dimsf[D];
  std::array<size_t,D> shape;
  herr_t status;
  int rank;

  hid_t file = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  dataset = H5Dopen(file, location.c_str(), H5P_DEFAULT);
  if( dataset < 0 )
    throw std::exception();

  dataspace = H5Dget_space(dataset);
  rank      = H5Sget_simple_extent_ndims(dataspace);
  status    = H5Sget_simple_extent_dims(dataspace, dimsf, NULL);

  if(rank != D)
    throw std::exception();

  for(int i=0; i<D; i++)
    shape[i] = dimsf[i];

  Array<T,D> x(shape);

  status = H5Dread(dataset, hdf5_mem_type_id<T>(), H5S_ALL, H5S_ALL, H5P_DEFAULT, x.data() );

  H5Sclose(dataspace);
  H5Dclose(dataset);
  H5Fclose(file);

  return x;
}



/*! @} */


}

#endif
