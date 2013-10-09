#ifndef NUMCPP_HDF5_H
#define NUMCPP_HDF5_H

#include <string>
//#include <fstream>
//#include <iostream>
#include <hdf5.h>
#include "../core.h"

namespace numcpp
{

/*!
@file

@addtogroup io
@{
*/

/*!
Create a hdf5 file and return the file handle.

This is a very thin wrapper around H5Fcreate that provides sensible default values
for the \a flags, \a create_plist, and \a access_plist.
An example of the usage can be found \ref example03 "here".
\sa h5open()
*/
hid_t h5create(std::string filename, unsigned flags = H5F_ACC_TRUNC,
             hid_t create_plist=H5P_DEFAULT, hid_t access_plist=H5P_DEFAULT );

/*!
Open a hdf5 file and return the file handle.

This is a very thin wrapper around H5Fopen that provides sensible default values
for the \a flags, \a create_plist, and \a access_plist.
An example of the usage can be found \ref example03 "here".
\sa h5open()
*/
hid_t h5open(std::string filename, unsigned flags = H5F_ACC_RDWR, hid_t access_plist=H5P_DEFAULT );

/*!
Close a hdf5 file.
An example of the usage can be found \ref example03 "here".
\sa h5open() \sa h5create()
*/
void h5close(hid_t file);

/*!
Write the array \a x into the hdf5 file \a filename at location \a location.
An example of the usage can be found \ref example03 "here".
\sa h5read()
*/
void h5write(const DynTypeArray& x, std::string filename, std::string location);

/*!
Write the array \a x into the hdf5 file \a file at location \a location.
An example of the usage can be found \ref example03 "here".
\sa h5read()
*/
void h5write(const DynTypeArray& x, hid_t file, std::string location);

/*!
Read an array from the hdf5 file \a filename located at \a location.
An example of the usage can be found \ref example03 "here".
\sa h5write()
*/
DynTypeArray h5readDynType(std::string filename, std::string location);

/*!
Read an array from the hdf5 file \a filename located at \a location.
An example of the usage can be found \ref example03 "here".
\sa h5write()
*/
template<class T>
Array<T> h5read(std::string filename, std::string location)
{
  return (Array<T>)(h5readDynType(filename,location));
}



/*! @} */


}

#endif
