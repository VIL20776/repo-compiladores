#pragma once

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

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
        std::set<char> symbols;
        std::vector<std::map<char, std::set<int>>>
        transition_table;

    public:

        Automaton(std::string postfix, FA_type type);
        Automaton(
            std::vector<std::map<char, std::set<int>>>
            transition_table, FA_type type
        );
        // ~Automaton();

        bool check(std::string input);

        const std::set<int> &get_acceptance();
        const std::set<int> &get_symbols();
        const int &get_size();

        std::set<int> e_closure(int i);
        std::set<int> e_closure(std::set<int> i);

        void graph_automaton(char *name);
    };
    
}
