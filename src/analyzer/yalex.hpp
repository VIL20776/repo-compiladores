#pragma once

#include <string>

namespace analyzer {

class Yalex
{
private:
    std::string yal_content;

    void read();
    
public:
    Yalex(std::string path);
    ~Yalex();


};

}