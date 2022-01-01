#pragma once

#include <string>
#include <unordered_map>

namespace hi
{

    std::string trim(const std::string &s);

    void parser_param(const std::string &data, std::unordered_map<std::string, std::string> &result, char c = '&', char cc = '=');
} // namespace hi
