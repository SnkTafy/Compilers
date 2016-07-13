////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////       Sianko Tayfoon 1439          //////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

//#define isspacetk 5 //
#define stringtk 6			// string
#define digittk 7			// number
#define plustk 8			// +
#define minustk 9			// -
#define divtk 10			// /
#define epitk 11			// *
#define plusequaltk 12		// +=
#define greatertk 13		// >
#define lowertk 14			// <
#define equaltk 15			// =
#define greaterequaltk 16	// >=
#define lowerequaltk 17		// <=	
#define eyesequaltk 18		// :=
#define semitk 19			// ;
#define commatk 20			// ,
#define leftpartk 21		// (
#define rightpartk 22		// )
#define leftagtk 23			// [
#define rightagtk 24		// ]
#define leftaggistrotk 25	// {
#define rightaggistrotk 26	// }
#define lowergreatertk 27	// <>
#define eoftk 28			// EOF



// Token words
#define andtk 40
#define intk 41
#define ortk 42
#define whiletk 43
#define calltk 44
#define inouttk 45
#define printtk 46
#define elsetk 47
#define inttk 48
#define programtk 49
#define fortk 50
#define inputtk 51
#define returntk 52
#define iftk 53
#define nottk 54
#define voidtk 55

////////////////////////////////////-----lex globals-----/////////////////////////////////////////
FILE *fp;

int rownum=1;


typedef struct twoinone
{
	int tkID;
	char tk[31];
}Twoinone;
////////////////////////////////////-----lex globals-----/////////////////////////////////////////

//////////////////////////////////-----syntax globals-----////////////////////////////////////////
Twoinone syntactikal;
Twoinone prevsyntax;
int yesorno = 1;
void program();
void Block();
void Declarations();
void Varlist();
void Subprograms();
void Procorfunc();
void Funcbody();
void Formalpars();
void Formalparlist();
void Formalparitem();
void Sequence();
void BracketSeq();
void BrackOrStat();
void Statement();
void AssingmentStat();
void IfStat();
void Elsepart();
void WhileStat();
void ForStat();
void InputStat();
void PrintStat();
void CallStat();
void Actualpars();
void Actualparlist();
void Actualparitem();
void ReturnStat();
//void Condition(labellist *Btrue, labellist *Bfalse);
//void Boolterm(labellist *Btrue, labellist *Bfalse);
//void Boolfactor(labellist *Btrue, labellist *Bfalse);
void Expression(char expressionup[35]);
void Term(char constorid1[35]);
void Factor(char factorreturns1[35]);
void RelationOper(char relationopup[35]);
void AddOper();
void MulOper();
void OptionalSing();
void readorno();
//////////////////////////////////-----syntax globals-----////////////////////////////////////////

////////////////////////////-----intermediate code globals-----///////////////////////////////////

typedef struct nextquadstruct
{
	char label[10];
	char op1[35];
	char op2[35];
	char op3[35];
	char op4[35];
	struct nextquadstruct *next;
}nextquadstruct;

typedef struct llist
{
	struct nextquadstruct *jumpto;
	struct llist *next;
}labellist;

void Condition(labellist **Btrue, labellist **Bfalse);
void Boolterm(labellist **Btrue, labellist **Bfalse);
void Boolfactor(labellist **Btrue, labellist **Bfalse);

nextquadstruct *frontp = NULL, *endp = NULL;
int label = 99;
int nexttempvar = 0;
int nextquad();
void genquad(char op1[35], char op2[35], char op3[35], char op4[35]);
char *newtemp();
labellist *emptylist();
labellist *makelist();
labellist *merge( labellist *list1, labellist *list2 );
void backpatch( labellist *list , char z[10] );

////////////////////////////-----intermediate code globals-----///////////////////////////////////

////////////////////////////-----symbols table globals-----///////////////////////////////////////

typedef struct funcinfo
{
	char name[35];
	int firstquad;
	int framelength;
	struct funcinfo *next;
}funcinfo;

typedef struct Argument
{
	char name[35];
	char parMode[10];
	struct Argument *next;
}Argument;

typedef struct Entity_var
{
	int offset;
}Entity_variable;

typedef struct Entity_func
{
	int startQuad;
	Argument *arglist;
	int framelength;
}Entity_function;

typedef struct Entity_stat
{
	char value[35];
}Entity_stathera;

typedef struct Entity_par
{
	char parMode[10];
	int offset;
}Entity_parameter;

typedef struct Entity_tempvar
{
	int offset;
}Entity_tempvariable;

typedef struct Entity
{
	char name[35];
	int type;
	int scopenumber;
	Entity_variable *variable;
	Entity_function *function;
	Entity_parameter *parameter;
	Entity_tempvariable *tempvariable;
	struct Entity *next;
}Entity;

typedef struct Scope
{
	Entity *newentity;
	int nestinglevel;
	struct Scope *next;
}Scope;

Scope *frontscopep=NULL, *endscopep=NULL;
Entity *nextentity=NULL;
funcinfo *frontfuncinfo = NULL;

int level=0;
int offsetcount = 8;

void functioninfo(char name[35], int fquad, int framelength);
void nextscope();
void newentity(char name[35], int type, char parMode[10], int startquad);
void newargument(char name[35], char parMode[10]);
void printfscope();
void deletescope();
void setoffset();

////////////////////////////-----symbols table globals-----///////////////////////////////////////

///////////////////////////////-----final code globals------//////////////////////////////////////
FILE *assembly_file;

Entity *scope_search(char name[35]);
void gnlvcode(char name[35]);
void loadvr(char var[35], char reg[10]);
void storerv(char reg[10], char var[35]);
void final(char startlabel[10], char endlabel[10]);
///////////////////////////////-----final code globals------//////////////////////////////////////

///////////////////////////////-------lex source-----/////////////////////////////////////////////
Twoinone lektikos()
{
	Twoinone tkandtkid;
	char nextchar, prevchar;
	char checklong[31];
	char checkdigit[6];
	int i=0, j=0, sum=0;

	memset(checklong, 0 ,31);
	memset(checkdigit, 0 , 6);
	

	while(1)						// To while mpainei gia ta sxolia otan ta diabasei kai ta kleisei de tha epistrepsei kati sth main, alla tha diabasei ton epomeno xarakthra kai tha sunexisei.
	{
		nextchar = fgetc(fp);
		if(nextchar == '\n')
		{
			rownum++;
		}
		
		while(isspace(nextchar) != 0)
		{
			nextchar = fgetc(fp);
			if(nextchar == '\n')		// Count the \n
			{
				rownum++;
			}
		}
		
		if(isalpha(nextchar) != 0)
		{
			i=0;
			checklong[i] = nextchar;
			nextchar = fgetc(fp);
			i++;
			while(((isalpha(nextchar) != 0) || (isdigit(nextchar) != 0))) 	// not sure for the isdigit  function
			{																//check if the nextchar variable is char or digit and is i<30
				if(i<30)
				{
					checklong[i] = nextchar;
					nextchar = fgetc(fp);
					i++;
				}
				else
				{
					nextchar = fgetc(fp);
				}
			}
			if(i >= 30)																	// if the word is bigger than 30 print warning
			{
				printf("Warning at line %d: The variable is more than 30 characters!!!\n", rownum);
			}
			if(strcmp("and", checklong) == 0)
			{
				ungetc(nextchar, fp); // put back the char in the file because the char which is in the nextchar variable stopped our while and we dont need it, if we dont put it back we may have problem with the \n counter!!!
				tkandtkid.tkID = 0;
				tkandtkid.tkID = andtk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("in", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = intk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("or", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = ortk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("while", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = whiletk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("call", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = calltk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("inout", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = inouttk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("print", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = printtk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("else", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = elsetk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("int", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = inttk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("program", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = programtk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("for", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = fortk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("input", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = inputtk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("return", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = returntk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("if", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = iftk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("not", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = nottk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			if(strcmp("void", checklong) == 0)
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = voidtk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
			else
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = stringtk;
				strcpy(tkandtkid.tk, checklong);				 
				return(tkandtkid);
			}
		}
	
		i=0;
		if(isdigit(nextchar) != 0)			// check if the nextchar is digit
		{
			checkdigit[i] = nextchar;        
			nextchar = fgetc(fp);

			while((isdigit(nextchar) != 0) && (i<5))
			{
				i++;
				checkdigit[i] = nextchar;
				nextchar = fgetc(fp);
			}
		
			if(i >= 5)
			{
				printf("Error at line %d: The number is bigger/smaller than 32676\n", rownum);
				exit(1);
			}
			else
			{
				for(j=0; j<i; j++)
				{
                    sum = atoi(checkdigit);
				}
				if(sum > 32676)
				{
					printf("Error at line %d: Number is bigger than 32676", rownum);
					exit(1);
				}
				else
				{
					ungetc(nextchar, fp);
					tkandtkid.tkID = digittk;
					strcpy(tkandtkid.tk, checkdigit);				 
					return(tkandtkid);
				}
			}
		}

		if(nextchar == '<')
		{
			nextchar = fgetc(fp);
			switch(nextchar)
			{
				case '>':
					tkandtkid.tkID = lowergreatertk;
					strcpy(tkandtkid.tk, "<>");
					return(tkandtkid);
				break;

				case '=':
					tkandtkid.tkID = lowerequaltk;
					strcpy(tkandtkid.tk, "<=");
					return(tkandtkid);
				break;

				default:
					ungetc(nextchar, fp);
					tkandtkid.tkID = lowertk;
					strcpy(tkandtkid.tk, "<");
					return(tkandtkid);
				break;
			}
		}

		if(nextchar == '>')
		{
			nextchar = fgetc(fp);
			switch(nextchar)
			{
				case '=':
					tkandtkid.tkID = greaterequaltk;
					strcpy(tkandtkid.tk, ">=");
					return(tkandtkid);
				break;

				default:
					ungetc(nextchar, fp);
					tkandtkid.tkID = greatertk;
					strcpy(tkandtkid.tk, ">");
					return(tkandtkid);
				break;
			}
		}

		if(nextchar == ':')
		{
			nextchar = fgetc(fp);
			switch(nextchar)
			{
				case '=':
					tkandtkid.tkID = eyesequaltk;
					strcpy(tkandtkid.tk, ":=");
					return(tkandtkid);
				break;

				default:
					ungetc(nextchar, fp);
					printf("Error at line %d: expected = after : (:=eyes)\n", rownum);
					exit(1);
				break;
			}
		}

		if(nextchar == '+')
		{
			nextchar = fgetc(fp);
			switch(nextchar)
			{
				case '=':
					tkandtkid.tkID = plusequaltk;
					strcpy(tkandtkid.tk, "+=");
					return(tkandtkid);
				break;

				default:
					ungetc(nextchar, fp);
					tkandtkid.tkID = plustk;
					strcpy(tkandtkid.tk, "+");
					return(tkandtkid);
				break;
			}
		}

		if(nextchar == '-')
		{
			tkandtkid.tkID = minustk;
			strcpy(tkandtkid.tk, "-");
			return(tkandtkid);
		}

		if(nextchar == '*')
		{
			tkandtkid.tkID = epitk;
			strcpy(tkandtkid.tk, "*");
			return(tkandtkid);
		}

		if(nextchar == ';')
		{
			tkandtkid.tkID = semitk;
			strcpy(tkandtkid.tk, ";");
			return(tkandtkid);
		}
		
		if(nextchar == '=')
		{
			tkandtkid.tkID = equaltk;
			strcpy(tkandtkid.tk, "=");
			return(tkandtkid);
		}

		if(nextchar == '(')
		{
			tkandtkid.tkID = leftpartk;
			strcpy(tkandtkid.tk, "(");
			return(tkandtkid);
		}

		if(nextchar == ')')
		{
			tkandtkid.tkID = rightpartk;
			strcpy(tkandtkid.tk, ")");
			return(tkandtkid);
		}

		if(nextchar == ',')
		{
			tkandtkid.tkID = commatk;
			strcpy(tkandtkid.tk, ",");
			return(tkandtkid);
		}

		if(nextchar == '[')
		{
			tkandtkid.tkID = leftagtk;
			strcpy(tkandtkid.tk, "[");
			return(tkandtkid);
		}

		if(nextchar == ']')
		{
			tkandtkid.tkID = rightagtk;
			strcpy(tkandtkid.tk, "]");
			return(tkandtkid);
		}

		if(nextchar == '{')
		{
			tkandtkid.tkID = leftaggistrotk;
			strcpy(tkandtkid.tk, "{");
			return(tkandtkid);
		}

		if(nextchar == '}')
		{
			tkandtkid.tkID = rightaggistrotk;
			strcpy(tkandtkid.tk, "}");
			return(tkandtkid);
		}
	
		if(nextchar == '/')
		{
			nextchar = fgetc(fp);
			if(nextchar != '*')
			{
				ungetc(nextchar, fp);
				tkandtkid.tkID = divtk;
				strcpy(tkandtkid.tk, "/");
				return(tkandtkid);
			}
			else
			{
				prevchar = 'a'; // Anathetou mia timh , gia na xanei to * kai ta ksanampainei sto while
				nextchar = fgetc(fp);
				while((prevchar != '*') && (nextchar != '/'))
				{
					prevchar = nextchar;
					nextchar = fgetc(fp);
					if(nextchar == EOF)
					{
						printf("Error at line %d: The comments are not closed!\n", rownum);
						exit(1);
					}
				}
			}
		}
		if(nextchar == EOF)
		{
			tkandtkid.tkID = eoftk;
			strcpy(tkandtkid.tk, "EOF");
			return(tkandtkid);
		}
	
		if(nextchar != '/')
		{
			printf("Error at line %d: Not supported characters\n", rownum);
			exit(1);
		}
	}
}
///////////////////////////////-------lex source-----/////////////////////////////////////////////

///////////////////////////////-----syntax source-----////////////////////////////////////////////
/*
With the readorno function we decide if we will or will not read new
lectical unit, which depend from the value of the yesorno variable.
*/

void readorno()
{
	if(yesorno == 1)
	{
		prevsyntax = syntactikal;
		syntactikal = lektikos();
		yesorno = 0;
	}
}

void program()
{
	char name[35];
	readorno();
	if(syntactikal.tkID == programtk)
	{
		yesorno = 1;
		readorno();
		if(syntactikal.tkID == stringtk)
		{
			strcpy(name,syntactikal.tk);
			genquad("begin_block",name,"_","_");
			yesorno = 1;
			nextscope();
			Block();
			genquad("end_block",name,"_","_");
			genquad("halt","_","_","_");
		}
		else
		{
			printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
	}
	else
	{
		printf("Error at line %d: Expexted PROGRAM after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Block()
{
	char startlabel[10], endlabel[10];
	
	sprintf(startlabel, "%d", label+1);
	readorno();
	if(syntactikal.tkID == leftaggistrotk)
	{
		yesorno = 1;
		Declarations();
		Subprograms();
		Sequence();
		readorno();
		if(syntactikal.tkID == rightaggistrotk)
		{
			yesorno = 1;
			printf("\nSuccess syntax\n");
		}
		else
		{
			printf("Error at line %d: Expected } after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		
	}
	else
	{
		printf("Error at line %d: Expected { after %s\n", rownum , prevsyntax.tk);
		exit(1);
	}
	sprintf(endlabel, "%d", label);
	final(startlabel, endlabel);
	deletescope();
	setoffset();
}

void Declarations()
{
	readorno();
	if(syntactikal.tkID == inttk)
	{
		yesorno = 1;
		Varlist();
		if(syntactikal.tkID != semitk)
		{
			printf("Error at line %d: Expected ; after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		else
		{
			yesorno = 1;
		}
	}
	else
	{
		printf("Error at line %d: Expected int after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Varlist()
{
	char parmode[10];

	readorno();
	if(syntactikal.tkID == stringtk)
	{
		newentity(syntactikal.tk, 1, parmode,0);
		yesorno = 1;
		readorno();
		while(syntactikal.tkID == commatk)
		{
			yesorno = 1;
			readorno();
			if(syntactikal.tkID == stringtk)
			{
				newentity(syntactikal.tk, 1, parmode,0);
				yesorno = 1;
				readorno();
			}
			else
			{
				printf("Error at line %d: Expected ID after %s\n", rownum, prevsyntax.tk);
				exit(1);
			}
		}
	}
}

void Subprograms()
{
	readorno();
	while(syntactikal.tkID == inttk || syntactikal.tkID == voidtk)
	{
		yesorno = 1;
		Procorfunc();
		readorno();
	}
}

void Procorfunc()
{
	char parmode[10];
	char name[35];
	int firstquad=0;
	//nextscope();
	readorno();
	if(syntactikal.tkID == stringtk)
	{
		//printf("\nI am here: %s\n", syntactikal.tk);
		//newentity(syntactikal.tk, 2, parmode,atoi(endp->label));
		strcpy(name, syntactikal.tk);
		genquad("begin_block",name,"_","_");
		newentity(syntactikal.tk, 2, parmode,atoi(endp->label));
		yesorno = 1;
		firstquad = label+1;
		offsetcount = 8;
		Funcbody();
		functioninfo(name, firstquad, offsetcount);
		genquad("end_block",name,"_","_");
	}
	else
	{
		printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Funcbody()
{
	Formalpars();
	Block();
}

void Formalpars()
{
	readorno();
	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		readorno();
		if(syntactikal.tkID == intk || syntactikal.tkID == inouttk)
		{
			Formalparlist();
			readorno();
		}

		if(syntactikal.tkID != rightpartk)
		{
			printf("Error at line %d: Expected ) after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		else
		{
			yesorno = 1;
		}
		
	}
	else
	{
		printf("Error at line %d: Expected ( after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Formalparlist()
{
	Argument *frontarg=NULL, *endarg=NULL;
	Entity *ent=NULL;

	Formalparitem();
	readorno();
	while(syntactikal.tkID == commatk)
	{
		yesorno = 1;
		Formalparitem();
		readorno();
	}
	ent = nextentity;
	nextscope();
	frontarg = ent->function->arglist;
	endarg = frontarg->next;
	while(endarg != NULL)
	{
		newentity(frontarg->name, 3, frontarg->parMode, 0);
		frontarg = endarg;
		endarg = endarg->next;
		if(endarg == NULL)
		{
			newentity(frontarg->name, 3, frontarg->parMode, 0);
		}
	}
}

void Formalparitem()
{
	char inorinout[35];
	memset(inorinout, 0 , 35);

	readorno();
	if(syntactikal.tkID == intk || syntactikal.tkID == inouttk)
	{
		strcpy(inorinout, syntactikal.tk);
		yesorno = 1;
		readorno();
	}
	
	if(syntactikal.tkID == stringtk)
	{
		newargument(syntactikal.tk, inorinout);
		//newentity(syntactikal.tk, 2, inorinout);
		if(strcmp(inorinout, "in") > 0)
		{
			genquad("par",syntactikal.tk,"REF","_");
		}
		else
		{
			genquad("par",syntactikal.tk,"CV","_");
		}
		yesorno = 1;
	}
	else
	{
		printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Sequence()
{
	Statement();
	readorno();
	while(syntactikal.tkID == semitk)
	{
		yesorno = 1;
		Statement();
		readorno();
	}
}

void BracketSeq()
{
	Sequence();
	readorno();
	if(syntactikal.tkID == rightaggistrotk )
	{
		yesorno = 1;
	}
	else
	{
		printf("Error at line %d: Expected } after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void BrackOrStat()
{
	readorno();
	if(syntactikal.tkID == leftaggistrotk )
	{
		yesorno = 1;
		BracketSeq();
	}
	else
	{
		yesorno = 1;
		Statement();
	}
}

void Statement()
{
	readorno();
	if(syntactikal.tkID == stringtk)
	{
		yesorno = 0;
		AssingmentStat();
	}
	else if(syntactikal.tkID == iftk)
	{
		yesorno = 1;
		IfStat();
	}
	else if(syntactikal.tkID == whiletk)
	{
		yesorno = 1;
		WhileStat();
	}
	else if(syntactikal.tkID == fortk)
	{
		yesorno = 1;
		ForStat();
	}
	else if(syntactikal.tkID == inputtk)
	{
		yesorno = 1;
		InputStat();
	}
	else if(syntactikal.tkID == printtk)
	{
		yesorno = 1;
		PrintStat();
	}
	else if(syntactikal.tkID == calltk)
	{
		yesorno = 1;
		CallStat();
	}
	else if(syntactikal.tkID == returntk)
	{
		yesorno = 1;
		ReturnStat();
	}
	else
	{
		yesorno = 0;
	}
}

void AssingmentStat()
{
	char name[35],expressionup[35];
	readorno();
	
	if(syntactikal.tkID == stringtk)
	{
		strcpy(name, syntactikal.tk);
		yesorno=1;
		readorno();
	}
	
	if(syntactikal.tkID == eyesequaltk)
	{
		yesorno = 1;
		Expression(expressionup);
		genquad(":=",expressionup,"_",name);						//??? = poios arithmos akrivos tha mpei ekei???
	}

	else
	{
		printf("Error at line %d: Expected := after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void IfStat()
{
	char nextlabel[10];
	labellist *Btrue=NULL, *Bfalse=NULL;

	memset(nextlabel, 0, 10);

	readorno();
	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		Condition(&Btrue, &Bfalse);
		readorno();
		if(syntactikal.tkID != rightpartk)
		{
			printf("Error at line %d: Expected ) after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		else
		{
			yesorno = 1;
		}
		sprintf(nextlabel, "%d", (label+1));
		backpatch(Btrue, nextlabel);
		BrackOrStat();
		sprintf(nextlabel,"%d",(label+1));
		backpatch(Bfalse, nextlabel);
		Elsepart();
	}
	else
	{
		printf("Error at line %d: Expected ( after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Elsepart()
{
	readorno();
	if(syntactikal.tkID == elsetk)
	{
		yesorno = 1;
		BrackOrStat();
	}
}

void WhileStat()
{
	char jumpfortrue[10], nextlabel[10];
	
	labellist *Btrue=NULL, *Bfalse=NULL;
	labellist *temp=NULL;

	readorno();
	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		readorno();
		sprintf(jumpfortrue,"%d",(label+1));
		Condition(&Btrue, &Bfalse);
		readorno();
		if(syntactikal.tkID == rightpartk)
		{
			yesorno = 1;
			readorno();
		}
		else
		{
			printf("Error at line %d: Expected ) after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		sprintf(nextlabel, "%d", (label+1));
		backpatch(Btrue, nextlabel);
		BrackOrStat();
		genquad("jump","_","_",jumpfortrue);

	}
	else
	{
		printf("Error at line %d: Expected ( after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
	sprintf(nextlabel, "%d", (label+1));
	backpatch(Bfalse, nextlabel);
}

void ForStat()
{
	char name[35],expressionup[35],temppros[35];
	char opsing[35];
	labellist *Btrue=NULL, *Bfalse=NULL, *temp=NULL;
	char nextlabel[10], jumpfortrue[10];
	readorno();

	memset(temppros,0,35);
	memset(expressionup,0,35);
	memset(name,0,35);
	memset(opsing,0,35);
	memset(nextlabel,0,10);
	memset(jumpfortrue,0,10);

	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		readorno();
		if(syntactikal.tkID == stringtk)
		{
			strcpy(name, syntactikal.tk);
			yesorno = 1;
			readorno();
			if(syntactikal.tkID == eyesequaltk)
			{
				yesorno = 1;
				Expression(expressionup);
				genquad(":=",expressionup,"_",name);			//??? = There is magic out there
				readorno();
				if(syntactikal.tkID == semitk)
				{
					yesorno = 1;
					sprintf(jumpfortrue,"%d",(label+1));
					Condition(&Btrue, &Bfalse);
					readorno();
					if(syntactikal.tkID == semitk)
					{
						yesorno = 1;
						readorno();
						if(syntactikal.tkID == stringtk)
						{
							strcpy(name, syntactikal.tk);						//intermediate
							yesorno = 1;
							readorno();
							if(syntactikal.tkID == plusequaltk)
							{
								yesorno = 1;
								OptionalSing();
								strcpy(opsing, syntactikal.tk);					//intermediate
								readorno();
								if(syntactikal.tkID == digittk)
								{
									yesorno = 1;
									strcpy(temppros, syntactikal.tk);
									if(strcmp(opsing, "-") != 0)				//intermediate
									{											//intermediate
										genquad("+",name,temppros,name);		//intermediate
										sprintf(nextlabel,"%d", (label+1));
										backpatch(Btrue, nextlabel);
									}											//intermediate
									else										//intermediate
									{											//intermediate
										genquad("-",name,temppros,name);		//intermediate
										sprintf(nextlabel,"%d", (label+1));
										backpatch(Btrue, nextlabel);
										free(Btrue);
									}											//intermediate
									
									readorno();
									if(syntactikal.tkID == rightpartk)
									{
										yesorno = 1;
										BrackOrStat();
										genquad("jump","_","_",jumpfortrue);
									}
									else
									{
										printf("Error at line %d: Expected ) after %s\n", rownum, prevsyntax.tk);
										exit(1);
									}
								}
								else
								{
									printf("Error at line %d: Expected digit after %s\n", rownum, prevsyntax.tk);
									exit(1);
								}
							}
							else
							{
								printf("Error at line %d: Expected += after %s\n", rownum, prevsyntax.tk);
								exit(1);
							}
						}
						else
						{
							printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
							exit(1);
						}
					}
					else
					{
						printf("Error at line %d: Expected ; after %s\n", rownum, prevsyntax.tk);
						exit(1);
					}
				}
				else
				{
					printf("Error at line %d: Expected ; after %s\n", rownum, prevsyntax.tk);
					exit(1);
				}
			}
			else
			{
				printf("Error at line %d: Expected := after %s\n", rownum, prevsyntax.tk);
				exit(1);
			}

		}
		else
		{
			printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
	}
	else
	{
		printf("Error at line %d: Expected ( after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
	sprintf(nextlabel, "%d", (label+1));
	backpatch(Bfalse, nextlabel);
}

void InputStat()
{
	readorno();
	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		readorno();
		if(syntactikal.tkID == stringtk)
		{
			yesorno = 1;
			readorno();
			if(syntactikal.tkID == rightpartk)
			{
				yesorno = 1;
			}
			else
			{
				printf("Error at line %d: Expectd ) after %s\n", rownum, prevsyntax.tk);
				exit(1);
			}
		}
		else
		{
			printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
	}
	else
	{
		printf("Error at line %d: Expected ( after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void PrintStat()
{
	char expressionup[35];
	readorno();
	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		Expression(expressionup);
		genquad("print","_","_",expressionup);
		readorno();
		if(syntactikal.tkID == rightpartk)
		{
			yesorno = 1;
		}
		else
		{
			printf("Error at line %d: Expected ) after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
	}
	else
	{
		printf("Error at line %d: Expectd ( after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void CallStat()
{
	//char parmode[10];

	readorno();
	if(syntactikal.tkID == stringtk)
	{
		//newentity(syntactikal.tk, 2, parmode);
		genquad("call",syntactikal.tk,"_","_");    // intermediate
		yesorno = 1;
		Actualpars();
	}
	else
	{
		printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Actualpars()
{
	readorno();
	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		readorno();
		if((syntactikal.tkID == intk) || (syntactikal.tkID == inouttk) )
		{
			Actualparlist();
		}
	
		if(syntactikal.tkID == rightpartk)
		{
			yesorno = 1;
		}
		else
		{
			printf("Error at line %d: Expected ) after %s", rownum, prevsyntax.tk);
			exit(1);
		}
	}

}

void Actualparlist()
{
	Actualparitem();
	readorno();
	while(syntactikal.tkID == commatk)
	{
		yesorno = 1;
		Actualparitem();
		readorno();
	}
}

void Actualparitem()
{
	char expressionup[35];
	char y[10], x[10];
	
	readorno();
	if(syntactikal.tkID == intk)
	{
		yesorno = 1;
		strcpy(x, syntactikal.tk);
		Expression(expressionup);
		//newargument(expressionup, x);
		genquad("par",expressionup,"CV","_");				//intermediate ??? = There is magic out there
	}
	else if (syntactikal.tkID == inouttk)
	{
		strcpy(y, syntactikal.tk);
		yesorno = 1;
		readorno();
		if(syntactikal.tkID != stringtk)
		{
			printf("Error at line %d: Expected variable after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		else
		{
			//newargument(syntactikal.tk, y);
			yesorno = 1;
		}
		genquad("par",syntactikal.tk,"REF","_");	//intermediate
	}
	else
	{
		printf("Error at line %d: Expected in or inout after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void ReturnStat()
{
	char expressionup[35];
	readorno();
	if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		Expression(expressionup);
		genquad("ret","_","_",expressionup);			//intermediate    -- mhpws thelei kai jump ekei pou kalesthke h sunarthsh?
		readorno();
		if(syntactikal.tkID == rightpartk)
		{
			yesorno = 1;
		}
		else
		{
			printf("Error at line %d: Expected ) after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
	}
	else
	{
		printf("Error at line %d: Expected ( after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void Condition(labellist **Btrue, labellist **Bfalse)
{
	labellist *Btruetemp=NULL, *Bfalsetemp=NULL;
	labellist *nextinbtrue=NULL, *temp=NULL;
	char nextlabel[10];

	Boolterm(&Btruetemp, &Bfalsetemp);
	*Btrue = nextinbtrue = Btruetemp;
	readorno();

	while(syntactikal.tkID == ortk)
	{
		yesorno = 1;
		sprintf(nextlabel,"%d", (label+1));
		backpatch(Bfalsetemp, nextlabel);
		Boolterm(&Btruetemp, &Bfalsetemp);
		*Btrue = merge(nextinbtrue,Btruetemp);
		readorno();
	}
	*Bfalse = Bfalsetemp;
}

void Boolterm(labellist **Btrue, labellist **Bfalse)
{
	labellist *Btruetemp=NULL, *Bfalsetemp=NULL;
	labellist *nextinbfalse=NULL;
	*Btrue=*Bfalse=NULL;
	char nextlabel[10];

	Boolfactor(&Btruetemp, &Bfalsetemp);
	*Bfalse = nextinbfalse = Bfalsetemp;
	readorno();

	while(syntactikal.tkID == andtk)
	{
		yesorno = 1;
		sprintf(nextlabel,"%d", (label+1));
		backpatch(Btruetemp, nextlabel);
		Boolfactor(&Btruetemp, &Bfalsetemp);
		*Bfalse = merge(nextinbfalse, Bfalsetemp);
		readorno();
	}
	*Btrue = Btruetemp;
	
}

void Boolfactor(labellist **Btrue, labellist **Bfalse)
{
	char expressup1[35], relationopup[35], expressup2[35];
	labellist *temp=NULL;
	labellist *Btruetemp=NULL, *Bfalsetemp=NULL;
	nextquadstruct *tempp = NULL;

	readorno();
	if(syntactikal.tkID == nottk)
	{
		yesorno = 1;
		readorno();
		if(syntactikal.tkID == leftagtk)
		{
			yesorno = 1;
			Condition(&Btruetemp, &Bfalsetemp);
			*Btrue = Btruetemp;
			*Bfalse = Bfalsetemp;
			temp = *Btrue;
			*Btrue = *Bfalse;
			*Bfalse = temp;

		}
		else
		{
			printf("Error at line %d: Expected [ after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		readorno();
		if(syntactikal.tkID == rightagtk)
		{
			yesorno = 1;
		}
		else
		{
			printf("Error at line %d: Expected ] after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
	}
	else if(syntactikal.tkID == leftagtk)
	{
		yesorno = 1;
		Condition(&Btruetemp, &Bfalsetemp);
		*Btrue = Btruetemp;
		*Bfalse = Bfalsetemp;
		readorno();
		if(syntactikal.tkID == rightagtk)
		{
			yesorno = 1;
			readorno();
		}
		else
		{
			printf("Error at line %d: Expected ] after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
	}
	else
	{
		Expression(expressup1);
		RelationOper(relationopup);
		Expression(expressup2);
		genquad(relationopup, expressup1, expressup2,"_");
		*Btrue = makelist();
		genquad("jump","_","_","_");
		*Bfalse = makelist();
	}
}

void Expression(char expressionup[35])
{
	char plusorminus[35], constorid1[35], constorid2[35], temppros[35];		////////////////
	char *temp = NULL,w[35];												//intermediate//
	char parmode[10];
	int flag=0;																////////////////
	memset(plusorminus,0,35);
	memset(constorid1,0,35);
	memset(constorid2,0,35);
	memset(temppros,0,35);
	memset(parmode,0,10);

	
	OptionalSing(plusorminus);
	Term(constorid1);
	readorno();

	if(strcmp(plusorminus, "-") == 0)										////////////////
	{																		////////////////
		sprintf(w, "-%s", constorid1);										//intermediate//
		strcpy(constorid1, w);												////////////////
	}																		////////////////
	else																	////////////////
	{																		////////////////
		sprintf(w, "%s", constorid1);										//intermediate//
		strcpy(constorid1, w);												////////////////
	}																		////////////////

	if(syntactikal.tkID != plustk || syntactikal.tkID != minustk)			////////////////
	{																		////////////////
		strcpy(w, constorid1);												//intermediate//
	}																		////////////////

	while(syntactikal.tkID == plustk || syntactikal.tkID == minustk)
	{
		strcpy(temppros, syntactikal.tk);
		temp = newtemp();													//intermediate//
		newentity(temp, 4, parmode,0);
		yesorno = 1;
		AddOper();
		Term(constorid2);
		if(flag == 0)														////////////////
		{																	////////////////
			genquad(temppros,constorid1,constorid2,temp);					////////////////
			strcpy(w, temp);												////////////////
			flag++;															////////////////
		}																	//intermediate//
		else																////////////////
		{																	////////////////
			genquad(syntactikal.tk,w,constorid2,temp);						////////////////
			strcpy(w, temp);												////////////////
		}	
		readorno();
	}
	strcpy(expressionup, w);												//intermediate//
}

void Term(char constorid[35])
{
	char factorreturns1[35];												////////////////
	char factorreturns2[35];												////////////////
	char *temp = NULL,w[35];												//intermediate//
	char parmode[10];
	int flag=0;																////////////////

	Factor(factorreturns1);
	readorno();
	if(syntactikal.tkID != epitk || syntactikal.tkID != divtk)				////////////////
	{																		////////////////
		strcpy(w, factorreturns1);											//intermediate//
	}																		//////////////

	while(syntactikal.tkID == epitk || syntactikal.tkID == divtk)
	{
		temp = newtemp();													//intermediate//
		newentity(temp, 4, parmode,0);
		yesorno = 1;
		MulOper();
		Factor(factorreturns2);
		if(flag == 0)														////////////////
		{																	////////////////
			genquad(syntactikal.tk,factorreturns1,factorreturns2,temp);		////////////////
			strcpy(w, temp);												////////////////
			flag++;															////////////////
		}																	//intermediate//
		else																////////////////
		{																	////////////////
			genquad(syntactikal.tk,w,factorreturns2,temp);					////////////////
			strcpy(w, temp);												////////////////
		}																	////////////////
		readorno();
	}
	strcpy(constorid, w);													//intermediate//
}

void Factor(char factorreturns[35])
{
	readorno();
	if(syntactikal.tkID == digittk)
	{
		strcpy(factorreturns, syntactikal.tk);
		yesorno = 1;
	}
	else if(syntactikal.tkID == leftpartk)
	{
		yesorno = 1;
		Expression(factorreturns);
		readorno();
		if(syntactikal.tkID != rightpartk)
		{
			printf("Error at line %d: Expected ) after %s\n", rownum, prevsyntax.tk);
			exit(1);
		}
		else
		{
			yesorno = 1;
		}
	}
	else if (syntactikal.tkID == stringtk)
	{
		strcpy(factorreturns, syntactikal.tk);
		yesorno = 1;
		Actualpars();
	}
	else
	{
		printf("Error at line %d: Expected constant or ( or variable after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}
}

void RelationOper(char relationup[35])
{
	//         =      |      < ( e  |  =  |  > )       |        >      (  e  |  = )
	readorno();
	if((syntactikal.tkID == equaltk) || (syntactikal.tkID == lowertk) || (syntactikal.tkID == lowerequaltk) || (syntactikal.tkID == lowergreatertk) || (syntactikal.tkID == greatertk) || (syntactikal.tkID == greaterequaltk))
	{
		yesorno = 1;
		strcpy(relationup, syntactikal.tk);
	}
	else
	{
		printf("Error at line %d: Expected = | < | <= | <> | > | >= after %s\n", rownum, prevsyntax.tk);
		exit(1);
	}

}

void AddOper()
{
}

void MulOper()
{
}

void OptionalSing(char plusorminus[35])
{
	readorno();
	if(syntactikal.tkID == plustk || syntactikal.tkID == minustk)
	{
		strcpy(plusorminus, syntactikal.tk);
		yesorno = 1;
		AddOper();
	}
}

///////////////////////////////-----syntax source-----////////////////////////////////////////////

///////////////////////////////-----intermediate code source-----/////////////////////////////////
int nextquad()
{
	label++;
	return(label);
}

void genquad(char op1[35], char op2[35], char op3[35], char op4[35])
{
	nextquadstruct *quad = NULL;
	char nextlabel[10];
	int llabel;

	llabel = nextquad();
	sprintf(nextlabel,"%d", llabel);	

	quad = (nextquadstruct *)malloc(sizeof(nextquadstruct));
	if(quad == NULL)
	{
		printf("Problem with malloc!!!\n");
		exit(1);
	}

	if(frontp == NULL)
	{
		frontp = quad;
		endp = quad;
		endp->next = NULL;
	}
	else
	{
		endp->next = quad;
		endp = endp->next;
		endp->next = NULL;
	}

	strcpy(endp->label, nextlabel);
	strcpy(endp->op1, op1);
	strcpy(endp->op2, op2);
	strcpy(endp->op3, op3);
	strcpy(endp->op4, op4);
}

char *newtemp()
{
		char *nextvar = NULL;
		nextvar = (char *)malloc(10*sizeof(char));
		nexttempvar++;
		sprintf(nextvar,"T_%d", nexttempvar);

		return(nextvar);
	}

labellist *emptylist()
{
	labellist *newnode=NULL;
	
	newnode = (labellist *)malloc(sizeof(labellist));
	if(newnode == NULL)
	{
		printf("Problem with malloc!!!\n");
		exit(1);
	}
	newnode->next=NULL;
	return(newnode);
}

labellist *makelist()
{
	labellist *newlist = NULL;
	newlist = emptylist();
	
	newlist->jumpto = endp; 
	return(newlist);
}

labellist *merge( labellist *list1, labellist *list2 )
{
	labellist *tempp=NULL;
	
	tempp=list1;
	
	if( list1 == NULL )
	{
	      return( list2 );
	}  
	
	while( tempp->next != NULL )
	{
	      tempp=tempp->next;
	}
	tempp->next=list2;
	return(list1);
}


void backpatch( labellist *list , char z[10] )
{
	labellist *tempp = NULL;
	//nextquadstruct *tempquad=NULL;
	tempp = list;
	while(tempp != NULL)
	{
		strcpy(tempp->jumpto->op4, z);
		tempp = tempp->next;

		free(list);
		list = tempp;
	}
}

///////////////////////////////-----intermediate code source-----/////////////////////////////////

//////////////////////////////////-----symbols table source------/////////////////////////////////

void functioninfo(char name[35], int fquad, int framelength)
{
	funcinfo *newfuncinfo, *temp=NULL;
	
	newfuncinfo = (funcinfo *)malloc(sizeof(funcinfo));
	if(newfuncinfo == NULL)
	{
		printf("\nCan not create new funcinfo\n");
		exit(1);
	}
	
	if(frontfuncinfo == NULL)
	{
		frontfuncinfo = newfuncinfo;
		frontfuncinfo->next = NULL;
		strcpy(frontfuncinfo->name, name);
		frontfuncinfo->firstquad = fquad;
		frontfuncinfo->framelength = framelength;
	}
	else
	{
		temp = frontfuncinfo;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newfuncinfo;
		newfuncinfo->next = NULL;
		strcpy(frontfuncinfo->name, name);
		frontfuncinfo->firstquad = fquad;
		frontfuncinfo->framelength = framelength;
	}
}

void nextscope()
{
	Scope *newscope;
	
	newscope = (Scope *)malloc(sizeof(Scope));
	if(newscope == NULL)
	{
		printf("\nNew scope creation failed\n");
		exit(1);
	}
	if(frontscopep == NULL)
	{
		level++;
		frontscopep = newscope;
		endscopep = newscope;
		endscopep->next = NULL;
		endscopep->nestinglevel = level;
		endscopep->newentity = NULL;
	}
	else
	{
		level++;
		endscopep->next = newscope;
		endscopep = newscope;
		endscopep->next = NULL;
		endscopep->nestinglevel = level;
		endscopep->newentity = NULL;
	}
}

void newentity(char name[35], int type, char parMode[10], int startquad)
{
	Entity *newentity=NULL;
	Entity_variable *newentvar=NULL;
	Entity_function *newentfunc=NULL;
	Entity_parameter *newentpar=NULL;
	Entity_tempvariable *newenttemp=NULL;
	funcinfo *tempfuncinfo = NULL;
	
	newentity = (Entity *)malloc(sizeof(Entity));
	if(newentity == NULL)
	{
		printf("\nNew entity creation failed\n");
		exit(1);
	}
	if(endscopep->newentity == NULL)
	{
		nextentity = newentity;
		nextentity->next = NULL;
		endscopep->newentity = nextentity;
		strcpy(nextentity->name, name);
		nextentity->type = type;
		nextentity->scopenumber = level;
		nextentity->variable = NULL;
		nextentity->function = NULL;
		nextentity->parameter = NULL;
		nextentity->tempvariable = NULL;
	}
	else
	{
		nextentity->next = newentity;
		nextentity = newentity;
		nextentity->next = NULL;
		strcpy(nextentity->name, name);
		nextentity->type = type;
		nextentity->scopenumber = level;
		nextentity->variable = NULL;
		nextentity->function = NULL;
		nextentity->parameter = NULL;
		nextentity->tempvariable = NULL;
	}
	
	if(type == 1)
	{
		newentvar = (Entity_variable *)malloc(sizeof(Entity_variable));
		if(newentvar == NULL)
		{
			printf("\nNew Entity variable creation failed\n");
			exit(1);
		}
		offsetcount += 4;
		newentvar->offset = offsetcount;
		nextentity->variable = newentvar;
	}
	else if(type == 2)
	{
		newentfunc = (Entity_function *)malloc(sizeof(Entity_function));
		if(newentfunc == NULL)
		{
			printf("\nNew Entity function creation failed\n");
			exit(1);
		}
		nextentity->function = newentfunc;
		nextentity->function->startQuad = startquad;
		newentfunc->arglist = NULL;
	}
	else if(type == 3)
	{
		newentpar = (Entity_parameter *)malloc(sizeof(Entity_parameter));
		if(newentpar == NULL)
		{
			printf("\nNew Entity parameter creation failed\n");
			exit(1);
		}
		strcpy(newentpar->parMode, parMode);
		offsetcount += 4;
		newentpar->offset = offsetcount;
		nextentity->parameter = newentpar;
	}
	else if(type == 4)
	{
		newenttemp = (Entity_tempvariable *)malloc(sizeof(Entity_tempvariable));
		if(newenttemp == NULL)
		{
			printf("\nNew Entity temporary variable creation failed\n");
			exit(1);
		}
		offsetcount += 4;
		newenttemp->offset = offsetcount;
		nextentity->tempvariable = newenttemp;
	}
	else
	{
		printf("\nWrong number of type\n");
		exit(1);
	}
}

void newargument(char name[35], char parMode[10])
{
	Argument *newarg, *temp=NULL;
	newarg = (Argument *)malloc(sizeof(Argument));
	if(newarg == NULL)
	{
		printf("\nNew argument creation failed\n");
		exit(1);
	}
	
	if(nextentity->function->arglist == NULL)
	{
		nextentity->function->arglist = newarg;
		newarg->next =  NULL;
	}
	else
	{
		temp = nextentity->function->arglist;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newarg;
		newarg->next = NULL;
	}
	
	memset(newarg->name, 0, 35);
	memset(newarg->parMode, 0, 10);
	strcpy(newarg->name, name);
	strcpy(newarg->parMode, parMode);
}

void printfscope()
{
	Entity *printentities=NULL;
	Argument *printargs=NULL;
		
	printentities = endscopep->newentity;
	printf("\n");

	while(printentities != NULL)
	{
		if(printentities->type == 1)
		{
			printf("Name: %s", printentities->name);
			printf(" \tOffset: %d\n", printentities->variable->offset);
		}
		else if(printentities->type == 2)
		{
			printf("Name: %s\n", printentities->name);
			if(printentities->function->arglist != NULL)
			{
				printargs = printentities->function->arglist;
				while(printargs != NULL)
				{
					printf("\tName: %s", printargs->name);
					printf("   ParMode: %s\n", printargs->parMode);
					printargs = printargs->next;
				}
			}
		}
		else if(printentities->type == 3)
		{
			printf("ParMode: %s", printentities->name);
			printf("\tOffset: %d\n", printentities->parameter->offset);
		}
		else
		{
			printf("Name: %s", printentities->name);
			printf("\tOffset: %d\n", printentities->tempvariable->offset);
		}
		printentities = printentities->next;
	}
}

void deletescope()
{
	Entity *tempfront=NULL, *tempend=NULL;
	Argument *argtempfront=NULL, *argtempend=NULL;
	Scope *temp=NULL;

	tempfront = endscopep->newentity;
	tempend = tempfront->next;
	printfscope();

	while(tempfront != nextentity)
	{
		if(tempfront->type != 2)
		{
			free(tempfront);
			tempfront = tempend;
			tempend = tempend->next;
		}
		else
		{
			argtempfront = tempfront->function->arglist;
			argtempend = argtempfront->next;
			while(argtempend != NULL)
			{
				free(argtempfront);
				argtempfront = argtempend;
				argtempend = argtempend->next;
			}
			free(argtempfront);
			free(tempfront);
			tempfront = tempend;
			tempend = tempend->next;
		}
	}
	free(tempfront);
	temp = frontscopep;
	if(frontscopep == endscopep)
	{
		free(endscopep);
	}
	else
	{
		while(temp->next != endscopep)
		{
			temp = temp->next;
		}
		free(endscopep);
		endscopep = temp;
		endscopep->next =NULL;
		nextentity = endscopep->newentity;
		while(nextentity->next != NULL)
		{
			nextentity = nextentity->next;
		}
	}
	level--;
}

void freefunctioninfo()
{
	funcinfo *temp=NULL;
	
	while(frontfuncinfo != NULL)
	{
		temp = frontfuncinfo->next;
		free(frontfuncinfo);
		frontfuncinfo = temp;
	}
}

void setoffset()
{
	Entity *ent=NULL;
	
	ent = endscopep->newentity;
	
	while(ent != NULL)
	{
		if(ent->type == 1)
		{
			offsetcount = ent->variable->offset;
		}
		else if(ent->type == 3)
		{
			offsetcount = ent->parameter->offset;
		}
		else if(ent->type == 4)
		{
			offsetcount = ent->tempvariable->offset;
		}
		ent = ent->next;
	}
}

//////////////////////////////////-----symbols table source------/////////////////////////////////

//////////////////////////////////-----final code source-------///////////////////////////////////

Entity *scope_search(char name[35])
{
	Entity *ent=NULL;
	Scope *scp=NULL;
	int flag=0;
	
	printf("name in search: %s", name);

	scp = frontscopep;
	while((scp	!= NULL) && (flag == 0))
	{
		ent = scp->newentity;
		while((ent!=NULL) && (flag==0))
		{
			if(strcmp(ent->name, name) == 0)
			{
				flag = 1;
			}
			ent = ent->next;
		}
		scp = scp->next;
	}

	if(flag != 0)
	{
		return(ent);
	}
	else
	{
		printf("Error: The %s does not exist!!!\n", name);
		exit(1);
	}
}

void gnlvcode(char name[35])
{
	Entity *ent=NULL;
	int looptime=0,i;
	
	ent = scope_search(name);
	
	fprintf(assembly_file, "movi R[255],M[4+R[0]]\n");
	looptime = (endscopep->nestinglevel) - (ent->scopenumber); //Etsi vriskoume posa scope tha katevei
	for(i=0; i<(looptime-1); i++)
	{
		fprintf(assembly_file, "movi R[255],M[4+R[255]]\n");
	}
	if(ent->type == 1)
	{
		fprintf(assembly_file, "movi R[254],%d\n", ent->variable->offset);
	}
	else if(ent->type == 3)
	{
		fprintf(assembly_file, "movi R[254],%d\n", ent->parameter->offset);
	}
	else if(ent->type == 4)
	{
		fprintf(assembly_file, "movi R[254],%d\n", ent->tempvariable->offset);
	}
	fprintf(assembly_file, "addi R[255],R[254],R[255]\n");
	
}

void loadvr(char var[35], char reg[10])
{
	Entity *ent=NULL;
	int tempvar=0;
	int flag = 0;

	printf("\n%s\n", var);
	if(isdigit(atoi(var)) != 0)
	{
		fprintf(assembly_file, "movi R[%s]\n", reg);
	}
	else
	{
	ent = scope_search(var);
	}
	
	if((ent->scopenumber == frontscopep->nestinglevel) && (ent->type==1))
	{
		tempvar = 600 + (ent->variable->offset);
		fprintf(assembly_file, "movi R[%s],M[%d]\n", reg, tempvar);
	}
	else if(((ent->type==1) || (ent->type==4) || ((ent->type==3)  && (strcmp(ent->parameter->parMode, "in") == 0))) &&  (ent->scopenumber == endscopep->nestinglevel))
	{
		if(ent->type == 1)
		{
			fprintf(assembly_file, "movi R[%s],M[%d+R[0]]\n", reg, ent->variable->offset);
		}
		else if(ent->type == 3)
		{
			fprintf(assembly_file, "movi R[%s],M[%d+R[0]]\n", reg, ent->parameter->offset);
		}
		else if(ent->type == 4)
		{
			fprintf(assembly_file, "movi R[%s],M[%d+R[0]]\n", reg, ent->tempvariable->offset);
		}
	}
	else if((ent->type == 3) && (strcmp(ent->parameter->parMode, "inout") == 0) && (ent->scopenumber == endscopep->nestinglevel))
	{
		fprintf(assembly_file, "movi R[255],M[%d+R[0]]\n", ent->parameter->offset);
		fprintf(assembly_file, "movi R[%s],M[%d+R[0]]\n", reg, ent->parameter->offset);
	}
	else if(((ent->type==1) || ((ent->type==3)  && (strcmp(ent->parameter->parMode, "in") == 0))) && (ent->scopenumber < endscopep->nestinglevel))
	{
		gnlvcode(ent->name);
		fprintf(assembly_file, "movi R[%s],M[R[255]]\n", reg);
	}
	else if((ent->type == 3) && (strcmp(ent->parameter->parMode, "inout") == 0) && (ent->scopenumber < endscopep->nestinglevel))
	{
		gnlvcode(ent->name);
		fprintf(assembly_file, "movi R[255],M[R[255]]\n");
		fprintf(assembly_file, "movi R[%s],M[R[255]]\n", reg);
	}
}

void storerv(char reg[10], char var[35])
{
	Entity *ent=NULL;
	int tempvar=0;
	ent = scope_search(var);
	printf("\nentity name: %s\n", ent->name);
	printf("level: %d", ent->scopenumber);
	if((ent->scopenumber == frontscopep->nestinglevel) && (ent->type==1))
	{printf("\nstrrrrrrrr000000\n");
		tempvar = 600 + (ent->variable->offset);
		fprintf(assembly_file, "movi M[%d],R[%s]\n",tempvar, reg );
	}
	else if(((ent->type==1) || ((ent->type==3)  && (strcmp(ent->parameter->parMode, "in") == 0))) &&  (ent->scopenumber == endscopep->nestinglevel))
	{printf("\nstrrrrrrrr2\n");
		if(ent->type == 1)
		{
			fprintf(assembly_file, "movi M[%d+R[0]],R[%s]\n",ent->variable->offset, reg);
		}
		else if(ent->type == 3)
		{
			fprintf(assembly_file, "movi M[%d+R[0]],R[%s]\n",ent->parameter->offset, reg);
		}
	}
	else if((ent->type == 3) && (strcmp(ent->parameter->parMode, "inout") == 0) && (ent->scopenumber == endscopep->nestinglevel))
	{printf("\nstrrrrrrrrrrrrrrrrrr3\n");
		fprintf(assembly_file, "movi R[255],M[%d+R[0]]\n", ent->parameter->offset);
		fprintf(assembly_file, "movi M[R[255]],R[%s]\n",reg);
	}
	else if(((ent->type==1) || (ent->type==4) || ((ent->type==3)  && (strcmp(ent->parameter->parMode, "in") == 0))) &&  (ent->scopenumber < endscopep->nestinglevel))
	{printf("\nstffffffffffffffffff4\n");
		gnlvcode(ent->name);
		fprintf(assembly_file, "movi M[R[255]],R[%s]\n",reg);
	}
	else if((ent->type == 3) && (strcmp(ent->parameter->parMode, "inout") == 0) && (ent->scopenumber < endscopep->nestinglevel))
	{printf("\nstffffffffff5\n");
		gnlvcode(ent->name);
		fprintf(assembly_file, "movi R[255],M[R[255]]\n");
		fprintf(assembly_file, "movi M[R[255]],R[%s]\n",reg);
	}
}

void final(char startlabel[10], char endlabel[10])
{	
	nextquadstruct *quad=NULL, *tempquad=NULL;
	Entity *ent=NULL;
	quad = frontp;
	int d=0, temp=0;

	//find the quad
	while(strcmp(quad->label, startlabel) != 0)
	{
		tempquad = quad;
		quad = quad->next;
	}
	temp = atoi(endlabel)+1;
	sprintf(endlabel, "%d", temp);
	while(strcmp(quad->label, endlabel) != 0)
	{
		if(strcmp(quad->op1, "jump") == 0)
		{
			fprintf(assembly_file, "jmp L%s:\n", quad->op4);
		}
		else if((strcmp(quad->op1, "=") == 0) || (strcmp(quad->op1, "<") == 0) || (strcmp(quad->op1, "<>") == 0) || (strcmp(quad->op1, ">") == 0) || (strcmp(quad->op1, ">=") == 0) || (strcmp(quad->op1, "<=") == 0))
		{
			loadvr(quad->op2, "1");
			loadvr(quad->op3, "2");
			fprintf(assembly_file, "cmpi R[1],R[2]\n");
			if((strcmp(quad->op1, "=") == 0))
			{
				fprintf(assembly_file, "je L%s\n", quad->op4);
			}
			else if((strcmp(quad->op1, "<") == 0))
			{
				fprintf(assembly_file, "ja L%s\n", quad->op4);
			}
			else if((strcmp(quad->op1, ">") == 0))
			{
				fprintf(assembly_file, "jb L%s\n", quad->op4);
			}
			else if((strcmp(quad->op1, "<>") == 0))
			{
				fprintf(assembly_file, "jne L%s\n", quad->op4);
			}
			else if((strcmp(quad->op1, ">=") == 0))
			{
				fprintf(assembly_file, "jbe L%s\n", quad->op4);
			}
			else if((strcmp(quad->op1, "<=") == 0))
			{
				fprintf(assembly_file, "jae L%s\n", quad->op4);
			}
			else
			{
				printf("\nRelop error\n");
				exit(1);
			}
		}
		else if((strcmp(quad->op1, ":=") == 0))
		{
			loadvr(quad->op2,"1");
			printf("\nfirst call of store:%s\n", quad->op4);
			storerv("1", quad->op4);
		}
		else if((strcmp(quad->op1, "+") == 0) || (strcmp(quad->op1, "-") == 0) || (strcmp(quad->op1, "*") == 0) || (strcmp(quad->op1, "/") == 0))
		{
			loadvr(quad->op2, "1");
			loadvr(quad->op3, "2");
			if(strcmp(quad->op1, "+") == 0)
			{
				fprintf(assembly_file, "addi R[3],R[1],R[2]\n");
			}
			else if(strcmp(quad->op1, "-") == 0)
			{
				fprintf(assembly_file, "subi R[3],R[1],R[2]\n");
			}
			else if(strcmp(quad->op1, "*") == 0)
			{
				fprintf(assembly_file, "muli R[3],R[1],R[2]\n");
			}
			else if(strcmp(quad->op1, "/") == 0)
			{
				fprintf(assembly_file, "divi R[3],R[1],R[2]\n");
			}
			else
			{
				printf("\nOp error\n");
				exit(1);
			}
			printf("\nSecond call of storerv:%s\n", quad->op4);
			storerv("3", quad->op4);
		}
		else if((strcmp(quad->op1, "print") == 0))
		{
			loadvr(quad->op4, "1");
			fprintf(assembly_file, "outi R[1]\n");
		}
		else if((strcmp(quad->op1, "ret") == 0))
		{
			loadvr(quad->op4, "1");
			fprintf(assembly_file, "movi R[255],M[8+R[0]]\n");
			fprintf(assembly_file, "movi M[R[255]],R[1]\n");
		}
		else if((strcmp(quad->op3, "par") == 0))
		{
			ent = scope_search(quad->op2);
			d = offsetcount + (ent->parameter->offset);
			if((strcmp(quad->op1, "CV") == 0))
			{
				loadvr(quad->op2, "255");
				fprintf(assembly_file, "movi M[d+R[0]],R[255]\n");
			}
			else if((strcmp(quad->op1, "REF") == 0))
			{
				if(endscopep->nestinglevel == ent->scopenumber)
				{
					if((ent->type == 1) || ((ent->type == 3) && strcmp(ent->parameter->parMode, "in")==0))
					{
						fprintf(assembly_file, "movi R[255],R[0]\n");
						if(ent->type == 1)
						{
							fprintf(assembly_file, "movi R[254],%d\n", ent->variable->offset);
						}
						else if(ent->type == 3)
						{
							fprintf(assembly_file, "movi R[254],%d\n", ent->parameter->offset);
						}
						fprintf(assembly_file, "addi R[255],R[254],R[255]\n");
						fprintf(assembly_file, "movi M[d+R[0]],R[255]\n");
					}
					else if(((ent->type == 3) && strcmp(ent->parameter->parMode, "inout")==0))
					{
						fprintf(assembly_file, "movi R[255],R[0]\n");
						fprintf(assembly_file, "movi R[254],%d\n", ent->parameter->offset);
						fprintf(assembly_file, "addi R[255],R[254],R[255]\n");
						fprintf(assembly_file, "movi R[1],M[R[255]]\n");
						fprintf(assembly_file, "movi M[d+R[0]],R[1]\n");
					}
				}
				else if(endscopep->nestinglevel != ent->scopenumber)
				{
					if(endscopep->nestinglevel == ent->scopenumber)
					{
						gnlvcode(ent->name);
						fprintf(assembly_file, "movi M[d+R[0]],R[255]\n");
					}
					else if(((ent->type == 3) && strcmp(ent->parameter->parMode, "inout")==0))
					{
						gnlvcode(ent->name);
						fprintf(assembly_file, "movi R[1],M[R[255]]\n");
						fprintf(assembly_file, "movi M[d+R[0]],R[1]\n");
					}
				}
			}
		}
		/*else if()
		{
			fprintf(assembly_file, "movi R[255],R[0]\n");
			fprintf(assembly_file, "movi R[254],%d\n", ent->  ->offset);
			fprintf(assembly_file, "addi R[255],R[254],R[255]\n");
			fprintf(assembly_file, "movi M[d+R[0]],R[1]\n");
		}*/
		else if((strcmp(quad->op1, "call") == 0))
		{
			ent = scope_search(quad->op2);
			if(endscopep->nestinglevel == ent->scopenumber)
			{
				fprintf(assembly_file, "movi R[255],M[4+R[0]\n");
				fprintf(assembly_file, "movi M[%d+4+R[0]],R[255]\n", offsetcount);
			}
			else
			{
				fprintf(assembly_file, "movi M[%d+4+R[0]],R[0]\n", offsetcount);
			}
			fprintf(assembly_file,"movi R[255],%d\n",offsetcount);
			fprintf(assembly_file,"addi R[0],R[255],R[0]\n");
			fprintf(assembly_file,"movi R[255],$\n");
			fprintf(assembly_file,"movi R[254],15\n");
			fprintf(assembly_file,"addi R[255],R[255],R[254]\n");
			fprintf(assembly_file,"movi M[R[0]],R[255]\n");
			fprintf(assembly_file,"jmp %d\n",ent->function->startQuad);
			fprintf(assembly_file,"subi R[0],R[0],R[255]\n");
		}
		else if((strcmp(quad->op1, "end_block") == 0))
		{
			fprintf(assembly_file, "jmp M[R[0]]\n");
		}
		else if((strcmp(quad->op1, "halt") == 0))
		{
			fprintf(assembly_file, "halt\n");
		}
		tempquad->next = quad->next;
		free(quad);
		quad = tempquad->next;
	}
}

//////////////////////////////////-----final code source-------///////////////////////////////////

main()
{
    int x=0;
	nextquadstruct *printlist = frontp;
	Twoinone tkIDandtk;   
	fp=fopen("test.txt","r");
	if(fp == NULL)
	{
        printf("Can not open file.\n");
        exit(1);
	}
	assembly_file = fopen("endfile.txt", "w");
	if(assembly_file == NULL)
	{
		printf("Can not open assembly_file\n");
		exit(1);
	}

	program();

	tkIDandtk = lektikos();
	/*while(tkIDandtk.tkID != eoftk)
	{
		printf("tokenid: %d\t", tkIDandtk.tkID);
		printf("token: %s\n", tkIDandtk.tk);
		tkIDandtk = lektikos();
	}*/
	
	/*printlist = frontp;
	while(printlist != endp)
	{
		printf("%s: %s,%s,%s,%s\n", printlist->label, printlist->op1, printlist->op2, printlist->op3, printlist->op4);
		printlist = printlist->next;
	}
	printf("%s: %s,%s,%s,%s\n", printlist->label, printlist->op1, printlist->op2, printlist->op3, printlist->op4);
*/
	freefunctioninfo();
	fclose(fp);
	fclose(assembly_file);
}
