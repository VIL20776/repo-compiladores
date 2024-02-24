#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "algorithms/algorithms.hpp"

enum valitation {succes, fail};
bool simulation_test(std::string regex, std::string word, bool validate)
{
    regex = algorithms::to_standard(regex);
    std::string postfix = algorithms::regex_to_postfix(regex);

    std::unique_ptr<algorithms::AutomataCreator> fa_creator {};
    std::unique_ptr<models::Automaton> nfa {};
    std::unique_ptr<models::Automaton> dfa {};

    fa_creator.reset(new algorithms::Direct(postfix));
    dfa = fa_creator->create_automata();
    bool result0 = dfa->simulate(word);

    fa_creator.reset(new algorithms::Thompson(postfix));
    nfa = fa_creator->create_automata();
    bool result1 = nfa->simulate(word);

    fa_creator.reset(new algorithms::Subsets(nfa.release()));
    dfa = fa_creator->create_automata();
    bool result2 = dfa->simulate(word);

    fa_creator.reset(new algorithms::Minimize(dfa.release()));
    dfa = fa_creator->create_automata();
    bool result3 = dfa->simulate(word);

    if (validate) {
        return result0 & result1 & result2 & result3;
    }

    return result0 | result1 | result2 | result3;
}

std::vector<std::string> regex_list = {
    "(a*|b*)c",
    "(b|b)*abb(a|b)*",
    "(a|$)b(a+|c)?",
    "(a|b)*a(a|b)(a|b)",
    "b*ab?",
    "b+abc+",
    "ab*ab*",
    "0(0|1)*0",
    "(($|0)1*)*",
    "(0|1)*0(0|1)(0|1)",
    "(00)*(11)*",
    "(0|1)1*(0|1)",
    "0?(1|$)?0*",
    "((1?)*)*",
    "(01)*(10)*"
};


std::vector<std::string> valid = {
    "c","ac","aaac","bbbc","bbbbbbbbc",
    "abb","babb","bbbabb","abbbabb","abbab",
    "ab","abaa","abc","b","bc",
    "aaa","aab","aba","baba","ababa",
    "ab","bab","bbab","bbbab","bbbab",
    "babc","bbabc","bbbabc","bbbbbabc","bbbabcccc",
    "aab","aa","aab","abab","abbabb",
    "00","010","0010","000","010110",
    "","0","1","0101010","000111000",
    "000","010","11011","0010","1011",
    "","11","0011","0000001111","000011111111",
    "10","010","111","0111","01110",
    "0","1","10","0100","00000",
    "","1","11","111","1111",
    "","10","0110","01011010","011010"
};

std::vector<std::string> invalid = {
    "1","b","ab","bcc","cab",
    "ab","ba","bab","aaabb","bbbba",
    "a","ac","bac","bbb","bb",
    "bb","baa","bbba","baaabbb","bbaa",
    "","b","bb","abb","bbabb",
    "abc","b","abbc","ab","bb",
    "a","bb","ba","bbb","baab",
    "","1","0101","101","11",
    "020","7","001v","01t11","test",
    "01","10","11","000100","1110",
    "0","001","1110","000111","001101",
    "0100","100","01010","000","110001",
    "11","011","1101","0101","11110",
    "0","01","001","1011","11101",
    "1","011001","101","111","0011",
};

TEST_CASE("Simulation succcess") {
    REQUIRE(valid.size() == 5*regex_list.size()); 

    int j = 0; //indice de regex_list
    for (size_t i = 0; i < valid.size(); ++i) {
        const std::string regex = regex_list[j];
        const std::string str = valid[i];
        
        SECTION("Regex: " + regex + ", Input: " + str) {
            bool result = simulation_test(regex, str, true);
            REQUIRE(result == true); 
        }

        int mod = (i + 1) % 5;
        if ( mod == 0) 
            j++;
    }
}

TEST_CASE("Simulation fail") {
    REQUIRE(invalid.size() == 5*regex_list.size()); 

    int j = 0; //indice de regex_list
    for (size_t i = 0; i < invalid.size(); ++i) {
        const std::string regex = regex_list[j];
        const std::string str = invalid[i];
        
        SECTION("Regex: " + regex + ", Input: " + str) {
            bool result = simulation_test(regex, str, false);
            REQUIRE(result == false); 
        }

        int mod = (i + 1) % 5;
        if ( mod == 0) 
            j++;
    }
}

