#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H

#include "rv32i_hart.h"

/**
* class cpu_single_hart is a subclass of rv32i_hart that is used to
* represent a CPU with a single hart
****************************************************************************/

class cpu_single_hart : public rv32i_hart{
	public:
    		cpu_single_hart(memory &mem) : rv32i_hart(mem) {};
    		void run(uint64_t exec_limit);
};

#endif
