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
        Automaton(
            std::set<int> acceptance,
            std::set<char> symbols,
            std::vector<std::map<char, std::set<int>>> 
            transition_table);
        // ~Automaton();

        const int &get_size();
        const std::set<int> &get_acceptance();
        const std::set<char> &get_symbols();
        const std::set<int> &transition(int i, char c);
        const std::vector<std::map<char, std::set<int>>>
        &get_table();

        bool check(std::string input);

        void graph_automaton(char *name);
    };
    
}
