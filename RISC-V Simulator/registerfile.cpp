#include "registerfile.h"

using namespace std;

/**
* This function constructs a registerfile object by calling the reset
* function, which initializes register x0 to zero and all other registers
* to 0xf0f0f0f0
****************************************************************************/

registerfile::registerfile(){
	reset();
}

/**
* This function initializes register x0 to zero and all other registers to
* 0xf0f0f0f0
****************************************************************************/

void registerfile::reset(){
	for(uint32_t x = 0; x < 32; x++){
        	if(x == 0)
            		regs.push_back(0);
       		else
            		regs.push_back(0xf0f0f0f0);
    	}
}

/**
* This function assigns register r the given value val if r is not zero
*
* @param uint32_t r - an unsigned 32 bit int representing a register
*       int32_t val - a signed 32 bit int
****************************************************************************/

void registerfile::set(uint32_t r, int32_t val){
	if(r != 0)
        	regs.at(r) = val;
}

/**
* This function returns the value of register r
*
* @param uint32_t r - an unsigned 32 bit int representing a register
*
* @return a signed 32 bit int representing the value of register r
****************************************************************************/

int32_t registerfile::get(uint32_t r) const{
	if(r == 0)
		return 0;
    	else
		return regs.at(r);
}

/**
* This function dumps the contents of the registers
*
* @param const std::string &hdr - a string that must be printed at the
*				  beginning of the output lines
****************************************************************************/

void registerfile::dump(const std::string &hdr) const{
	int k = 0;

    	for(uint32_t x = 0; x < 4; x++){
        	if(k % 8 == 0){
            		string temp = "x" + std::to_string(k);
            		cout << hdr << right << setw(3) << temp;
        	}

        	for(uint32_t j = 0; j < 8; j++,k++){
            		if(j == 4)
                		cout << " ";
            		cout << " " << hex::to_hex32(regs.at(k));
        	}
        cout << endl;
    	}
}
