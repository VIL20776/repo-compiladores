#pragma once

#include "algorithms/models/automaton.hpp"
#include "algorithms/models/syntax_tree.hpp"

#include <string>
#include <memory>

namespace algorithms {
    std::unique_ptr<models::Automaton> 
    regex_to_dfa (std::string expression); 
}