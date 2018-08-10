#include <iostream>
#include <string>
#include <cmath>
#include "token.hpp"

#define TEST 0

using namespace std;




/* レキシカルアナライザ */

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
        else if(current_char == '%'){
            token->type = MOD_TOKEN;
            return;
        }
        else if(current_char == '^'){
            token->type = POW_TOKEN;
            return;
        }
        else if(current_char == '('){
            token->type = LEFT_PAREN_TOKEN;
            return;
        }
        else if(current_char == ')'){
            token->type = RIGHT_PAREN_TOKEN;
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
            if(current_char == 'e'){
                string t = "e";
                for(int i = 0; i < 3; i++){
                    current_char = line[line_pos++];
                    if(current_char == '\n'){
                        cerr << "bad character(e)" << endl;
                        exit(1); 
                    }
                    t += current_char;
                }
                if(t == "exit"){
                    exit(0);
                }
                else{
                    cerr << "bad character(e)" << endl;
                    exit(1);
                }
            }
            cerr << "bad character(" << current_char << ")" << endl;
            exit(1); 
        }
    }
}

void set_line(string input){
    line = input;
    line_pos = 0;
}

void lexer_test(string s){
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

/* レキシカルアナライザ 終 */



/* パーサ */

s_token look_ahead_token;
bool look_ahead_token_exists;

void my_get_token(s_token *token){
    if(look_ahead_token_exists){
        *token = look_ahead_token;
        look_ahead_token_exists = false;
    }
    else{
        get_token(token);
    }
}

void unget_token(s_token *token){
    look_ahead_token = *token;
    look_ahead_token_exists = true;
}

double parse_expression();

double parse_primary_expression(){
    s_token token;
    double value;
    bool minus_flag = false;

    my_get_token(&token);
    if(token.type == SUB_TOKEN){
        minus_flag = true;
    }
    else{
        unget_token(&token);
    }

    my_get_token(&token);
    if(token.type == NUMBER_TOKEN){
        return token.value;
    }
    else if(token.type == LEFT_PAREN_TOKEN){
        value = parse_expression();
        my_get_token(&token);
        if(token.type != RIGHT_PAREN_TOKEN){
            cerr << "missing ')' error." << endl;
            exit(1);
        }
    }
    else{
        unget_token(&token);
    }

    return (!minus_flag)? value : -value;
}

double parse_term(){
    double v1, v2;
    s_token token;

    v1 = parse_primary_expression();
    for(;;){
        my_get_token(&token);
        if(token.type != MUL_TOKEN && token.type != DIV_TOKEN && token.type != MOD_TOKEN && token.type != POW_TOKEN){
            unget_token(&token);
            break;
        }
        v2 = parse_primary_expression();
        if(token.type == MUL_TOKEN){
            v1 *= v2;
        }
        else if(token.type == DIV_TOKEN){
            v1 /= v2;
        }
        else if(token.type == MOD_TOKEN){
            v1 = (int)v1 % (int)v2;
        }
        else if(token.type == POW_TOKEN){
            v1 = pow(v1, v2);
        }
    }

    return v1;
}

double parse_expression(){
    double v1, v2;
    s_token token;

    v1 = parse_term();
    for(;;){
        my_get_token(&token);
        if(token.type != ADD_TOKEN && token.type != SUB_TOKEN){
            unget_token(&token);
            break;
        }
        v2 = parse_term();
        if(token.type == ADD_TOKEN){
            v1 += v2;
        }
        else if(token.type == SUB_TOKEN){
            v1 -= v2;
        }
        else{
            unget_token(&token);
        }
    }

    return v1;
}

double parse_line(){
    double value;

    look_ahead_token_exists = false;
    value = parse_expression();

    return value;
}

/* パーサ 終 */

int main(){
    string s;
    double value;

    if(TEST){
        while(getline(cin, s)){
            lexer_test(s + '\n');
        }
        return 0;
    }

    while(getline(cin, s)){
        set_line(s + '\n');
        value = parse_line();
        cout << ">> " << value << endl;
    }
}