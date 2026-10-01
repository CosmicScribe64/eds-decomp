#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef u8 bool8;
typedef u32 bool32;

#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)

/* Pull a not-yet-decompiled function's assembly into a C translation unit, in place.
 * Usage (file scope):  INCLUDE_ASM("asm/nonmatching/code_08000228", sub_08000228);  */
#ifdef OBJDIFF_BASE
/* objdiff "base" build (make objdiff-report): leave not-yet-decompiled functions out entirely,
 * so they count as unmatched instead of trivially matching their own assembly. */
#define INCLUDE_ASM(DIR, NAME) extern int __objdiff_skipped_asm
#else
#define INCLUDE_ASM(DIR, NAME) asm(".include \"" DIR "/" #NAME ".s\"")
#endif

asm(".include \"asm/macros.inc\"");

#endif /* GUARD_GLOBAL_H */
