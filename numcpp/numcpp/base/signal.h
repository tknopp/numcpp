#ifndef SPECIAL_FUNCTIONS
#define SPECIAL_FUNCTIONS

namespace numcpp
{

// convolution functions

enum class convType { full };//, same, valid };

template<class T, class U, class R, class V>
  Array<COMMON_TYPE(T,U)>
  conv(const AbstractArray<T,R>& x, const AbstractArray<U,V>& y, convType t )
{
  size_t N = x.size();
  size_t M = y.size();

  switch(t)
  {
    case convType.full:
      Array<COMMON_TYPE(T,U)> z = zeros(x.size() + y.size() - 1);
      for(size_t n=0; n<N; n++)
        for(size_t m=0; m<M; m++)
          z(n) += x(m)*y(n-m)
      break;
    /*case convType.same:

      break;
    case convType.valid:

      break;*/

  }
}
}

#endif
