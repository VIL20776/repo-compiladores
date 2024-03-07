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
    std::string regex = 
        "\"let\"#" // let 4
        "|\"rule\"#" // rule 9
        "|\"(*\"#" //open comment 12
        "|\"*)\"#" //close comment 15
        "|'|'#" //entry_or 17
        "|[\"\\t\\n\\s\"]#" //delim 21
        "|'='#" // assign 23
        "|['a'-'z''A'-'Z']+#" //ident 128
        "|['!'-'~']+#" //regexp 379
        "|{([\"\\t\\n\\s\"'a'-'z''A'-'Z'])+}#"; //action 638
        ;

    std::string word = "{ return WHITESPACE }";

    if (argc == 3)
    {
        regex = std::string(argv[1]);
        word = std::string(argv[2]);
    }

    auto yalex = analyzer::Yalex();
    yalex.compile("slr-1.yal");

    std::cout << "Regex: " << yalex.get_entrypoint() << std::endl;


    // regex = algorithms::to_standard(regex);
    // std::cout << "Regex: " << regex << std::endl;
    // std::string postfix = algorithms::regex_to_postfix(regex);
    // std::cout << "Postfix: " << postfix << std::endl;

    // std::unique_ptr<algorithms::AutomataCreator> fa_creator {};
    // std::unique_ptr<models::Automaton> dfa {};

    // fa_creator.reset(new algorithms::Direct(postfix));
    // dfa = fa_creator->create_automata();
    // simulation_mesage(dfa->simulate(word,true));
    // dfa->graph_automaton(std::string("Direct.png").data());

    return 0;
}
