#include "Decode.h"
#include "Opcodes.h"

#include <assert.h>

// lookup table from opcode to type
// every undefined instruction has type UNKNOWN (= 0)
static const instruction_type_t instruction_types[2 << 6] = {
    [OPCODE_ADD]    = BINARY_ARITHMETIC,
    [OPCODE_ADDI]   = BINARY_ARITHMETIC,
    [OPCODE_SUB]    = BINARY_ARITHMETIC,
    [OPCODE_SUBI]   = BINARY_ARITHMETIC,
    [OPCODE_AND]    = BINARY_ARITHMETIC,
    [OPCODE_ANDI]   = BINARY_ARITHMETIC,
    [OPCODE_OR]     = BINARY_ARITHMETIC,
    [OPCODE_ORI]    = BINARY_ARITHMETIC,
    [OPCODE_NOT]    = UNARY_ARITHMETIC,
    [OPCODE_SHL]    = BINARY_ARITHMETIC,
    [OPCODE_SHLI]   = BINARY_ARITHMETIC,
    [OPCODE_SHRA]   = BINARY_ARITHMETIC,
    [OPCODE_SHRAI]  = BINARY_ARITHMETIC,
    [OPCODE_SHRL]   = BINARY_ARITHMETIC,
    [OPCODE_SHRLI]  = BINARY_ARITHMETIC,
    [OPCODE_JMP]    = JUMP,
    [OPCODE_JMPR]   = JUMP,
    [OPCODE_BRA]    = BRANCH,
    [OPCODE_BRR]    = BRANCH,
    [OPCODE_CEQ]    = COMPARE,
    [OPCODE_CEQI]   = COMPARE,
    [OPCODE_CLTU]   = COMPARE,
    [OPCODE_CLTUI]  = COMPARE,
    [OPCODE_CLTS]   = COMPARE,
    [OPCODE_CLTSI]  = COMPARE,
    [OPCODE_CGTU]   = COMPARE,
    [OPCODE_CGTUI]  = COMPARE,
    [OPCODE_CGTS]   = COMPARE,
    [OPCODE_CGTSI]  = COMPARE,
    [OPCODE_MOVE]   = UNARY_ARITHMETIC,
    [OPCODE_MOVI]   = UNARY_ARITHMETIC,
    [OPCODE_LOAD]   = IO,
    [OPCODE_STORE]  = IO,
    [OPCODE_NOP]    = MISC,
    [OPCODE_HALT]   = MISC
};

/**
    @brief Decodes the type of an instruction.

    @param inst The instruction for which to extract the type. This
    instruction has to be valid.

    @return The type of the instruction, if found (or UNKNOWN otherwise)
*/
instruction_type_t instruction_decode_type(const instruction_t inst)
{
    uint32_t opcode = inst & 0b00111111;
    instruction_type_t ret = instruction_types[opcode];

    return ret;
}

/**
    @brief Decode the opcode for an instruction

    @param inst The instruction from which to extract the opcode.
    This function returns garbage, iff the instruction is invalid!

    @return The opcode (6-bit) as an unsigned 8-Bit integer. The top 2 bits
    are guaranteed to be 0 iff the instruction is valid.
*/
uint8_t instruction_decode_opcode(const instruction_t inst) {
    return (uint8_t) (inst & 0x0000003f);
}

/*!
    @abstract
        Check whether the supplied instruction takes an
        immediate value for its second parameter.

    @param inst 
        The instruction to analyze

    @return 
        true, iff the second operand is supposed to be
        an immediate value.
*/
bool instruction_is_immediate_variant(const instruction_t inst)
{
    return ((bool)(inst & 0x1) && (inst != OPCODE_STORE));
}

/*!
    @abstract Decode the register in which to place the result
    of the instruction

    @param inst 
        The instruction from which to extract the
        destination register

    @return 
        The register index of the destination

    @warning 
        It is a runtime error to call this function
        with an instruction that has no register destination,
        like a JUMP instruction.
*/
uint32_t instruction_decode_destination(const instruction_t inst)
{
    const uint32_t type = instruction_decode_type(inst);
    assert((type == BINARY_ARITHMETIC) || (type == UNARY_ARITHMETIC) || (type == IO));

    uint32_t dest = (inst & 0x000007c0) >> 6;

    // destination should always be a register
    // (in the case of str, destination is the register to store)
    assert(dest <= 31);

    return dest;
}

/*
    sign_extent returns the first argument with its top (32 - nbits)
    bits set to 1 iff the current top bit is 1. Otherwise, the
    input is not changed.
*/
static uint32_t sign_extent(const uint32_t a, const uint32_t nbits)
{
    // bit pattern of all 1s iff the highest bit is set. Otherwise, this bit pattern is all 0s.
    const uint32_t mask = 0 - ((a & (1 << (nbits - 1))) >> (nbits - 1));

    // set all top bits to the highest bit
    return a | (mask << (nbits - 1));
}

/*!
    @abstract Decode the `operandn`-th operand for the specified instruction `inst`.

    @param operandn 
        The operand to decode. Operands are addressed from 1.
    @param inst
        The corresponding instruction

    @return
        The decoded operand. If the operand is an immediate value, this value has
        already been sign-extended. A register operand is not fetched.

    @warning It is a runtime error to call this function with an instruction requesting
    the i-th argument if the instruction does not have at least i arguments!
*/
uint32_t instruction_decode_operand(const uint32_t operandn, const instruction_t inst)
{
    const instruction_type_t type = instruction_decode_type(inst);

    switch(type) {
        case BINARY_ARITHMETIC:
            assert((operandn == 1) || (operandn == 2));

            // in the case of binary arithmetic instructions,
            // the first operand is always a register!
            if (operandn == 1) {
                return (0x0000f800 & inst) >> 11;
            } else {
                // and the second operand can be a register or an immediate value.
                if (instruction_is_immediate_variant(inst)) {
                    return sign_extent((0xffff0000 & inst) >> 16, 32 - 16);
                } else {
                    return (0x001f0000 & inst) >> 16;
                }
            }
            break;
        case UNARY_ARITHMETIC:
            assert(operandn == 1);

            if (instruction_is_immediate_variant(inst)) {
                // special case for MOVI, NOTI(?)
                return sign_extent((0xfffff800 & inst) >> 11, 21);
            } else {
                return (0x0000f800 & inst) >> 11;
            }
            break;
        case COMPARE:
            assert((operandn == 1) || (operandn == 2));

            // The first operand is always a register.
            if (operandn == 1) {
                return (inst & 0x000007c0) >> 6;
            } else {
                // The second operand can be either a register or an immediate value
                if (instruction_is_immediate_variant(inst)) {
                    return sign_extent((0xfffff800 & inst) >> 11, 32 - 11);
                } else {
                    return (0x0000f800 & inst) >> 11;
                }
            }
            break;
        case BRANCH:
            // fallthrough -> JUMP handling
        case JUMP:
            if (instruction_is_immediate_variant(inst)) {
                // Immediate value (offset)
                return sign_extent((0xffffffc0 & inst) >> 6, 32 - 6);
            } else {
                // register
                return (0x000007c0 & inst) >> 6;
            }
            break;
        case IO:
            assert((operandn == 1) || (operandn == 2));

            // The first operand is always a register.
            if (operandn == 1) {
                return (0x0000f800 & inst) >> 11;
            } else {
                return sign_extent((0xffff0000 & inst) >> 16, 32 - 16);
            }
        case MISC:
        case UNKNOWN:
        default:
            assert(false && "Instruction not supported or invalid arguments.");
    }
}
