#include "yalex.hpp"

#include <fstream>
#include <iostream>

namespace analyzer {
    
    void Yalex::read(std::string filename)
    {
        std::ifstream archivo("archivo.txt");

        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo." << std::endl;
            return;
        }

        char ch;
        while (archivo.eof()) {
            // Mostrar el carácter leído
            archivo.get(ch);
            
        }

        archivo.close();
   
        return;
    }

    Yalex::Yalex()
    {
        std::string yalex_tokens = 
            "\"let\"#|"
            "\"rule\"#|"
            "[\"\\t\\n\\s\"]#|" //delim
            "\"[a-zA-Z]+#|" //ident
            "\\\"_+\\\""; //regexp

        std::unique_ptr<algorithms::AutomataCreator> fa_creator {};
        fa_creator.reset(new algorithms::Direct(yalex_tokens));

        dfa = fa_creator->create_automata();
    }


}