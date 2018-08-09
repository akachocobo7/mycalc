#include <iostream>
#include <string>
#include "token.h"

using namespace std;

string line;
int line_pos;

enum lexer_status{
    INITIAL_STATUS,
    INT_PART_STATUS,
    DOT_STATUS,
    FRAC_PART_STATUS
};

void get_token(s_token *token){
    char current_char;
    lexer_status status = INITIAL_STATUS;
    token->type = BAD_TOKEN;
    while(line[line_pos] != '\0'){
        current_char = line[line_pos];

        if((status == INT_PART_STATUS || status == FRAC_PART_STATUS) && !isdigit(current_char) && current_char != '.'){
            token->type = NUMBER_TOKEN;
            token->value = stod(token->s);
            return;
        }

        if(isspace(current_char)){
            if(current_char == '\n'){
                token->type = END_TOKEN;
                return;
            }
            line_pos++;
            continue;
        }

        token->s += line[line_pos];
        line_pos++;

        if(current_char == '+'){
            token->type = ADD_TOKEN;
            return;
        }
        else if(current_char == '-'){
            token->type = SUB_TOKEN;
            return;
        }
        else if(current_char == '*'){
            token->type = MUL_TOKEN;
            return;
        }
        else if(current_char == '/'){
            token->type = DIV_TOKEN;
            return;
        }
        else if(isdigit(current_char)){
            if(status == INITIAL_STATUS){
                status = INT_PART_STATUS;
            }
            else if(status == DOT_STATUS){
                status = FRAC_PART_STATUS;
            }
        }
        else if(current_char == '.'){
            if(status == INT_PART_STATUS){
                status = DOT_STATUS;
            }
            else{
                cerr << "syntax error." << endl;
                exit(1);
            }
        }
        else{
            cerr << "bad character(" << current_char << ")" << endl;
            exit(1); 
        }
    }
}

void set_line(string input){
    line = input;
    line_pos = 0;
}

void test(string s){
    s_token token;

    set_line(s);

    for(;;){
        get_token(&token);

        if(token.type == END_TOKEN){
            break;
        }
        else{
            cout << "type: " << token.type << ", str: " << token.s << endl;
        }
    }
}

int main(){
    string s;
    while(getline(cin, s)){
        test(s + '\n');
    }
}