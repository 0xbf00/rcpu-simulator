/*!
    @header ALU operations
    This header contains functions comprising the majority of
    our processor's capabilities.
    Control flow and memory access is managed by the respective stage
    in the pipeline.

    @related ALUOps.h

    @language c
    @author Jakob Rieck
*/
#ifndef INSTRUCTION__ALUOPS_H
#define INSTRUCTION__ALUOPS_H

#include <stdint.h>
#include <stdbool.h>

/*  
    ALU operations are provided as function.
    Care has been taken to make sure all functions
    in here are reentrant.
*/

/*!
    @abstract 
        type of a binary function
        :: (uint32_t, uint32_t) -> uint32_t
*/
typedef uint32_t (*binary_arithmetic_func_t)(uint32_t, uint32_t);

/*! 
    @abstract
        type of a unary function
        :: uint32_t -> uint32_t
*/
typedef uint32_t (*unary_arithmetic_func_t)(uint32_t);

/*! 
    @abstract 
        type of a comparison function
        :: (uint32_t, uint32_t) -> bool
*/
typedef bool (*compare_func_t)(uint32_t, uint32_t);

/********************************************************************
    lookup tables
    Each has length 2^6, to allow for indexing with the opcode.
 *******************************************************************/

/*! 
    @abstract
        table for all binary functions of the ALU
*/
extern const binary_arithmetic_func_t binary_functions[2 << 6];

/*!
    @abstract 
        table for all unary functions of the ALU
*/
extern const unary_arithmetic_func_t unary_functions[2 << 6];

/*! 
    @abstract
        table for all comparison functions of the ALU
*/
extern const compare_func_t compare_functions[2 << 6];

/*!
    @abstract 
        Adds two unsigned 32-bit integers.

    @param a
        first summand
    @param b
        second summand

    @return
        (a + b) mod 2^32
*/
uint32_t add(const uint32_t a, const uint32_t b);

/*!
    @abstract 
        Subtracts one unsigned 32-bit integer from
        another.

    @param a 
        The minuend
    @param b
        The subtrahend

    @return
        (a - b) mod 2^32
*/
uint32_t sub(const uint32_t a, const uint32_t b);

/*!
    @abstract 
        Compute the bitwise AND of two unsigned 32-bit
        integers.

    @param a 
        First parameter
    @param b 
        Second parameter

    @return 
        For each bit: 1, iff both bits are 1
        in the original value, otherwise 0.
*/ 
uint32_t and(const uint32_t a, const uint32_t b);

/*!
    @abstract 
        Compute the bitwise OR of two unsigned 32-bit
        integers.

    @param a 
        First parameter
    @param b 
        Second parameter

    @return 
        For each bit: 1, iff at least one of the
        corresponding initial bits was 1, otherwise 0.
*/
uint32_t or(const uint32_t a, const uint32_t b);

/*!
    @abstract 
        Compute the bitwise NOT of the argument

    @param a 
        First parameter

    @return 
        For each bit: 1, iff the original bit was 0,
        otherwise 0.
*/
uint32_t not(const uint32_t a);

/*!
    @abstract
        Shift Left
    @discussion
        Due to the limited complexity of our hardware
        implementation, only shifts by 1 bit or by 8 bit
        are supported.
    @param a
        The value to be shifted
    @param b
        Can be either 0 or non-0.

    @return 
        Yields the first parameter shifted left by 1 bit if the second parameter was zero,
        otherwise yields the first parameter shifted left by 8 bit.
*/
uint32_t shl(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Arithmetic Shift Right
    @discussion
        shl's limitations also apply here

    @param a
        The value to be shifted
    @param b
        Can be either 0 or non-0, corresponding to
        a shift by 1 bit or 8 bits.

    @return
        The returned value is the input value shifted right by n bits,
        according to the second parameter. The top bits of the return
        value are set according to the top bit of the first parameter.
*/
uint32_t shra(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Logical Shift Right
    @discussion
        The same limitations that applied to all previous shift operations
        also apply in this case.

    @param a
        The value to be shifted
    @param b
        Can be either 0 or non-0, corresponding to
        a shift right by 1 bit or 8 bits.

    @return
        The returned value is the input value shifted right n bits,
        according to the second parameter. The top n bits are set to
        0.
*/
uint32_t shrl(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Compare equality
    
    @param a
        The first parameter
    @param b
        The second parameter

    @return
        true iff both parameters are equal, otherwise false
*/
bool ceq(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Compare less than unsigned
    
    @param a
        The first parameter
    @param b
        The second parameter

    @return
        This function treats it two arguments as
        unsigned integers and compares them, returning
        true only if the first parameter is less than the
        second paramter.
*/
bool cltu(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Compare less than signed
    
    @param a
        The first parameter
    @param b
        The second parameter

    @return
        This function treats it two arguments as
        signed integers and compares them, returning
        true only if the first parameter is less than
        the second parameter.
*/
bool clts(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Compare greater than unsigned
    
    @param a
        The first parameter
    @param b
        The second parameter

    @return
        This function treats it two arguments as
        unsigned integers and compares them, returning
        true only if the first parameter is greater than
        the second parameter.
*/
bool cgtu(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Compare greater than signed
    
    @param a
        The first parameter
    @param b
        The second parameter

    @return
        This function treats it two arguments as
        signed integers and compares them, returning
        true only if the first parameter is greater than
        the second parameter.
*/
bool cgts(const uint32_t a, const uint32_t b);

/*!
    @abstract
        Move
    @discussion
        The move instruction moves the contents of one register
        into another register. This function is provided to
        model the fact that move is part of the ALU.
        Our implementation of this function is just the identity function.

    @param a
        Our only parameter
    @return
        This function returns its own parameter unchanged.
*/
uint32_t mov(const uint32_t a);

#endif /* INSTRUCTION__ALUOPS_H */
