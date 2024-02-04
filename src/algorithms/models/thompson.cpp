#include "thompson.hpp"

#include <algorithm>
#include <stack>

namespace algorithms {

    struct transition
    {
        int origin, destiny;
        char symbol;
    };

    namespace {
        using std::vector;


        vector<transition> kleene (const vector<transition> &deltas)
        {
            vector<transition> result {{0, 1, '$'}};

            int max_state = 0; 
            for (auto &t : deltas) {
                result.push_back({t.origin + 1, t.destiny+ 1, t.symbol});
                max_state = std::max({max_state, t.origin, t.destiny});
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

            result.push_back({max, max + 1, '$'});

            max += 1;
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

            int max = temp_max + 1;
            result.push_back({0, max, '$'});
            for (auto &t : deltas2) {
                result.push_back({t.origin + temp_max, t.destiny + temp_max, t.symbol});
                max = std::max({max, t.origin + temp_max, t.destiny});
            }

            vector<transition> new_trasitions {
                {temp_max, max + 1, '$'},
                {max, max + 1, '$'}
            };

            result.insert(result.end(), new_trasitions.begin(), new_trasitions.end());
        }
    }

    std::vector<std::map<char, std::set<int>>>
    algorithms::thompson (std::string expression)
    {
        using std::vector, std::map, std::set;
        typedef std::vector<std::map<char, std::set<int>>> table;

        std::stack<vector<transition>> transitions {};
        for (auto &c: expression)
        {
            vector<transition> d1 {};
            vector<transition> d2 {};
            switch (c)
            {
            case '.':
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

        //build the transition table
        vector<transition> t_vector = transitions.top();

        int states = 0;
        for (auto &t: t_vector) 
            states = std::max(states, t.origin, t.destiny);
        
        table transition_table (states + 1);
        for (auto &t: t_vector) {
            map<char, set<int>> *t_map = &transition_table.at(t.origin);
            if (!t_map->contains(t.symbol)) {
                t_map->insert({t.symbol, {t.destiny}});
                continue;
            }

            set<int> *dest_set = &t_map->at(t.symbol);
            dest_set->insert(t.destiny);
        }

        return transition_table;
    }
  
}