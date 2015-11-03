
//if we are compiling on windoes compile these functions
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
int main(int argc, char **argv){

 /* Print version and Exit information*/
  puts("Klisp version -0.1");
  puts("Press Ctrl-C to Exit\n");
  
  while(1){
    //fputs("klisp>>",stdout);

    char *input = readline("lispy>>");
    //read a line of user input of maximum size 2048
    //fgets(input,2048,stdin);
    add_history(input);

    printf("No you are a %s",input);
    free(input);//free retrieved input
  }

return 0;
} 
