#include "syntax_tree.hpp"

#include <string>
#include <stack>
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
        stack<node*> nodes {}; 
        int position = 1;
        for (char &c: regex) {
            if (isalnum(c) || c == '#') {
                nodes.push(new node(c, position));
                position++;
                continue;
            }
            if (c == '|' || c == '.') {
                node *root = new node();
                root->append(nodes.top(), node::RIGHT); nodes.pop();
                root->append(nodes.top(), node::LEFT); nodes.pop();

                nodes.push(root);
                continue;
            }
            if (c == '*') {
                node *root = new node();
                root->append(nodes.top(), node::LEFT); nodes.pop();

                nodes.push(root);
                continue;
            }
        }
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
        switch (node->value)
        {
        case '*': case '$':
            return true;
            
        case '|':
            return (nullable(node->left) | nullable(node->right));

        case '.':
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
        case '.':
            if (nullable(node->left))
            {
                first_pos.merge(this->first_pos(node->left));
                first_pos.merge(this->first_pos(node->right));
                return first_pos;
            } else 
                return this->first_pos(node->left);
        
        case '*':
            return this->first_pos(node->left);
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
        case '.':
            if (this->nullable(node->left))
            {
                last_pos.merge(this->last_pos(node->left));
                last_pos.merge(this->last_pos(node->right));
                return last_pos;
            } else 
                return this->last_pos(node->right);
        
        case '*':
            return this->last_pos(node->left);
        }
    }

    set<int> S_tree::follow_pos(int i)
    {
        set<int> follow_pos {};
        S_tree::node *origin = find_node(root, i);
        S_tree::node *temp = origin->root;

        while (temp != nullptr) {
            set<int> check {};
            switch (temp->value)
            {
            case '.':
                check = last_pos(temp->left);

                if (check.find(i) != check.end())
                    follow_pos.merge(first_pos(temp->right));
                break;
            
            case '*':
                check = last_pos(temp);
                if (check.find(i) != check.end())
                    follow_pos.merge(first_pos(temp));
                break;
            
            default:
                break;
            }

            temp = temp->root;
        }

        return follow_pos;
    }
}