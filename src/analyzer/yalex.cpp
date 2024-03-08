#include "yalex.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

namespace analyzer {

    enum Token {
        LET = 4,
        RULE = 9,
        ENTRY_OR = 11,
        O_COMMENT = 14,
        C_COMMENT = 17,
        DELIM = 21,
        ASSIGN = 23,
        IDENT = 128,
        REGEXP = 323,
        ACTION = 436,
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
            "|'|'#" //entry_or 17
            "|\"(*\"#" //comment open
            "|\"*)\"#" //comment close
            "|[\"\\t\\n\\s\"]#" //delim 21
            "|'='#" // assign 23
            "|['a'-'z''A'-'Z']+#" //ident 128
            "|(\"' '\"|['!'-'~'])+#" //regexp 317
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
        using std::string, std::set;
        std::ifstream archivo(filename, std::ios::in);

        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo." << std::endl;
            return -1;
        }

        // flags
        bool assign_ident = false;
        bool assign_entry = false;
        
        string buffer = "";
        while (!archivo.eof()) 
            buffer.push_back(archivo.get());
        
        archivo.close();

        set<int> expected = {LET,RULE,DELIM,O_COMMENT};

        string token = "";
        string ident_key = "";

        int acceptance = 0;

        int offset = 0;
        int npos = 0;
        while (buffer.size() > offset) {
            for (size_t i = offset; i < buffer.size(); i++) {
                char ch = std::abs(buffer.at(i));
                token.push_back(ch);
                set<int> result = dfa->simulate(token,true);
                if (result.empty()) 
                    continue;

                npos = i - offset + 1;
                acceptance = *result.begin();
            }


            if (expected.contains(C_COMMENT)) {
                if (acceptance == C_COMMENT) {
                    auto it = expected.find(C_COMMENT);
                    expected.erase(it);
                }
                token = "";
                offset += npos;
                continue;
            }
            
            if (assign_entry && !expected.contains(IDENT) && acceptance == IDENT) 
                acceptance = REGEXP;               
 
            if (!(acceptance == DELIM || acceptance == O_COMMENT))
                expected.clear();   

            token = buffer.substr(offset,npos);
            token.shrink_to_fit();
            offset += npos;

            switch (acceptance)
            {
            case LET:
                expected.insert({IDENT, DELIM, O_COMMENT});
                assign_ident = true;
                break;
            case RULE:
                expected.insert({IDENT, DELIM, O_COMMENT});
                assign_entry = true;
                break;
            case IDENT:
                expected.insert({ASSIGN, DELIM, O_COMMENT});
                if (assign_ident) {
                    ident_key = token;
                    idents[ident_key] = "";
                }
                if (assign_entry) 
                    entry_key = token;
                break;
            case ASSIGN:
                expected.insert({REGEXP, DELIM, O_COMMENT});
                break;
            case REGEXP:
                if (assign_ident) {
                    expected.insert({LET, RULE, DELIM, O_COMMENT});
                    idents[ident_key] = token;
                    assign_ident = false;
                }

                if (assign_entry) {
                    expected.insert({ENTRY_OR, DELIM, ACTION, O_COMMENT});
                    entrypoint.append(token);
                }
                break;
            case ENTRY_OR:
                expected.insert({REGEXP, DELIM, O_COMMENT});
                entrypoint.append(string("#|"));
                break;
            case O_COMMENT:
                expected.insert(C_COMMENT);
                break;
            case DELIM:
                break;
            case ACTION:
                // TODO
                break;
            default:
                break;
            }
            
            token = "";
        }
        
        entrypoint = replace_idents(entrypoint);
        return 0;
    }

    const std::string &Yalex::get_entrypoint() {
        return entrypoint;
    }
    

}