#include <oslib/platform.hpp>

#if !defined(OSLIB_DETAIL_POSIX_PIPES_HPP) && defined(OSLIB_PLATFORM_POSIX)
#define OSLIB_DETAIL_POSIX_PIPES_HPP

#include <oslib/native/posix_fdstream.hpp>
#include <oslib/fwd/pipes.hpp>

#include <istream>
#include <ostream>
#include <system_error>

#define ERR_THROW(...) (__VA_ARGS__)? throw std::system_error(errno, std::generic_category()) : (void) 0

namespace oslib {
  
  template <class CharT, class Traits>
  class basic_opipestream : public std::basic_ostream<CharT, Traits> {
  public:
    ~basic_opipestream() {
      delete this->rdbuf();
    }
  private:
    friend class ::oslib::basic_pipe<CharT, Traits>;
    basic_opipestream(int fd) :
      std::basic_ostream<CharT, Traits>(new posix::basic_fdbuf<CharT, Traits>(fd, std::ios::out)){}
  };
  template <class CharT, class Traits>
  class basic_ipipestream : public std::basic_istream<CharT, Traits> {
  public:
    ~basic_ipipestream() {
      delete this->rdbuf();
    }
  private:
    friend class ::oslib::basic_pipe<CharT, Traits>;
    basic_ipipestream(int fd) :
      std::basic_istream<CharT, Traits>(new posix::basic_fdbuf<CharT, Traits>(fd, std::ios::in)){}
  };
  
  template <class CharT, class Traits>
  class basic_pipe final {
  public:
    basic_pipe() {
      int fds[2] = {-1, -1};
      // throw if pipe(fds) fails
      ERR_THROW(pipe(fds) == -1);
      
      new(&m_istream) basic_ipipestream<CharT, Traits>(fds[0]);
      new(&m_ostream) basic_opipestream<CharT, Traits>(fds[1]);
    }
    ~basic_pipe() {
      close(static_cast<posix::basic_fdbuf<CharT, Traits>*>(m_istream.rdbuf())->fd());
      close(static_cast<posix::basic_fdbuf<CharT, Traits>*>(m_ostream.rdbuf())->fd());
      // we need to manually destruct the streams
      // because C++ doesn't know which union member we inited
      m_istream.~basic_ipipestream<CharT, Traits>();
      m_ostream.~basic_opipestream<CharT, Traits>();
    }
    
    basic_ipipestream<CharT, Traits>& istream() {
      return m_istream;
    }
    basic_opipestream<CharT, Traits>& ostream() {
      return m_ostream;
    }
  private:
    // these unions defer initialization
    union {
      [[no_unique_address]] char d_istream;
      basic_ipipestream<CharT, Traits> m_istream;
    };
    union {
      [[no_unique_address]] char d_ostream;
      basic_opipestream<CharT, Traits> m_ostream;
    };
  };
}
#undef ERR_THROW
#endif