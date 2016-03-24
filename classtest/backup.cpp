#include "stdafx.h"
#include "interpretmain.h"
#include <ctime>
auto SBStartTime = Clock::now();
time_t timer;
struct tm y2k = { 0 };


int main(int iArgc, char** cppArgv)
{
	setDefaultArrs();
	//Testing area, reserved for testing new functions-------
	//openglmain(iArgc, cppArgv);
	//cout << isString(caps("val(right$(time$,2))")) << endl;
	
	cout << getIndex("CLS") << endl;
	
	system("pause");
	//-------------------------------------------------------



	
	
	//Setting the array, currently being completely redone
	
	ifstream infile(filePath);
	std::string readline;
	std::string stack;
	int pLength = 1;
	int lNum = 1;
	std::string tSFC;
	int comLinePos;

	while (std::getline(infile, readline)) {
		stack = "";
		comLinePos = 0;
		for (int i = 0; i <= readline.size(); i++){


			if (readline[i] != ' ' && i < readline.size() && readline[i] != ':'){ 

				//Skip parenthesis
				if (readline[i] == quoteChar()){
					i++;
					stack += quoteChar();
					while (readline[i] != quoteChar() && i < readline.size()){
						stack += readline[i];
						i++;
					}
				}
				
				tSFC = readline[i]; stack += caps(tSFC);
				

			}
			else{
				if (stack[0] != quoteChar()) removeSpaces(stack);
				if (stack != ""){

					//@label setting
					if (stack[0] == '@' && comLinePos == 0){
						if (Variable.FindVal(stack) != 0){ cout << "Duplicate label (" << stack << ")\n"; system("pause"); }
						else{
							Variable.AddItem(stack, pLength);
						}
					}

					//ignore line when REM is read
					if (stack == "REM"){ break; }

					SBarr[pLength] = stack;
					SBarrLine[pLength] = lNum;
					stack = "";
					pLength++;
					comLinePos++;
				}
			}

			
			
		}

		lNum++;
		


	};


	infile.close();

	//std::cout << "\nPrint program array\n\n";
	//printPRG(SBarr, pLength, SBarrLine);
	//cout << Variable.FindVal("@LOOP");
	//cout << "\nDone\n";
	//system("pause");
	
	
	std::srand(time(0));
	auto SBStartTime = Clock::now();

	//Code execution loop
	for (line = 1; line < pLength; line++){
		synerr = 1; //If synerr != 0 by the end nothing was triggered which means a typo was made or an error happened

		switch getIndex(SBarr[line]){
		case 1:
			break;
		}

		//console
		if (SBarr[line] == "CLS"){ CLS(); synerr = 0; }

		if (SBarr[line] == "PRINT" || SBarr[line] == "?"){ 
			if (getElem(SBarr[line + 1]) == 0){ synerr = 0; PRINT(""); }
			if (getElem(SBarr[line + 1]) == 1){
				synerr = 0;
				PRINT(numToString(solve(SBarr[line + 1])));
				addline++;
			}
			if (getElem(SBarr[line + 1]) == 2){
				
				synerr = 0;
				PRINT(getString(SBarr[line + 1]));
				addline++;
			}
		}


		//Data
		isEqualsCheck = isEquals(SBarr[line]);
		if (isEqualsCheck){ equalsSet(SBarr[line], isEqualsCheck); synerr = 0; }

		if (SBarr[line] == "SWAP"){
			std::string s1 = getCommadContent(SBarr[line + 1], 0);
			std::string s2 = getCommadContent(SBarr[line + 1], 1);
			if (isString(s1)){
				std::string ss1 = getString(s1);
				std::string ss2 = getString(s2);
				Strings.RemoveItem(s1);
				Strings.AddItem(s1, ss2);
				Strings.RemoveItem(s2);
				Strings.AddItem(s2, ss1);
			}
			else{
				int ss1 = solve(s1);
				int ss2 = solve(s2);
				Variable.RemoveItem(s1);
				Variable.AddItem(s1, ss2);
				Variable.RemoveItem(s2);
				Variable.AddItem(s2, ss1);
			}
			synerr = 0;
			addline++;
		}

		//runtime
		if (SBarr[line] == "END" || SBarr[line] == "STOP"){ ret = 1; synerr = 0; break; }
		if (SBarr[line] == "GOTO"){ 
			setline = Variable.FindVal(SBarr[line + 1]); synerr = 0;
			if (setline == 0){ synerr = 1; ret = 3; }
		}
		if (isLabel(SBarr[line])){ synerr = 0; }

		if (SBarr[line] == "GOSUB"){
			gosub[gosubNu] = line;
			gosubNu++;
			if (gosubNu == 256){ ret = 5; break; }
			setline = Variable.FindVal(SBarr[line + 1]); synerr = 0;
			if (setline == 0){ synerr = 1; ret = 4; }
		}

		if (SBarr[line] == "RETURN"){
			gosubNu--;
			if (gosubNu == -1){ ret = 6; break; }
			setline = gosub[gosubNu] + 1;
			synerr = 0;
		}

		if (SBarr[line] == "FOR"){
			if (sbforNu == 256){ ret = 7; break; }
			addline++;
			

			equalsSet(SBarr[line + addline], 1);
			
			std::string tempbugfix = SBarr[line + addline];
			
			sbforVar[sbforNu] = getWo2(SBarr[line+addline]);

			

			addline++;
			

			if (SBarr[line + addline] != "TO"){ break; }
			addline++;

			sbforVal[sbforNu] = solve(SBarr[line + addline]);
			if (SBarr[line + addline + 1] == "STEP"){ sbforStep[sbforNu] = solve(SBarr[line + addline + 2]); addline += 2; synerr = 0; }
			else{ sbforStep[sbforNu] = 1; }
			sbfor[sbforNu] = line + addline;
			sbforNu++;
			synerr = 0;
		}

		if (SBarr[line] == "NEXT"){
			if (sbforNu == 0){ ret = 8; break; }
			if (Variable.FindVal(sbforVar[sbforNu - 1]) < sbforVal[sbforNu - 1]){ 
				setline = sbfor[sbforNu-1];
				int varToSet = Variable.FindVal(sbforVar[sbforNu - 1]) + sbforStep[sbforNu - 1];
				Variable.RemoveItem(sbforVar[sbforNu - 1]);
				Variable.AddItem(sbforVar[sbforNu - 1], varToSet);
				synerr = 0;
			}
			else{
				sbforNu--;
				synerr = 0;
			}
		}

		if (SBarr[line] == "IF"){
			if (solve(SBarr[line + ++addline])){ 
			//if the requirement is met
				//cout << "If triggered\n";
				//globRESULT = 1;
				addline += 1;

				if (SBarr[line + addline + 1][0] == '@'){
					setline = Variable.FindVal(SBarr[line + addline + 1]);
					if (setline == 0){ synerr = 1; ret = 3; }
					addline = 0;
				
				}
				synerr = 0;
			}
			else
			{//get to the end of the statement (or the ELSE)
				
				int lineo = SBarrLine[line + addline];
				while (lineo == SBarrLine[line + addline] && SBarr[line + addline] != "ELSE"){
					addline++;
				}
				if (SBarr[line + addline] != "ELSE"){ addline--; }
				else{
					if (SBarr[line + addline + 1][0] == '@'){
						setline = Variable.FindVal(SBarr[line + addline + 1]); synerr = 0;
						if (setline == 0){ synerr = 1; ret = 3; }
						addline = 0;
					}
				}
				synerr = 0;
			}
		}

		if (SBarr[line] == "ELSE"){
			int lineo = SBarrLine[line + addline];
			while (lineo == SBarrLine[line + addline]){
				addline++;
			}
			addline--;
			synerr = 0;
		}

		if (SBarr[line] == "WAIT" || SBarr[line] == "VSYNC"){
			synerr = 0;
			
			int waitfor;
			if (isNumeric(SBarr[line + 1])){
				addline++;
				waitfor = solve(SBarr[line + 1]);
			}
			else{
				waitfor = 1;
			}

			for (int i = 0; i < waitfor; i++){
				int time2 = getTime();
				do{
					//cout << time2 << endl;
				} while (time2 == getTime());
				
			}
			
		}

		if (SBarr[line] == "ON"){
			int onvar = solve(SBarr[line + 1]);

			if (SBarr[line + 2] == "GOSUB"){
				synerr = 0;


				std::string gotlabel = getCommadContent(SBarr[line + 3], onvar);

				if (gotlabel != ""){
					setline = Variable.FindVal(gotlabel);
					if (setline == 0){ synerr = 1; ret = 3; }
					gosub[gosubNu] = line + 2;
					gosubNu++;
					if (gosubNu == 256){ ret = 5; break; }
				}
				else
				{
					addline += 3;
				}
			
			}
			else{//Goto is assumed
				synerr = 0;
				std::string gotlabel = getCommadContent(SBarr[line + 3], onvar);

				if (gotlabel != ""){
					setline = Variable.FindVal(gotlabel);
					if (setline == 0){ synerr = 1; ret = 3; }
				}
				else{
					addline += 3;
				}
				
			}
		}

		//Input
		if (SBarr[line] == "INPUT"){ addline++; sbINPUT(SBarr[line + addline]); synerr = 0; }



		//syntax error check
		if (synerr){ ret = 2; break; }

		if (addline){ line += addline; addline = 0; }
		if (setline){ line = setline; setline = 0; }
	}
	
	
	switch (ret){
	case 0:
		std::cout << "Ok" << endl;
		break;

	case 1:
		cout << "END at element " << line << " " << SBarr[line] << endl << endl;
		if (line == 1){ cout << "Open 'run.txt' to edit your program, save it and try again.\nSee 'Functionality (ReadMe).txt' for usage." << endl; }
		break;

	case 2:
		cout << "Syntax error " << "(" << line << ", " << SBarr[line] << ")" << endl << endl;
		//printPRG(SBarr, pLength, SBarrLine);
		break;

	case 3:
		cout << "GOTO undefined label " << "(" << line << ", " << SBarr[line] << ")" << endl << endl;
		break;

	case 4:
		cout << "GOSUB undefined label " << "(" << line << ", " << SBarr[line] << ")" << endl;
		break;

	case 5:
		cout << "Out of subroutine memory " << "(" << line << ", " << SBarr[line] << ")" << endl;
		break;

	case 6:
		cout << "RETURN without GOSUB " << "(" << line << ", " << SBarr[line] << ")" << endl;
		break;

	case 7:
		cout << "Out of FOR memory " << "(" << line << ", " << SBarr[line] << ")" << endl;
		break;

	case 8:
		cout << "NEXT without FOR " << "(" << line << ", " << SBarr[line] << ")" << endl;
		break;
	}




	
		
	//	<< endl << "Break in element " << line << endl;
	//printPRG(SBarr, pLength, SBarrLine);
	system("PAUSE");
	return 0;
}

void printPRG(std::string arr[], int size, int larr[]){
	for (int u = 0; u<size; u++){ 
		cout << u << " " << arr[u] << " (" << larr[u] << ")" << endl;
	}
	return;
}

std::string caps(std::string us){
	//97-122 lowercase
	//65-90 uppercase
	std::string usr = "";
	for (int u = 0; u < us.size(); u++){
		if (static_cast<int>(us[u]) > 96 && static_cast<int>(us[u]) < 123){ us[u] -= 32; }
	}
	return us;
}

//Returns the global string "gPassString" along with it's own integer
//Returns;
//0 a function
//1 number
//2 string
int getElem(std::string str){
	//gPassString = "";
	if (isFunc(caps(str))){ return 0; };
	if (isNumeric(caps(str))){ /*gPassString = numToString(solve(caps(str)));*/ return 1; }
	if (isString(str)){ return 2; }
	//gPassString = "0";
	return 0;
}

bool isFunc(std::string us){
	bool ret = 0;
	for (int u = 0; u < globcnum; u++){
		if (us == globclist[u]){ ret = 1; break; }
	}
	return ret;
}

//Better detection should be implemented before == is implemented
int isEquals(string str){
	//cout << str << endl;
	int ret = 0;
	int st = 0;
	for (int i = 0; i < str.length(); i++){
		if (str[i] == '='){ ret = 1; }
		if (str[i] == '$' || static_cast<int>(str[i]) == 34){ st = 1; }
	}
	

	if (ret){ return ret + st; }
	return ret;
}

bool isNumeric(std::string us){
	int u = 0;
	int mathchars = 0;
	int u1 = 0;
	std::string getp;
	//Check to make sure the string is a numeric expression	

for (u1 = 0; u1 < us.size(); u1++){ //Going through the string	
	for (u = 0; u < globmnum; u++){ //Going through the valid characters
	
			if (us[u1] == globmlist[u]){ mathchars += 1; }	
			
			if (us[u1] == '('){
				std::string hh = us;
				std::string getp = getParenthesis(hh.replace(0, u1, ""),0);
				u1 += getp.length() + 1;
				mathchars += getp.size() + 2;
				u = globmnum;
			}
		}
	}
	return (mathchars >= us.size());
}

bool isString(std::string str){
	if (str == ""){ return 0; }
	bool ret = 0;
	int i;
	bool reqDol = 0;

	for (i = 0; i < str.length(); i++){
		if (isLetter(str[i])){ reqDol = 1; }
		if (reqDol && str[i] == '$'){ reqDol = 0; }
		if (reqDol && !( isLetter(str[i]) || isNumber(str[i]))){ i--; break; }
		if (str[i] == '='){ i--; break; }
		if (str[i] == quoteChar()){
			i++;
			while (i < str.length() && str[i] != quoteChar()){ i++; }
		}
		if (str[i] == '('){
			i++;
			while (i < str.length() && str[i] != ')'){ i++; }
		}
	}

	if (reqDol){ i = 0; }
	return (str.length() <= i);
}

bool isLabel(std::string str){
	int ret = 0;
	if (str.length() < 2){ return 0; }
	if (str[0] == '@' && isLetter(str[1])){ ret = 1; }
	return ret;
}

bool isLetter(char i){
	bool ret = 0;
	if ((static_cast<int>(i) > 96 && static_cast<int>(i) < 123) || (static_cast<int>(i) > 64 && static_cast<int>(i) < 91)){
		ret = 1;
	}
	else{
		ret = 0;
	}
	return ret;
}

bool isNumber(char i){
	bool ret = 0;
	if (static_cast<int>(i) > 47 && static_cast<int>(i) < 58){
		ret = 1;
	}
	else{
		ret = 0;
	}
	return ret;
}

bool isSystemVar(std::string str){
	bool result = 0;
	for (int u = 0; u < globvnum; u++){
		if (str == globvlist[u]){ result = 1; break; }
	}
	return result;
}

bool isSystemStr(std::string str){
	bool result = 0;
	for (int u = 0; u < globsnum; u++){
		if (str == globslist[u]){ result = 1; break; }
	}
	return result;
}

std::string numToString(double Number){
	std::string Result;
	ostringstream convert;	
	convert << Number;	
	return convert.str();
}

int stringToNum(std::string myString){	
	return atoi(myString.c_str());
}

template< typename intToHexT >
std::string intToHex(intToHexT i)
{
	std::stringstream stream;
	stream //<< "0x"
		//<< std::setfill('0') << std::setw(sizeof(intToHexT) * 2)
		<< std::hex << i;
	return caps(stream.str());
}

std::string getWo(std::string str, int &strPos){
	std::string result = "";
	while (1){
		if (isLetter(str[strPos])){ result += str[strPos]; strPos++; }
		else { break; }
		if (strPos > str.length()){ break; }
	}
	return result;
}

std::string getWo2(std::string str){
	std::string result = "";
	int strPos = 0;
	while (1){
		if (isLetter(str[strPos])){ result += str[strPos]; strPos++; }
		else { break; }
	}
	return result;
}

std::string getQuotes(std::string str, int &strPos){
	std::string result = "";
	strPos++;
	
	while (1){
		if (str[strPos] != quoteChar()){ result += str[strPos]; strPos++; }
		else{ break; }
		if (strPos > str.length()){ break; }
	}
	return result;
}

//Get Comma'd content
std::string getCommadContent(std::string str, int pos){
	
	std::string result = "";
	int com = 0;
	bool reqquote = 0;
	int unsigned reqpara = 0;
	for (int u = 0; u < str.size(); u++){
		

		if (str[u] == quoteChar()){ reqquote = !reqquote; }

		if (str[u] == '('){ reqpara++; }
		if (str[u] == ')'){ reqpara--; }

		if (str[u] == ',' && reqquote == 0 && reqpara == 0){ com++; u++; }
		if (com == pos){ result += str[u]; }
	}


	return result;
}

double getSystemVar(std::string var, std::string sentParenthesis = ""){
	if (var == "TRUE"){ return 1; }
	if (var == "FALSE"){ return 0; }
	if (var == "RND"){
		unsigned int rnd = rand();
		int limit = solve(sentParenthesis);
		return (rnd % limit);
	}
	if (var == "PI"){ return 3.14159265359; }
	if (var == "FLOOR"){
		int res = solve(sentParenthesis);
		return res;
	}
	if (var == "ABS"){ return std::abs(solve(sentParenthesis));}
	if (var == "ASC"){ 
		std::string res = getString(sentParenthesis);
		return static_cast<int>(res[0]);
	}
	if (var == "ATAN"){ return std::atan(solve(sentParenthesis)); }
	if (var == "ERL"){ return 0; }
	if (var == "ERR"){ return 0; }
	if (var == "CANCEL"){ return -1; }
	if (var == "COS"){ return std::cos(solve(sentParenthesis)); }
	if (var == "DEG"){ return (solve(sentParenthesis) / 3.14159265359) * 180;} 
	if (var == "EXP"){ return std::exp(solve(sentParenthesis)); }
	if (var == "FREEMEM"){ return 1000; }
	if (var == "FREEVAR"){ return 1000; }
	if (var == "LEN"){ return getString(sentParenthesis).size(); }
	if (var == "LOG"){ return std::log(solve(sentParenthesis)); }
	if (var == "RAD"){ return (solve(sentParenthesis) / 180)*3.14159265359;} 
	if (var == "RESULT"){ return globRESULT; }
	if (var == "SGN"){
		double res = solve(sentParenthesis);
		if (res > 0){ return 1; }
		if (res < 0){ return -1; }
		if (res == 0){ return 0; }
	}
	if (var == "SIN"){ return std::sin(solve(sentParenthesis)); }
	if (var == "SQR"){ return std::sqrt(solve(sentParenthesis)); }
	if (var == "TAN"){ return std::tan(solve(sentParenthesis)); }
	if (var == "VAL"){ 
		int temp = 0;
		return getNu(getString(sentParenthesis), temp);
	}
	if (var == "VERSION"){ return 8224; }

	cout << "getSystemVar() error\n"; system("pause");
	return 0;
}

std::string getSystemStr(std::string str, std::string sentParenthesis = ""){
	if (str == "CHR"){ 
		char resc = solve(sentParenthesis);
		std::string ress = "";
		ss << resc;
		ss >> ress;
		return ress;
	}
	if (str == "DATE"){ return getDateString(); }
	if (str == "HEX"){ 
		int res = solve(sentParenthesis);
		return intToHex(res);
	}

	if (str == "LEFT"){
		std::string result = "";
		std::string midstr = getString(getCommadContent(sentParenthesis, 0));
		int length = solve(getCommadContent(sentParenthesis, 1));
		midstr.replace(length, midstr.size(), "");
		return midstr;
	}

	if (str == "MID"){
		std::string result = "";
		std::string midstr = getString(getCommadContent(sentParenthesis, 0));
		int position = solve(getCommadContent(sentParenthesis, 1));
		int length = solve(getCommadContent(sentParenthesis, 2));
		midstr.replace(position + length, midstr.size(), "");
		midstr.replace(0, position, "");
		return midstr;
	}

	if (str == "PRGNAME"){
		return "RUN";
	}

	if (str == "RIGHT"){
		std::string result = "";
		std::string midstr = getString(getCommadContent(sentParenthesis, 0));
		int length = solve(getCommadContent(sentParenthesis, 1));
		midstr.replace(0, midstr.size() - length, "");
		return midstr;
	}

	if (str == "STR"){
		return numToString(solve(sentParenthesis));
	}

	if (str == "SUBST"){
		std::string origstr = getString(getCommadContent(sentParenthesis, 0));
		int position = solve(getCommadContent(sentParenthesis, 1));
		int replace = solve(getCommadContent(sentParenthesis, 2));
		std::string repstr = getString(getCommadContent(sentParenthesis, 3));

		origstr.replace(position, position + replace-2, "");
		origstr.insert(position, repstr);
		return origstr;
	}

	if (str == "TIME"){
		return getTimeString();
	}

	return "getSystemStr_error";
}

double getVal(std::string &str, int &strPos){
	if (isLetter(str[strPos])){ return getVa(str, strPos); }
	if (isNumber(str[strPos])){ return getNu(str, strPos); }
	if (str[strPos]){
		std::string gp = getParenthesis(str, strPos);
		int res = solve(gp);
		strPos += gp.size() + 1;
		return res;
	}
	std::cout << "getVal() error\n";
	cout << str << endl;
	cout << str[strPos] << endl;
	system("pause");
	return 0;
}

//Decimals bugged somewhat
double getNu(std::string str, int &strPos){
	double result = 0;
	double decimalCount = 1;
	int go = 1;
	//char strc = str[3]; Uhhh

	while (go){
		int strv = str[strPos];
		strv -= '0';
		if (isNumber(str[strPos])){ result *= 10; result += strv; strPos++; }
			else { break; }
		}
		if (str[strPos] == '.'){ 
			strPos++;
			go = 1;
			while (go){
				int strv = str[strPos];
				strv -= '0';
				if (isNumber(str[strPos])){ decimalCount *= 0.1; result += decimalCount*strv; strPos++; }
				else { go = 0; }
			}
		}
	return result;
}

double getVa(std::string str, int &strPos){
	std::string result = "";
	int go = 1;
	while (go){
		if (isLetter(str[strPos]) || isNumber(str[strPos])){ result += str[strPos]; strPos++; }
		else { break; }
	}


	if (isSystemVar(result)){ 
		std::string parenthesis;
		if (str[strPos] == '('){
			parenthesis = getParenthesis(str.replace(0, strPos, ""), 0);
			strPos += parenthesis.size() + 1;
		}


		return getSystemVar(result, parenthesis);
	
	}
	
	
	//cout << "getVa pass\n";
	return Variable.FindVal(result);
}

int getIndex(std::string key){
	int hash = 0;
	int index;
	for (int i = 0; i < key.length(); i++){
		hash += (int)key[i];

	}

	index = hash;// % cTableSize;
	return index;
}

char quoteChar(){
	std::string str = "\"";
	return str[0];
}

//Returns how long the program has been running in 1/60ths of seconds
long int getTime(){
	auto t2 = Clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - SBStartTime).count() / 16666666;
}

std::string getTimeString(){
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%I:%M:%S", timeinfo);
	std::string str(buffer);


	return str;

}

std::string getDateString(){
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%Y/%m/%d", timeinfo);
	std::string str(buffer);


	return str;




}

void removeSpaces(std::string &input){
	for (int i = input.size() - 1; i >= 0; --i) {
		if (input[i] == ' ' || input[i] == ':')
			input.erase(i, 1);
	}
}



//This section of the code solves a math string (something like "(4+97)*7") and returns a single value.
double solve(std::string str){	
	if (str == ""){ return 0; }
	double result = 0;
	double cache = 0;
	double positive = 1;
	bool div = 0;
	bool mult = 0;
	bool mod = 0;

	for (int strPos = 0; strPos < str.length(); strPos++){
		if (isLetter(str[strPos]) || isNumber(str[strPos]) || str[strPos]=='('){ 
			if (!div && !mult && !mod){ cache = getVal(str, strPos)*positive; }
			else{
				if (mult){ cache *= getVal(str, strPos); mult = 0; }
				if (div){ cache /= getVal(str, strPos); div = 0; }
				if (mod){
					cache = fmod(cache, getVal(str, strPos)); mod = 0;
				}
			}
		}
		if (str[strPos] == '+'){ result += cache; cache = 0; positive = 1; }
		if (str[strPos] == '-'){ result += cache; cache = 0; positive = -1; }
		if (str[strPos] == '*'){ mult = 1; }
		if (str[strPos] == '/'){ div = 1; }
		if (str[strPos] == '%'){ mod = 1; }
	}
	return result + cache;
}

//This function is like the solve() function except it handles strings ("String"+" String "+NAME$ = "String String Bob")
string getString(string str){
	if (str == ""){ return ""; }
	std::string varToSet;
	std::string result = "";
	for (int strPos = 0; strPos < str.length(); strPos++){
		if (isLetter(str[strPos])){
			std::string name = getWo(str, strPos);
			
			
			if (isSystemStr(name)){
				std::string parenthesis;
				if (str[strPos + 1] == '('){
					std::string str2 = str;
					parenthesis = getParenthesis(str2.replace(0, strPos, ""), 0);
					strPos += parenthesis.size() + 2;
				}

				result += getSystemStr(name, parenthesis);

			}
			else{


				result += (Strings.FindVal(name + "$"));
				strPos++;
			}
		}
		if (str[strPos] == quoteChar()){
			result += getQuotes(str, strPos);
		}
	}
	return result;
}



//Old solve() code
//This code was bugged and horrible so I redid it. It's much better now.
//cout << "solve start\n";//Why does this print 3 times??
//str = str + "+0";
//double result = 0;
//double cache = 0;
//double temp = 0;
//double addArr[50];
//for (int i = 0; i < 50; i++){addArr[i] = 0;}
//int addArrPos = 0;
//int positive = 1;
//int strPos = 0;
//int go = 1;
//
//
//
//for (strPos = 0; strPos < str.length();strPos++){
//	if (isNumber(str[strPos])){ cache = getNu(str, strPos); };
//
//	cout << strPos << endl;
//	if (strPos >= str.length()){ result += cache*positive; break; }
//	system("pause")
//
//	if (isLetter(str[strPos])){ cache = getVa(str, strPos); };
//	if (str[strPos] == '-'){ addArr[addArrPos] = cache*positive; cache = 0; positive = -1; cout << "-" << endl; addArrPos++; } //if it gets a symbol it flushes the cache
//	if (str[strPos] == '+'){ addArr[addArrPos] = cache*positive; cache = 0; positive = 1;   /*cout << "+" << endl;*/ addArrPos++; }
//	if (str[strPos] == '*'){ strPos++; cache = cache * getNu(str, strPos); }
//	if (str[strPos] == '/'){ strPos++; cache = cache / getNu(str, strPos); }
//	
//	if (strPos >= str.length()){ result += cache*positive; break; }
//}
//
////system("pause");
//
//for (int i = 0; i <= addArrPos; i++){result += addArr[i];}
//cout << "solve end pass\n" << endl;
//return result;

//This function returns the contents of the first detected parenthesis.
std::string getParenthesis(std::string str, int i = 0){
	int firstParenthesis = -1;
	int closingParenthesis = -1;
	int closingParenthesisNeeded = 0;

	for (i = i; i < str.size(); i++){
		if (static_cast<int>(str[i]) == 40){ firstParenthesis = i; closingParenthesisNeeded = 1; break; }
	}

	while (closingParenthesisNeeded){
		i++;
		if (str[i] == '('){ closingParenthesisNeeded++; }
		if (str[i] == ')'){ closingParenthesisNeeded--; }
	
	}
	if (firstParenthesis > -1) {
		closingParenthesis = i;



		if (firstParenthesis != -1){
			std::string str2 = str.replace(closingParenthesis, str.size(), "");
			std::string str1 = str.replace(0, firstParenthesis+1, "");
		}
	}
	return str;
}


void equalsSet(std::string str, int type){
	
	if (type == 1){

		int strPos = 0;
		std::string varToSet = getWo(str, strPos);
		//cout << line << endl;
		if (str[strPos] != '='){ cout << "Equalset error\n"; system("pause"); }
		strPos++;
		double num = solve(str.replace(0, strPos, ""));
		Variable.RemoveItem(varToSet);
		Variable.AddItem(varToSet, num);
	}
	else{
		int strPos = 0;
		std::string varToSet = getWo(str, strPos) + "$";
		strPos++;
		if (str[strPos] != '='){ cout << "\nString equalset error\n" << str << endl << "strPos = " << strPos << endl; system("pause"); }
		strPos++;
		std::string text = getString(str.replace(0, strPos, ""));
		Strings.RemoveItem(varToSet);
		Strings.AddItem(varToSet, text);

	}
}

void setDefaultArrs(){
	//default grp pallete array, hard coded, probably not the best way
	GRPColArr[0][0] = 0;    GRPColArr[0][1] = 0;    GRPColArr[0][2] = 0;
	GRPColArr[1][0] = 58;   GRPColArr[1][1] = 57;   GRPColArr[1][2] = 58;
	GRPColArr[2][0] = 255;  GRPColArr[2][1] = 24;   GRPColArr[2][2] = 0;
	GRPColArr[3][0] = 255;  GRPColArr[3][1] = 89;   GRPColArr[3][2] = 197;
	GRPColArr[4][0] = 0;    GRPColArr[4][1] = 57;   GRPColArr[4][2] = 247;
	GRPColArr[5][0] = 123;  GRPColArr[5][1] = 57;   GRPColArr[5][2] = 255;
	GRPColArr[6][0] = 0;    GRPColArr[6][1] = 186;  GRPColArr[6][2] = 255;
	GRPColArr[7][0] = 148;  GRPColArr[7][1] = 89;   GRPColArr[7][2] = 41;
	GRPColArr[8][0] = 255;  GRPColArr[8][1] = 162;  GRPColArr[8][2] = 0;
	GRPColArr[9][0] = 255;  GRPColArr[9][1] = 202;  GRPColArr[9][2] = 168;
	GRPColArr[10][0] = 0;   GRPColArr[10][1] = 121; GRPColArr[10][2] = 0;
	GRPColArr[11][0] = 0;   GRPColArr[11][1] = 243; GRPColArr[11][2] = 25;
	GRPColArr[12][0] = 255; GRPColArr[12][1] = 227; GRPColArr[12][2] = 0;
	GRPColArr[13][0] = 189; GRPColArr[13][1] = 186; GRPColArr[13][2] = 189;
	GRPColArr[14][0] = 0;   GRPColArr[14][1] = 0;   GRPColArr[14][2] = 0;
	GRPColArr[15][0] = 255; GRPColArr[15][1] = 251; GRPColArr[15][2] = 255;
}

//SmileBASIC graphics functions
//Graphics related functions will be updated to run on openGL when it's implemented
//Could probably go in their own file for organization ¯\_(ツ)_/¯

void CLS(){
	system("CLS");
}

void PRINT(std::string str = ""){
	cout << str << endl;
}

void PRINT(double str){
	cout << str << endl;
}

void sbINPUT(std::string varToSet){
	std::string num;
	cout << "?" << endl;
	cin >> num;
	if (isString(varToSet)){
		Strings.RemoveItem(varToSet);
		Strings.AddItem(varToSet, num);
	}
	else{
		Variable.RemoveItem(varToSet);
		Variable.AddItem(varToSet, solve(num));
	}
}


//OpenGL rendering code here

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void Initialize() {
	glClearColor(0.0, 0.0, 0.5, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glutPostRedisplay();
}

int openglmain(int iArgc, char** cppArgv) {
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(256, 192*2);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("SBInterpret");
	Initialize();
	glutDisplayFunc(Draw);

	glutMainLoop();
	return 0;
}
