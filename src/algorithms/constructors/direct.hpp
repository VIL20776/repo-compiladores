#pragma once

#include "fa_factory.hpp"
#include "algorithms/models/syntax_tree.hpp"

#include <string>


namespace algorithms {
    class Direct : public AutomataCreator
    {
        models::Syntax_tree tree;

        public:
        Direct(std::string expression);
        std::unique_ptr<models::Automaton> create_automata (); 
    };
}