#include <iostream>

#include "algorithms/postfix.hpp"
#include "algorithms/models/automaton.hpp"

int main(int argc, char const *argv[])
{
    std::string regex = "(a|b)*abab";
    std::string postfix = algorithms::regex_to_postfix(regex);
    std::cout << postfix << "\n";

    models::Automaton nfa (postfix, models::NFA);
    std::string nfa_name = "Thompson.png";
    nfa.graph_automaton(nfa_name.data());
    return 0;
}
