#include "cpu_single_hart.h"

using namespace std;

/**
* This function sets register x2 to the memory size and runs our simulated
* hart
*
* @param uint64_t exec_limit - an unsigned 64 bit integer representing the
*			       execution limit
***************************************************************************/

void cpu_single_hart::run(uint64_t exec_limit){
	regs.set(2, mem.get_size());

	if(exec_limit != 0){
                while(!is_halted() && (exec_limit != get_insn_counter()))
                        tick();
        }

    	else if(exec_limit == 0){
        	while(!is_halted())
            		tick();
    	}

    	if(get_halt_reason() != "none"){
        	cout << "Execution terminated. Reason:";
        	cout << get_halt_reason() << endl;
    	}

        cout << get_insn_counter() << " instructions executed" << endl;
}

