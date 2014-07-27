/************************************************************/
/* Linker command file for simulator only                   */
/************************************************************/

/* Contains only the bare necessities for running a unit test
 * in the TI CCS 3.3 Simulator 
 */

MEMORY
{
PAGE 0 : /* Program Memory */

    /* Flash as one big segment */
    FLASH    : origin = 0x300000, length = 0x040000

PAGE 1 : /* Data Memory */

    /* Internal SARAM */
    RAM_0    : origin = 0x000000, length = 0x002000  
    /* 0x2000 to 0x3FFF: Not defined */
    STACK    : origin = 0x004000, length = 0x004000 /* stack needs own segment in low RAM */
    RAM_1    : origin = 0x008000, length = 0x008000 

    /* External memory */
    EMEM     : origin = 0x100000, length = 0x030000
}

SECTIONS
{   
    /* segment allocation */
    .cinit   : >  FLASH, PAGE = 0 /* initialized internal RAM-data */
    .pinit   : >  FLASH, PAGE = 0 /* initialized constructors */
    .stack   : >  STACK, PAGE = 1
    .text    : >  FLASH, PAGE = 0
    .bss     : >  RAM_0, PAGE = 1
    .ebss    : >  RAM_1, FILL = 0x0020, PAGE = 1 /* uninitialized external RAM-data */
    .econst  : >  FLASH, PAGE = 0
    .esysmem : >  EMEM,  PAGE = 1 /* not used if no malloc calls in code */
    .cio     : >  RAM_0, PAGE = 1  
 
    /* keep the linker from complaining */
    .reset   : >  FLASH, PAGE = 0, TYPE = DSECT /* not used */
}
