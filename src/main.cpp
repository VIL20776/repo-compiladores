#include <iostream>

#include "algorithms/algorithms.hpp"
#include "analyzer/yalex.hpp"

void simulation_mesage(std::set<int> result)
{
    if (result.empty())
    {
        std::cout << "The word is not accepted by the automaton" << std::endl;
        return;
    }
    int acceptance = *result.begin();
    std::cout << "The word is accepted by the automaton: Aceptance " << acceptance << std::endl;
}

int main(int argc, char const *argv[])
{

    auto yalex = analyzer::Yalex();
    yalex.compile("slr-2.yal");

    std::cout << "Regex: " << yalex.get_entrypoint() << std::endl;


    std::string regex = yalex.get_entrypoint();
    // std::string regex = "(\"' '\"|['!'-'~'])+";
    // std::string word = "';'";
    regex = algorithms::to_standard(regex);
    // std::cout << "Regex: " << regex << std::endl;
    std::string postfix = algorithms::regex_to_postfix(regex);
    // std::cout << "Postfix: " << postfix << std::endl;


    // std::unique_ptr<algorithms::AutomataCreator> fa_creator {};
    // fa_creator.reset(new algorithms::Direct(postfix));

    // std::unique_ptr<models::Automaton> dfa = fa_creator->create_automata();
    // std::set<int> result = dfa->simulate(word,true);
    // dfa->graph_automaton("Direct.png");

    postfix.append("\x80&");
    models::Syntax_tree tree = models::Syntax_tree(postfix);
    tree.draw_syntax_tree("Syntax_tree.png");

    return 0;
}
