#if defined(__linux__)
#define OSLIB_PLATFORM_POSIX
#define OSLIB_PLATFORM_LINUX
#elif defined(__APPLE__)
#define OSLIB_PLATFORM_POSIX
#define OSLIB_PLATFORM_APPLE
#elif defined(__unix__)
#define OSLIB_PLATFORM_POSIX
#elif defined(_WIN32)
#define OSLIB_PLATFORM_WIN32
#endif