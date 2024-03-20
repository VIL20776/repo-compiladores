#include "./direct.hpp"

#include <algorithm>
#include <iterator>

#include "models/syntax_tree.hpp"

namespace algorithms {

    std::unique_ptr<models::Automaton> regex_to_dfa (std::string expression)
    {
        typedef std::vector<std::map<char, std::set<int>>> table;
        using std::set, std::map, std::vector;

        models::Syntax_tree tree (expression + "\x80&");

        set<int> acceptance = tree.sharp();

        set<char> symbols = tree.get_symbols();
        map<int,char> value_map = tree.get_values();

        map<int,int> new_acceptance {};
        table new_table {};

        vector<set<int>> found_states {};
        found_states.push_back(tree.first_pos());

        int mark = 0;
        while (mark < found_states.size())
        {
            set<int> marked_state = found_states.at(mark);
            map<char, set<int>> new_tran {};
            for (auto &c : symbols)
            {
                set<int> new_state {};
                for (auto &s :marked_state)
                    if (value_map.at(s) == c) 
                        new_state.merge(tree.follow_pos(s));

                if (new_state.empty()) {
                    new_tran[c] = {};
                    continue;
                }

                auto iter = std::find(found_states.begin(), found_states.end(), new_state);
                int index = std::distance(found_states.begin(), iter);
                if (index == found_states.size())
                {
                    found_states.push_back(new_state);
                }
                new_tran[c] = { index };

            }

            vector<int> intersection = {};
            std::set_intersection(
                marked_state.begin(), marked_state.end(),
                acceptance.begin(), acceptance.end(),
                std::back_inserter(intersection)
            );
            if (!intersection.empty()) {
                int id = intersection.at(0);
                new_acceptance.insert({mark, id});
            }
                
            new_table.push_back(new_tran);
            mark++;
        }
        
        
        return std::make_unique<models::Automaton>(new_acceptance, symbols, new_table);
    }

}