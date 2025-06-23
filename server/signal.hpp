// server/signal.hpp
#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <csignal>
#include <iostream>
#include <unistd.h>

inline void signal_handler(int signum) {
    std::cout << "\nStopping server (signal " << signum << ")\n";
    exit(0);
}

#endif