template<typename T> T foo(T x) { return 2*x; }
extern template int foo<int>(int x);

template<class T>
class Bar
{
public:
  T value;
  Bar(T val);
};

template<class T>
Bar<T>::Bar(T val) { value = val;}

extern template class Bar<int>;
