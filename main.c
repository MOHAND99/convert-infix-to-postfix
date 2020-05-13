#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define  STACKSIZE 64

/*
* Item: An item that is being pushed to or popped from the stack
* It may be float (to be used while evaluating the postfix)
* It may be char (to be used while converting infix to postfix)
*/
typedef union
{
    float fData;
    char  cData;
    int num;
} Item;
typedef struct
{
    int top;
    Item items[STACKSIZE];
} Stack;

Stack * initialize()
{
    Stack *s=malloc(sizeof(Stack));
    s->top=0;
    return s;
}
int isEmpty(Stack *s)
{
    return !(s->top);
}
Item top(Stack *s)
{
    return s->items[s->top-1];
}
Item pop(Stack *s)
{
    return s->items[--s->top];
}
void push(Stack *s, Item val)
{
    s->items[s->top++] = val;
}
void destroy(Stack* s1)
{
    free(s1);
}
void pushChar(Stack* stak, char* str)
{
    Item i;
    i.cData = str[0];
    push(stak, i);
}
void postPush(Stack* s, char* postfix)
{
    char ch = pop(s).cData;
    strncat(postfix, &ch, 1);
    ch = ' ';
    strncat(postfix, &ch, 1);
}
/*
* infixToPostfix: converts an expression in infix notation to
* to a postfix notation (Reverse-Polish Notation)
* ASSUME single-digit operands
* ASSUME ^*+-/ operators
* e.g., 22+31 --> 22 31 +
* e.g., (2+3)*4 --> 2 3 + 4 *

*/

void infixToPostfix(char* infix, char* postfix)
{
    //postfix[
    Stack* operations = initialize();
    char* token = strtok(infix, " ");
    while (token != NULL)
    {
        if(!stricmp(token, "("))
            pushChar(operations, token);
        else if (!stricmp(token, "^"))
        {
            if(top(operations).cData=='^')
            {
                postPush(operations, postfix);
            }
            pushChar(operations, token);
        }
        else if (!stricmp(token, "*") || !stricmp(token, "/"))
        {
            while(top(operations).cData=='/' || top(operations).cData=='*' || top(operations).cData=='^')
            {
                postPush(operations, postfix);
            }
            pushChar(operations, token);
        }
        else if (!stricmp(token, "+") || !stricmp(token, "-"))
        {
            while(top(operations).cData!='(' && !isEmpty(operations))
            {
                postPush(operations, postfix);
            }
            pushChar(operations, token);
        }
        else if (!stricmp(token, ")"))
        {
            while(top(operations).cData!='(')
                postPush(operations, postfix);
            pop(operations);
        }
        else
        {
            strcat(postfix, token);
            strcat(postfix, " ");
        }
        token = strtok(NULL, " ");
    }
    while (!isEmpty(operations))
    {
        postPush(operations, postfix);
    }
    destroy(operations);
}

Item evalItems(Item itm1, Item itm2, char op)
{
    Item result;
    switch(op)
    {
    case '+':
        result.fData = (itm2.fData+itm1.fData);
        return result;
    case '*':
        result.fData = (itm2.fData*itm1.fData);
        return result;

    case '-':
        result.fData = (itm2.fData-itm1.fData);
        return result;
    case '/':
        result.fData = (itm1.fData/itm2.fData);
        return result;
        break;
    case '^':
        result.fData = pow(itm1.fData, itm2.fData);
        return result;
        break;
    }
}

Item evaluatePostfix(char *postfx)
{
    Stack *s = initialize();
    int i = 0;
    char *token = strtok(postfx, " ");
    char ch;
    Item data, opernd1, opernd2, result;

    while(token != NULL)
    {
        if(atof(token) ){
            data.fData = atof(token);
            push(s, data);
        }
        else
        {
            ch = *token;
            opernd1 = pop(s);
            opernd2 = pop(s);
            switch(ch)
            {
            case '+':
                push(s, evalItems(opernd2, opernd1,ch));
                break;
            case '-':
                push(s, evalItems(opernd2, opernd1, ch));
                break;
            case '*':
                push(s, evalItems(opernd2, opernd1, ch));
                break;
            case '/':
                push(s, evalItems(opernd2, opernd1, ch));
                break;
            case '^':
                push(s, evalItems(opernd2, opernd1, ch));
            }
        }
        token = strtok(NULL, " ");
    }
result = pop(s);
destroy(s);
postfx[0] = '\0';
return result;
}



void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}

int main(int argc, char**argv)
{
    system("color f0");
    char infixExpr[256] = "";
    char postfixExpr[256] = "";
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);
        infixToPostfix(infixExpr, postfixExpr);
        printf("Postfix : %s\n", postfixExpr);
        Item result = evaluatePostfix(postfixExpr);
        printf("Result: %.3f\n\n", result.fData);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    }
    return 0;
}
