#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>

namespace models
{
    enum FA_type {DFA, NFA};

    class Automaton
    {
    
    private:
        int size;
        std::set<int> acceptance;
        std::vector<std::map<char, std::set<int>>>
        transition_table;

    public:

        Automaton(std::string postfix, FA_type type);
        Automaton(
            std::vector<std::map<char, std::set<int>>>
            transition_table, FA_type type
        );
        ~Automaton();

    };
    
} // namespace models
