#include "minimize.hpp"

#include <algorithm>
#include <list>

namespace algorithms {

    typedef std::vector<std::set<int>> partition;
    using std::set, std::map;


    partition Minimize::new_partition (partition part)
    {
        partition new_part {};
        for (auto &group: part) {
            map<set<int>, set<int>> new_groups {};
            for (auto &s: group) {
                set<int> transitions {};
                for (auto &c: automaton->get_symbols()){
                    set<int> found = automaton->get_table().at(s).at(c);
                    transitions.merge(found);
                }
                if (new_groups.find(transitions) == new_groups.end()) {
                    new_groups.insert({transitions, {s}});
                } else {
                    set<int> *group = &new_groups.at(transitions);
                    group->insert(s);
                }
            }   

            for (auto iter = new_groups.begin(); iter != new_groups.end(); ++iter)
                new_part.push_back(iter->second);
        }
        return new_part;  
    }

    Minimize::Minimize(models::Automaton *automaton)
    {
        this->automaton = automaton;
    }


    std::unique_ptr<models::Automaton> Minimize::create_automata()
    {

        // initial partition
        std::set<int> acceptance = automaton->get_acceptance();
        std::set<int> non_acceptance {};
        for (size_t i = 0; i < automaton->get_size(); i++)
            if (acceptance.find(i) == acceptance.end())
                non_acceptance.insert(i);


        partition part {};
        partition temp_part {non_acceptance, acceptance};
        while (part != temp_part)
        {
            part = temp_part;
            temp_part = new_partition(part);
        }

        //initialize transition table
        typedef std::vector<std::map<char, std::set<int>>> table;
        table transition_table {};
        for (size_t i = 0; i < part.size(); i++) {
            map<char,set<int>> init {};
            for (auto &c: automaton->get_symbols()) {
                init[c] = {};
            }
            transition_table.push_back(init);
        }
        
        // Make transitions
        for (int i = 0; i < part.size(); i++) //origin states
        {
            set<int> group = part.at(i);
            int state = *group.begin();

            for (auto &c: automaton->get_symbols())
            {
                int found = *automaton->transition(state, c).begin();
                //find destiny state
                for (int j = 0; j < part.size(); j++){
                    set<int> match = part.at(j);
                    if (match.find(found) != match.end()) {
                        transition_table.at(i)[c] = {j};
                        break;
                    }
                }
            }
        }

        //find new acceptance states
        set<int> new_acceptance {};
        for (int i = 0; i < part.size(); i++)
        {
            set<int> state = part.at(i);
            if (
                std::includes(
                    acceptance.begin(), acceptance.end(),
                    state.begin(), state.end()
                )
            )   new_acceptance.insert(i);
        }

        return  std::make_unique<models::Automaton>
                (new_acceptance,automaton->get_symbols(), transition_table);
    }
}