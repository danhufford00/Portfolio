#include "hex.h"

/**
* This function uses an output stream class object to format a string of 2 hex
* digits representing an unsigned int of bit length 8
*
* @uint8_t i is an unsigned int of bit length 8
*
* @return a string of 2 hex digits representing an unsigned int of bit length 8
*********************************************************************************/

std::string hex::to_hex8(uint8_t i){
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
	return os.str();
}

/**
* This function uses an output stream class object to format a string of 8 hex
* digits representing an unsigned int of bit length 32
*
* @uint32_t i is an unsigned int of bit length 32
*
* @return a string of 8 hex digits representing an unsigned int of bit length 32
*********************************************************************************/

std::string hex::to_hex32(uint32_t i){
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(i);
	return os.str();
}

/**
* This function uses an output stream class object to format a string of 8 hex
* digits preceded by 0x representing an unsigned int of bit length 32
*
* @uint32_t i is an unsigned int of bit length 32
*
* @return a string of 8 hex digits preceded by 0x representing an unsigned int of
* bit length 32
*********************************************************************************/

std::string hex::to_hex0x32(uint32_t i){
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(i);
	return std::string("0x")+to_hex32(i);
}

/**
* This function takes a uint32_t variable and returns a string beginning with 0x
* and followed by the 5 hex digits representing the 20 bits of the i argument
*
* @uint32_t i is an unsigned int of bit length 32
*
* @return a string of 5 hex digits preceded by 0x representing i
*********************************************************************************/

std::string hex::to_hex0x20(uint32_t i){
    	std::ostringstream os;
    	os << std::string("0x") << std::hex << std::setfill('0') << std::setw(5) << i;
    	return os.str();
}

/**
* This function takes a uint32_t variable and returns a string beginning with 0x
* and followed by the 3 hex digits representing the 12 bits of the i argument
*
* @uint32_t i is an unsigned int of bit length 32
*
* @return a string of 3 hex digits preceded by 0x representing i
*********************************************************************************/

std::string hex::to_hex0x12(uint32_t i){
	std::ostringstream os;
    	os << std::setfill('0') << std::setw(10) << std::hex << i;
    	return "0x" + os.str().substr(7);
}
