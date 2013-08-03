#include "hdf5.h"
#include "../core.h"

namespace numcpp
{

hid_t hdf5_mem_type_id(Type t)
{
  switch(t)
  {
    case Type::Int:
      return H5T_NATIVE_INT;
    case Type::Double:
      return H5T_NATIVE_DOUBLE;
    case Type::Float:
      return H5T_NATIVE_FLOAT;
    default:
      throw std::invalid_argument("Type not supported!");
  }
}

Type dtype(hid_t t)
{
  if(H5Tequal(t,H5T_NATIVE_INT))
      return Type::Int;
  if(H5Tequal(t,H5T_NATIVE_DOUBLE))
        return Type::Double;
  if(H5Tequal(t,H5T_NATIVE_FLOAT))
        return Type::Float;

  std::stringstream s;
  s << "dtype(hid_t t): Type not supported, t = " << t << " !";
  throw std::invalid_argument(s.str());
}


hid_t h5create(std::string filename, unsigned flags,
               hid_t create_plist, hid_t access_plist )
{
  hid_t file = H5Fcreate(filename.c_str(), flags, create_plist, access_plist);
  return file;
}

hid_t h5open(std::string filename, unsigned flags, hid_t access_plist )
{
  hid_t file = H5Fopen(filename.c_str(), flags, access_plist);
  return file;
}

void h5close(hid_t file)
{
    H5Fclose(file);
}

void h5write(const DynTypeArray& x, std::string filename, std::string location)
{
  hid_t file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  //hid_t file = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

  h5write(x, file, location);

  H5Fclose(file);
}

void h5write(const DynTypeArray& x, hid_t file, std::string location)
{
  hid_t dataset, dataspace;
  hsize_t dimsf[x.ndims()];
  herr_t status;

  /* write response */
  std::copy(x.shape().begin(), x.shape().end(), dimsf);

  dataspace = H5Screate_simple(x.ndims(), dimsf, NULL);
  dataset = H5Dcreate(file, location.c_str(), hdf5_mem_type_id(x.elemType()),
                      dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  status = H5Dwrite(dataset, hdf5_mem_type_id(x.elemType()), H5S_ALL, H5S_ALL, H5P_DEFAULT, x.data() );

  H5Sclose(dataspace);
  H5Dclose(dataset);
}

DynTypeArray h5readDynType(std::string filename, std::string location)
{
  hid_t dataset, dataspace, type;
  std::vector<size_t> shape;
  herr_t status;
  int rank;

  hid_t file = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  dataset = H5Dopen(file, location.c_str(), H5P_DEFAULT);
  if( dataset < 0 )
    throw std::exception();

  type = H5Dget_type(dataset);
  dataspace = H5Dget_space(dataset);
  rank      = H5Sget_simple_extent_ndims(dataspace);
  hsize_t dimsf[rank];
  status    = H5Sget_simple_extent_dims(dataspace, dimsf, NULL);

  shape.resize(rank);

  for(int i=0; i<rank; i++)
    shape[i] = dimsf[i];

  Type t = dtype(type);
  DynTypeArray x(t, nbytes(t), shape);

  status = H5Dread(dataset, hdf5_mem_type_id(x.elemType()), H5S_ALL, H5S_ALL, H5P_DEFAULT, x.data() );

  H5Sclose(dataspace);
  H5Dclose(dataset);
  H5Fclose(file);

  return x;
}

}
