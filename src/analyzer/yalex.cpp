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
        REGEXP = 317,
        ACTION = 430
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
            "|[' '-'~']+#" //regexp 379
            "|{[\"\\t\\n\\s\"'a'-'z''A'-'Z']+}#" //action 638
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
        int error = 0;

        std::string token = "";
        std::string ident_key = "";
        std::string entry_key = "";

        int prev_acceptance = 0;
        while (!archivo.eof()) {

            if (error) {
                std::cerr << "Error sintactico: " << token << std::endl;
                archivo.close();
                return error;
            }

            char ch = archivo.get();
            token.push_back(ch);

            if (ignore && token.ends_with("*)")) {
                token = ""; ignore = false;
                continue;
            }

            if (ignore) continue;

            std::set<int> result = dfa->simulate(token,true);
            if (result.empty()) 
                continue;
            
            std::string temp_token = token;
            temp_token.push_back(archivo.peek());
            std::set<int> temp_result = dfa->simulate(temp_token,true);

            if (!temp_result.empty()) 
                continue;

            int acceptance = *result.begin();
            switch (acceptance)
            {
            case LET:
                assign_ident = true;
                break;
            case RULE:
                assign_entry = true;
                break;
            case O_COMMENT:
                ignore = true;
                break;
            case C_COMMENT:
                ignore = false;
                break;
            case ENTRY_OR:
                if (!(prev_acceptance == REGEXP))
                    entrypoint.append("#" + token);
                break;
            case ASSIGN:
                if (!(prev_acceptance == IDENT && (assign_ident || assign_entry)))
                    error = 1;
                break;
            case IDENT:
                if (assign_ident && !assign_entry) {
                    idents[token] = "";
                    ident_key = token;   
                }
                if (assign_entry && !assign_ident)
                    entry_key = token;
                break;
            case REGEXP:
                if (assign_ident) {
                    idents[ident_key] = token;
                    assign_ident = false;
                }

                if (assign_entry)
                    entrypoint.append(token);
                break;
            case ACTION:
                // TODO: implement action
                break;
            
            default:
                break;
            }

            token = "";
            prev_acceptance = acceptance != DELIM ? acceptance : prev_acceptance;
        }

        archivo.close();

        entrypoint = replace_idents(entrypoint);
        return 0;
    }

    const std::string &Yalex::get_entrypoint(){
        return entrypoint;
    }
    

}