#include <stdio.h> 
#include <ctype.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

int Fsize=50; /* max string length of formulas*/
int inputs=10;


int i;
int j;


/*put all your functions here.  You will need
1.
int parse(char *g) which returns 1 if a proposition, 2 if neg, 3 if binary, ow 0
Of course you will almost certainly need other functions.

For binary formulas you will also need functions that return the first part and the second part of the binary formula.

char *partone(char *g)

char *parttwo(char *g)


You may vary this program provided it reads 10 formulas in a file called "input.txt" and outputs in the way indicated below to a file called "output.txt".
*/

const int PR = 0; const int BC = 1; const int LB = 2; const int RB = 3; const int NG = 4; const int ER = 5;


int matcher(char c){

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

int countToCloseBrace(char *g){
  int counter = 0;
  for(int i = 0; *(g+i) != '\0'; i++) {
      if( matcher(g+i) == LB){
        counter ++;
      }
      if( matcher(g+i) == RB){
        counter --;
      }
      if( counter == 0 ){
        return i;
      }
  }

  if(counter != 0){
    return -1;
  }
}

int parseBrace(char *g, int length){
  //i is LB and length is RB
  int i = 1;
  int acceptBC = 0;
  int acceptNG = 1;
  int acceptPR = 1;
  while(i < length){
    switch (matcher(g + i))
      {
      case PR:
        if(acceptPR){
          acceptPR = 0;
          acceptNG = 0;
          acceptBC = 1;
        }else{
          return 0;
        }

        break;
      
      case LB:
        if(acceptPR){
          int lengthToRB = countToCloseBrace(g+i);
          if(parseBrace(g+i,lengthToRB) != 0){
            acceptPR = 0;
            acceptNG = 0;
            acceptBC = 1;
            i += lengthToRB;
          }
        }else{
          return 0;
        }

        break;

      case BC:
        if(acceptBC){
          acceptPR = 1;
          acceptNG = 1;
          acceptBC = 0;
        }else{
          return 0;
        }

        break;


      case NG:
        if(!acceptNG){
          return 0;
        }
        break;


      default:
        return 0;
        break;
      }
      i++;
  }

  if(i==length){
    return 1;
  }
  return 0;


}


int parse(char *g) {

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
    int lengthToRB = countToCloseBrace(g);
    if( lengthToRB == strlen(g)){
        if(parseBrace(g, lengthToRB) != 0){
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


char *partone(char *g){
  
}

char *parttwo(char *g){

}




int main()
{ /*input 10 strings from "input.txt" */
  char *names[inputs];/*to store each of the input strings*/

  for (i=0;i<inputs;i++) names[i]=malloc(Fsize);/*create enough space*/



  FILE *fp, *fpout, *fopen();

  if ((  fp=fopen("input.txt","r"))==NULL){ printf("Error opening file");exit(1); }
  if ((  fpout=fopen("output.txt","w"))==NULL){ printf("Error opening file");exit(1); }/*ouputs to be sent to "output.txt"*/

  fscanf(fp,"%s %s %s %s %s %s",names[0],names[1], names[2], names[3],names[4],names[5],names[6],names[7],names[8],names[9]);/*read input strings from "input.txt"*/
 
  /*lets check your parser*/
  for(i=0;i<inputs;i++)
    {j=parse(names[i]);
      switch(j)
	{
	case(0):fprintf(fpout,"%s is not a formula\n", names[i]);break;
	case(1):fprintf(fpout,"%s is a proposition\n",names[i]);break;
	case(2):fprintf(fpout,"%s is a negation\n",names[i]);break;
	case(3):fprintf(fpout,"%s is a binary formula, the first part is %s and the second part is %s\n",names[i]), partone(names[i]), parttwo(names[i]);break;
	default:fprintf(fpout,"%s is not a formula\n",names[i]);break;
	}
    }
 
 
  fclose(fp);
  fclose(fpout);
 
  return(0);
}