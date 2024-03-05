#include <iostream>

#include "algorithms/algorithms.hpp"
#include "analyzer/yalex.hpp"

int main(int argc, char const *argv[])
{
    std::string regex = "[\'a\'-\'z\'\'A\'-\'Z\']";
    std::string word = "let";

    if (argc == 3)
    {
        regex = std::string(argv[1]);
        word = std::string(argv[2]);
    }

    // analyzer::Yalex("./slr-1.yal");

    std::cout << regex << "\n";
    regex = algorithms::to_standard(regex);
    std::cout << regex << "\n";
    std::string postfix = algorithms::regex_to_postfix(regex);
    std::cout << postfix << "\n";

    std::unique_ptr<algorithms::AutomataCreator> fa_creator {};
    // std::unique_ptr<models::Automaton> nfa {};
    std::unique_ptr<models::Automaton> dfa {};


    fa_creator.reset(new algorithms::Direct(postfix));
    dfa = fa_creator->create_automata();
    // bool result1 = simulation_mesage(dfa->simulate(word));
    dfa->graph_automaton(std::string("Direct.png").data());

    return 0;
}
