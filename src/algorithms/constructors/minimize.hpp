#pragma once

#include "fa_factory.hpp"

#include <string>


namespace algorithms {
    class Minimize : public AutomataCreator
    {
        models::Automaton *automaton;

        std::vector<std::set<int>> new_partition (std::vector<std::set<int>> part);

        public:
        Minimize(models::Automaton *automaton);
        std::unique_ptr<models::Automaton> create_automata (); 
    };
}