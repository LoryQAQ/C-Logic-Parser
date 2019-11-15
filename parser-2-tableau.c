#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */



int Fsize=50; /*maximum formula length*/
int inputs =10;/* number of formulas expected in input.txt*/
int i;/* in case you need it */
int ThSize=100;/* maximum size of set of formulas*/
int TabSize=500; /*maximum length of tableau queue*/



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


/*You need to change this next bit and include functions for parse, closed and complete.*/
int parse(char *g){return(0);}
int closed(struct tableau *t) {return(0);}
void complete(struct tableau *t){}

/*You should not need to alter the program below.*/
int main()

{ /*input 10 strings from "input.txt" */


  char *name = malloc(Fsize);
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<inputs;j++)
    {
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name))
      {
          case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
          case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
          case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
          case(3):fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
          default:fprintf(fpout, "What the f***!  ");
              
      }

    
      struct set S={name, NULL};
      struct tableau t={&S, NULL};
      if (parse(name)!=0)
        {complete(&t);
          if (closed(&t))  fprintf(fpout, "%s is not satisfiable.\n", name);
          else fprintf(fpout, "%s is satisfiable.\n", name);
        }
        else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
      }

 
  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);
}

