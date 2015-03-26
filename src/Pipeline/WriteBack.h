/*!
    @header Write Back (WB)
    In the Write Back stage, the register bank is updated
    with new values that were calculated in an earlier stage
    or loaded from memory

    @language c
    @author Jakob Rieck
*/
#ifndef _WRITE_BACK_H
#define _WRITE_BACK_H

#include "Pipeline.h"

struct mem_result;
typedef struct mem_result mem_result_t;

/*!
    @abstract
        Execute the final stage of the pipeline.
    @discussion
        The final stage of the pipeline consists of
        storing values into registers. These values were
        either calculated in the Execute phase, or were loaded
        from memory in the Memory Access phase.

    @param in
        The output of the forth stage: Memory Access or
        NULL if there is no such output. After processing,
        the memory for this object is freed.
*/
void write_back(const mem_result_t * const in);

#endif // _WRITE_BACK_H