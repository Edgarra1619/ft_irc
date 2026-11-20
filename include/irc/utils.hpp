#ifndef UTILS_HPP
#define UTILS_HPP
#include <irc/NumericReplyCode.hpp>

#include <string>

int	parse_num(const std::string &str, const std::string &param_name);
std::string	NumericReplyString(NumericReplyCode a, const std::string &prefix = "");

#endif
