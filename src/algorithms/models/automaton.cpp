#include "automaton.hpp"

#include <stack>

namespace models {


Automaton::Automaton(
    std::set<int> acceptance,
    std::set<char> symbols,
    std::vector<std::map<char, std::set<int>>> 
    transition_table)
{   

    this->size = transition_table.size();
    this->acceptance = acceptance;
    this->symbols = symbols;
    this->transition_table = transition_table;
    
}

const int &Automaton::get_size() { return size; }
const std::set<int> &Automaton::get_acceptance() { return acceptance; }
const std::set<char> &Automaton::get_symbols() { return symbols; }
const std::set<int> &Automaton::transition(int i, char c) { return transition_table.at(i).at(c); }
const std::vector<std::map<char, std::set<int>>>
&Automaton::get_table() { return transition_table; }

std::set<int> Automaton::e_closure(const int &state)
    {
        if (symbols.find('$') == symbols.end())
            return {state};

        using std::set, std::stack;

        set<int> lock {state};
        stack<int> s_stack {};
        s_stack.push(state);
        while (!s_stack.empty()) {
            set<int> found = transition(s_stack.top(),'$');

            s_stack.pop();
            for (auto &s: found) if (lock.find(s) == lock.end()) {
                s_stack.push(s);
                lock.insert(s);
            }
        }
        
        return lock;
    }

    std::set<int> Automaton::e_closure(const std::set<int> &states) 
    {
        if (symbols.find('$') == symbols.end())
            return {states};

        using std::set;

        set<int> lock {states};
        for (auto &s: states) {
            set<int> found = e_closure(s);
            lock.merge(found);
        }

        return lock;    
    }

    std::set<int> Automaton::move(const std::set<int> &states, const char &c)
    {
        using std::set;
    
        set<int> next {};
        for (auto &s: states) {
            set<int> found = transition(s,c);
            next.merge(found);
        }

        return next;   
    }

bool Automaton::simulate(std::string word)
{
    using std::set, std::map;
    for (auto &c: word)
        if (symbols.find(c) == symbols.end()) return false;

    set<int> states = e_closure(0);
    for (auto &c: word) {
        set<int> new_states = {};
        for (auto &s : states) {
            set<int> found_states = e_closure(move(states, c));
            new_states.merge(found_states);
        }
        if (new_states.empty()) return false;
        states = new_states;
    }

    for (auto &s: states)
        if (acceptance.find(s) != acceptance.end()) return true;
    
    return false;
}



void Automaton::graph_automaton(char* name)
{
    Agraph_t* G;
    GVC_t* gvc;

    gvc = gvContext();
    G = agopen(name, Agdirected, 0);

    agattr(G, AGNODE, "shape", "circle");
    agattr(G, AGRAPH, "rankdir", "LR");
    agattr(G, AGEDGE, "label", "_");
    
    std::vector<Agnode_t*> nodes;
    for (int i = 0; i < size; i++)
    {
        char* node_name = std::to_string(i).data();
        Agnode_t *node = agnode(G, node_name, true);
        nodes.push_back(node);
        if (acceptance.find(i) != acceptance.end()) 
            agset(node, "shape", "doublecircle");
    }
    

    std::vector<Agedge_t*> edges;
    for (int i = 0; i < size; i++) {
        for (auto &c: symbols) {
            std::string edge_name(1,c);
            if (transition_table[i].at(c).empty()) continue;
            for (auto &d: transition_table[i].at(c)) {
                Agedge_t *edge = agedge(G, nodes[i], nodes[d], edge_name.data(), true);
                agset(edge, "label", edge_name.data());
                edges.push_back(edge);
            }
        }
    }

    gvLayout(gvc, G, "dot");
    gvRenderFilename(gvc, G, "png", name);
    gvFreeLayout(gvc, G);
    gvFreeContext(gvc);
    agclose(G);
}

}
