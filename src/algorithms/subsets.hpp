#pragma once

#include <string>
#include <memory>

#include "models/automaton.hpp"

namespace algorithms {
    std::unique_ptr<models::Automaton> 
    nfa_to_dfa(models::Automaton *automaton); 
}