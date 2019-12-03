#define _GNU_SOURCE //https://gist.github.com/emilisto/9620134 & https://stackoverflow.com/questions/9168150/implicit-declaration-using-std-c99
#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */


#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
//for some colours in the terminal

int Fsize=50 * sizeof(char); /*maximum formula length*/
int inputs =10;/* number of formulas expected in input.txt*/
int ThSize=100;/* maximum size of set of formulas*/
int TabSize=500; /*maximum length of tableau queue*/




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
  int i = 0;
  for(i = 0; *(g+i) != '\0'; i++) {
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
  char left[Fsize];
  strcpy(left, (g+1));
  *(left+lengthToBC) = '\0';

  return strdup(left);
}

char *parttwo(char *g){
  int lengthToBC = countToRBorBC(g+1, 1);
  char right[Fsize];
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
  char *newstr = malloc(Fsize);
  *newstr = '-';
  strcpy((newstr+1), string);
  
  return newstr;

}
struct tableau * pushSetToTableau(struct tableau *t,  struct set *setToPush){
  struct tableau * newTableau = (struct tableau *)malloc(sizeof(struct tableau));
  newTableau->S = setToPush;
  newTableau->rest = NULL;

  if(t == NULL){
    t = newTableau;
  }else{
    struct tableau * current = t;
    while(current->rest!=NULL){
      current = current->rest;
    }
    current->rest = newTableau;
  }
  return t;
}

struct set *cloneSet(struct set *setToClone)
{
    if(setToClone==NULL) return NULL;
    struct set *temp=(struct set *) malloc(sizeof(struct set));
    char * newItem = strdup(setToClone->item);
    temp->item=newItem;
    temp->tail=cloneSet(setToClone->tail);
    return temp;
}

struct tableau * addToTableauList(struct tableau *t ,struct set * headOfSet, struct set * newSet){

  struct set * newSetWithHead = (struct set *)malloc(sizeof(struct set));

  if(headOfSet != NULL){
    newSetWithHead = cloneSet(headOfSet);
    struct set * temp = newSetWithHead;
    while(temp->tail!=NULL){
      temp = temp->tail;
    }
    temp->tail = newSet;
  }else{
    newSetWithHead = cloneSet(newSet);
  }

  return pushSetToTableau(t, newSetWithHead);
}

void printTableau(struct tableau *t){
  struct tableau * current = t;
  printf(MAGENTA "\n[ ");
  while(current != NULL){
    printf("{ ");
    struct set * currentSet = current->S;
    while (currentSet!=NULL)
    {
      char *g = currentSet->item;
      printf("%s ",g);
      currentSet = currentSet->tail;
    }
    current = current -> rest;
    printf("} ");
  }
  printf("]\n\n" RESET);
}



int isFullyExtended(struct tableau *t){
  struct tableau * current = t;
  while(current != NULL){
    struct set * currentSet = current->S;
    while (currentSet!=NULL)
    {
      char *g = currentSet->item;
      if(*(g)=='-'){
        g = g+1;
      }
      if(matcher(g)!=PR){
        return 0;
      }
      currentSet = currentSet->tail;
    }
    current = current -> rest;
  }
  return 1;
}


int closed(struct tableau *t) {
  struct tableau * current = t;
    while(current != NULL){

      struct set * currentSet = current->S;
      
      int branchClosed = 0;
      while (currentSet!=NULL && !branchClosed)
      {
        struct set * currentSet2 = current->S;
        char *g = currentSet->item;

        if(*(g)=='-'){
          g = g+1;
          
          while (currentSet2!=NULL && !branchClosed)
          {
            char *g2 = currentSet2->item;
              if(*g == *g2){
                branchClosed = 1;
              }
            currentSet2 = currentSet2->tail;
            
          }
        }

        if(!branchClosed)
        currentSet = currentSet->tail;
      }

      if(currentSet==NULL){ //current branch open, so satisfiable
        printf(GREEN ">>>>> Tableau is satisfiable <<<<<\n" RESET);
        return 0;
      }
      
      current = current -> rest;
    }
    printf(RED ">>>>> Tableau is unsatisfiable (No branch is open) <<<<<\n" RESET);
    return 1;
}

struct tableau * complete(struct tableau *t){
  printf(YELLOW "\n\n====================== Completing %s ======================\n\n" RESET,t->S->item);

  while(t != NULL && !isFullyExtended(t)){

    char *left = malloc(Fsize);
    char *right = malloc(Fsize);
    int rule = 0; // proposition = 0; alpha = 1; beta = 2;
    
    struct set * currentSet = t->S;
    struct set * setsBefore = NULL;
    struct set * setsBeforeHead = NULL;

    while(rule == 0 && currentSet!=NULL){
      char *g = currentSet->item;
      int res = parse(g);

      printf(MAGENTA "Now Completing: %s\n" RESET,g);

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
        printf(CYAN "- Binary - " RESET);
        printf("BC[%c] Rule[%i] Left[%s] Right[%s]\n\n",bc,rule,left,right);
        continue;

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
          printf(CYAN "- Negated binary - " RESET);
          printf("BC[%c] Rule[%i] Left[%s] Right[%s]\n\n",bc,rule,left,right);
          continue;

        }else if(*(g+1) == '-'){ //double negation
          rule = 1;
          left = (g+2);
          right = NULL;

          printf(CYAN "- Double negation - " RESET);
          printf("left[%s]\n\n",left);
          continue;
        
        }else{ //proposition
          rule = 0;
        }
        

      }else{ //proposition
        rule = 0;
      }

      printf(CYAN "- Skipped propositions %s - \n\n" RESET, g);

      if(setsBefore!=NULL){
        struct set * temp = (struct set *)malloc(sizeof(struct set));
        temp->item = strdup(currentSet->item);
        temp->tail = NULL;
        setsBefore->tail = temp;
        setsBefore = setsBefore->tail;
      }else{
        setsBefore = (struct set *)malloc(sizeof(struct set));
        setsBefore->item = strdup(currentSet->item);
        setsBefore->tail = NULL;
        setsBeforeHead = setsBefore;
      }
      currentSet = currentSet->tail;
      
    }


    if(rule == 1){ //alpha
      t=t->rest;
      currentSet->item = left;
      if(right!=NULL){
        struct set * temp = (struct set *)malloc(sizeof(struct set));
        temp->item=right;
        temp->tail=currentSet->tail;
        currentSet->tail = temp;
      }
      t = addToTableauList(t,setsBeforeHead,currentSet);


    }else if(rule == 2){ //beta
    // printf("test1 %p\n",t);
      t=t->rest;

        struct set * newSetLeft = (struct set *)malloc(sizeof(struct set));
        struct set * newSetRight = (struct set *)malloc(sizeof(struct set));
        newSetLeft->item = left;
        newSetLeft->tail = cloneSet(currentSet->tail);
        newSetRight->item = right;
        newSetRight->tail = cloneSet(currentSet->tail);
      
      if(left!=NULL){t = addToTableauList(t,setsBeforeHead,newSetLeft);}
      if(right!=NULL){t = addToTableauList(t,setsBeforeHead,newSetRight);}


    }else if(rule == 0){ //proposition
      t = pushSetToTableau(t, t->S);
      t=t->rest;
      printf(BLUE "\nBranch complete, proceding to next tableau..." RESET);
      printTableau(t);
    }
  }


  printf(GREEN "---------- Tableau now fully extended ----------" RESET);
  printTableau(t);

  return t;

}




//----------------------------------- main ----------------------------------- 


int main(){

    char *name = malloc(Fsize);
    

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
            // struct set S= {name,NULL};
            // struct tableau t= {&S, NULL};
            struct set * S= malloc(sizeof(struct set));
            struct tableau * t= malloc(sizeof(struct tableau));
            S->item = name;
            S->tail = NULL;
            t->S = S;
            t->rest = NULL;
            // struct tableau * BITCH ;
            t = complete(t);
            // printf("%p %p",&t);
            if (closed(t))  fprintf(fpout, "%s is not satisfiable.\n", name);
            else fprintf(fpout, "%s is satisfiable.\n", name);
            // free(&S);
            // free(&t);
        }
        else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
        
    }
    fclose(fp);
    fclose(fpout);
    free(name);
    

  return(0);
}