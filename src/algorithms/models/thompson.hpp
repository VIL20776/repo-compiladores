# pragma once

#include <string>
#include <map>
#include <set>
#include <vector>

namespace algorithms {
    std::vector<std::map<char, std::set<int>>> 
    thompson (std::string expression);
}