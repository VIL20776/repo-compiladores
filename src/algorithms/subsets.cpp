#include "subsets.hpp"

#include <stack>
#include <algorithm>
#include <iterator>

namespace algorithms {

    std::set<int> Subsets::e_closure(const int &state)
    {
        using std::set, std::stack;

        set<int> lock {state};
        stack<int> s_stack {};
        s_stack.push(state);
        while (!s_stack.empty()) {
            set<int> found = automaton->get_table().at(s_stack.top()).at('$');

            s_stack.pop();
            for (auto &s: found) if (lock.find(s) == lock.end()) {
                s_stack.push(s);
                lock.insert(s);
            }
        }
        
        return lock;
    }

    std::set<int> Subsets::e_closure(const std::set<int> &states) 
    {
        using std::set;

        set<int> lock {states};
        for (auto &s: states) {
            set<int> found = e_closure(s);
            lock.insert(found.begin(), found.end());
        }

        return lock;    
    }

    std::set<int> Subsets::move(const std::set<int> &states, const char &c)
    {
        using std::set;
    
        set<int> lock {};
        for (auto &s: states) {
            set<int> found = automaton->get_table().at(s).at(c);
            lock.insert(found.begin(), found.end());
        }

        return lock;   
    }

    Subsets::Subsets(models::Automaton *automaton)
    {
        this->automaton = automaton;
    }

    models::Automaton *Subsets::create_automata ()
    {
        typedef std::vector<std::map<char, std::set<int>>> table;
        using std::set, std::map, std::vector;

        const table transition_table = automaton->get_table();
        const set<int> acceptance = automaton->get_acceptance();

        set<char> symbols = automaton->get_symbols();
        symbols.extract('$');
        set<int> new_acceptance {};
        table new_table {};

        vector<set<int>> found_states {};
        found_states.push_back(e_closure(0));

        int mark = 0;
        while (mark < found_states.size())
        {
            set<int> marked_state = found_states.at(mark);
            map<char, set<int>> new_tran {};
            for (auto &c : symbols)
            {
                set<int> new_state = e_closure(move(marked_state, c));
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
                new_acceptance.insert(mark);
                
            new_table.push_back(new_tran);
            mark++;
        }
        
        return new models::Automaton {new_acceptance, symbols, new_table};
    }

}