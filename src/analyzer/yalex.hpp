#pragma once

#include <string>

namespace analyzer {

class Yalex
{
private:
    char* yal_content;

    void read();
    
public:
    Yalex(std::string path);


};

}