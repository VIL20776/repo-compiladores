#pragma once

#include <set>
#include <map>
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
        std::map<int, char> values;
        std::set<int> sharp_pos;

        node *const find_node(node *node, int i);
        void release_nodes(node *current);
        
        bool nullable(node *node);
        std::set<int> first_pos(node *node);
        std::set<int> last_pos(node *node);
    public:
        Syntax_tree(std::string regex);
        ~Syntax_tree();


        std::set<int> first_pos();
        std::set<int> follow_pos(int i);
        const std::map<int,char> &get_values();
        std::set<int> sharp();

        void draw_syntax_tree (std::string name);
    };

}