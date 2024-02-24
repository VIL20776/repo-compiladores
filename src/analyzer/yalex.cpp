#include "yalex.hpp"

#include <fstream>

#include "algorithms/algorithms.hpp"

namespace analyzer {
    
    void Yalex::read()
    {
        

    }

    Yalex::Yalex(std::string path)
    {
        std::ifstream file {};
        file.open(path, std::ios_base::in);
        if (file.is_open())
            file >> yal_content;
        file.close();
    }


}