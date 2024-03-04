#include "direct.hpp"

#include <algorithm>
#include <iterator>

#include "algorithms/models/syntax_tree.hpp"

namespace algorithms {

    Direct::Direct(std::string expression): 
        tree(expression + "#&") {};

    std::unique_ptr<models::Automaton> Direct::create_automata ()
    {
        typedef std::vector<std::map<char, std::set<int>>> table;
        using std::set, std::map, std::vector;

        set<int> acceptance = tree.sharp();

        set<char> symbols {};
        map<int,char> value_map = tree.get_values();
        for (auto iter = value_map.begin(); iter != value_map.end(); ++iter)
            symbols.insert(iter->second);
        symbols.extract('#');

        set<int> new_acceptance {};
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
            if (!intersection.empty()) 
                new_acceptance.insert(mark);
                
            new_table.push_back(new_tran);
            mark++;
        }
        
        
        return std::make_unique<models::Automaton>(new_acceptance, symbols, new_table);
    }

}