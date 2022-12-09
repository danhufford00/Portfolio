#include <iostream>
#include <iomanip>
#include "simplesim.h"
#include "sml.h"

using namespace std;

bool check_underflow(int);
bool check_overflow(int);

/*****************************************
Function: simplesim()

Use: Initializes the defualt values for a
     simplesim object

Arguments: None

Returns: None

Notes:None
*****************************************/

simplesim::simplesim(){
	for (int x = 0; x < 100; x++)//Sets all memory values to 4444
		memory[x] = 4444;
	accumulator = 0;//Variable values all set to 0
	instruction_counter = 0;
	instruction_register = 0;
	operation_code = 0;
	operand = 0;
}

/*****************************************
Function: bool load_program()

Use: Loads the correct values into the
     memory of a given simplesim object

Arguments: None

Returns: A boolean value representing if
	 the load was successful (true) or
	 not (false)

Notes: None
*****************************************/

bool simplesim::load_program(){

	bool cont = true;
	for(int x = 0; x < 101; x++){//goes through each potential integer

        	if(101 <= x){//If there are too many numbers trying to be put in
			accumulator = 0;
			cout << "*** ABEND: pgm load: pgm too large ***" << endl;
			return false;
        	}

		cin >> memory[x];//Takes the integer and puts it in simplesim

       	 	if(cin.eof()){//If a number is not valid
			cout << "*** ABEND: invalid opcode ***\n";
			instruction_register = 4444;
			operation_code = 44;
			operand = 44;//Values set as per instructions
			dump();
			exit(1);
        	}
		if(cont){
			if(memory[x] == 3400){
	        		instruction_register = 4444;
        	        	operand = 44;
                		operation_code = 44;
        		}
        	}
		cont = false;

        	if(memory[x] == -99999){//If the special number insinuating to end reading is found
        		memory[x] = 4444;
        		return true;
        	}
        	if(check_underflow(memory[x]) || check_overflow(memory[x])){//If there are no overflow problems
        		if(memory[x] != -99999){//If the program is not supposed to end
                		memory[x] = 4444;
                		cout << "*** ABEND: pgm load: invalid word ***" << endl;
                		return false;
                	}
        	}

	}// end for
	cout << "*** ABEND: pgm load: pgm too large ***" << endl;
	accumulator = 0;
	return false;
}

/*****************************************
Function: void execute_program()

Use: executes the simplesim program

Arguments: None

Returns: None

Notes: None
*****************************************/

void simplesim::execute_program(){

    	bool done = false;
   	int num;
	accumulator = 0;

	while(done == false){

        if(instruction_counter >= 0 && instruction_counter <= 99){//If the data is in range
        	instruction_register = memory[instruction_counter];//instruction_register will hold a copy of our word from memory
        }
	else{//If the data is not in range
        	cout << "*** ABEND: addressability error ***" << endl;
        	done = true;
        }

        operation_code = instruction_register / 100;//Left two digits of the instruction_register
        operand = instruction_register % 100;//Right two digits of the instruction_register

	switch(operation_code){
		case READ:
		{
                	cin >> num;

                        if(check_underflow(num) || check_overflow(num)){//If the number is too large or too small
                        	cout << "*** ABEND: illegal input ***" << endl;
                                done = true;//Done is set to true, so the loop will not continue
                        }
			else{//Otherwise we proceed normally
                        	memory[operand] = num;
                        	cout << "READ: " << setw(5) << setfill('0') << internal << showpos << num << endl;
                                setfill(' ');
                            }
                	break;
		}
		case WRITE://Writes the "word" (number) to standard output
		{
			cout << setw(5) << setfill('0') << internal << showpos << memory[operand]  << endl;
                	break;
		}
		case STORE://Stores the value in the accumulator into the memory
		{
                	memory[operand] = accumulator;
                	break;
		}
		case LOAD://Loads the memory from memory into the accumulator
		{
                	accumulator = memory[operand];
                	break;
		}
		case ADD:
		{
               		int temp = accumulator + memory[operand];

                	if(check_overflow(temp)){//If the number is too big
                		cout << "*** ABEND: overflow ***" <<endl;
                  		done = true;
                	}

			else if(check_underflow(temp)){//If the number is too small
                    		cout << "*** ABEND: underflow ***" <<endl;
                    		done = true;
                	}

			else//If the number is not too small or too large
                		accumulator = accumulator + memory[operand];
                	break;
		}
		case SUBTRACT://Same logic as ADD, except we are subtracting
		{
                	int temp = accumulator - memory[operand];

                	if(check_overflow(temp)){
                  	cout << "*** ABEND: overflow ***" <<endl;
                  	done = true;
                	}

			else if(check_underflow(temp)){
                    	cout << "*** ABEND: underflow ***" <<endl;
                    	done = true;
			}

			else
                		accumulator = accumulator - memory[operand];
                	break;
		}
     	       	case MULTIPLY:
		{
                	int temp = accumulator * memory[operand];

                	if(check_overflow(temp)){
                  		cout << "*** ABEND: overflow ***" <<endl;
                  		done = true;
                	}

			else if(check_underflow(temp)){
                    		cout << "*** ABEND: underflow ***" <<endl;
                    		done = true;
                	}

			else
                		accumulator = accumulator * memory[operand];
                	break;
		}
            	case DIVIDE:
		{
                	if(memory[operand] == 0){//If division by 0 is attempted
                    		cout << "*** ABEND: attempted division by 0 ***" << endl;
                    		done = true;
                	}

			else//If not
                		accumulator = accumulator / memory[operand];
                	break;
		}
           	case BRANCH:
		{
  	        	instruction_counter = operand;
         		break;
		}
           	case BRANCHZERO:
		{
     	       		if(accumulator == 0)
                    		instruction_counter = operand;
                	else
                  		instruction_counter++;
                	break;
		}
		case BRANCHNEG:
		{
                	if(accumulator < 0)
                    		instruction_counter = operand;
                	else
                  		instruction_counter++;
                	break;
		}
     	        case HALT:
		{
                	cout << "*** Simplesim execution terminated ***" << endl;//The program has reached it's conclusion
                	done = true;
                	break;
		}
		default:
		{
                	cout << "*** ABEND: invalid opcode ***" << endl;//If the command does not match any case
                	done = true;
                	break;
		}
        }//end of switch statement

        if(done == false && operation_code != 31 && operation_code != 32 && operation_code != 33)
		instruction_counter++;
        }//end of while loop
}

/*****************************************
Function: void dump()

Use: Prints the contents of the simplesim
     object at the end of the program

Arguments: None

Returns: None

Notes: None
*****************************************/

void simplesim::dump(){
	cout << endl << "REGISTERS:" << setfill(' ')<< endl;
  	cout << "accumulator:\t\t" << showpos << right<< setfill('0') << setw(5) << internal << accumulator << endl;
 	cout << setfill(' ');//setfill must be set back to nothing (' ') in order to not fill the next word
  	cout << "instruction_counter:\t" << noshowpos << right<< setw(2) << setfill('0') << instruction_counter << endl;
  	cout << setfill(' ');
  	cout << left << setw(21) << "instruction_register:\t" << showpos << setfill('0') << setw(5) << right << internal << instruction_register << endl;
  	cout << setfill(' ');
  	cout << "operation_code:" << "\t\t" << right << noshowpos << setfill('0') << setw(2) << operation_code << endl;
  	cout << setfill(' ');
  	cout << "operand:\t\t" << right << setfill('0') << setw(2)<< operand << endl;
  	cout << setfill(' ');

  	cout << endl <<  "MEMORY:" << endl;
  	cout << right << setw(8) << 0;

	for(int x = 1; x < 10; x++)//Prints out the 1-9 for the columns
        	cout << right << setw(6)<< x;

  	cout << endl << setw(2) << right << 0;
  	for(int i = 0; i < 100; i++){
        	cout << " "<<  setfill('0') <<  showpos << setw(5) << internal << memory[i];
        	if(i % 10 == 9 && i != 99){
          		cout << endl;
         		 cout << noshowpos << i+1 ;
        	}
  	}
  	cout << endl;
}

/*****************************************
Function: bool check_overflow(int num)

Use: Checks to see if there is an overflow
     error

Arguments: int num - an integer representing
	   the number to be checked

Returns: A boolean value representing if
	 there is an overflow or not (true
	 = yes, false = no)

Notes: None
*****************************************/

bool check_overflow(int num){
	if(num > 9999)
  		return true;
	else
		return false;
}

/*****************************************
Function: bool check_underflow(int num)

Use: Checks to see if there is an underflow
     error

Arguments: int num - an integer representing
	   the number to be checked

Returns: A boolean value representing if
	 there is an underflow or not (true
	 = yes, false = no)

Notes: None
*****************************************/

bool check_underflow(int num){
	if(num < -9999)
		return true;
	else
        	return false;
}
