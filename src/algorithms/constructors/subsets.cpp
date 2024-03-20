#include "subsets.hpp"

#include <stack>
#include <algorithm>
#include <iterator>

namespace algorithms {

    std::unique_ptr<models::Automaton> nfa_to_dfa (models::Automaton *automaton)
    {
        typedef std::vector<std::map<char, std::set<int>>> table;
        using std::set, std::map, std::vector;

        const table transition_table = automaton->get_table();
        const map<int,int> acceptance = automaton->get_acceptance();

        set<char> symbols = automaton->get_symbols();
        symbols.extract('$');
        map<int,int> new_acceptance {};
        table new_table {};

        vector<set<int>> found_states {};
        found_states.push_back(automaton->e_closure(0));

        int mark = 0;
        while (mark < found_states.size())
        {
            set<int> marked_state = found_states.at(mark);
            map<char, set<int>> new_tran {};
            for (auto &c : symbols)
            {
                set<int> new_state = automaton->e_closure(automaton->move(marked_state, c));
                if (new_state.empty()) {
                    new_tran[c] = {};
                    continue;
                };
                auto iter = std::find(found_states.begin(), found_states.end(), new_state);
                int index = std::distance(found_states.begin(), iter);
                if (index == found_states.size())
                {
                    found_states.push_back(new_state);
                }
                new_tran[c] = { index };

            }
            if (std::includes(marked_state.begin(), marked_state.end(),
            acceptance.begin(), acceptance.end())) 
                new_acceptance.insert(mark, mark);
                
            new_table.push_back(new_tran);
            mark++;
        }
        
        return std::make_unique<models::Automaton>(new_acceptance, symbols, new_table);
    }

}