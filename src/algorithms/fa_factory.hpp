#pragma once

#include <memory>
#include "models/automaton.hpp"

namespace algorithms {
    class AutomataCreator {
        public:
        virtual ~AutomataCreator() = default;
        virtual std::unique_ptr<models::Automaton> create_automata() = 0;
    };
}