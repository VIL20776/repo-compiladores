#pragma once

#include "models/automaton.hpp"

#include <string>
#include <memory>

namespace algorithms {
    std::unique_ptr<models::Automaton> 
    minimize_dfa (models::Automaton *automaton); 
}