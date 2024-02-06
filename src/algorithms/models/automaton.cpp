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

bool Automaton::check(std::string word)
{
    using std::set, std::map;
    for (auto &c: word)
        if (symbols.find(c) == symbols.end()) return false;

    set<int> states = {0};
    for (auto &c: word) {
        set<int> new_states = {};
        for (auto &s : states) {
            set<int> found_states = transition_table.at(s).at(c);
            new_states.insert(found_states.begin(), found_states.end());
        }
        states = new_states;
    }

    for (auto &s: states)
        if (acceptance.find(s) == acceptance.end()) return false;
    
    return true;
}



void Automaton::graph_automaton(char* name)
{
    Agraph_t* G;
    GVC_t* gvc;

    gvc = gvContext();
    G = agopen(name, Agdirected, 0);

    agattr(G, AGNODE, "shape", "circle");
    agattr(G, AGRAPH, "rankdir", "LR");
    agattr(G, AGEDGE, "label", "__");
    
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
        for (char c: symbols) {
            char* edge_name = &c;
            if (transition_table[i].at(c).empty()) continue;
            for (auto &d: transition_table[i].at(c)) {
                Agedge_t *edge = agedge(G, nodes[i], nodes[d], edge_name, true);
                agset(edge, "label", edge_name);
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
