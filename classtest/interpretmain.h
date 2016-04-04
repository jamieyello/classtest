#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdlib> // used for rand().
#include <ctime> // used for time().
#include <iomanip>
#include "hashtable.h"
#include "hashtables.h"
#include "glut.h"

using namespace std;
stringstream ss;

//Type
//1 func or return value (0-1)
//2 minimum number of required values (>=0)
//3 max number of required values (>=0)
//4 takes in string
//5 takes in number

//Words that are prevented from being used as variable names.
extern const std::string globclist[] = { "PRINT", "?", "CLS", "END", "GOTO", "STOP", "IF", "THEN", "ELSE", "INPUT", "GOSUB", "RETURN", "FOR", "TO", "NEXT", "WAIT", "VSYNC", "REM", "SWAP", "GCLS", "GBOX", "GCOLOR", "GCIRCLE", "LOCATE", "PNLSTR", "GPSET", "DIM", "GPAGE", "ACLS", "DATA", "READ", "RESTORE" };
extern const int globcnum = 32;
extern const std::string globvlist[] = { "TRUE", "FALSE", "RND", "PI", "ERR", "ERL", "FLOOR", "ABS", "ATAN", "COS", "DEG", "EXP", "FREEMEM", "FREEVAR", "LOG", "RAD", "SGN", "SIN", "SQR", "TAN", "VERSION", "CANCEL", "ASC", "LEN", "VAL", "RESULT", "MAINCNTL", "BTRIG" };
extern const int globvnum =                                                                                                                                                                                                    28;
extern const std::string globslist[] = { "CHR", "DATE", "HEX", "LEFT", "MID", "PRGNAME", "RIGHT", "STR", "SUBST", "TIME"};
extern const int globsnum =                                                                             10;
extern const char globmlist[] = { '.', '%', '*', '+', '-', '/', '~', '!', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
extern const int globmnum = 17 + 26; //(removed '(' and ')')
extern const char globgluechars[] = { '+', '-', '*', '%', ',', '=', '%', '(', ')', '!', '~' };
extern const int globgnum = 11;

//Small enough list to hard code
//extern const std::string globgluewords[] = { "AND", "OR", "NOT", "XOR" };
//extern const int globgwnum = 4;

extern std::string gPassString = "";
extern bool globRESULT = 0;

//Hash table function examples
//
//Variable.AddItem("Paul", 0);
//Variable.PrintItemsInIndex(2);
//Variable.RemoveItem(name);
//Variable.PrintTable();
//Variable.FindVal(name);

hashtable Variable;
hashtables Strings;

extern int ug;
extern std::string gFormulaToParse;
extern int gFormulaToParsep = 0;
std::string getParenthesis(std::string, int);

int const SBarrSize = 1000000;
std::string filePath = "run.txt";
//std::string filePath = "RPRG000.PTC";
int line;
int setline = 0;
int addline = 0;
int ret;
bool stopr;
extern int pLength = 0;
//0 End of program
//1 END command
//2 Syntax error
//std::string us;
int u;
char uc;
bool synerr;
int isEqualsCheck;

int gosub[256];
int gosubNu = 0;
int sbfor[256];
int sbforVal[256];
double sbforStep[256];
int sbforNu = 0;
int cTableSize = 200;

//Array that stores addresses where DATA commands are for faster reading
int DATALoc[10000]; //Probably needs a better way to determine the size, will break if limit is passed
int DATALine = 0;
int DATALinePos = 0;

bool getCommadContentEnd = 0;
std::string DATAreadData;

//Function prototypes
int interpretMain(int &);


//Interpreter functions
void printPRG(std::string[], int, int a[]);

bool isFunc(std::string);
bool isNumeric(std::string);
bool isString(std::string);
bool isLabel(std::string);
bool isSystemVar(std::string);
bool isSystemStr(std::string);

int getElem(std::string);
std::string caps(std::string);
std::string numToString(double);
int stringToNum(std::string);



//"get"
double solve(std::string);
double solve2(std::string);
string getString(std::string);
double getVal(std::string &, int &);
double getNu(std::string, int &);
double getVa(std::string, int &);
std::string getQuotes(std::string, int &);
std::string getWo(std::string, int &);
std::string getWo2(std::string);
double getSystemVar(std::string, std::string);
std::string getSystemStr(std::string, std::string);
std::string getCommadContent(std::string, int);
int getIndex(std::string);

int isEquals(std::string);
void equalsSet(std::string, int);
bool isLetter(char);
bool isNumber(char);
char quoteChar();
char apostropheChar();
std::string getTimeString();
std::string getDateString();

void removeSpaces(std::string &);
void setDefaultArrs();


double GRPColArr[256][3];

extern int buttonCode = 0;
extern int globBTRIG = 0;
extern long int globBTRIGReset = 0;

extern int globGCOLOR = 0;



template< typename intToHexT >
std::string intToHex(intToHexT i);


//std::string fix[2000];




std::string sbforVar[256];
std::string SBarr[SBarrSize];
int SBarrLine[SBarrSize];


typedef std::chrono::high_resolution_clock Clock;
long int getTime();
auto SBStartTime = Clock::now();
time_t timer;
struct tm y2k = { 0 };

//Petit computer graphics related functions
void PRINT(std::string);
void CLS();
void sbINPUT(std::string);
void GCLS(int);
void GCOLOR(int);
void GLINE(std::string);
void GBOX(std::string);
void GCIRCLE(std::string);
void GPSET(std::string);



//OpenGL functions
void Draw();
void Initialize();
int openglmain(int iArgc, char** cppArgv);
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key, int x, int y);
void keyboard(unsigned char key, int x, int y);

