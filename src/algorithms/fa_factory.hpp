#pragma once

#include "models/automaton.hpp"

namespace algorithms {
    class AutomataCreator {
        public:
        virtual ~AutomataCreator() = default;
        virtual models::Automaton create_automata() = 0;
    };
}