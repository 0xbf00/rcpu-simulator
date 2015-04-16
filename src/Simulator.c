#include "Pipeline/Pipeline.h"

#include "Instruction/Disassemble.h"
#include "ProgramLoading.h"

#include <stdlib.h> // EXIT_SUCCESS

#ifdef __linux__
#include <malloc.h> // malloc
#include <string.h> // strcmp
#endif

#include <assert.h> // assert
#include <strings.h> // bzero

/*
    Register file.
    PC is the only special register, saved at index 31 
*/
uint32_t registers[32];

/*
    memory is the memory image of our processor.
*/
memory_image_t memory;

void print_usage(const char *program)
{
    printf("[Usage:] %s --program-kind [textual | binary] --program binary [--single-stepping]\n", program);
}

int main(int argc, char *argv[])
{
    bool programKindSet = false;
    LOAD_OPTION programKind = OPT_BINARY;
    bool singleStepping = false;
    char *programString = NULL;

    // preliminary parameter parsing
    for (unsigned int i = 1; i < argc; ++i) {
        if (strcmp("--single-stepping", argv[i]) == 0)
            singleStepping = true;
        else if (strcmp("--program-kind", argv[i]) == 0) {
            if ((i + 1) < argc) {
                if (strcmp("binary", argv[i+1]) == 0) {
                    programKindSet = true;
                    programKind = OPT_BINARY;
                } else if (strcmp("textual", argv[i+1]) == 0) {
                    programKindSet = true;
                    programKind = OPT_TEXTUAL;
                } else {
                    print_usage(argv[0]);
                    return EXIT_FAILURE;
                }
            }
        }
        else if (strcmp("--program", argv[i]) == 0) {
            if ((i + 1) < argc) {
                programString = argv[i+1];
            }
        }
    }

    // If not all required parameters have been set
    if (!programKindSet || !programString) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Read in program
    if (load_program_from_path(programKind, programString, &memory.code, &memory.code_size) != 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize data memory with 1 MB of zeroes.
    memory.data_size = 1024 * 1024; // One MB for now
    memory.data = malloc(memory.data_size);
    assert((memory.data != NULL) && "Failed to allocate memory");

    bzero(memory.data, memory.data_size);

    if_result_t * r1 = NULL;
    id_result_t * r2 = NULL;
    ex_result_t * r3 = NULL;
    mem_result_t * r4 = NULL;

    do {
        // By going the 'wrong' way,
        // we don't have to deal with
        // mutexes etc

        write_back(r4);
        r4 = memory_access(r3);
        r3 = execute(r2);
        r2 = instruction_decode(r1);
        r1 = instruction_fetch();

        if (singleStepping) {
            // Debug print
            
            // Escape sequence that makes the terminal window look like its empty
            fprintf(stdout, "\033[2J\033[1;1H");

            // Print register bank
            fprintf(stdout, "--------------------------------------------------------------------------------\n");
            for (int i = 0; i < sizeof(registers) / sizeof(*registers); ++i) {
                if (i % 4 == 0)
                    fprintf(stdout, "======= ");

                fprintf(stdout, "r%02d: 0x%08x\t", i, registers[i]);

                if (i % 4 == 3)
                    fprintf(stdout, "========\n");
            }
            fprintf(stdout, "--------------------------------------------------------------------------------\n");

            if (r1) {
                const char * instruction_text = instruction_disassemble(r1->inst);
                fprintf(stdout, "IF:\n\tinstruction: \t[0x%08x]: \"%s\"\n", r1->n_pc - 1, instruction_text);
                free((void *)instruction_text);
            }
            if (r2) {
                const char * instruction_text = instruction_disassemble(r2->inst);
                fprintf(stdout, "ID:\n\tinstruction: \t[0x%08x]: \"%s\"\n", r2->n_pc - 1, instruction_text);
                fprintf(stdout, "\tOperand 1:\t0x%08x\n\tOperand 2:\t0x%08x\n\tIO Operand:\t0x%08x\n", r2->op1, r2->op2, r2->io_op);
                free((void *)instruction_text);
            }
            if (r3) {
                const char * instruction_text = instruction_disassemble(r3->inst);
                fprintf(stdout, "EX:\n\tinstruction: \t[0x%08x]: \"%s\"\n", r3->n_pc - 1, instruction_text);
                fprintf(stdout, "\tbranch_taken:\t0x%x\n\tresult:\t\t0x%08x\n", r3->branch_taken, r3->result);
                free((void *)instruction_text);
            }
            if (r4) {
                const char * instruction_text = instruction_disassemble(r4->inst);
                fprintf(stdout, "MEM:\n\tinstruction: \t[0x%08x]: \"%s\"\n", r4->n_pc - 1, instruction_text);
                free((void *)instruction_text);
            }
            
            // Wait for user input
            int c = getchar();
            if (c == EOF) // Hit CTRL+D to invoke this.
                singleStepping = false;
        }

    } while (r1 || r2 || r3 || r4);

    printf("Printing results: \n");
    dump_memory_protocol();

    return EXIT_SUCCESS;
}
