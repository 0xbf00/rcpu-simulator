#include "MemoryAccess.h"
#include "../Misc/LinkedList.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static linked_list_t * memory_protocol = NULL;

static void print_element(const void * element)
{
    uint32_t addr = (uint32_t)element;
    printf("[0x%x]: 0x%x\n", addr, memory.data[addr]);
}

void dump_memory_protocol()
{
    ll_map(memory_protocol, &print_element, 1);
    ll_free(memory_protocol, 0);
}

mem_result_t * memory_access(const ex_result_t * const in)
{
    if (in == NULL)
        return NULL;

    mem_result_t * const res = calloc(1, sizeof(mem_result_t));

    res->n_pc = in->n_pc;
    res->inst = in->inst;
    res->result = in->result;
    res->io_op = in->io_op;

    const uint8_t type = instruction_decode_type(res->inst);

    switch(type) {
        case IO:
            {
                const uint32_t opcode = instruction_decode_opcode(res->inst);
                if (opcode == OPCODE_LOAD) {
                    res->result = memory.data[in->result];
                } else if (opcode == OPCODE_STORE) {
                    memory.data[in->result] = in->io_op;

                    // Search for duplicates would be nice
                    memory_protocol = ll_prepend_element(memory_protocol, (void *)((uint64_t)in->result));
                }
                break;
            }
        case BINARY_ARITHMETIC:
        case UNARY_ARITHMETIC:
        case COMPARE:
        case MISC:
                break;
        case BRANCH:
            {
                if (!in->branch_taken)
                    break;
                else
                    ; // fall-through into jump section
            }
        case JUMP:
            {
                res->n_pc = in->result;
                registers[pc] = res->n_pc;

                break;
            }
        case UNKNOWN:
        default:
            assert(false && "Instruction not supported.");
    }

    free((void *)in);
    return res;
}
