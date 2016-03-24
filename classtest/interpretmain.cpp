#include "stdafx.h"
#include "interpretmain.h"





int main(int iArgc, char** cppArgv)
{
	setDefaultArrs();
	cout << "-------SBInterpret 0.6-------\n";
	cout << "core: mostly finished\n";
	cout << "graphics: GLINE and GCLS only\n\n";
	cout << "Edit 'run.txt' and see 'Functionality (ReadMe).txt' for usage\n\n";
	cout << "Alt + Tab and close the console window if opengl window is unresponsive\n\n";
	//Testing area, reserved for testing new functions-------

	//cout << solve("!(0)") << endl;

	//cout << getIndex("GPSET") << endl;

	//system("pause");
	//-------------------------------------------------------




	//Setting the array. Obviously optimization is not very important here so when it works it will be done.

	ifstream infile(filePath);
	std::string readline;
	std::string stack;
	pLength = 1;
	int lNum = 1;
	std::string tSFC;
	int comLinePos;
	bool gnext;
	bool glast;
	bool gglue;

	bool Dequals = 0;
	bool Eequals = 0;
	bool Sequals = 0;
	bool Mequals = 0;

	std::string gnextpass;

	while (std::getline(infile, readline)) {
		stack = "";
		std::string srepl = "";
		comLinePos = 0;


		gglue = 0;
		gnext = 0; //Resetting space removal flags
		glast = 0;


		gPassString = "";

		for (int i = 0; i <= readline.size(); i++){

			if (readline[i] != ' ' && readline[i] != '	' && i < readline.size() && readline[i] != ':'){

				//Skip quotes
				if (readline[i] == quoteChar()){
					i++;
					stack += quoteChar();
					while (readline[i] != quoteChar() && i < readline.size()){
						stack += readline[i];
						i++;
					}
				}

				//Replace '==' with '~E~'

				if (readline[i] == '='){
					if (Eequals){
						srepl = "E~";
						stack.replace(stack.size() - 1, stack.size(), "~"); Eequals = 0;
					}
					else{ Eequals = 1; }

				}
				else{ Eequals = 0; }

				//Replace '!=' with '~D~'
				if (readline[i] == '=' && Dequals){ Dequals = 0; stack.replace(stack.size() - 1, stack.size(), "~"); srepl = "D~"; }
				if (readline[i] == '!'){

					Dequals = 1;
				}
				else
				{
					Dequals = 0;
				}


				//Replace '>=' with '~S~'
				//Replace '>' with '~L~'
				if (Sequals && (readline[i] != '=')){ Mequals = 0; stack.replace(stack.size() - 1, stack.size(), "~L~"); }
				if (readline[i] == '=' && Sequals){ Sequals = 0; stack.replace(stack.size() - 1, stack.size(), "~"); srepl = "S~"; }
				if (readline[i] == '>'){

					Sequals = 1;
				}
				else
				{
					Sequals = 0;
				}


				//Replace '<=' with '~M~'
				//Replace '<' with '~P~'
				if (Mequals && (readline[i] != '=')){ Mequals = 0; stack.replace(stack.size() - 1, stack.size(), "~P~"); }
				if (readline[i] == '=' && Mequals){ Mequals = 0; stack.replace(stack.size() - 1, stack.size(), "~"); srepl = "M~"; }
				if (readline[i] == '<'){

					Mequals = 1;
				}
				else
				{
					Mequals = 0;
				}


				if (srepl == ""){
					tSFC = readline[i]; stack += caps(tSFC);
				}
				else
				{
					tSFC = srepl; stack += caps(tSFC); srepl = "";
				}

			}
			else{

				if (stack[0] != quoteChar()) removeSpaces(stack); //Remove spaces unless the first character is ". Probably not needed since spaces are filtered out before here.

				if (stack != ""){

					//@label setting
					if (stack[0] == '@' && comLinePos == 0){
						if (Variable.FindVal(stack) != 0){ cout << "Duplicate label (" << stack << ")\n"; system("pause"); }
						else{
							Variable.AddItem(stack, pLength);
						}
					}

					//ignore line when REM is read
					if (stack == "REM" || stack[0] == apostropheChar()){ break; }

					//Replace AND and related operators with ~(operator first letter)
					//"Glue" them to the next and last read words in the line (space removal) by triggering the gnext and glast flags.
					if (stack == "AND"){ stack = "~A~"; gglue = 1; }
					if (stack == "NOT"){ stack = "~N~"; gnext = 1; }
					if (stack == "OR"){ stack = "~O~"; gglue = 1; }
					if (stack == "XOR"){ stack = "~X~"; gglue = 1; }



					for (int i = 0; i < globgnum; i++){
						if (stack[0] == globgluechars[i]){ glast = 1; }
						if (stack[stack.size() - 1] == globgluechars[i]){ gnext = 1; }

					}

					if (glast && isFunc(SBarr[pLength - 1])){ glast = 0; }


					if (gnext && glast){ glast = 0; gnext = 0; gglue = 1; }

					if (!glast && !gnext && !gglue){
						SBarr[pLength] = stack;
						SBarrLine[pLength] = lNum;
						stack = "";
						pLength++;
						comLinePos++;
					}

					//Glue last, space removal, combine with the last read word.
					if (glast){

						SBarr[pLength - 1] += stack;
						SBarrLine[pLength] = lNum;
						stack = "";
						glast = 0;
					}

					//gglue combines the next the current and the last
					if (gglue){

						//last (working?)
						SBarr[pLength - 1] += stack;
						SBarrLine[pLength] = lNum;
						stack = "";



						glast = 1;
						gglue = 0;
					}


					if (gnext){
						SBarr[pLength] = gPassString + stack;
						SBarrLine[pLength] = lNum;
						stack = "";
						pLength++;
						comLinePos++;
						glast = 1;
						gnext = 0;
					}
				}
			}



		}

		lNum++;



	};


	infile.close();

	//Debug code below

	//std::cout << "\nPrint program array\n\n";
	//printPRG(SBarr, pLength, SBarrLine);
	//cout << Variable.FindVal("@LOOP");
	//cout << "\nDone\n";
	//system("pause");


	std::srand(time(0)); //Seeding the random number generator with the system time


	

	auto SBStartTime = Clock::now();


	openglmain(iArgc, cppArgv);
}


int interpretMain(int &line){

	//Code execution loop
	//cout << "pLength = " << pLength << endl;
	int gotElem;

	synerr = 1;

	isEqualsCheck = isEquals(SBarr[line]);
	if (isEqualsCheck){ equalsSet(SBarr[line], isEqualsCheck); synerr = 0; }

	if (isLabel(SBarr[line])){ synerr = 0; }

	switch (getIndex(SBarr[line])){



		//console
	case 226: //CLS
		cout << "CLS\n";
		CLS(); synerr = 0;
		break;

	case 397: //PRINT
	case 63: //?
		gotElem = getElem(SBarr[line + 1]);
		if (gotElem == 0){ synerr = 0; PRINT(""); }
		if (gotElem == 1){
			synerr = 0;
			PRINT(numToString(solve(SBarr[line + 1])));
			addline++;
		}
		if (gotElem == 2){
			//cout << "getting string in print\n";
			//cout << Strings.FindVal(SBarr[line + 1]) << endl;
			synerr = 0;
			PRINT(getString(SBarr[line + 1]));
			addline++;
		}
		break;

	case 483://PNLSTR
		addline++;
		cout << "Unsupported command PNLSTR\n";
		synerr = 0;
		break;

	case 440:
		addline++;
		cout << "Unsupported command LOCATE\n";
		synerr = 0;
		break;

		//Data

	case 315: //SWAP
	{std::string s1 = getCommadContent(SBarr[line + 1], 0);
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
	addline++; }
	break;


	//runtime
	case 326: //STOP
	case 215: //END
	ret = 1;
	synerr = 0;
	break;

	case 313: //GOTO
		setline = Variable.FindVal(SBarr[line + 1]); synerr = 0;
		if (setline == 0){ synerr = 1; ret = 3; }
		break;


	case 384: //GOSUB
		gosub[gosubNu] = line;
		gosubNu++;
		if (gosubNu == 256){ ret = 5; break; }
		setline = Variable.FindVal(SBarr[line + 1]); synerr = 0;
		if (setline == 0){ synerr = 1; ret = 4; }
		break;

	case 480: //RETURN

		gosubNu--;
		if (gosubNu == -1){ ret = 6; break; }
		setline = gosub[gosubNu] + 1;
		synerr = 0;
		break;

	case 231: //FOR


		if (sbforNu == 256){ ret = 7; break; }
		addline++;

		equalsSet(SBarr[line + addline], 1);
		sbforVar[sbforNu] = getWo2(SBarr[line + addline]);

		addline++;

		if (SBarr[line + addline] != "TO"){ break; }
		addline++;

		sbforVal[sbforNu] = solve(SBarr[line + addline]);
		if (SBarr[line + addline + 1] == "STEP"){ sbforStep[sbforNu] = solve(SBarr[line + addline + 2]); addline += 2; synerr = 0; }
		else{ sbforStep[sbforNu] = 1; }
		sbfor[sbforNu] = line + addline;
		sbforNu++;
		synerr = 0;
		break;


	case 319:
		if (SBarr[line] == "NEXT"){
			if (sbforNu == 0){ ret = 8; break; }
			if (Variable.FindVal(sbforVar[sbforNu - 1]) < sbforVal[sbforNu - 1]){
				setline = sbfor[sbforNu - 1];
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
		break;

	case 143: //IF

		//system("pause");

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
		break;

	case 297: //ELSE //GCLS (ELSE and GCLS share the same index)

	if (SBarr[line] == "ELSE"){int lineo = SBarrLine[line + addline];
	while (lineo == SBarrLine[line + addline]){
		addline++;
	}
	addline--;
	synerr = 0; }

	else{ //GCLS is assumed
		gotElem = getElem(SBarr[line + 1]);
		if (gotElem == 0){ synerr = 0; GCLS(globGCOLOR); }
		if (gotElem == 1){
			synerr = 0;
			GCLS(solve(SBarr[line + 1]));
			addline++;
		}
	}





	break;

	case 309: //WAIT
	case 403: //VSYNC

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

		break;

	case 157:

	{int onvar = solve(SBarr[line + 1]);

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

	break; }

	//Input
	case 400://INPUT
		addline++; sbINPUT(SBarr[line + 1]); synerr = 0;
		break;


	case 454://GCOLOR

		GCOLOR(solve(SBarr[line + 1]));
		addline++;
		synerr = 0;
		break;


	case 367://GLINE
		addline++;
		GLINE(SBarr[line + 1]);
		synerr = 0;

		break;

	case 304://GBOX
		addline++;
		GBOX(SBarr[line + 1]);
		synerr = 0;
		break;

	case 505: //GCIRCLE
		addline++;
		GCIRCLE(SBarr[line + 1]);
		synerr = 0;
		break;

	case 387:
		addline++;
		GPSET(SBarr[line + 1]);
		synerr = 0;
		break;


		//syntax error check
		//if (synerr){ ret = 2; break; }

	}

	if (addline){ line += addline; addline = 0; }
	if (setline){ line = setline; setline = 0; }

	if (synerr){ cout << "\nSyntax error\n" << SBarr[line] << endl; system("pause"); }
	if (ret){ cout << "\nEND\n"; system("pause"); }
	return synerr;
}




//switch (ret){
//case 0:
//	std::cout << "Ok" << endl;
//	break;
//
//case 1:
//	cout << "END at element " << line << " " << SBarr[line] << endl << endl;
//	if (line == 1){ cout << "Open 'run.txt' to edit your program, save it and try again.\nSee 'Functionality (ReadMe).txt' for usage." << endl; }
//	break;
//
//case 2:
//	cout << "Syntax error " << "(" << line << ", " << SBarr[line] << ")" << endl << endl;
//	//printPRG(SBarr, pLength, SBarrLine);
//	break;
//
//case 3:
//	cout << "GOTO undefined label " << "(" << line << ", " << SBarr[line] << ")" << endl << endl;
//	break;
//
//case 4:
//	cout << "GOSUB undefined label " << "(" << line << ", " << SBarr[line] << ")" << endl;
//	break;
//
//case 5:
//	cout << "Out of subroutine memory " << "(" << line << ", " << SBarr[line] << ")" << endl;
//	break;
//
//case 6:
//	cout << "RETURN without GOSUB " << "(" << line << ", " << SBarr[line] << ")" << endl;
//	break;
//
//case 7:
//	cout << "Out of FOR memory " << "(" << line << ", " << SBarr[line] << ")" << endl;
//	break;
//
//case 8:
//	cout << "NEXT without FOR " << "(" << line << ", " << SBarr[line] << ")" << endl;
//	break;
//
		
	//	<< endl << "Break in element " << line << endl;
	//printPRG(SBarr, pLength, SBarrLine);
//	system("PAUSE");
//	return 0;
//}

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
	if (str.length() < 2){ return 0; }
	if (str[0] == '@' && isLetter(str[1])){ return 1; }
	return 0;
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
		if (isLetter(str[strPos]) || isNumber(str[strPos])){ result += str[strPos]; strPos++; }
		else { break; }
		if (strPos > str.length()){ break; }
	}
	return result;
}

std::string getWo2(std::string str){
	std::string result = "";
	int strPos = 0;
	while (1){
		if (isLetter(str[strPos]) || isNumber(str[strPos])){ result += str[strPos]; strPos++; }
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
std::string getCommadContent(std::string str, int pos = 0){
	
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
	if (var == "BTRIG"){ return globBTRIG; }
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
	if (var == "MAINCNTL"){ return getTime(); }
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

//Needs array support
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
	
	std::string parenthesis;
	if (str[strPos] == '('){
		parenthesis = getParenthesis(str.replace(0, strPos, ""), 0);
		strPos += parenthesis.size() + 1;
		if (getCommadContent(parenthesis, 1) == ""){
			result += "(" + numToString(solve(parenthesis)) + ")";
		}
		else{
			result += "(" + numToString(solve(getCommadContent(parenthesis, 0))) + "," + numToString(solve(getCommadContent(parenthesis, 1))) + ")";
		}
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

char apostropheChar(){
	std::string str = "'";
	return str[0];
}

//Returns how long the program has been running in 1/60ths of seconds
long int getTime(){
	auto t2 = Clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - SBStartTime).count() / 16666666;
}

std::string getTimeString(){
	time_t rawtime;
	struct errno_t * timeinfo;
	char buffer[80];

	time(&rawtime);
	//timeinfo = localtime_s(&rawtime);

	//strftime(buffer, 80, "%I:%M:%S", timeinfo);
	std::string str(buffer);


	return str;

}

std::string getDateString(){
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	//timeinfo = localtime(&rawtime);

	//strftime(buffer, 80, "%Y/%m/%d", timeinfo);
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

//First solve checks for 1st priority operators (AND NOT ==) (Converted to ~letter~ on reading for simplicity)
double solve(std::string str){
	if (str == ""){ return 0; }
	int neededParenthesis = 0;

	for (int i = str.size(); i > 0; i--){

		if (str[i] == '~'){
			i-=2;
			if (str[i + 1] == 'A'){ 
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) && solve(secondHalf);
			}
			if (str[i + 1] == 'N'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return -solve(secondHalf);
			}
			if (str[i + 1] == 'O'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) || solve(secondHalf);
			}
			if (str[i + 1] == 'X'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return !solve(firstHalf) != !solve(secondHalf);
			}
			if (str[i + 1] == 'E'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) == solve(secondHalf);
			}
			if (str[i + 1] == 'D'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) != solve(secondHalf);
			}
			if (str[i + 1] == 'S'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) >= solve(secondHalf);
			}
			if (str[i + 1] == 'M'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) <= solve(secondHalf);
			}
			if (str[i + 1] == 'L'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) > solve(secondHalf);
			}
			if (str[i + 1] == 'P'){
				std::string firstHalf = str; firstHalf.replace(i, str.size(), "");
				std::string secondHalf = str; secondHalf.replace(0, i + 3, "");
				return solve(firstHalf) < solve(secondHalf);
			}


			cout << "\noperator error\n"; system("pause");
		}


		do{
			if (str[i] == ')'){ neededParenthesis++; }
			if (str[i] == '('){ neededParenthesis--; }
			if (neededParenthesis){ i--; }
		} while (neededParenthesis && (i > 0));

	}

	return solve2(str);
}

//Second solve does the rest
double solve2(std::string str){	
	if (str == ""){ return 0; }
	double result = 0;
	double cache = 0;
	double positive = 1;
	bool div = 0;
	bool mult = 0;
	bool mod = 0;
	bool not = 0;

	for (int strPos = 0; strPos < str.length(); strPos++){
		if (isLetter(str[strPos]) || isNumber(str[strPos]) || str[strPos]=='('){ 
			if (!div && !mult && !mod){ 
				if (!not){
					cache = getVal(str, strPos)*positive;
				}
				else{
					cache = !getVal(str, strPos)*positive; not = 0;
				}
			}
			else{
				if (mult){ 
					if (!not){
						cache *= getVal(str, strPos); mult = 0;
					}
					else{
						cache *= !getVal(str, strPos); mult = 0; not = 0;
					}

				}
				if (div){ 
					if (!not){
						cache /= getVal(str, strPos); div = 0; 
					}
					else{
						cache /= !getVal(str, strPos); div = 0; not = 0;
					}
				}
					
				if (mod){
					if (!not){
						cache = fmod(cache, getVal(str, strPos)); mod = 0;
					}
					else{
						cache = !fmod(cache, getVal(str, strPos)); mod = 0; not = 0;
					}
					
				}
			}
		}
		if (str[strPos] == '+'){ result += cache; cache = 0; positive = 1; }
		if (str[strPos] == '-'){ result += cache; cache = 0; positive = -1; }
		if (str[strPos] == '*'){ mult = 1; }
		if (str[strPos] == '/'){ div = 1; }
		if (str[strPos] == '%'){ mod = 1; }
		if (str[strPos] == '!'){ not = !not; }
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

				name += "$";
				strPos++;

				std::string parenthesis;
				if (str[strPos] == '('){
					parenthesis = getParenthesis(str.replace(0, strPos, ""), 0);
					strPos += parenthesis.size() + 1;
					if (getCommadContent(parenthesis, 1) == ""){
						name += "(" + numToString(solve(parenthesis)) + ")";
					}
					else{
						name += "(" + numToString(solve(getCommadContent(parenthesis, 0))) + "," + numToString(solve(getCommadContent(parenthesis, 1))) + ")";
					}
				}


				//cout << "result is " + name << endl;

				result += (Strings.FindVal(name));
				
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
		if (str[strPos] == '('){ 
			std::string gp = getParenthesis(str);
			std::string arrContent;
			if (getCommadContent(gp, 1) == ""){
				arrContent = "(" + numToString(solve(gp)) + ")";
			}
			else{
				arrContent = "(" + numToString(solve(getCommadContent(gp, 0))) + "," + numToString(solve(getCommadContent(gp, 1))) + ")";
			}
			varToSet += arrContent;
			strPos += gp.size() + 2;
			//cout << arrContent;
			//Arrays working in variable equalsSet
		}
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
		if (str[strPos] == '('){
			std::string gp = getParenthesis(str);
			std::string arrContent;
			if (getCommadContent(gp, 1) == ""){
				arrContent = "(" + numToString(solve(gp)) + ")";
			}
			else{
				arrContent = "(" + numToString(solve(getCommadContent(gp, 0))) + "," + numToString(solve(getCommadContent(gp, 1))) + ")";
			}
			strPos += gp.size() + 2;
			varToSet += arrContent;
			//cout << "arrcontent is " + arrContent;
			//Arrays working in string equalsSet
		}
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

	for (int i = 0; i < 16; i++){
		for (int i2 = 0; i2 < 3; i2++){
			GRPColArr[i][i2] = GRPColArr[i][i2]/256;
		}
	}
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


//Graphic commands need to write to their own buffers but for now they just write directly to the main screen
void GCOLOR(int num){
	globGCOLOR = num;

}

void GCLS(int num){
	glClearColor(GRPColArr[num][0], GRPColArr[num][1], GRPColArr[num][2], 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLINE(std::string sentcontent){ //Massively unoptimized
	double c1 = solve(getCommadContent(sentcontent, 0))/256;
	double c2 = 1-solve(getCommadContent(sentcontent, 1))/384;
	double c3 = solve(getCommadContent(sentcontent, 2))/256;
	double c4 = 1-solve(getCommadContent(sentcontent, 3))/384;
	std::string c5 = getCommadContent(sentcontent, 4);
	int num;
	if (c5 != ""){ num = solve(c5); }
	else{ num = globGCOLOR; }

	glBegin(GL_LINES);
	glColor3f(GRPColArr[num][0], GRPColArr[num][1], GRPColArr[num][2]);
	glVertex3f(c1, c2, 0.0);
	glVertex3f(c3, c4, 0.0);
	glEnd();

	//glBegin(GL_LINES); //Line drawing example
	//glVertex3f(0, 1, 0.0);
	//glVertex3f(1, 0.5, 0.0);
	//glEnd();
}

void GBOX(std::string sentcontent){
	double c1 = solve(getCommadContent(sentcontent, 0)) / 256;
	double c2 = 1 - solve(getCommadContent(sentcontent, 1)) / 384;
	double c3 = solve(getCommadContent(sentcontent, 2)) / 256;
	double c4 = 1 - solve(getCommadContent(sentcontent, 3)) / 384;
	std::string c5 = getCommadContent(sentcontent, 4);
	int num;
	if (c5 != ""){ num = solve(c5); }
	else{ num = globGCOLOR; }

	glColor3f(GRPColArr[num][0], GRPColArr[num][1], GRPColArr[num][2]);

	glBegin(GL_LINES);
	glVertex3f(c1, c2, 0.0);
	glVertex3f(c3, c2, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(c3, c2, 0.0);
	glVertex3f(c3, c4, 0.0);
	glEnd();	
	
	glBegin(GL_LINES);
	glVertex3f(c3, c4, 0.0);
	glVertex3f(c1, c4, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(c1, c4, 0.0);
	glVertex3f(c1, c2, 0.0);
	glEnd();
}

void GCIRCLE(std::string sentcontent){
	double c1 = solve(getCommadContent(sentcontent, 0)) / 256;
	double c2 = 1 - solve(getCommadContent(sentcontent, 1)) / 384;
	double c3 = solve(getCommadContent(sentcontent, 2)) / 256;

}

void GPSET(std::string sentcontent){
	double c1 = solve(getCommadContent(sentcontent, 0)) / 256;
	double c2 = 1 - solve(getCommadContent(sentcontent, 1)) / 384;
	std::string c3 = getCommadContent(sentcontent, 2);
	int num;
	if (c3 != ""){ num = solve(c3); }
	else{ num = globGCOLOR; }

	glColor3f(GRPColArr[num][0], GRPColArr[num][1], GRPColArr[num][2]);
	glBegin(GL_POINTS);
	glVertex3f(c1, c2, 0.0);
	glEnd();
}


//OpenGL rendering code here



void Draw() {
	//glutKeyboardFunc(processNormalKeys);
	

	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(keyboard);
	

	line++;
	if (line >= pLength){ cout << "End of program\n"; system("pause"); }
	interpretMain(line); //Calls the main program
	

	//glClear(GL_COLOR_BUFFER_BIT);
	
	
	glFlush();//Render everything
	
	if (globBTRIGReset != getTime()){ globBTRIG = 0; globBTRIGReset = getTime(); }

	glutPostRedisplay();

}

void Initialize() {
	int p = 0;
	glClearColor(GRPColArr[p][0], GRPColArr[p][1], GRPColArr[p][2], 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int openglmain(int iArgc, char** cppArgv) {
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(256, 192*2);
	glutInitWindowPosition(700, 200);
	glutCreateWindow("SBInterpret");
	Initialize();
	glutDisplayFunc(Draw);
	glutMainLoop();
	return 0;
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int x, int y) {
	
	switch (key) {
		
	case GLUT_KEY_UP:
		globBTRIG += 1;
		break;
	case GLUT_KEY_DOWN:
		globBTRIG += 2;
		break;
	case GLUT_KEY_LEFT:
		globBTRIG += 4;
		break;
	case GLUT_KEY_RIGHT:
		globBTRIG += 8;
		break;

	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 13:

		//cout << "Pressed Start" << endl;
		globBTRIG += 1024;
		break;

	case 27:
		//cout << "Pressed Escape" << endl;
		break;

	case 88://x
	case 120://X
		//cout << "Pressed A" << endl;
		globBTRIG += 16;
		break;

	case 90://z
	case 122://Z
		//cout << "Pressed B" << endl;
		globBTRIG += 32;
		break;

	case 83://s
	case 115://S
		//cout << "Pressed X" << endl;
		globBTRIG += 64;
		break;

	case 65://a
	case 97://A
		//cout << "Pressed Y" << endl;
		globBTRIG += 128;
		break;

	case 81://q
	case 113://Q
		//cout << "Pressed L" << endl;
		globBTRIG += 256;
		break;

	case 69://e
	case 101://E
		//cout << "Pressed R" << endl;
		globBTRIG += 512;
		break;
	}
}