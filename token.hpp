#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>

using namespace std;

enum token_type{
    BAD_TOKEN,
    NUMBER_TOKEN,
    ADD_TOKEN,
    SUB_TOKEN,
    MUL_TOKEN,
    DIV_TOKEN,
    MOD_TOKEN,
    POW_TOKEN,
    LEFT_PAREN_TOKEN,
    RIGHT_PAREN_TOKEN,
    END_TOKEN
};

struct s_token{
    token_type type;
    double value;
    string s;
};

void set_line(string input);
void get_token(s_token *token);

#endif