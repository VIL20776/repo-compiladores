#pragma once

#include "models/automaton.hpp"

#include <string>
#include <memory>

namespace algorithms {
    std::unique_ptr<models::Automaton> 
    regex_to_dfa (std::string expression); 
}