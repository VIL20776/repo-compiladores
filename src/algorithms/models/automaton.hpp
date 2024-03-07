#pragma once

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

#include <vector>
#include <map>
#include <set>
#include <string>


namespace models
{
    class Automaton
    {
    
    private:
        int size;
        std::map<int,int> acceptance;
        std::set<char> symbols;
        std::vector<std::map<char, std::set<int>>> 
        transition_table;

    public:
        Automaton(
            std::map<int,int> acceptance,
            std::set<char> symbols,
            std::vector<std::map<char, std::set<int>>> 
            transition_table);
        // ~Automaton();

        const int &get_size();
        const std::map<int,int> &get_acceptance();
        const std::set<char> &get_symbols();
        const std::set<int> &transition(int i, char c);
        const std::vector<std::map<char, std::set<int>>>
        &get_table();

        std::set<int> e_closure(const int &state);
        std::set<int> e_closure(const std::set<int> &states);
        std::set<int> move(const std::set<int> &states, const char &c);
    
        std::set<int> simulate(std::string input, bool deterministic);

        void graph_automaton(char *name);
    };
    
}
