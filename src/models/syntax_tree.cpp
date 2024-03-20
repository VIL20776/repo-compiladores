#include "syntax_tree.hpp"

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>

namespace models {

    using std::set, std::map, std::string, std::stack;
    using S_tree = Syntax_tree;

    // Syntax tree node methods
    S_tree::node::node()
    {
        value = 0; position = 0;
        root = nullptr; left = root; right = root;
    }
    
    S_tree::node::node(char value, int position)
    {
        this->value = value;
        this->position = position;
        root = nullptr; left = root; right = root;
    }

    void S_tree::node::append(S_tree::node *node, S_tree::node::leaf side)
    {
        node->root = this;
    
        switch(side) {
        case LEFT:
            this->left = node;
            break;

        case RIGHT:
            this->right = node;
            break;
        }
    }


    S_tree::Syntax_tree(string regex)
    {
        values = {};

        stack<node*> nodes {}; 
        int position = 1;
        bool literal = false; bool concat = false;
        for (size_t i = 0; i < regex.size(); i++){
            char c = regex.at(i);
            if (literal && !(c == '\"')) {
                nodes.push(new node(c, position));
                values.insert({position, c});
                symbols.insert(c);
                position++;
                if (concat) {
                    node *root = new node();
                    root->append(nodes.top(), node::RIGHT); nodes.pop();
                    root->append(nodes.top(), node::LEFT); nodes.pop();
                    root->value = '&';
                    nodes.push(root);
                }
                concat = true;
                continue;
            }

            concat = false;
            node *root = new node();
            switch (c)
            {
            case '|':
            case '&':
                root->append(nodes.top(), node::RIGHT); nodes.pop();
                root->append(nodes.top(), node::LEFT); nodes.pop();
                root->value = c;
                nodes.push(root);
                break;
            case '*':
                root->append(nodes.top(), node::LEFT); nodes.pop();
                root->value = c;
                nodes.push(root);
                break;
            case '$':
                delete root;
                nodes.push(new node(c, 0));
                break;
            case '\'':
                delete root;
                c = regex.at(++i);
                nodes.push(new node(c, position));
                values.insert({position, c});
                symbols.insert(c);
                position++; i++;
                break;
            case '\"':
                delete root;
                literal = !literal;
                break;
            default:
                delete root;
                nodes.push(new node(c, position));
                if (c == '\x80') 
                    sharp_pos.insert(position);
                else
                    symbols.insert(c);

                values.insert({position, c});
                position++;
                break;
            }
        }
        root = nodes.top();
    }

    S_tree::~Syntax_tree()
    {
        if (root != nullptr) {
            release_nodes(root);
            root = nullptr;
        }
    }

    S_tree::node *const S_tree::find_node(S_tree::node *node, int i)
    {
        if (node != nullptr) {
            if (node->position == i)
                return node;
            
            S_tree::node *temp;
            temp = find_node(node->left, i);
            if (temp != nullptr) return temp;

            temp = find_node(node->right, i);
            if (temp != nullptr) return temp;
        }

        return nullptr;      
    }

    void S_tree::release_nodes(node *current)
    {
        if (current == nullptr)
            return;
        
        release_nodes(current->left);
        release_nodes(current->right);
        delete current;
    }

    bool S_tree::nullable(S_tree::node *node)
    {
        if (node->position > 0)
            return false;
            
        switch (node->value)
        {
        case '*': case '$':
            return true;
            
        case '|':
            return (nullable(node->left) | nullable(node->right));

        case '&':
            return (nullable(node->left) & nullable(node->right));

        default:
            return false;
        }
    }

    set<int> S_tree::first_pos(S_tree::node *node)
    {
        if (node->position > 0)
            return {node->position};

        set<int> first_pos {};
        switch (node->value)
        {
        case '$':
            return first_pos;
        
        case '|':
            first_pos.merge(this->first_pos(node->left));
            first_pos.merge(this->first_pos(node->right));
            return first_pos;
        case '&':
            if (nullable(node->left))
            {
                first_pos.merge(this->first_pos(node->left));
                first_pos.merge(this->first_pos(node->right));
                return first_pos;
            } else 
                return this->first_pos(node->left);
        
        case '*':
            return this->first_pos(node->left);

        default:
            return {node->position};

        }
    }

    set<int> S_tree::last_pos(node *node)
    {
        if (node->position > 0)
            return {node->position};

        set<int> last_pos {};
        switch (node->value)
        {
        case '$':
            return last_pos;
        
        case '|':
            last_pos.merge(this->last_pos(node->left));
            last_pos.merge(this->last_pos(node->right));
            return last_pos;
        case '&':
            if (this->nullable(node->right))
            {
                last_pos.merge(this->last_pos(node->left));
                last_pos.merge(this->last_pos(node->right));
                return last_pos;
            } else 
                return this->last_pos(node->right);
        
        case '*':
            return this->last_pos(node->left);
        
        default:
            return {node->position};
        }
    }

    std::set<int> S_tree::first_pos() { return first_pos(root); }

    set<int> S_tree::follow_pos(int i)
    {
        set<int> follow_pos {};
        S_tree::node *origin = find_node(root, i);
        S_tree::node *temp = origin->root;

        while (temp != nullptr) {
            set<int> check {};
            if (temp->position == 0)
            switch (temp->value)
            {
            case '&':
                check = last_pos(temp->left);

                if (check.contains(i))
                    follow_pos.merge(first_pos(temp->right));
                break;
            
            case '*':
                check = last_pos(temp);
                if (check.contains(i))
                    follow_pos.merge(first_pos(temp));
                break;
            
            default:
                break;
            }

            temp = temp->root;
        }

        return follow_pos;
    }

    const std::map<int,char> &S_tree::get_values() { return values; }
    const std::set<char> &S_tree::get_symbols() { return symbols; }
    std::set<int> S_tree::sharp() { return sharp_pos; }

    void S_tree::draw_syntax_tree(std::string name)
    {
        using std::string;
        const std::map<char, std::string> non_printable = {
            {0, "NUL"},  {1, "SOH"},  {2, "STX"},  {3, "ETX"},
            {4, "EOT"},  {5, "ENQ"},  {6, "ACK"},  {7, "BEL"},
            {8, "BS"},   {9, "HT"},   {10, "LF"},  {11, "VT"},
            {12, "FF"},  {13, "CR"},  {14, "SO"},  {15, "SI"},
            {16, "DLE"}, {17, "DC1"}, {18, "DC2"}, {19, "DC3"},
            {20, "DC4"}, {21, "NAK"}, {22, "SYN"}, {23, "ETB"},
            {24, "CAN"}, {25, "EM"},  {26, "SUB"}, {27, "ESC"},
            {28, "FS"},  {29, "GS"},  {30, "RS"},  {31, "US"},
            {32, "(space)"}, {127, "DEL"}
        };

        Agraph_t* G;
        GVC_t* gvc;

        gvc = gvContext();
        G = agopen(name.data(), Agdirected, 0);

        agattr(G, AGNODE, string("shape").data(), string("circle").data());

        std::queue<S_tree::node*> nodeQueue;
        std::map<S_tree::node*, Agnode_t*> nodeMap;

        nodeQueue.push(root);
        nodeMap[root] = agnode(G, nullptr, true);
        std::string label = (non_printable.contains(root->value)) ?
            non_printable.at(root->value): 
            string(1, root->value);
        agset(nodeMap.at(root), string("label").data(), label.data());

        while (!nodeQueue.empty())
        {
            S_tree::node* currentNode = nodeQueue.front();
            nodeQueue.pop();
            // if (non_printable.contains(currentNode->right->value)) {
                
            // }
            Agnode_t* agNode = nodeMap.at(currentNode);

            if (currentNode->left != nullptr)
            {
                S_tree::node* leftNode = currentNode->left;
                nodeQueue.push(leftNode);
                nodeMap[leftNode] = agnode(G, nullptr, true);
                std::string label;
                if (sharp_pos.contains(leftNode->position))
                    label = "Accept";
                else
                    label = (non_printable.contains(leftNode->value)) ?
                        non_printable.at(leftNode->value): 
                        string(1, leftNode->value);
                agset(nodeMap.at(leftNode), string("label").data(), label.data());
                Agedge_t* edge = agedge(G, agNode, nodeMap.at(leftNode), nullptr, true);
            }

            if (currentNode->right != nullptr)
            {
                S_tree::node* rightNode = currentNode->right;
                nodeQueue.push(rightNode);
                nodeMap[rightNode] = agnode(G, nullptr, true);
                std::string label = (non_printable.contains(rightNode->value)) ?
                    non_printable.at(rightNode->value): 
                    string(1, rightNode->value);
                if (sharp_pos.contains(rightNode->position))
                    label = "Accept";
                agset(nodeMap.at(rightNode), string("label").data(), label.data());
                Agedge_t* edge = agedge(G, agNode, nodeMap.at(rightNode), nullptr, true);
            }
        }

        gvLayout(gvc, G, "dot");
        gvRenderFilename(gvc, G, "png", name.data());
        gvFreeLayout(gvc, G);
        gvFreeContext(gvc);
        agclose(G);
    }
}