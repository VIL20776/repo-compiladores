#include "yalex.hpp"

#include <fstream>
#include <iostream>

#include "algorithms/algorithms.hpp"

namespace analyzer {
    
    void Yalex::read()
    {
        return;
    }

    Yalex::Yalex(std::string path)
    {
        char ch = 0;
        std::ifstream file {};
        file.open(path, std::ios_base::in);
        while (file.get(ch)){
            std::cout << ch;
            if (file.eof())
                break;
        }
            
        file.close();
    }


}