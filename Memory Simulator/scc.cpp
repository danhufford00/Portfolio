#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "sml.h"
#include "inpost.h"

using namespace std;

struct table_entry{
    int symbol;
    char type;//'C' constant, 'L' Simple line number, 'V' variable
    int location;//Simplesim address (00 to MEMSIZE-1)
};

class scc{
	public:
   		static const int MEMORY_SIZE = 100;
   		static const int SYMBOL_TABLE_SIZE = 1000;
        	static const int NOTFOUND = -1;//Used for if something in the symbol table is not found

		scc();
		void first_pass();
   		void second_pass();
   		void print_program() const;
   		void memory_check() const;
   		int search_symbol_table(int, char);
        	int process(string);
        	void check_stack(int) const;

	private:
   		int memory[MEMORY_SIZE];
   		int data[MEMORY_SIZE];
   		int ndata;

   		table_entry symbol_table[SYMBOL_TABLE_SIZE];
   		int flags[MEMORY_SIZE];

   		int next_instruction_addr;
   		int next_const_or_var_addr;
   		int next_symbol_table_idx;
        	int next_stack_idx;
};

int main(){
	scc compiler;
    	compiler.first_pass();
    	compiler.second_pass();
    	compiler.print_program();

    	return 0;
}

/****************************************
Function: scc()

Use: Initializes the defualt values of an
     scc object

Arguments: None

Return: None

None: None
****************************************/
scc::scc(){
	for (int x = 0; x < MEMORY_SIZE; x++){
		memory[x] = 4444;
		flags[x] = -1;
	}

	for (int x =0; x < SYMBOL_TABLE_SIZE; x++)
		symbol_table[x].type = ' ';//By defualt, we do not know the type of any given item in the symbol table

	next_instruction_addr = 0;//Except for next_const_or_var_addr, the values of each variable will be 0 by defualt
	next_stack_idx = 0;
	next_const_or_var_addr = MEMORY_SIZE - 1;
    	ndata = 0;
    	next_symbol_table_idx = 0;

}

/****************************************
Function: void first_pass()

Use: Executes the first pass of the compiler

Arguments: None

Return: None

None: None
****************************************/

void scc::first_pass(){
	string buffer1, buffer2, line_number, command;

	while (getline(cin, buffer1)){
    		buffer2 = buffer1;//buffer2 used for 'let'
    		istringstream ss(buffer1);

    		ss >> line_number;//We first input the line number in the symbol table

    		symbol_table[next_symbol_table_idx].type='L';
    		symbol_table[next_symbol_table_idx].symbol=stoi(line_number);
    		symbol_table[next_symbol_table_idx].location=next_instruction_addr;
    		next_symbol_table_idx++;//next_symbol_table_idx is incremented many times over the course of this program, as we are not just changing the values of the one address

    		ss >> command;//The command variable is the word right after the line number, that indicates what should be done with any given line

    		if (command == "input"){//Stores input for later variable initialization
            		string variable;
            		int loc;
            		ss >> variable;
            		loc = process(variable);

            		memory_check();//We will be checking to see if we have enough memory whenever we change something in memory
            		memory[next_instruction_addr] = READ * 100 + loc;//If we are inputing something, we need the READ case and what variable to do it to(that's why we add location)
            		next_instruction_addr++;//As per usual, we increment next_instruction_addr
    		}
    		else if (command == "data"){
        		string value;
        		ss >> value;

            		if(ndata > MEMORY_SIZE -1){//If we have more data than memory
        			cerr << "*** ERROR: too many data lines ***" << endl;
        			exit(1);
			}

			data[ndata]=stoi(value);//If there are no data or memory issues, we continue by putting the value into the data array
			ndata++;
    		}
		else if (command == "let"){
            		string variable, sub_string, op, postfix;
            		int location, opLocation;
            		std::size_t temp, temp2;

            		ss >> variable;//Here we are storing the variable to be assigned into the variable named variable
            		location = process(variable);

            		temp = buffer2.find('=');//Checks if '=' is in our temp variable
            		temp2 = buffer2.find_first_not_of(' ', temp+1);//finds the first thing in buffer2, we don't care what it is
            		sub_string = buffer2.substr(temp2);//Puts everything from the temp2 into the sub_string variable
            		postfix = convert(sub_string);//Here we call our convert function from the inpost.cpp file to convert this from infix to postfix

            		istringstream tt(postfix);//I chose tt because t comes after s in the alphabet
            		while (tt >> op){
                		if (isdigit(op[0]) || islower(op[0])){//If op is a constant or a variable
                    			opLocation = process(op);//Checks where op is in memory

                    			memory_check();
                    			memory[next_instruction_addr] = LOAD * 100 + opLocation;//Adds the appropriate LOAD command into memory
                    			next_instruction_addr++;

                    			memory_check();
                    			memory[next_instruction_addr] = STORE * 100;//Adds the appropriate STORE command into memory
                    			flags[next_instruction_addr] = -3 - next_stack_idx;//The flags must also be set
                    			next_instruction_addr++;

                    			next_stack_idx++;
                		}
                		else{//If op is an operator. Also, the logic here is pretty similair to the aboce logic, except addresses are omitted and more flags are set
                    			if (op[0] == '+' || op[0] == '*'){//If op is either the + or * operator
                        			memory[next_instruction_addr] = LOAD * 100;//Addresses aren't needed because these are neither constants nor variables
                        			next_stack_idx--;
                        			flags[next_instruction_addr] = -3 - next_stack_idx;//The appropriate flags are set
                        			next_instruction_addr++;//The next_instruction_addr variable must be incremented after each addition to memory

                        			memory[next_instruction_addr] = ((op[0] == '+') ? ADD:MULTIPLY) * 100;//Both this block of code and the below block use similair logic to the above block
                        			next_stack_idx--;
                        			flags[next_instruction_addr] = -3 - next_stack_idx;
                        			next_instruction_addr++;

                        			memory[next_instruction_addr] = STORE * 100;
                        			flags[next_instruction_addr] = -3 - next_stack_idx;
                        			next_stack_idx++;
                        			next_instruction_addr ++;
  					}
                    			if (op[0] == '-' || op[0] == '/'){//As stated above, every block of code here fundamentally uses the aforementioned explained logic
                        			memory[next_instruction_addr] = LOAD * 100;
                        			next_stack_idx--;
                        			flags[next_instruction_addr] = -3 - next_stack_idx;
                        			next_instruction_addr++;

                        			memory[next_instruction_addr] = STORE * 100;
                        			flags[next_instruction_addr] = -2;
                        			next_instruction_addr++;

                        			memory[next_instruction_addr] = LOAD * 100;
                        			next_stack_idx--;
                        			flags[next_instruction_addr] = -3 - next_stack_idx;
                        			next_instruction_addr++;

                        			memory[next_instruction_addr] = ((op[0] == '-') ? SUBTRACT:DIVIDE) * 100;
                        			flags[next_instruction_addr] = -2;
                        			next_instruction_addr++;

                        			memory[next_instruction_addr] = STORE * 100;
                        			flags[next_instruction_addr] = -3 - next_stack_idx;
                        			next_stack_idx++;
                        			next_instruction_addr++;
                    			}
                		}
            		}
            		memory_check();//Here we check again if we are out of memory
            		memory[next_instruction_addr] = LOAD * 100;//Here we insert the LOAD command
            		next_stack_idx--;
            		flags[next_instruction_addr] = -3 - next_stack_idx;
            		next_instruction_addr++;
            		memory_check();
            		memory[next_instruction_addr] = STORE * 100 + location;//Here we insert the STORE command, with what to store as well
            		next_instruction_addr++;
		}
		else if (command == "print"){
            		string variable;
            		int loc;
            		ss >> variable;//What we are printing is stored in the variable variable
            		loc = process(variable);//As per usual, we are checking if the value is in memory already

            		memory_check();//Also as per usual, we are checking if we are out of memory
            		memory[next_instruction_addr] = WRITE * 100 + loc;
            		next_instruction_addr++;
        	}
    		else if (command == "goto"){
            		string variable;
            		int result, item_to_add;
            		ss >> variable;
            		item_to_add = stoi(variable);
            		result = search_symbol_table(item_to_add, 'L');
            		memory_check();//If we are not out of memory

            		if (result != NOTFOUND)//If result is not in the symbol table, we add it
                		memory[next_instruction_addr] = BRANCH * 100 + symbol_table[result].location;
            		else{//If result is in the symbol table
                		memory[next_instruction_addr] = BRANCH * 100;//Here we insert the BRANCH command without the location because it is not in the symbol table
                		flags[next_instruction_addr] = item_to_add;
            		}
            		next_instruction_addr++;
        	}
    		else if (command == "if"){
            		string lOp, rOp, relOp, go_to, lineNum;

            		ss >> lOp;
            		ss >> relOp;
            		ss >> rOp;
            		ss >> go_to;//We only put this value in go_to because we do not need it and do not want it in lineNum
            		ss >> lineNum;

            		int Llocation, Rlocation, result;
            		Llocation = process(lOp);//Holds the location of lOp
            		Rlocation = process(rOp);//Holds the location of rOp

            		if (relOp == "<" || relOp == ">"){//If the relational operator is less than or greater than
                		int f, s;

                		if (relOp == "<"){//Depending on if the relational operator is less than or greater than,
                    			f = Llocation;
                    			s = Rlocation;
                		}
                		else{//If the relational operator is greater than
                    			f = Rlocation;
                    			s = Llocation;
                		}
                		memory_check();
                		memory[next_instruction_addr] = LOAD * 100 + f;//Here we are loading the first value
                		next_instruction_addr++;

                		memory_check();
                		memory[next_instruction_addr] = SUBTRACT * 100 + s;//We are setting a command to subtract the value of the second variable from the first here
                		next_instruction_addr++;

                		result = search_symbol_table(stoi(lineNum), 'L');
                		memory_check();

                	if (result != NOTFOUND)//If the result is not in the symbol table we add it
                    			memory[next_instruction_addr]= BRANCHNEG*100 + symbol_table[result].location;
                		else{
                    			memory[next_instruction_addr] = BRANCHNEG * 100;  // omit address (forward reference)
                    			flags[next_instruction_addr] = stoi(lineNum);
                		}
                		next_instruction_addr++;
            		}
            		else if (relOp == "<=" || relOp == ">=" ){//The logic here is similair to the above logic, except it's for less than or equal to and greater than or equal to
                		int f, s;

                		if (relOp == "<="){//Depending on if the relational operator is less than or equal to or greater than or equal to, we choose which value is first and which is second
                    			f = Llocation;
                    			s = Rlocation;
                		}
                		else{
                    			f = Rlocation;
                    			s = Llocation;
                		}
                		memory_check();
                		memory[next_instruction_addr] = LOAD * 100 + f;//Here we load the value that represents the left operand
                		next_instruction_addr++;

               			memory_check();
                		memory[next_instruction_addr] = SUBTRACT * 100 + s;//Here load a "word" (4 integer number) representing the commadn telling the program to subtract the value of the second variable from the first
                		next_instruction_addr++;

                		result = search_symbol_table(stoi(lineNum), 'L');
                		if (result != NOTFOUND){//If the result is not in the symbol table, we again must add it
                    			memory_check();
                    			memory[next_instruction_addr] = BRANCHNEG * 100 + symbol_table[result].location;
                    			next_instruction_addr++;
                    			memory_check();
                    			memory[next_instruction_addr] = BRANCHZERO * 100 + symbol_table[result].location;
                		}
                		else{//The result is in the symbol table
                    			memory_check();
                    			memory[next_instruction_addr] = BRANCHNEG * 100;//Since what we want is already in the symbol table, we do not need to add f or s for either
                    			flags[next_instruction_addr] = stoi(lineNum);//The flags again must be set
                    			next_instruction_addr++;
                    			memory_check();
                    			memory[next_instruction_addr] = BRANCHZERO * 100;//Same as above
                    			flags[next_instruction_addr] = stoi(lineNum);
                		}
                		next_instruction_addr++;
            		}
            		else if (relOp == "=="){//If the relational operator is equals
            			memory_check();
                		memory[next_instruction_addr] = LOAD * 100 + Llocation;//Here we load the left operand as a word in Simplesim
                		next_instruction_addr++;

                		memory_check();
                		memory[next_instruction_addr] = SUBTRACT * 100 + Rlocation;//Here we load a word telling Simplesim to subtract the second value by the one we just loaded
                		next_instruction_addr++;

                		result = search_symbol_table(stoi(lineNum), 'L');
                		if (result != NOTFOUND){//If result is not in the symbol table, we BRANCHZERO
                			memory_check();
                    			memory[next_instruction_addr]= BRANCHZERO * 100 + symbol_table[result].location;
                		}
                		else{//If the result is not in the symbol table, we don't need anything besides BRANCHZERO * 100 for the value in memory
                    			memory_check();
                    			memory[next_instruction_addr] = BRANCHZERO * 100;
                    			flags[next_instruction_addr] = stoi(lineNum);
                		}
                		next_instruction_addr++;
            		}
            		else if (relOp == "!="){//If the relational operator is not equals to
                		memory_check();
                		memory[next_instruction_addr] = LOAD * 100 + Llocation;//As per usual we load what is to be the left operand
                		next_instruction_addr++;

                		memory_check();
                		memory[next_instruction_addr] = SUBTRACT * 100 + Rlocation;//We set a command to subtract what is to be the right oeprand
                		next_instruction_addr++;

                		result = search_symbol_table(stoi(lineNum), 'L');//We search the symbol table for this result
                		if (result != NOTFOUND){//If the result is not found
                    			memory_check();
                    			memory[next_instruction_addr]= BRANCHZERO * 100 + next_instruction_addr+2;
                    			next_instruction_addr++;
                    			memory_check();
                    			memory[next_instruction_addr]= BRANCH*100 + symbol_table[result].location;
                    			next_instruction_addr++;
                		}
                		else{//If the result is found
                    			memory_check();
                    			memory[next_instruction_addr]= BRANCHZERO*100 + next_instruction_addr+2;
                    			next_instruction_addr++;
                    			memory_check();
                    			memory[next_instruction_addr] = BRANCH*100;   // omit address  (forward reference)
                    			flags[next_instruction_addr] = stoi(lineNum);
                    			next_instruction_addr++;
                		}
            		}
        	}
    		else if (command == "end"){//If the command tells us to halt a halt command in the Simple language
			memory_check();
			memory[next_instruction_addr] = HALT * 100;//HALT * 100 or 3400 is added
			next_instruction_addr++;
    		}
    		else if (command == "rem"){//Everything after rem is discarded, as it is just the command to put in a comment
            		string line;
            		ss >> line;
            		while (ss.rdbuf()->in_avail() != 0)//While there is room in line for the input, we put it in
                		ss >> line;
    		}
	}
}

/****************************************
Function: void second_pass()

Use: Executes the seconds pass of the
     compiler, largely based off of flags
     set in the first pass

Arguments: None

Return: None

None: None
****************************************/
void scc::second_pass(){
	int special = next_const_or_var_addr;
	int start = next_const_or_var_addr -1;
	int index = 0;
	int addr;
	int location;

    	for (int x = 0; x < MEMORY_SIZE; x++){
        	if (flags[x] != -1){//-1 here is our marker that there is nothing left to do with this value
            		if (flags[x] > 0){//If the index of flag[x] is greater than zero
                		location = symbol_table[search_symbol_table(flags[x],'L')].location;//We search the symbol table
                		memory[x] += location;//We add where we found this to memory
            		}
            		if (flags[x] == -2)
                		memory[x] += special;
            		if (flags[x] < -2){//If the value of flags[x] < -2, it was because it was -3 minus something
                		index = -3 - flags[x];
                		addr = start - index;
                		check_stack(addr);
                		memory[x] += addr;
            		}
        	}
    	}
}
/****************************************
Function: void print_program() const

Use: Prints the contents of the .s once
     they have been translated to Simple

Arguments: None

Return: None

None: None
****************************************/
void scc::print_program() const{
	cout << setfill('0') << internal << showpos;//We want to show if it is positive or negative, with 4 integer places always

	for (int x = 0; x < MEMORY_SIZE; x++)
		cout << setw(5) << memory[x] << endl;//Here we print out all the contents of the memory

	cout << -99999 << endl;//Signals the end of the program
	cout << setfill(' ') << noshowpos;//We have to say here that we do not want to show the parity and do not want every integer to be displayed to 4 integers

	for (int x = 0; x < ndata; x++)
		cout << data[x] << endl;
}

/****************************************
Function: void memory_check() const

Use: Checks if there is room in the memory,
     and if not, ends the program

Arguments: None

Return: None

None: None
****************************************/
void scc::memory_check() const{
	if (next_instruction_addr >= next_const_or_var_addr){//If there is not enough memory, we print the appropriate error and exit the program
		cerr << "*** ERROR: ran out Simplesim memory ***\n";
		exit(1);
	}
}

/****************************************
Function: void check_stack(int num) const

Use: Checks if the hole in the stack can
     fit another value

Arguments: int num - the address of the
	   instruction to be added

Return: None

None: None
****************************************/
void scc::check_stack(int num) const{
    	if (num < next_instruction_addr){//If there is no room in the stack hole
        	cerr << "*** ERROR: insufficient stack space ***\n";
        	exit(1);
    	}
}

/****************************************
Function: int search_symbol_table(int symbol,
          char type)

Use: Searches the symbol table for a given
     (technically two) value(s)

Arguments: int symbol - an integer
           representing a given symbol we
	   are searching for

	  char type - a character
	  representing a given type we are
	  searching for

Return: An integer representing where in
	memory the value(s) is (are
	concurrently)

None: None
****************************************/
int scc::search_symbol_table(int symbol, char type){
	for(int x = 0; x < next_symbol_table_idx; x++){//We are going to go through each element of the symbol table
		if (symbol_table[x].symbol == symbol && symbol_table[x].type == type)//If the symbol and type we are searching for are concurrently present in an element, this is the element we return
			return x;
	}//If we finish our search without finding the given element, we return that the value was not found
    	return NOTFOUND;
}

/****************************************
Function: int process(string in)

Use: Searches the memory for a given item,
     and if found, is added to the symbol
     table and also has it's locaiton
     returned as an integer

Arguments: string in - a string representing
	   the item we are seraching for

Return: An integer representing the item's
	location

None: None
****************************************/

int scc::process(string in){
	int result, address, symbol;
	char type;

    	if (islower(in[0])){//If in is a variable
        	type = 'V';
        	symbol = in[0];
	}
    	else{//If in is a constant
        	type = 'C';
        	symbol = stoi(in);
    	}
    	result = search_symbol_table(symbol, type);//We are checking if this element is in the symbol table
	if (result == NOTFOUND){//If this is not yet in the symbol table, we must add it
        	symbol_table[next_symbol_table_idx].type = type;
        	symbol_table[next_symbol_table_idx].symbol = symbol;
        	symbol_table[next_symbol_table_idx].location = next_const_or_var_addr;
        	next_symbol_table_idx++;//As per usual, we increment next_symbol_table_idx
        	address = next_const_or_var_addr;//The location is next_const_or_var_addr
        	memory_check();//Are we out of memory?

        	if (type == 'V')//If we are not out of memory and the type is a variable
			memory[next_const_or_var_addr] = 0;
        	else//If the type is a constant
			memory[next_const_or_var_addr]= symbol;
        	next_const_or_var_addr--;
    	}
    	else
        	address = symbol_table[result].location;
    	return address;
}
