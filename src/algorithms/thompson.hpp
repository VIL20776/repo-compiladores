# pragma once

#include "models/automaton.hpp"

#include <memory>
#include <string>
#include <map>
#include <set>
#include <vector>

namespace algorithms {
    std::unique_ptr<models::Automaton> 
    regex_to_nfa(const std::string &expression);
}
