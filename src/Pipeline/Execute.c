#include "Execute.h"

#include "../Instruction/ALUOps.h"

#include <stdlib.h>
#include <assert.h>

/*
    our only flag.
    True iff the previous comparison was true
*/
static bool flag = false;

ex_result_t * execute(const id_result_t * const in)
{
    // if no input, return no output
    if (in == NULL)
        return NULL;

    ex_result_t * const res = calloc(1, sizeof(ex_result_t));

    res->n_pc = in->n_pc;
    res->inst = in->inst;
    res->branch_taken = 0;

    res->io_op = in->io_op;

    const uint8_t opcode = instruction_decode_opcode(res->inst);
    const instruction_type_t type = instruction_decode_type(res->inst);

    switch (type) {
        case BINARY_ARITHMETIC:
            {
                res->result = binary_functions[opcode](in->op1, in->op2);
                break;
            }
        case UNARY_ARITHMETIC:
            {
                res->result = unary_functions[opcode](in->op1);
                break;
            }
        case COMPARE:
            {
                flag = compare_functions[opcode](in->op1, in->op2);
                break;
            }
        case BRANCH:
            {
                res->branch_taken = flag ? 1 : 0;
                res->result = in->op1 + in->op2;
                break;
            }
        case JUMP:
            {
                res->branch_taken = 1;
                res->result = in->op1 + in->op2;
                break;
            }
        case IO:
            res->result = in->op1 + in->op2;

            assert((res->result >= 0)
               && ((res->result * 4) < memory.data_size)
               && "Illegal offset.");

            break;
        case MISC:
            ; // NOP
              // HALT is handled seperately below.
            break;
        case UNKNOWN:
        default:
            assert(false && "Instruction not supported.");
    }

    free((void *)in);
    return res;
}
