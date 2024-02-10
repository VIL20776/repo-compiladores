#include <iostream>

#include "algorithms.hpp"

int simulation_mesage(bool result)
{
    if(result)
        std::cout << "The recived word is valid" << '\n';
    else
        std::cout << "The recived word is invalid" << '\n';

    return 0;
}

int main(int argc, char const *argv[])
{
    std::string regex = "(a|b)*a(a|b)(a|b)";
    std::string word = "baaab";

    if (argc == 3)
    {
        regex = std::string(argv[1]);
        word = std::string(argv[2]);
    }

    bool check = algorithms::check_parenthesis(regex);
    if (!check) {
        std::cout << "Falta un parentesis de cierre o apertura. Saliendo del programa" << '\n';
        return 1;
    }

    regex = algorithms::to_standard(regex);
    std::cout << regex << "\n";
    std::string postfix = algorithms::regex_to_postfix(regex);
    std::cout << postfix << "\n";

    std::unique_ptr<algorithms::AutomataCreator> fa_creator {};
    std::unique_ptr<models::Automaton> nfa {};
    std::unique_ptr<models::Automaton> dfa {};


    fa_creator.reset(new algorithms::Direct(postfix));
    dfa = fa_creator->create_automata();
    bool result1 = simulation_mesage(dfa->simulate(word));
    dfa->graph_automaton(std::string("Direct.png").data());

    fa_creator.reset(new algorithms::Thompson(postfix));
    nfa = fa_creator->create_automata();
    std::string nfa_name = "Thompson.png";
    simulation_mesage(nfa->simulate(word));
    nfa->graph_automaton(nfa_name.data());

    fa_creator.reset(new algorithms::Subsets(nfa.release()));
    dfa = fa_creator->create_automata();
    simulation_mesage(dfa->simulate(word));
    std::string dfa_name = "Subsets.png";
    dfa->graph_automaton(dfa_name.data());

    fa_creator.reset(new algorithms::Minimize(dfa.release()));
    dfa = fa_creator->create_automata();
    simulation_mesage(dfa->simulate(word));
    std::string dfa_min_name = "Minimize.png";
    dfa->graph_automaton(dfa_min_name.data());

    return 0;
}
