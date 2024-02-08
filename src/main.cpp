#include <iostream>

#include "algorithms.hpp"

int main(int argc, char const *argv[])
{
    std::string regex = "(a|b)*abab";
    std::string postfix = algorithms::regex_to_postfix(regex);
    std::cout << postfix << "\n";

    algorithms::AutomataCreator* fa_creator = new algorithms::Thompson(postfix);
    models::Automaton* nfa = fa_creator->create_automata();
    std::string nfa_name = "Thompson.png";
    nfa->graph_automaton(nfa_name.data());

    fa_creator = new algorithms::Subsets(nfa);
    models::Automaton* dfa = fa_creator->create_automata();
    std::string dfa_name = "Subsets.png";
    dfa->graph_automaton(dfa_name.data());


    delete fa_creator;
    delete nfa;
    delete dfa;

    return 0;
}
