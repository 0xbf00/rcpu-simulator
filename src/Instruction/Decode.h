/*!
    @header Decoding instructions
    Functions provided here allow decoding of the instruction type, decoding
    of operands- and destination registers.
    
    @related Disassemble.h

    @language c
    @author Jakob Rieck
*/
#ifndef INSTRUCTION__DECODE_H
#define INSTRUCTION__DECODE_H

#include <stdint.h>
#include <stdbool.h>

/*!
    @abstract
        The PC is the last register in the register bank.
*/
#define pc 31

/*! 
    @abstract
        An instruction is just a 32-bit unsigned integer.
*/
typedef uint32_t instruction_t;

/*!
    @abstract 
        Enumeration representing all possible instruction
        types. An unknown instruction has type 'UNKNOWN'.
*/
typedef enum {
    UNKNOWN = 0,
    BINARY_ARITHMETIC, // add, sub, ...
    UNARY_ARITHMETIC,  // not, (mov)
    COMPARE,           // ceq, cltu
    JUMP,              // jmp, jmpr
    BRANCH,            // bra, brr
    IO,                // ldr, str
    MISC               // nop, halt
} instruction_type_t;

/********************************************************************
    functions that allow decoding certain parts of the instruction
    words.
 *******************************************************************/

/*!
    @abstract 
        Decodes the type of an instruction.

    @param inst 
        The instruction for which to extract the type. This
        instruction has to be valid.

    @return 
        The type of the instruction, if found (or UNKNOWN otherwise)
*/
instruction_type_t instruction_decode_type(const instruction_t inst);

/*!
    @abstract 
        Decode the opcode for an instruction

    @param inst 
        The instruction from which to extract the opcode.
        This function returns garbage, iff the instruction is invalid!

    @return 
        The opcode (6-bit) as an unsigned 8-Bit integer. The top 2 bits
        are guaranteed to be 0 iff the instruction is valid.
*/
uint8_t instruction_decode_opcode(const instruction_t inst);

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
bool instruction_is_immediate_variant(const instruction_t inst);

/*!
    @abstract 
        Decode the register in which to place the result
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
uint32_t instruction_decode_destination(const instruction_t inst);

/*!
    @abstract 
        Decode the n-th operand for the specified instruction.

    @param operandn 
        The operand to decode. Operands are addressed from 1.
    @param inst
        The corresponding instruction

    @return 
        The decoded operand. If the operand is an immediate value, this value has
        already been sign-extended. A register operand is not fetched.

    @warning 
        It is a runtime error to call this function with an instruction requesting
        the i-th argument if the instruction does not have at least i arguments!
*/
uint32_t instruction_decode_operand(const uint32_t operandn, const instruction_t inst);

#endif /* INSTRUCTION__DECODE_H */
