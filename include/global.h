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

/* Number of elements of an array object (not of a pointer). */
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

/* Byte offset of a struct member as a constant expression (agbcc has no <stddef.h> offsetof). */
#define OFFSET_OF(type, member) ((u32)&((type *)0)->member)

/* File-scope compile-time check: a false condition gives a negative array size, which does not compile.
 * Usage: STATIC_ASSERT(sizeof(struct Main) == 0x488C, MainSize);  (name: unique per translation unit) */
#define STATIC_ASSERT(cond, name) typedef char static_assert_##name[(cond) ? 1 : -1]

/* Pull a not-yet-decompiled function's assembly into a C translation unit, in place.
 * Usage (file scope):  INCLUDE_ASM("asm/nonmatching/bustup_scene", GetDuelistName);  */
#ifdef OBJDIFF_BASE
/* objdiff "base" build (make objdiff-report): leave not-yet-decompiled functions out entirely,
 * so they count as unmatched instead of trivially matching their own assembly. */
#define INCLUDE_ASM(DIR, NAME) extern int __objdiff_skipped_asm
#else
#define INCLUDE_ASM(DIR, NAME) asm(".include \"" DIR "/" #NAME ".s\"")
#endif

asm(".include \"asm/macros.inc\"");

#endif /* GUARD_GLOBAL_H */
