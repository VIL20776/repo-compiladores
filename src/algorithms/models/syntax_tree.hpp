#pragma once

#include <set>
#include <cstddef>
#include <string>

namespace models {

    class Syntax_tree
    {
        struct node
        {
            enum leaf {LEFT, RIGHT};

            char value;
            int position;
            node *root;
            node *left;
            node *right;

            node();
            node(char value, int position);
            void append(node *node, leaf side);
        };

    private:
        node *root;

        node *const find_node(node *node, int i);
        void release_nodes(node *current);
        
    public:
        Syntax_tree(std::string regex);
        ~Syntax_tree();


        bool nullable(node *node);
        std::set<int> first_pos(node *node);
        std::set<int> last_pos(node *node);
        std::set<int> follow_pos(int i);

    };

}