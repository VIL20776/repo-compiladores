#include "yalex.hpp"

#include <fstream>
#include <iostream>

namespace analyzer {

    enum Token {
        LET = 4,
        RULE = 9,
        O_COMMENT = 12,
        C_COMMENT = 15,
        ENTRY_OR = 17,
        DELIM = 21,
        ASSIGN = 23,
        IDENT = 128,
        REGEXP = 412,
        ACTION = 525
    };

    std::string Yalex::replace_idents(const std::string &entrypoint)
    {
        std::string entry = entrypoint;
        for (auto iter = idents.begin(); iter != idents.end(); ++iter) {
            size_t pos = entry.find(iter->first);
            while (pos != std::string::npos) {
                entry.replace(pos, iter->first.length(), iter->second);
                pos = entry.find(iter->first, pos + iter->second.length());
            }
        }
        if (entry != entrypoint)
            return replace_idents(entry);
        
        return entry;
    }

    Yalex::Yalex()
    {
        std::string yalex_tokens =  
            "\"let\"#" // let 4
            "|\"rule\"#" // rule 9
            "|\"(*\"#" //open comment 12
            "|\"*)\"#" //close comment 15
            "|'|'#" //entry_or 17
            "|[\"\\t\\n\\s\"]#" //delim 21
            "|'='#" // assign 23
            "|['a'-'z''A'-'Z']+#" //ident 128
            "|['!'-'~']((\\s['!'-'~'])|['!'-'~'])*#" //regexp 317
            "|{[\"\\t\\n\\s\"'a'-'z''A'-'Z']+}#" //action 430
            ;

        yalex_tokens = algorithms::to_standard(yalex_tokens);
        yalex_tokens = algorithms::regex_to_postfix(yalex_tokens);
        std::unique_ptr<algorithms::AutomataCreator> fa_creator {};
        fa_creator.reset(new algorithms::Direct(yalex_tokens));

        dfa = fa_creator->create_automata();
    }
    
    int Yalex::compile(std::string filename)
    {
        std::ifstream archivo(filename, std::ios::in);

        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo." << std::endl;
            return -1;
        }

        // flags
        bool ignore = false;
        bool assign_ident = false;
        bool assign_entry = false;
        int expected = 0;
        int error = 0;

        std::string token = "";
        std::string ident_key = "";
        std::string entry_key = "";

        int acceptance = 0;
        int prev_acceptance = 0;
        while (!archivo.eof()) {

            if (error) {
                std::cerr << "Error sintactico: " << token << std::endl;
                archivo.close();
                return error;
            }

            char ch = archivo.get();
            token.push_back(ch);

            if (token.starts_with("(*")) {
                while (!token.ends_with("*)")) {
                    ch = archivo.get();
                    token.push_back(ch);
                }
                token = "";
                continue;
            }

            std::set<int> result = dfa->simulate(token,true);
            while (acceptance != expected || result.empty()) {
                if (result.empty()) {
                    if (acceptance == prev_acceptance) {
                        error = 1;
                        break;
                    }
                    prev_acceptance = acceptance;
                    token.pop_back();
                    result = dfa->simulate(token,true);
                    continue;
                }
                prev_acceptance = acceptance;
                token.push_back(archivo.get());
                result = dfa->simulate(token,true);
            }
            
            
            acceptance = *result.begin();
            if (acceptance == DELIM) {
                token = "";
                continue;
            }

        }

        archivo.close();

        entrypoint = replace_idents(entrypoint);
        return 0;
    }

    const std::string &Yalex::get_entrypoint(){
        return entrypoint;
    }
    

}