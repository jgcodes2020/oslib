#ifndef OSLIB_DL_HPP_INCLUDED
#define OSLIB_DL_HPP_INCLUDED

#include <oslib/platform.hpp>
#include <filesystem>
#include <type_traits>

#if defined(OSLIB_PLATFORM_POSIX)
using native_handle_type = void*;
#elif defined(OSLIB_PLATFORM_WIN32)
using native_handle_type = HMODULE;
#else
  #error Platform currently unsupported
#endif

namespace oslib {

  class shared_library {
  public:
#if defined(OSLIB_PLATFORM_POSIX)
    using native_handle_type = void*;
#elif defined(OSLIB_PLATFORM_WIN32)
    using native_handle_type = HMODULE;
#endif
    native_handle_type native_handle() {
      return handle;
    }
    
    /*?doc?
    Loads a shared library.
    :param path: Path to the shared library.
    */
    shared_library(const std::filesystem::path& path);
    ~shared_library();
    
    void* symbol(const char* str);
    void* symbol(const std::string& str) {
      return symbol(str.c_str());
    }
    void* symbol(std::string_view str) {
      return symbol(std::string(str));
    }
    
  private:
    native_handle_type handle;
    bool owning;
  };
}  // namespace oslib

#endif