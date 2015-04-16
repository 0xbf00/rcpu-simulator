#include "WriteBack.h"

#include <stdlib.h>

void write_back(const mem_result_t * const in)
{
    if (in == NULL)
        return;

    const uint8_t opcode = instruction_decode_opcode(in->inst);
    const uint8_t type = instruction_decode_type(in->inst);

    if (type == BINARY_ARITHMETIC || type == UNARY_ARITHMETIC)
    {
        // decode destination
        const uint32_t dest = instruction_decode_destination(in->inst);
        registers[dest] = in->result;
    } else if (opcode == OPCODE_LOAD) {
        registers[in->io_op] = in->result;
    }

    free((void *)in);
}
