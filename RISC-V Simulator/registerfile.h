#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "hex.h"

#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

/**
* The registerfile class stores the state of the general-purpose registers
* one RISC-V hart
***************************************************************************/

class registerfile{
	public:
    		registerfile();
    		void reset();
    		void set(uint32_t r, int32_t val);
    		int32_t get(uint32_t r) const;
    		void dump(const std::string &hdr) const;

	private:
    		std::vector<int32_t> regs;
};

#endif
