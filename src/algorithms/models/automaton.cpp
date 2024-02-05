#include "automaton.hpp"
#include "thompson.hpp"

namespace models {

Automaton::Automaton(std::string postfix, FA_type type)
{   
    if (type == NFA)
        transition_table = algorithms::thompson(postfix);

    if (type == DFA) 
        

    size = transition_table.size();
    acceptance = {size - 1};

}

}
