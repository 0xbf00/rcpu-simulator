#include "Disassemble.h" // Instruction/Disassemble.h
#include "Decode.h" // Instruction/Decode.h
#include "Opcodes.h"

#ifdef __linux__
#define _XOPEN_SOURCE 700
#endif

#include <stdio.h>   // snprintf
#include <strings.h> // bzero
#include <string.h>  // strdup
#include <assert.h>  // assert

static const char *instruction_identifiers[2 << 6] = {
    [OPCODE_ADD]    = "ADD",
    [OPCODE_ADDI]   = "ADDI",
    [OPCODE_SUB]    = "SUB",
    [OPCODE_SUBI]   = "SUBI",
    [OPCODE_AND]    = "AND",
    [OPCODE_ANDI]   = "ANDI",
    [OPCODE_OR]     = "OR",
    [OPCODE_ORI]    = "ORI",
    [OPCODE_NOT]    = "NOT",
    [OPCODE_SHL]    = "SHL",
    [OPCODE_SHLI]   = "SHLI",
    [OPCODE_SHRA]   = "SHRA",
    [OPCODE_SHRAI]  = "SHRAI",
    [OPCODE_SHRL]   = "SHRL",
    [OPCODE_SHRLI]  = "SHRLI",
    [OPCODE_JMP]    = "JMP",
    [OPCODE_JMPR]   = "JMPR",
    [OPCODE_BRA]    = "BRA",
    [OPCODE_BRR]    = "BRR",
    [OPCODE_CEQ]    = "CEQ",
    [OPCODE_CEQI]   = "CEQI",
    [OPCODE_CLTU]   = "CLTU",
    [OPCODE_CLTUI]  = "CLTUI",
    [OPCODE_CLTS]   = "CLTS",
    [OPCODE_CLTSI]  = "CLTSI",
    [OPCODE_CGTU]   = "CGTU",
    [OPCODE_CGTUI]  = "CGTUI",
    [OPCODE_CGTS]   = "CGTS",
    [OPCODE_CGTSI]  = "CGTSI",
    [OPCODE_MOVE]   = "MOVE",
    [OPCODE_MOVI]   = "MOVI",
    [OPCODE_LOAD]   = "LOAD",
    [OPCODE_STORE]  = "STORE",
    [OPCODE_NOP]    = "NOP",
    [OPCODE_HALT]   = "HALT"
};

/**
    @brief Returns a textual representation of the specified instruction.

    @param inst The instruction to disassemble

    @return A null terminated C-string containing the disassembly.
    The caller is responsible to free the returned pointer.
*/
char *instruction_disassemble(const instruction_t inst)
{
    // output buffer, 30 bytes should suffice
    char out_buffer[30];
    bzero(out_buffer, sizeof(out_buffer));

    const instruction_type_t instruction_type = instruction_decode_type(inst);
    const char *instruction_identifier = instruction_identifiers[instruction_decode_opcode(inst)];

    assert(instruction_identifier != NULL);

    switch(instruction_type) {
        case BINARY_ARITHMETIC:
            {
                const uint32_t dest = instruction_decode_destination(inst);
                uint32_t op1, op2;
                op1 = instruction_decode_operand(1, inst);
                op2 = instruction_decode_operand(2, inst);

                if (instruction_is_immediate_variant(inst)) {
                    const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                             "%s\tr%02u, r%02u, %08d",
                                             instruction_identifier, dest, op1, (int32_t)op2);
                    assert(ret >= 0 && ret <= sizeof(out_buffer));
                } else {
                    const int ret = snprintf(out_buffer, sizeof(out_buffer), 
                                             "%s\tr%02u, r%02u, r%02u", 
                                             instruction_identifier, dest, op1, op2);
                    assert(ret >= 0 && ret <= sizeof(out_buffer));
                }

                break;
            }
        case UNARY_ARITHMETIC:
            {
                const uint32_t dest = instruction_decode_destination(inst);
                uint32_t op1 = instruction_decode_operand(1, inst);

                if (instruction_is_immediate_variant(inst)) {
                    const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                             "%s\tr%02u, %08d",
                                             instruction_identifier, dest, (int32_t)op1);
                    assert(ret >= 0 && ret <= sizeof(out_buffer));
                } else {
                    const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                             "%s\tr%02u, r%02u",
                                             instruction_identifier, dest, op1);
                    assert(ret >= 0 && ret <= sizeof(out_buffer));
                }

                break;
            }
        case COMPARE:
            {
                uint32_t op1, op2;
                op1 = instruction_decode_operand(1, inst);
                op2 = instruction_decode_operand(2, inst);

                if (instruction_is_immediate_variant(inst)) {
                    const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                             "%s\tr%02u, %08d",
                                             instruction_identifier, op1, (int32_t)op2);
                    assert(ret >= 0 && ret <= sizeof(out_buffer));
                } else {
                    const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                             "%s\tr%02u, r%02u",
                                             instruction_identifier, op1, op2);
                    assert(ret >= 0 && ret <= sizeof(out_buffer));
                }

                break;
            }
        case BRANCH:
            // fall-through
        case JUMP:
            {
                const uint32_t target = instruction_decode_operand(1, inst);
                if (instruction_is_immediate_variant(inst)) {
                    // PC-relative jump
                    const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                             "%s\t%+08d",
                                             instruction_identifier, (int32_t)target);
                    assert(ret >= 0 && ret < sizeof(out_buffer));
                } else {
                    // absolute jump
                    const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                             "%s\tr%02u",
                                             instruction_identifier, target);
                    assert(ret >= 0 && ret < sizeof(out_buffer));
                }

                break;
            }
        case IO:
            {
                const uint32_t dest = instruction_decode_destination(inst);
                uint32_t op1, op2;
                op1 = instruction_decode_operand(1, inst);
                op2 = instruction_decode_operand(2, inst);

                const int ret = snprintf(out_buffer, sizeof(out_buffer),
                                         "%s\tr%02u, r%02u, %08d",
                                         instruction_identifier, dest, op1, (int32_t)op2);
                assert(ret >= 0 && ret < sizeof(out_buffer));

                break;
            }
        case MISC:
            {
                const int ret = snprintf(out_buffer, sizeof(out_buffer), "%s", instruction_identifier);
                assert(ret >= 0 && ret < sizeof(out_buffer));

                break;
            }
        default:
            assert(false && "Unknown instruction");
    }

    return strndup(out_buffer, sizeof(out_buffer));
}
