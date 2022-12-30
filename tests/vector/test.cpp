#include <iostream>

struct S {
  S(int i) { std::cout << "int"; }
  S(double d) { std::cout << "double"; }
};
int main() { S(1); }
