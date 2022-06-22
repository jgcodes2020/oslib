#ifndef OSLIB_FWD_PIPES_HPP_INCLUDED
#define OSLIB_FWD_PIPES_HPP_INCLUDED

#include <iostream>
#include <string>

namespace oslib {
  template <class CharT, class Traits = std::char_traits<CharT>>
  class basic_pipe;
  
  template <class CharT, class Traits = std::char_traits<CharT>>
  class basic_opipestream;
  template <class CharT, class Traits = std::char_traits<CharT>>
  class basic_ipipestream;
}
#endif