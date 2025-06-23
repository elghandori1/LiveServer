// server/utils.hpp
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

std::string read_file(const std::string& path);
std::string get_mime_type(const std::string& path);

#endif
