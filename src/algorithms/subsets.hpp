#pragma once

#include "fa_factory.hpp"

#include <string>


namespace algorithms {
    class Subsets : public AutomataCreator
    {
        models::Automaton *automaton;

        std::set<int> e_closure(const std::set<int> &states); 
        std::set<int> e_closure(const int &state); 
    std::set<int> move(const std::set<int> &states, const char &c); 

        public:
        Subsets(models::Automaton *automaton);
        models::Automaton *create_automata (); 
    };
}