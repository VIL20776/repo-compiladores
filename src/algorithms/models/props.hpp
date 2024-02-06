#pragma once

#include <vector>
#include <map>
#include <set>

namespace algorithms {
    struct automaton_props {
        std::set<int> acceptance;
        std::set<char> symbols;
        std::vector<std::map<char, std::set<int>>> 
        transition_table;
    };
}