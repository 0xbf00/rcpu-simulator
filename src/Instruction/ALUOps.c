#include "ALUOps.h"
#include "Opcodes.h"

/**
    @brief Lookup table for binary functions. All elements
    that are not specified are NULL.
*/
const binary_arithmetic_func_t binary_functions[2 << 6] = {
    [OPCODE_ADD]    = &add,
    [OPCODE_ADDI]   = &add,
    [OPCODE_SUB]    = &sub,
    [OPCODE_SUBI]   = &sub,
    [OPCODE_AND]    = &and,
    [OPCODE_ANDI]   = &and,
    [OPCODE_OR]     = &or,
    [OPCODE_ORI]    = &or,
    [OPCODE_SHL]    = &shl,
    [OPCODE_SHLI]   = &shl,
    [OPCODE_SHRA]   = &shra,
    [OPCODE_SHRAI]  = &shra,
    [OPCODE_SHRL]   = &shrl,
    [OPCODE_SHRLI]  = &shrl,
};

/**
    @brief Lookup table for unary functions. All elements
    that are not explicitly specified are NULL.
*/
const unary_arithmetic_func_t unary_functions[2 << 6] = {
    [OPCODE_NOT]    = &not,
    [OPCODE_MOVE]   = &mov,
    [OPCODE_MOVI]   = &mov,
};

/**
    @brief Lookup table for compare functions.

    @see unary_functions, binary_functions
*/
const compare_func_t compare_functions[2 << 6] = {
    [OPCODE_CEQ]    = &ceq,
    [OPCODE_CEQI]   = &ceq,
    [OPCODE_CLTU]   = &cltu,
    [OPCODE_CLTUI]  = &cltu,
    [OPCODE_CLTS]   = &clts,
    [OPCODE_CLTSI]  = &clts,
    [OPCODE_CGTU]   = &cgtu,
    [OPCODE_CGTUI]  = &cgtu,
    [OPCODE_CGTS]   = &cgts,
    [OPCODE_CGTSI]  = &cgts
};

/**
    @brief Definition for both ADD and ADDI,
    different behaviour in main loop

    @param a The first parameter. On an application level,
    this are the contents of a register.
    @param b The second parameter. Can be either a register
    or an immediate value.

    @return unsigned addition: a + b
*/
uint32_t add(const uint32_t a, const uint32_t b)
{
    return a + b;
}

/*
    definition of both SUB and SUBI,
    different behaviour in main loop

    SUB R1, R2, R3
=>  r1 = sub(r2, r3);

    SUBI R1, R2, Imm
=>  r1 = sub(R2, Imm);
*/
uint32_t sub(const uint32_t a, const uint32_t b)
{
    return a - b;
}

/*
    definition of both AND and ANDI,
    different behaviour in main loop

    AND R1, R2, R3
=>  r1 = and(r2, r3);

    ANDI R1, R2, Imm
=>  r1 = and(R2, Imm);
*/
uint32_t and(uint32_t a, uint32_t b)
{
    return a & b;
}

/*
    definition of both OR and ORI,
    different behaviour in main loop

    OR R1, R2, R3
=>  r1 = or(r2, r3);

    ORI R1, R2, Imm
=>  r1 = or(R2, Imm);
*/
uint32_t or(uint32_t a, uint32_t b)
{
    return a | b;
}

/*
    definition of NOT
    
    NOT R1, R2
=>  r1 = not(r2);
*/
uint32_t not(uint32_t a)
{
    return ~a;
}

/*
    definition of both SHL and SHLI,
    different behaviour in main loop

    SHL R1, R2, R3
=>  r1 = shl(r2, r3);

    SHLI R1, R2, Imm
=>  r1 = shl(R2, Imm);
*/
uint32_t shl(uint32_t a, uint32_t b)
{
    if (!b)
        return a << 1;
    else
        return a << 8;
}

/*
    definition of both SHRA and SHRAI,
    different behaviour in main loop

    SHRA R1, R2, R3
=>  r1 = shra(r2, r3);

    SHRAI R1, R2, Imm
=>  r1 = shra(R2, Imm);
*/
uint32_t shra(uint32_t a, uint32_t b)
{
    if (!b) { // shift right by one bit
        return (a >> 1) | (a & (1 << 31));
    } else { // shift right by 8 bit
        const uint32_t highestBit = a & (1 << 31);
        const uint32_t bitMask = ~((highestBit >> 31) - 1);

        return (a >> 8) | (bitMask << (31-8));
    }
}

/*
    definition of both SHRL and SHRLI,
    different behaviour in main loop

    SHRL R1, R2, R3
=>  r1 = shrl(r2, r3);

    SHRLI R1, R2, Imm
=>  r1 = shrl(R2, Imm);
*/
uint32_t shrl(uint32_t a, uint32_t b)
{
    if (!b) { // shift right by one bit
        return a >> 1;
    } else { // shift right by 8 bit
        return a >> 8;
    }
}

/*
    definition of both CEQ and CEQI,
    different behaviour in main loop

    CEQ R1, R2
=>  flag = ceq(r1, r2)

    CEQI R1, Imm
=>  flag = ceq(R2, Imm)
*/
bool ceq(uint32_t a, uint32_t b)
{
    return a == b;
}

/*
    definition of both CLTU and CLTUI,
    different behaviour in main loop

    CLTU R1, R2
=>  flag = cltu(r1, r2)

    CLTUI R1, Imm
=>  flag = cltui(R2, Imm)
*/
bool cltu(uint32_t a, uint32_t b)
{
    return a < b;
}

/*
    definition of both CLTS and CLTSI,
    different behaviour in main loop

    CLTS R1, R2
=>  flag = clts(r1, r2)

    CLTSI R1, Imm
=>  flag = cltsi(R2, Imm)
*/
bool clts(uint32_t a, uint32_t b)
{
    return (int32_t)a < (int32_t)b;
}

/*
    definition of both CGTU and CGTUI,
    different behaviour in main loop

    CGTU R1, R2
=>  flag = cgtu(r1, r2)

    CGTUI R1, Imm
=>  flag = cgtui(R2, Imm)
*/
bool cgtu(uint32_t a, uint32_t b)
{
    return a > b;
}

/*
    definition of both CGTS and CGTSI,
    different behaviour in main loop

    CGTS R1, R2
=>  flag = cgts(r1, r2)

    CGTSI R1, Imm
=>  flag = cgtsi(R2, Imm)
*/
bool cgts(uint32_t a, uint32_t b)
{
    return (int32_t)a > (int32_t)b;
}

/*
    definition of both MOVE and MOVI,
    different behaviour in main loop

    In this implementation, mov is effectively id.

    MOVE R1, R2
=>  R1 = mov(R2);

    MOVI R1, Imm
=>  R1 = mov(Imm);
*/
uint32_t mov(uint32_t a)
{
    return a;
}