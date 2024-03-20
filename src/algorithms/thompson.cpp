#include "thompson.hpp"

#include <algorithm>
#include <stack>

namespace algorithms {

    typedef std::vector<std::map<char, std::set<int>>> table;
    using std::vector, std::map, std::set;

    struct transition
    {
        int origin, destiny;
        char symbol;
    };

    vector<transition> kleene (const vector<transition> &deltas)
    {
        vector<transition> result {{0, 1, '$'}};

        int max_state = 0; 
        for (auto &t : deltas) {
            result.push_back({t.origin + 1, t.destiny+ 1, t.symbol});
            max_state = std::max({max_state, t.origin + 1, t.destiny + 1});
        }

        vector<transition> new_trasitions = {
            {max_state, 1, '$'},
            {0, max_state + 1, '$'},
            {max_state, max_state + 1, '$'},
        };

        result.insert(result.end(), new_trasitions.begin(), new_trasitions.end());

        return result;
    }
    
    vector<transition> concat 
    (const vector<transition> &deltas1, const vector<transition> &deltas2)
    {
        vector<transition> result {};

        result.insert
        (result.end(), deltas1.begin(), deltas1.end());

        int max = 0;
        for (auto &t: deltas1)
            max = std::max({max, t.origin, t.destiny});

        for (auto &t: deltas2)
            result.push_back({t.origin + max, t.destiny + max, t.symbol});

        return result;
    }

    vector<transition> unite 
    (const vector<transition> &deltas1, const vector<transition> &deltas2)
    {
        vector<transition> result {{0, 1, '$'}};

        int temp_max = 0; 
        for (auto &t : deltas1) {
            result.push_back({t.origin + 1, t.destiny + 1, t.symbol});
            temp_max = std::max({temp_max, t.origin + 1, t.destiny + 1});
        }

        int offset = temp_max + 1;
        int max = offset;
        result.push_back({0, max, '$'});
        for (auto &t : deltas2) {
            result.push_back({t.origin + offset, t.destiny + offset, t.symbol});
            max = std::max({max, (t.origin + offset), (t.destiny + offset)});
        }

        vector<transition> new_trasitions {
            {temp_max, max + 1, '$'},
            {max, max + 1, '$'}
        };

        result.insert(result.end(), new_trasitions.begin(), new_trasitions.end());
        return result;
    }

    vector<transition> list_transitions (std::string expression)
    {
        using std::vector;

        std::stack<vector<transition>> transitions {};
        for (auto &c: expression)
        {
            vector<transition> d1 {};
            vector<transition> d2 {};
            switch (c)
            {
            case '&':
                d2 = transitions.top(); transitions.pop();
                d1 = transitions.top(); transitions.pop();

                transitions.push(concat(d1, d2));
                break;
                
            case '|':
                d2 = transitions.top(); transitions.pop();
                d1 = transitions.top(); transitions.pop();

                transitions.push(unite(d1, d2));
                break;

            case '*':
                d1 = transitions.top(); transitions.pop();

                transitions.push(kleene(d1));
                break;

            default:
                transitions.push({{0, 1, c}});
                break;
            }
        }

        return transitions.top();
    }

    std::unique_ptr<models::Automaton> regex_to_nfa(const std::string &expression)
    { 
        using std::vector;
        vector<transition> t_vector = list_transitions(expression);

        int states = 0;
        set<char> symbols {};
        for (auto &t: t_vector) {
            states = std::max({states, t.origin, t.destiny});
            symbols.insert(t.symbol);
        }
        
        //Transition table initialization
        table transition_table {};
        for (size_t i = 0; i < (states + 1); i++) {
            map<char,set<int>> init {};
            for (auto &c: symbols) {
                init[c] = {};
            }
            transition_table.push_back(init);
        }

        for (auto &t: t_vector) {
            map<char, set<int>> *t_map = &transition_table.at(t.origin);
            if (t_map->find(t.symbol) == t_map->end()) {
                t_map->insert({t.symbol, {t.destiny}});
                continue;
            }

            set<int> *dest_set = &t_map->at(t.symbol);
            dest_set->insert(t.destiny);
        }

        return std::make_unique<models::Automaton>(map<int,int>({ states, states }), symbols, transition_table);
    }


    
  
}