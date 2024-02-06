#include "subsets.hpp"

#include <stack>

namespace algorithms {

    std::set<int> e_closure(const int s,
    const std::vector<std::map<char, std::set<int>>> &transition_table)
    {
        using std::set, std::stack;

        set<int> lock {};
        stack<int> s_stack {};
        s_stack.push(s);
        while (!s_stack.empty()) {
            set<int> found = transition_table.at(s_stack.top()).at('$');

            s_stack.pop();
            for (auto &s: found) if (lock.find(s) == lock.end()) {
                s_stack.push(s);
            }
            lock.insert(s);
        }
        
        return lock;
    }

    std::set<int> e_closure(std::set<int> states, 
    const std::vector<std::map<char, std::set<int>>> &trasition_table)
    {
        using std::set;

        set<int> lock {states};
        for (auto &s: states) {
            set<int> found = e_closure(s, trasition_table);
            lock.insert(found.begin(), found.end());
        }

        return lock;    
    }

    std::set<int> move(std::set<int> states, char c, 
    const std::vector<std::map<char, std::set<int>>> transition_table)
    {
        using std::set;
    
        set<int> lock {states};
        for (auto &s: states) {
            set<int> found = transition_table.at(s).at(c);
            lock.insert(found.begin(), found.end());
        }

        return lock;   
    }



}