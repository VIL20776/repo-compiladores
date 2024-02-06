#include <set>
#include <vector>
#include <map>

namespace algorithms {
    struct automaton_props {
        int size;
        std::set<int> acceptance;
        std::set<char> symbols;
        std::vector<std::map<char, std::set<int>>> 
        transition_table;
    };

    automaton_props thompson (std::string expression);
}
}