#ifndef CPP_ARGV_HPP
#define CPP_ARGV_HPP

#include <iostream>

#define CPP_ARGV_TEST_HELP_REQUEST(argc, argv0, prog_name, arg_msg)            \
  if (argc == 1) {                                                             \
    const char *const program_name = (argv0 == nullptr ? prog_name : argv0);   \
    std::cout << "Synopsis:" << std::endl                                      \
              << '\t' << program_name << ' ' << arg_msg << std::endl;          \
    return EXIT_SUCCESS;                                                       \
  }

#define CPP_ARGV_TEST_ARG_NUM(argc, num)                                       \
  if (argc != num) {                                                           \
    std::cerr << "Bad argument number" << std::endl;                           \
    return EXIT_FAILURE;                                                       \
  }

#endif
