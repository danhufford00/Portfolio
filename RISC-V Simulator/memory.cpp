#include "memory.h"

using namespace std;

/**
* This function constructs a memory object by allocating siz bytes into the mem
* vector, the value of each being 0xa5
*
* @param uint32_t siz is the size of the mem vector
*********************************************************************************/

memory::memory(uint32_t siz){
	siz = (siz + 15)&0xfffffff0; //rounds up by 15 and uses 0xf to round down
	std::vector <uint8_t> mem (siz, 0xa5); //the mem vector is initialized with size of siz and value of 0xa5
	this -> mem = mem; //specifies that this does not refer to the mem in memory.h
}

/**
* This function deconstructs a given memory object by calling the clear function,
* which removes all elements from the mem vector
*********************************************************************************/

memory::~memory(){
	mem.clear();
}

/**
* This function checks to see if an address is out of range
*
* @param uint32_t i is the address in memory
*
* @return a bool, true if the address is out of range and false if not
*********************************************************************************/

bool memory::check_illegal(uint32_t i) const{
	if (i >= mem.size()){ //if the address is in memory
		cout << "WARNING: Address out of range: " << to_hex0x32(i) << endl;
		return true;
	}
	else
		return false;

}

/**
* This function returns the size of the vector mem
*
* @return a uint32_t variable representing the size of the vector mem
*********************************************************************************/

uint32_t memory::get_size() const{
	return mem.size();
}

/**
* This function checks if a given address is in memory, and if so, returns it as
* an unsigned 8 bit number
*
* @param uint32_t addr is the address
*
* @return an unsigned uint8_t representing the address in memory (if it's there)
* of size 8 bits
*********************************************************************************/

uint8_t memory::get8(uint32_t addr) const{
	if(!check_illegal(addr)) //if the address is in the vector
		return mem[addr]; //fetch it
	else
		return 0;
}

/**
* Returns an address as an unsigned 16 bit number
*
* @param uint32_t addr is the address
*
* @return an address(typecasted) in little endian format with the address
* represented as an unsigned 16 bit value
*********************************************************************************/

uint16_t memory::get16(uint32_t addr) const{
	return (uint16_t) get8(addr) + ((uint16_t) get8(addr + 1) << 8); //The least significant value is stored first in the lower address
}

/**
* This function returns an address as an unsigned 32 bit number
*
* @param uint32_t addr is the address
*
* @return an address(typecasted) in little endian format with the address
* represented as an unsigned 32 bit value
*********************************************************************************/

uint32_t memory::get32(uint32_t addr) const{
	return (uint32_t) get16(addr) + ((uint32_t) get16(addr + 2) << 16);
}

/**
* This function sign extends an 8 bit value in memory and returns it as an 32 bit
* signed int
*
* @param uint32_t addr is the address
*
* @return the sign extended value of addr as a 32 bit signed integer
*********************************************************************************/

int32_t memory::get8_sx(uint32_t addr) const{
	return (int8_t) get8(addr); //Sign extension takes the leftmost bit and extends it before adding the rest
}

/**
* This function sign extends a 16 bit value in memory and returns it as a 32 bit
* signed int
*
* @param uint32_t addr is the address in memory
*
* @return the sign-extended value of addr of length 16 as a 32 bit signed integer
*********************************************************************************/

int32_t memory::get16_sx(uint32_t addr) const{
	return (int16_t) get16(addr);
}

/**
* This function sign extends a 32 bit value in memory and returns it as a 32 bit
* signed int
*
* @param uint32_t addr is the address in memory
*
* @return the sign-extended value of addr as a 32 bit signed integer
*********************************************************************************/

int32_t memory::get32_sx(uint32_t addr) const{
	return (int32_t) get32(addr);
}

/**
* This function inputs a given value (8 bits long) in an address
*
* @param uint32_t addr is the address in memory
*        uint8_t val is the value to be put into memory
*********************************************************************************/

void memory::set8(uint32_t addr, uint8_t val){
	if(!check_illegal(addr)) //if the address is valid
		mem[addr] = val; //val is input in the spot give by addr
}

/**
* This function inputs a given value (16 bits long) in memory in little endian
* order
*
* @param uint32_t addr is the address
*        uint16_t val is the value to be put in the address
*********************************************************************************/

void memory::set16(uint32_t addr, uint16_t val){
	set8(addr, val&0x0000ffff); //signed masking
	set8(addr + 1, val >> 8); //shift right 8 times
}

/**
* This function stores a given value (32 bits long) in memory in little endian
* order
*
* @param uint32_t addr is the address
*        uint32_t val  is the value
*********************************************************************************/

void memory::set32(uint32_t addr, uint32_t val){
	set16(addr, val&0xffffffff); //signed masking
	set16(addr + 2, val >> 16); //shift right 16 times
}

/**
* This function dumps the entire contents of simulated memory in hex with the
* corresponding ASCII characters
*********************************************************************************/

void memory::dump() const{
	for(uint32_t x = 0; x < mem.size(); x++){ //read through vector mem to get the left most values
		if(x % 0x10 == 0){ //print out every 10th address
			cout << to_hex32(x) << ": "; //output as a 32 bit unsigned int

			for(uint32_t y = 0; y < 16; y++){ //the hex value is output to the right of the addresses
				cout << to_hex8(get8(x + y)) << ' ';

				if(y == 7)
					cout << " ";
			}

			cout << "*"; //print out the hex

			for(uint32_t z = 0; z < 16; z++){
				uint8_t c = get8(x + z);
				c = isprint(c) ? c : '.';
				cout << (char) c;
			}
			cout << "*";
		}

		if(x % 0x10 == 0x0f)
			cout << endl;
	}
}

/**
* This function checks if a file can be accessed, and if so, opens and reads from
* it
*
* @param string &fname is the file name
*
* @return a bool, false if the file can't be opened or the program is too big, or
* true if the file is successfully loaded
*********************************************************************************/

bool memory::load_file(const std::string &fname){

	std::ifstream file(fname, std::ios::in|std::ios::binary);

	if(file.fail()){
		return false;
		cerr << "Can't Open file 'testdata' for reading" << endl;
	}
	else{ //if the file is successfully opened
		uint8_t x;
		file >> std::noskipws;
		for(uint32_t addr = 0; file >> x; ++addr){
			if(check_illegal(addr)){
				cerr << "Program too big." << endl;
				return false;
			}
			else
				set8(addr, x);
		}
	}
	return true;
}

