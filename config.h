#define HAVE_REGEX_H 1
#define HAVE_STDBOOL_H 1
#define HAVE_TERMIOS_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_SYS_STATFS_H 1
#define HAVE_SYS_MOUNT_H 1
#define HAVE_MNTENT_H 1
#define HAVE_BFD_H 1
#define HAVE_LINK_H 1
#define HAVE_EXECINFO_H 1

#define HAVE_SIGSET_T 1
#define HAVE_PIPE 1
#define HAVE_FORK 1
#define HAVE_WAITPID 1
#define HAVE_USLEEP 1
#define HAVE_STAT64 1
#define HAVE_LSTAT64 1
#define HAVE_STRUCT_STAT64 1
#define HAVE_MKDTEMP 1
#define HAVE_MKSTEMP 1

#define HAVE_BFD_INIT 1
#define HAVE_BACKTRACE_SYMBOLS 1
#define HAVE_BACKTRACE 1

#define HAVE_PTHREAD_SETSCHEDPRIO 1
#define HAVE_PTHREAD_SETNAME_NP 1

#ifndef CONFIG_DIR
  #define CONFIG_DIR "/etc"
#endif
#ifndef RUNTIME_DIR
  #define RUNTIME_DIR "/run"
#endif
#ifndef RUN_DIR
  #define RUN_DIR "/run"
#endif
#ifndef TLS_DIR
  #define TLS_DIR "/etc/ssl"
#endif

#define MKDIR_ARGUMENTS_COUNT 2
#define HAVE_STATIC_ASSERT 1
