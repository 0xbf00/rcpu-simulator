#include "InstructionFetch.h"

#include <stdlib.h>

if_result_t * instruction_fetch()
{
    if_result_t * const res = calloc(1, sizeof(if_result_t));
    res->n_pc = registers[pc] + 1;
    res->inst = memory.code[registers[pc]];

    if (instruction_decode_opcode(res->inst) != OPCODE_HALT)
        registers[pc]++;
    else {
        free(res);
        // Return NULL on failure or if end was found
        return NULL;
    }
    return res;
}
