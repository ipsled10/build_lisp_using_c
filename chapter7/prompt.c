#include<stdio.h>
#include "mpc.h"

//if we are compiling on windows compile these functions
#ifdef _WIN32
#include<string.h>
// Decle a buffer for user input of size  2048
//static char input[2048];//the static keyword makes this variable local to this file
static char buffer[2048];
//now lets write the fake readline function for windows ayeee windows yeserhen yestehe
char *readline(char *prompt){
  fputs(prompt,stdout);
  fgets(buffer,2048,stdin);
  char *cpy = malloc(strlen(buffer)+1);
  strcpy(cpy,buffer);
  cpy[strlen(cpy)- 1] = "\0";
  return cpy;
}

// and now lets add_history function
void add_history(char *unused){}

//otherwise include this editline headers
#else
#include<stdio.h>
#include<editline/readline.h>
#include<editline/history.h>
#endif

long eval_op(long x, char* op,long y){
  if(strcmp(op,"+") == 0){return x + y;}
  if(strcmp(op,"-") == 0){return x - y;}
  if(strcmp(op,"*") == 0){return x *  y;}
  if(strcmp(op,"/") == 0){return x / y;}
}

long eval(mpc_ast_t* t){
  //if tagged as number return it immediately
  if(strstr(t->tag,"number")){
    return atoi(t->contents);
  }

  //dont forget that '(' is the first child

  char* op = t->children[1]->contents;
  //we store the third child in x
  long x = eval(t->children[2]);

  int i = 3;
  while(strstr(t->children[i]->tag,"expr")){
    x = eval_op(x, op, eval(t->children[i]));
    i++;
   }
   return x;
  }

//start of main function
int main(int argc, char **argv){


mpc_parser_t* Number   = mpc_new("number");
mpc_parser_t* Operator = mpc_new("operator");
mpc_parser_t* Expr     = mpc_new("expr");
mpc_parser_t* Lispy    = mpc_new("lispy");

/* Define them with the following Language */
mpca_lang(MPCA_LANG_DEFAULT,
  "                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' | '%'|;                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    lispy    : /^/ <operator> <expr>+ /$/ ;             \
  ",
  Number, Operator, Expr, Lispy);
 /* Print version and Exit information*/
  puts("Klisp version -0.1");
  puts("Press Ctrl-C to Exit\n");
  
  while(1){
    //fputs("klisp>>",stdout);

    char *input = readline("lispy>>");
    //read a line of user input of maximum size 2048
    //fgets(input,2048,stdin);
    add_history(input);
    // now we will attempt to Parse the user Input
    mpc_result_t r;
    if(mpc_parse("<stdin>",input,Lispy,&r)){
      long result = eval(r.output);
      printf("%li\n",result);
     
      /*on success Print the AST */
      //mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    }
    else{
     /* Otherwise Print the Error */
     mpc_err_print(r.error);
     mpc_err_delete(r.error);
    }   

    free(input);//free retrieved input
  }

//finishing touches
mpc_cleanup(4, Number, Operator, Expr, Lispy);
return 0;
} 
