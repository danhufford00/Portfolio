#include "cpu_single_hart.h"
#include "hex.h"
#include "cpu_single_hart.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

/**
 * This function prints out the appropriate usage error message and Pattern to
 * standard output before terminating the program in the traditional manner
 ********************************************************************************/

static void usage(){
	cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
	cerr << "    -d show disassembly before program execution" << endl;
    	cerr << "    -i show instruction printing during execution" << endl;
    	cerr << "    -l maximum number of instructions to exec" << endl;
    	cerr << "    -m specify memory size (default = 0x100)" << endl;
	cerr << "    -r show register printing during execution" << endl;
    	cerr << "    -z show a dump of the regs & memory after simulation" << endl;

    	exit(1);
}

/**
 * Starts the process of disassembling and decoind the instructions
 *
 * @param const memory &mem - a memory object to be decoded 32 bits at a time
 ********************************************************************************/
static void disassemble(const memory &mem){
	uint32_t pc = 0;

	while(pc < mem.get_size()){
		cout << hex::to_hex32(pc) << ":";
        	cout << " " << hex::to_hex32(mem.get32(pc)) << "  ";
       	 	cout << rv32i_decode::decode((uint32_t)pc, (uint32_t)mem.get32(pc));
        	cout << endl;

		pc+=4;
    	}
}

/**
 * This functions is the main driver for the program, checking args, calling the
 * disassemble function, and dumping memory where appropriate
 *
 * @param int argc - the argument count
 *     char **argc - a character array of arguments
 *
 * @return an int representing whether or not the program successfully completed
 ********************************************************************************/
int main(int argc, char **argv){
	uint32_t memory_limit = 0x100;	//default memory size = 256 bytes
	int execution_limit = 0; 	//default is limitless
	int opt;

	bool dFlag = false;
    	bool zFlag = false;
    	bool rFlag = false;
    	bool iFlag = false;

	while ((opt = getopt(argc, argv, "m:dil:rz")) != -1){
		switch(opt){
			case 'm':{ //-m specifies size
				istringstream iss(optarg);
				iss >> std::hex >> memory_limit;
			}
			break;
        		case 'd':{ //-d prints a disassembly of memory before the program begins
                		dFlag = true;
            		}
			break;
        		case 'i':{ //-i shows instruction printing during execution
                		iFlag = true;
            		}
			break;
        		case 'l':{ //-l gives the maximum amount of instructions to execute.
                		execution_limit = atoi(optarg);
            		}
			break;
        		case 'r':{ //-r shows a dump of the hart status before each instruction is executed
                		rFlag = true;
            		}
			break;
        		case 'z':{ //-z shows a dump of the hart status after the simulation is over
                		zFlag = true;
            		}
			break;
			default:
			usage();
		}
	}

	if (argc <= optind)
		usage();

	memory mem(memory_limit);
    	cpu_single_hart hart(mem);

	if (!mem.load_file(argv[optind]))
		usage();

    	if(dFlag == true){
        	disassemble(mem);
        	hart.reset();
    	}

    	if(rFlag == true)
        	hart.set_show_registers(rFlag);

    	if(iFlag == true)
        	hart.set_show_instructions(iFlag);

    	hart.run(execution_limit);

	if (zFlag == true){
		hart.dump();
		mem.dump();
	}

	return 0;
}
