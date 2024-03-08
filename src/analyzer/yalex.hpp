#pragma once

#include <string>
#include <vector>
#include <memory>

#include "algorithms/algorithms.hpp"

namespace analyzer {

class Yalex
{
private:
    std::unique_ptr<models::Automaton> dfa;
    std::map<std::string, std::string> idents;
    std::string entrypoint;
    std::string entry_key;

    std::string replace_idents(const std::string &entrypoint);
    
public:
    Yalex();
    const std::string &get_entrypoint();
    int compile(std::string filename);
};

}