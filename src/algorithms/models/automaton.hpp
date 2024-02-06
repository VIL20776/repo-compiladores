#pragma once

#include "props.hpp"

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

        bool check(std::string input);

        std::set<int> e_closure(const int s);
        std::set<int> e_closure(std::set<int> states);
        std::set<int> move(std::set<int> states, char c);

        void graph_automaton(char *name);
    };
    
}
