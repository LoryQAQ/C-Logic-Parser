#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */



int Fsize=50; /*maximum formula length*/
int inputs =1;/* number of formulas expected in input.txt*/
int ThSize=100;/* maximum size of set of formulas, if needed*/
int TabSize=500; /*maximum length of tableau queue, if needed*/





/*You need to change this next bit and include functions for parse, closed and complete.*/

//----------------------------------- part 1 ----------------------------------- 
enum symbols {PR, BC, LB, RB, NG, ER};

int matcher(char * symbol){
  char c = *symbol;
  // printf("Matched:%c\n",c);

  if(c == 'p' || c == 'q' || c == 'r'){
    return PR; //"PR"
  }else if(c == 'v' || c == '^' || c == '>'){
    return BC; //"BC";
  }else if(c == '('){
    return LB; //"LB";
  }else if(c == ')'){
    return RB; //"RB";
  }else if(c == '-'){
    return NG; //"NG";
  }else{
    return ER; //"ER";
  }

}

int countToRBorBC(char *g, int findBC){
  int counter = 0;
  for(int i = 0; *(g+i) != '\0'; i++) {
      if( matcher(g+i) == LB){
        counter ++;
      }
      if( matcher(g+i) == RB ){
        counter --;
      }

      if( counter == 0 ){
        if(!findBC){
          return i;
        }
        if(findBC && matcher(g+i) == BC){
          return i;
        }
      }
  }

  if(counter != 0){
    return -1;
  }
  return -1;
}



char *partone(char *g){
  int lengthToBC = countToRBorBC(g+1, 1);
  char left[50];
  strcpy(left, (g+1));
  *(left+lengthToBC) = '\0';

  return strdup(left);
}

char *parttwo(char *g){
  int lengthToBC = countToRBorBC(g+1, 1);
  char right[50];
  strcpy(right, (g+lengthToBC+2));
  *(right+strlen(right)-1) = '\0';
  return strdup(right);
}


int parse(char *g) {
 
  // printf("--paring[%s]--\n",g);
  // printf("%i\n",matcher(g));

  switch (matcher(g))
  {
  case PR:

    if(*(g+1) == '\0'){
      return 1;
    }else{
      return 0;
    }
    break;
  
  case LB:
  { 
    
    int lengthToRB = countToRBorBC(g, 0);
    
    if( lengthToRB + 1 == strlen(g)){
        int lengthToBC = countToRBorBC(g+1, 1);
        if(lengthToBC == -1){ break; }
        char *left = partone(g);
        char *right = parttwo(g);

        // printf("left[%s];right[%s]\n",left,right);

        if(parse(left)!=0 && parse(right)!=0){
          return 3;
        }
    }
    break;
  }

  case NG:
    if(parse(g+1) != 0){
      return 2;
    }
    break;


  default:
    return 0;
    break;
  }

  return 0;
}



//----------------------------------- part 2 ----------------------------------- 

/* A set will contain a list of words. Use NULL for emptyset.  */
struct set{
  char *item;/*first word of non-empty set*/
  struct set *tail;/*remaining words in the set*/
};

/* A tableau will contain a list of pointers to sets (of words).  Use NULL for empty list.*/
struct tableau {
  struct set * S; /* pointer to first set in non-empty list */
  struct tableau *rest; /*list of pointers to other sets*/
};

char * prependNg(char * string){
  char *newstr = malloc(sizeof(string));
  *newstr = '-';
  strcpy((newstr+1), string);
  
  return newstr;

}


int closed(struct tableau *t) {

  return(0);
}

void complete(struct tableau *t){
  char *g = t->S->item;
  int res = parse(g);
  char *left;
  char *right;
  int rule = -1; // proposition = 0; alpha = 1; beta = 2;

  printf("res[%s];\n",g);

  if(res == 3){//binary
    int bcloc = countToRBorBC(g+1, 1);
    left = partone(g);
    right = parttwo(g);
    char bc = *(g+1+bcloc);
    if(bc == '^'){
      rule = 1;
    }else if(bc == 'v'){
      rule = 2;
    }else if(bc == '>'){
      rule = 2;
      left = prependNg(left);
    }
    printf("bc[%c];rule[%i];left[%s];right[%s];\n",bc,rule,left,right);


  }else if(res == 2){ //negation

    if(*(g+1) == '('){ //negated binary
      int bcloc = countToRBorBC(g+2, 1);
      left = partone(g+1);
      right = parttwo(g+1);
      char bc = *(g+2+bcloc);
      if(bc == '^'){
        rule = 2;
        left = prependNg(left);
        right = prependNg(right);
      }else if(bc == 'v'){
        rule = 1;
        left = prependNg(left);
        right = prependNg(right);
      }else if(bc == '>'){
        rule = 1;
        right = prependNg(right);
      }
      printf("bc[%c];rule[%i];left[%s];right[%s];\n",bc,rule,left,right);

    }else if(*(g+1) == '-'){ //double negation
      left = (g+2);
      right = NULL;

      printf("left[%s]\n",left);
    
    }else{ //proposition
      rule = 0;
    }
    

  }else{ //proposition
    rule = 0;
  }


}




//----------------------------------- main ----------------------------------- 


int main(){

    char *name = malloc(Fsize);
    struct set * S=(struct set *)malloc(sizeof(struct set));
    struct tableau * t=(struct tableau *)malloc(sizeof(struct tableau));

/*You should not need to alter the program below.*/


 /*input 10 strings from "input.txt" */
    FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
    if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

    int j;
    
    for(j=0;j<inputs;j++)
    {
        fscanf(fp, "%s",name);/*read formula*/
        int parsed = parse(name);
        switch (parsed)
        {
            case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
            case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
            case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
            case(3): fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
            default: fprintf(fpout, "What the f***!  ");
        }

        if (parsed!=0)
        {
            S->item = name;
            t->S = S;
            complete(t);
            if (closed(&t))  fprintf(fpout, "%s is not satisfiable.\n", name);
            else fprintf(fpout, "%s is satisfiable.\n", name);
        }
        else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }

 
    fclose(fp);
    fclose(fpout);
    free(name);
    free(S);
    free(t);

  return(0);
}