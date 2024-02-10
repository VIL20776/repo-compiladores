#pragma once

#include "fa_factory.hpp"

#include <string>


namespace algorithms {
    class Subsets : public AutomataCreator
    {
        models::Automaton *automaton;

        public:
        Subsets(models::Automaton *automaton);
        std::unique_ptr<models::Automaton> create_automata (); 
    };
}