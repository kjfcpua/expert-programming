// http://en.cppreference.com/w/cpp/language/attributes

int main(int argc, char* argv[]) {
  // error: unused variable 'n'
  // if -Werror=unused-variable or -Wall on
  int n1 __attribute__((__unused__));  // before C++11
  int n2 [[ gnu::unused ]];            // since  C++11 gun attr
  int n3 [[ maybe_unused ]];           // since  C++11 standard

  return 0;
}
