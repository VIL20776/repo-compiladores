#pragma once

#include <string>
#include <memory>

#include "algorithms/algorithms.hpp"

namespace analyzer {

class Yalex
{
private:
    std::unique_ptr<models::Automaton> dfa;

    void read(std::string filename);
    
public:
    Yalex();


};

}