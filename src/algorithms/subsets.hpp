#pragma once

#include "fa_factory.hpp"

#include <string>


namespace algorithms {
    class Subset : public AutomataCreator
    {
        models::Automaton create_automata (); 
    };
}