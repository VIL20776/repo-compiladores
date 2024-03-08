#include "yalex.hpp"

#include <fstream>
#include <iostream>

namespace analyzer {

    enum Token {
        LET = 4,
        RULE = 9,
        ENTRY_OR = 11,
        DELIM = 15,
        ASSIGN = 17,
        IDENT = 122,
        REGEXP = 314,
        ACTION = 427,
        COMMENT = 560
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
            "|[\"\\t\\n\\s\"]#" //delim 21
            "|'='#" // assign 23
            "|['a'-'z''A'-'Z']+#" //ident 128
            "|(' '|['!'-'~'])+\\n#" //regexp 317
            "|{[\"\\t\\n\\s\"'a'-'z''A'-'Z']+}#" //action 430
            "|\"(*\"(_)*\"*)\"#" //comment
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

        set<int> expected = {LET,RULE,DELIM,COMMENT};

        string token = "";
        string ident_key = "";

        int acceptance = 0;

        int npos = 0;
        int buf_len = buffer.size(); 
        while (buf_len > npos) {
            for (size_t i = npos; i < buf_len; i++) {
                token.push_back(buffer.at(i));
                set<int> result = dfa->simulate(token,true);
                if (result.empty()) 
                    continue;

                npos = i;
                acceptance = *result.begin();
            }

            npos++;

            if (!(acceptance == DELIM || acceptance == COMMENT))
                expected.clear();

            token = buffer.substr(0,npos);
            token.shrink_to_fit();

            switch (acceptance)
            {
            case LET:
                expected.insert({IDENT, DELIM, COMMENT});
                assign_ident = true;
                break;
            case RULE:
                expected.insert({IDENT, DELIM, COMMENT});
                assign_entry = true;
            case IDENT:
                expected.insert({ASSIGN, DELIM, COMMENT});
                if (assign_ident) {
                    ident_key = token;
                    idents[ident_key] = "";
                }
                if (assign_entry) 
                    entry_key = token;
                break;
            case ASSIGN:
                expected.insert({REGEXP, DELIM, COMMENT});
                break;
            case REGEXP:
                if (assign_ident) {
                    expected.insert({LET, RULE, DELIM, COMMENT});
                    idents[ident_key] = token;
                    assign_ident = false;
                }

                if (assign_entry) {
                    expected.insert({ENTRY_OR, DELIM, ACTION, COMMENT});
                    entrypoint.append(token);
                }
                break;
            case ENTRY_OR:
                expected.insert({REGEXP, DELIM, COMMENT});
                entrypoint.append(string("#|") + token);
                break;
            case COMMENT:
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

    const std::string &Yalex::get_entrypoint(){
        return entrypoint;
    }
    

}