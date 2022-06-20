//
// Created by jgcodes on 20/06/22.
//
#include <exception>
#include <stdexcept>
#include <iostream>
#include <syncstream>
#include <oslib/dl.hpp>

namespace fs = std::filesystem;

#if defined(OSLIB_PLATFORM_POSIX)
#include <dlfcn.h>

namespace oslib {
  shared_library::shared_library(const fs::path& path) :
    handle([&path]() -> void* {
      if (void* res = dlopen(path.c_str(), RTLD_LAZY)) {
        return res;
      }
      const char* err = dlerror();
      throw std::runtime_error(err);
    }()) {}
  
  shared_library::~shared_library() {
    if (dlclose(handle)) {
      const char* err = dlerror();
      // This just crashes and burns
      std::osyncstream(std::cerr) << "FATAL ERROR: " << err;
      std::terminate();
    }
  }
  
  void* shared_library::symbol(const char* str) {
    dlerror();
    void* res = dlsym(handle, str);
    if (const char* err = dlerror()) {
      throw std::runtime_error(err);
    }
    return res;
  }
}
#endif