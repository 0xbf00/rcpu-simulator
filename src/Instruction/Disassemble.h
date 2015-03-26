/*!
    @header Disassemble instructions
    The function provided here can be used to retrieve a human-readable
    representation of an instruction.
    
    @related Decode.h

    @language c
    @author Jakob Rieck
*/
#ifndef INSTRUCTION__DISASSEMBLE_H
#define INSTRUCTION__DISASSEMBLE_H

#include "Decode.h"

/*!
    @abstract
        Returns a textual representation of the specified instruction.

    @param inst
        The instruction to disassemble

    @return
        A null terminated C-string containing the disassembly.
        The caller is responsible to free the returned pointer.

    @warning
        This functions terminates program execution iff the
        resulting string would be longer than 30 characters, which should
        not happen under normal circumstances.
*/
char *instruction_disassemble(const instruction_t inst);

#endif /* INSTRUCTION__DISASSEMBLE_H */
