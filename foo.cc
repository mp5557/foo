#include <iostream>
#include <vector>

#include <fmt/format.h>

#define DBG_MACRO_NO_WARNING
#include <dbg.h>

#include <eigen3/Eigen/Eigen>

#include "compressed_row_storage.h"

void Foo() {
  std::cout << "1!!1hello world";
}

int main() {
  std::cout << "hello world\n";
  std::vector<int> a = {1, 2, 3};
  std::cout << a.front() << ' ' << a.back() << std::endl;
  a.emplace(a.end(), 4);
  std::cout << a.front() << ' ' << a.back() << std::endl;

  Eigen::Vector3f vec;

  Foo();
  dbg(a);
}
