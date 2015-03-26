/*!
    @header Memory Access (MEM)
    In this stage, load and store operations are actually carried out.
    Furthermore, control flow instructions change the value of the program
    counter. All other instructions do not affect this stage and are simply
    forwarded to the last stage    

    @language c
    @author Jakob Rieck
*/
#ifndef _MEMORY_ACCESS_H
#define _MEMORY_ACCESS_H

#include "Pipeline.h"

/*! 
    @abstract
        Type for result of Memory Access stage.
*/
typedef struct mem_result {
    uint32_t n_pc;
    uint32_t inst;

    uint32_t result;

    // Destination where to save 'result'
    // iff inst == LOAD
    uint32_t io_op;
} mem_result_t;

// Forward declarations
struct ex_result;
typedef struct ex_result ex_result_t;

/*!
    @abstract
        Print memory protocol
    @discussion
        This function can be used to print a precise
        description of memory stores. Memory loads are
        not logged.
        This is useful both for debugging and for
        reading out values after calculation, as there
        is no console or something comparable.
*/
void dump_memory_protocol();

/*!
    @abstract Execute forth stage of pipeline

    @param in
        The output of the third stage: Execute (EX)
        Can also be NULL, in which case the output will also be NULL.

    @return
        Returns a pointer to a structure containing the results of this
        stage. Iff the input is NULL, no calculation is performed and NULL is returned.
        Otherwise, the returned pointer has to be released by the caller.
*/
mem_result_t * memory_access(const ex_result_t * const in);

#endif