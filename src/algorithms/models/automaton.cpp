#include "automaton.hpp"

#include <stack>

namespace models {


Automaton::Automaton(
    std::map<int,int> acceptance,
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
const std::map<int,int> &Automaton::get_acceptance() { return acceptance; }
const std::set<char> &Automaton::get_symbols() { return symbols; }
const std::set<int> &Automaton::transition(int i, char c) { return transition_table.at(i).at(c); }
const std::vector<std::map<char, std::set<int>>>
&Automaton::get_table() { return transition_table; }

std::set<int> Automaton::e_closure(const int &state)
{
    if (!symbols.contains('$'))
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
    if (!symbols.contains('$'))
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

std::set<int> Automaton::simulate(std::string word, bool deterministic)
{
    using std::set, std::map;
    for (auto &c: word)
        if (!symbols.contains(c)) return {};

    set<int> states = deterministic ? 
        set<int>({0}): 
        e_closure(0);
    for (auto &c: word) {
        set<int> new_states = {};
        for (auto &s : states) {
            set<int> found_states = deterministic ? 
                move(states,c): 
                e_closure(move(states, c));
            new_states.merge(found_states);
        }
        if (new_states.empty()) return {};
        states = new_states;
    }

    for (auto &s: states)
        if (acceptance.contains(s)) 
        return {acceptance.at(s)};
    
    return {-1};
}



void Automaton::graph_automaton(char* name)
{
    using std::string;
    const std::map<char,std::string> non_printable = {
        {0, "NUL"},{1, "SOH"},{2, "STX"},{3, "ETX"},
        {4, "EOT"},{5, "ENQ"},{6, "ACK"},{7, "BEL"},
        {8, "BS"},{9, "HT"},{10, "LF"},{11, "VT"},
        {12, "FF"},{13, "CR"},{14, "SO"},{15, "SI"},
        {16, "DLE"},{17, "DC1"},{18, "DC2"},{19, "DC3"},
        {20, "DC4"},{21, "NAK"},{22, "SYN"},{23, "ETB"},
        {24, "CAN"},{25, "EM"},{26, "SUB"},{27, "ESC"},
        {28, "FS"},{29, "GS"},{30, "RS"},{31, "US"},
        {32, "(space)"},{127, "DEL"}
    };

    Agraph_t* G;
    GVC_t* gvc;

    gvc = gvContext();
    G = agopen(name, Agdirected, 0);

    agattr(G, AGNODE, string("shape").data(), string("circle").data());
    agattr(G, AGRAPH, string("rankdir").data(), string("LR").data());
    agattr(G, AGEDGE, string("label").data(), string("_").data());
    
    std::vector<Agnode_t*> nodes;
    for (int i = 0; i < size; i++)
    {
        char* node_name = std::to_string(i).data();
        Agnode_t *node = agnode(G, node_name, true);
        nodes.push_back(node);
        if (acceptance.contains(i)) 
            agset(node, string("shape").data(), string("doublecircle").data());
    }
    

    std::vector<Agedge_t*> edges;
    for (int i = 0; i < size; i++) {
        for (auto &c: symbols) {
            std::string edge_name = (non_printable.contains(c)) ?
                non_printable.at(c):
                std::string(1, c);

            if (transition_table[i].at(c).empty()) continue;
            for (auto &d: transition_table[i].at(c)) {
                Agedge_t *edge = agedge(G, nodes[i], nodes[d], edge_name.data(), true);
                agset(edge, string("label").data(), edge_name.data());
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
