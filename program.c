#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ID_EDIT_BOX 101
#define ID_BUTTON   102

HWND hEdit;

double GetEuro() {


    return 16.0;
}

double GetDolar() {


    return 12.0;
}

enum type {
    
    operand,
    plus_operator,
    sub_operator,
    div_operator,
    mul_operator,
    percent_operator,
    unary_sub
};

struct token {

    enum type t;

    union {
        int precedence;
        double operand;
    } fields;

};

#define PLUS 10 /* + */
#define SUB 10  /* - */
#define MUL 11 /* * */
#define DIV 11 /* / */
#define PER 11 /* p */
#define UNARY  12 /* unary - */
 
struct token tokens[32] ;
struct token stack[32] ;
struct token output[32] ;
double eval_stack[32] ;
char lex[32];
int k; /* count struct token lists */

int lexer(unsigned const char* source) {
    k = 0;
    memset(eval_stack, 0, sizeof(eval_stack));
    memset(tokens,0,sizeof(tokens));
    memset(stack, 0, sizeof(stack));
    memset(output, 0, sizeof(output));

    for (int i = 0; source[i] != '\0'; ) {
        int j = 0; /* index lex array */
        if (isdigit(source[i])) {

            while (isdigit(source[i])) {

                lex[j++] = source[i++];
            }

            if (source[i] == '.' && isdigit(source[i + 1])) {
                lex[j++] = source[i++];
                while (isdigit(source[i])) {
                    lex[j++] = source[i++];
                }

            }

            lex[j] = '\0';
            tokens[k].t = operand;
            tokens[k].fields.operand = strtod(lex, NULL);
            k++;
        }

        else if (isspace(source[i])) {
            while (isspace(source[i]))i++;
        }
        else if (source[i] == '+')tokens[k].t = plus_operator, tokens[k].fields.precedence = PLUS, k++, i++;
        else if (source[i] == '-')tokens[k].t = sub_operator, tokens[k].fields.precedence = SUB, k++, i++;
        else if (source[i] == 'p')tokens[k].t = percent_operator, tokens[k].fields.precedence = PER, k++, i++;
        else if (source[i] == '/')tokens[k].t = div_operator, tokens[k].fields.precedence = DIV, k++, i++;
        else if (source[i] == '*')tokens[k].t = mul_operator, tokens[k].fields.precedence = MUL, k++, i++;
        else if (source[i] == 'e')tokens[k].t = operand, tokens[k].fields.operand = GetEuro(), k++, i++;
        else if (source[i] == 'd')tokens[k].t = operand, tokens[k].fields.operand = GetDolar(), k++, i++;
        else {

            char ii[16];
            snprintf(ii, 4, "%d", i);
            MessageBoxA(NULL, "lexer error", ii, 1);
            return -1;

        }

    }

}

int parser() {
    
    for (int i = 0; i < k;) {
           
        if (i < k - 1 && tokens[i].t == operand && 
            tokens[i + 1].t >= plus_operator &&
            tokens[i + 1].t <= percent_operator) {
            i += 2;
        }
        else if (i == k - 1 && tokens[i].t == operand &&
            tokens[i - 1].t >= plus_operator &&
            tokens[i - 1].t <= percent_operator) {
            i += 1;
        }
        else if (i == 0 && tokens[i].t == sub_operator && tokens[i + 1].t == operand) {

            tokens[i].t = unary_sub;
            tokens[i].fields.precedence = UNARY;
            i += 2;
        }
        else if (i > 0 && tokens[i-1].t >= plus_operator && 
                tokens[i - 1].t <= percent_operator && 
                tokens[i].t == sub_operator && 
                tokens[i + 1].t == operand ) {

            tokens[i].t = unary_sub;
            tokens[i].fields.precedence = UNARY;
            i += 2;
        }
        
        else {
            
            char ii[16];
            snprintf(ii, 4, "%d", k);
            MessageBoxA(NULL, "parser error", ii, 1);
            return -1;
            
        }
        

    }

}

void postfix() {
/* shuting yard */
    
    int t = 0; /* index output */
    int top = 0; /* index stack */
    for (int i = 0; i < k; i++) {

        if (tokens[i].t == operand) {
             output[t++] = tokens[i];
             
        }
        else if (tokens[i].t >= plus_operator &&
            tokens[i].t <= unary_sub) {
            while (top && stack[top].fields.precedence >= tokens[i].fields.precedence) {
                output[t++] = stack[--top];
            }
            stack[top++] = tokens[i];

        }

    }

    while (top) {

        output[t++] = stack[--top];

    }
    
}

void eval(char * result , int size) {
   
    int top = 0; /* eval stack */
    double a, b;
    for (int i = 0; i < k; i++) {

        switch (output[i].t) {
        
        case operand:
            eval_stack[top++] = output[i].fields.operand;
            break;
            
        case plus_operator:
            a = eval_stack[--top];
            b = eval_stack[--top];
            eval_stack[top++] = b + a;
            break;

        case sub_operator:
            a = eval_stack[--top];
            b = eval_stack[--top];
            eval_stack[top++] = b - a;
            break;
        
        case mul_operator:
            a = eval_stack[--top];
            b = eval_stack[--top];
            eval_stack[top++] = b * a;
            break;
        
        case div_operator:
            a = eval_stack[--top];
            b = eval_stack[--top];
            eval_stack[top++] = b / a;
            break;
        
        case percent_operator:
            a = eval_stack[--top];
            b = eval_stack[--top];
            eval_stack[top++] = b * (a/100);
            break;

        case unary_sub:
            a = eval_stack[--top]; 
            eval_stack[top++] = -a;
            break;
        
        }
        
    }

    double re = eval_stack[0];
    snprintf(result, size , "%.3f", re);
    return;
}

void exec(unsigned const char* source,char * result, int size) {

    if (lexer(source) == -1) {
        return;
    }
    if (parser() == -1) {
        return;
    }
    postfix();
    eval(result,size);

}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {

    case WM_CREATE:
        hEdit = CreateWindowExA(
            0, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
            50, 50, 300, 25,
            hwnd, (HMENU)ID_EDIT_BOX, NULL, NULL
        );
             
        CreateWindowExA(
            0, "BUTTON", "=",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 90, 100, 30,
            hwnd, (HMENU)ID_BUTTON, NULL, NULL
        );

        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON && HIWORD(wParam) == BN_CLICKED) {
            unsigned char buffer[256];
            GetWindowTextA(hEdit, (char*)buffer, 256);
            
            char result[32] = {0};
            
            exec(buffer, result, sizeof(result));

            MessageBoxA(NULL,result,"re",1);

        }
        return 0;

 
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

    char *CLASS_NAME = "WindowClass";
        
    WNDCLASSA wc = {0};
    
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );
    
    ShowWindow(hwnd, nShowCmd);
    
    MSG msg = {0};
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

}