#include "linalg/lapack.h"
#include "linalg/decomp.h"
#include "linalg/solver.h"

/*!
@defgroup linalg Linalg
@brief Linear algebra module containing basic linear algebra functions

For instance the linalg module contains functions for solving linear systems of equations or carrying out
singular value decompositions.  The Linalg module uses the C interface of the LAPACK library and therefore
requires to link against libclapack.
*/
