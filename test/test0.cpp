#include <oslib/pipes.hpp>
#include <string>
#include <thread>

int main() {
  oslib::basic_pipe<char> pipe;
  
  std::thread t2([&]() {
    std::string str;
    std::getline(pipe.istream(), str);
    std::cout << str << std::endl;
  });
  pipe.ostream() << "This is one line of input" << std::endl;
  if (t2.joinable())
    t2.join();
}