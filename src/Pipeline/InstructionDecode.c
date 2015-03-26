#include "InstructionDecode.h"

#include <stdlib.h>
#include <assert.h>

/*
    Returns the contents of register op
*/
static uint32_t fetch_operand(const uint32_t op)
{
    assert(op <= pc);

    return registers[op];
}

id_result_t * instruction_decode(const if_result_t * const in)
{
    if (in == NULL)
        return NULL;

    const instruction_type_t type = instruction_decode_type(in->inst);

    id_result_t * const res = calloc(1, sizeof(id_result_t));

    res->n_pc = in->n_pc;
    res->inst = in->inst;
    res->op1 = res->op2 = 0;

    switch (type) {
        case BINARY_ARITHMETIC:
        case COMPARE:
            {
                res->op1 = fetch_operand(instruction_decode_operand(1, res->inst));
                res->op2 = instruction_decode_operand(2, res->inst);
                res->op2 = (instruction_is_immediate_variant(res->inst)) ? res->op2 : fetch_operand(res->op2);
                break;
            }
        case UNARY_ARITHMETIC:
            {
                res->op1 = instruction_decode_operand(1, res->inst);
                res->op1 = (instruction_is_immediate_variant(res->inst)) ? res->op1 : fetch_operand(res->op1);
                break;
            }
        case BRANCH:
        case JUMP:
            {
                res->op1 = instruction_decode_operand(1, res->inst);
                if (instruction_is_immediate_variant(res->inst))
                    res->op1 += res->n_pc;
                else
                    res->op1 = fetch_operand(res->op1);
                break;
            }
        case IO:
            {
                const uint8_t opcode = instruction_decode_opcode(res->inst);
                if (opcode == OPCODE_LOAD) {
                    res->op1 = fetch_operand(instruction_decode_operand(1, res->inst));
                    res->op2 = instruction_decode_operand(2, res->inst);

                    res->io_op = instruction_decode_destination(res->inst);

                } else if (opcode == OPCODE_STORE) {
                    res->op1 = fetch_operand(instruction_decode_operand(1, res->inst));
                    res->op2 = instruction_decode_operand(2, res->inst);

                    res->io_op = fetch_operand(instruction_decode_destination(res->inst));
                } else {
                    assert(false && "Instruction not supported.");
                }
            }
        case MISC:
            break;
        case UNKNOWN:
        default:
            assert(false && "Instruction not supported.");
    }

    // The input is no longer needed, so free the memory
    free((void *)in);
    return res;
}
