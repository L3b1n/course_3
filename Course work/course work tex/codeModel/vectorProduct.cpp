template<typename T> T vectorProduct(const std::vector<T> &v)
{
  T product = 1;
  for (auto &i : v) {
    if (i > 0) { product *= i;}
  }
  return product;
}