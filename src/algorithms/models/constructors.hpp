# pragma once

#include <string>
#include <map>
#include <set>
#include <vector>

namespace algorithms {
    struct automaton_props {
        int size;
        std::set<int> acceptance;
        std::set<char> symbols;
        std::vector<std::map<char, std::set<int>>> 
        transition_table;
    };

    automaton_props thompson (std::string expression);
    automaton_props subsets (automaton_props props);
    automaton_props minimize (automaton_props props);
    automaton_props direct (std::string expresion);
}