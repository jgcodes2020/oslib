#include <oslib/platform.hpp>

#if !defined(OSLIB_NATIVE_POSIX_FDSTREAM_HPP_INCLUDED) && defined(OSLIB_PLATFORM_POSIX)
#define OSLIB_NATIVE_POSIX_FDSTREAM_HPP_INCLUDED
#include <exception>
#include <ios>
#include <memory>
#include <stdexcept>
#include <streambuf>
#include <string>

#include <fcntl.h>

#define ERR_THROW(...) (__VA_ARGS__)? throw std::system_error(errno, std::generic_category()) : (void) 0

namespace oslib::posix {
  /*?doc?
  Stream buffer wrapping a POSIX file descriptor
  */
  template <class CharT, class Traits = std::char_traits<CharT>>
  class basic_fdbuf : public std::basic_streambuf<CharT, Traits> {
  private:
    using typename std::basic_streambuf<CharT, Traits>::int_type;
    
    static constexpr size_t chunk_size = 1024;
    static constexpr std::ios::openmode mode_output = (std::ios::app | std::ios::out);
  public:
    basic_fdbuf(int fd, std::ios::openmode mode = (std::ios::in | std::ios::out)) :
      m_fd(fd),
      m_mode(mode),
      m_ibuf((mode & std::ios::in)? new CharT[chunk_size] : nullptr),
      m_obuf((mode & mode_output)? new CharT[chunk_size] : nullptr) {
      this->setg(m_ibuf.get(), m_ibuf.get(), m_ibuf.get() + chunk_size);
      this->setp(m_obuf.get(), m_obuf.get() + chunk_size);
    }
    
    int fd() {
      return m_fd;
    }
    
    int bind_fd() {
      int res = m_fd;
      m_fd = -1;
      return res;
    }
    
  protected:
    virtual int_type underflow() override {
      if (m_fd < 0)
        throw std::logic_error("File descriptor has been bound, cannot read");
      if (!(m_mode & std::ios::in))
        throw std::invalid_argument("Stream does not support input");
      
      ssize_t read_len = read(m_fd, m_ibuf.get(), chunk_size * sizeof(typename Traits::char_type));
      ERR_THROW(read_len == -1);
      this->setg(m_ibuf.get(), m_ibuf.get(), m_ibuf.get() + read_len);
      return chunk_size;
    }
    virtual int_type overflow(int_type type) override {
      if (m_fd < 0)
        throw std::logic_error("File descriptor has been bound, cannot write");
      if (!(m_mode & mode_output))
        throw std::invalid_argument("Stream does not support output");
      
      ERR_THROW(write(m_fd, m_obuf.get(), chunk_size * sizeof(typename Traits::char_type)) == -1);
      this->setp(m_obuf.get(), m_obuf.get() + chunk_size);
      return chunk_size * sizeof(typename Traits::char_type);
    }
    virtual int sync() override {
      if (m_fd < 0)
        throw std::logic_error("File descriptor has been bound, cannot write");
      if (!(m_mode & mode_output))
        throw std::invalid_argument("Stream does not support output");
      CharT* const beg = this->pbase();
      CharT* const pos = this->pptr();
      CharT* const end = this->epptr();
      
      ERR_THROW(write(m_fd, m_obuf.get(), (pos - beg) * sizeof(typename Traits::char_type)) == -1);
      this->setp(m_obuf.get(), m_obuf.get() + chunk_size);
      return pos - beg;
    }
  private:
    int m_fd;
    std::ios::openmode m_mode;
    std::unique_ptr<CharT[]> m_ibuf;
    std::unique_ptr<CharT[]> m_obuf;
  };
}  // namespace oslib::posix
#undef ERR_THROW
#endif