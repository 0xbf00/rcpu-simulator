/*!
    @header Instruction opcodes
    This header contains makro definitions for
    all instructions of our toy CPU.

    @language c
    @author Jakob Rieck
*/
#ifndef INSTRUCTION__OPCODES_H
#define INSTRUCTION__OPCODES_H

#pragma mark ALU operations

#define OPCODE_ADD      0b100000
#define OPCODE_ADDI     (OPCODE_ADD  | 1)
#define OPCODE_SUB      0b100010
#define OPCODE_SUBI     (OPCODE_SUB  | 1)
#define OPCODE_AND      0b100100
#define OPCODE_ANDI     (OPCODE_AND  | 1)
#define OPCODE_OR       0b100110
#define OPCODE_ORI      (OPCODE_OR   | 1)
#define OPCODE_NOT      0b101000
#define OPCODE_SHL      0b101010
#define OPCODE_SHLI     (OPCODE_SHL  | 1)
#define OPCODE_SHRA     0b101100
#define OPCODE_SHRAI    (OPCODE_SHRA | 1)
#define OPCODE_SHRL     0b101110
#define OPCODE_SHRLI    (OPCODE_SHRL | 1)
#define OPCODE_MOVE     0b001110
#define OPCODE_MOVI     (OPCODE_MOVE | 1)

#pragma mark Control flow instructions

#define OPCODE_JMP      0b000000
#define OPCODE_JMPR     (OPCODE_JMP  | 1)
#define OPCODE_BRA      0b000010
#define OPCODE_BRR      (OPCODE_BRA  | 1)

#pragma mark Compare instructions

#define OPCODE_CEQ      0b000100
#define OPCODE_CEQI     (OPCODE_CEQ  | 1)
#define OPCODE_CLTU     0b000110
#define OPCODE_CLTUI    (OPCODE_CLTU | 1)
#define OPCODE_CLTS     0b001000
#define OPCODE_CLTSI    (OPCODE_CLTS | 1)
#define OPCODE_CGTU     0b001010
#define OPCODE_CGTUI    (OPCODE_CGTU | 1)
#define OPCODE_CGTS     0b001100
#define OPCODE_CGTSI    (OPCODE_CGTS | 1)

#pragma mark IO instructions

#define OPCODE_LOAD     0b010000
#define OPCODE_STORE    0b010001

#pragma mark MISC instructions

#define OPCODE_NOP      0b010010
#define OPCODE_HALT     0b010011

#endif // INSTRUCTION__OPCODES_H
