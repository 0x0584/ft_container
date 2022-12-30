#include "ft.hpp"

int main(int argc, const char *argv[]) { 

  {
    using value_type = int;
    using unique_ptr = ft::unique_ptr<value_type>;

    unique_ptr::raw_pointer p = new value_type{};
    unique_ptr ptr(p);
  }
}
