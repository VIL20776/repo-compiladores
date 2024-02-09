# pragma once

#include "fa_factory.hpp"

#include <string>
#include <map>
#include <set>
#include <vector>

namespace algorithms {
    class Thompson : public AutomataCreator
    {
    private:
        std::string expression;
    public:
        Thompson(const std::string &expression);
        std::unique_ptr<models::Automaton> create_automata();
    };

}
