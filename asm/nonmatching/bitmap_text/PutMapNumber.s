	thumb_func_start PutMapNumber
PutMapNumber: @ 0x08079700
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	str r3, [sp, #0xC]
	ldr r3, [sp, #0x38]
	ldr r4, [sp, #0x3C]
	ldr r5, [sp, #0x40]
	ldr r6, [sp, #0x44]
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r9, r1
	lsl r2, r2, #0x18
	lsr r0, r2, #0x18
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	mov r2, r8
	lsl r4, r4, #0x10
	lsr r3, r4, #0x10
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	mov sl, r5
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	cmp r0, #0
	beq _08079744
	cmp r0, #1
	beq _080797A2
	b _08079824
_08079744:
	mov r5, #0
	cmp r5, r9
	bcs _08079824
	add r0, r3, #0
	mov r1, #0x1F
	and r0, r1
	lsl r0, r0, #0x10
	str r0, [sp, #0x10]
_08079754:
	add r0, r7, #0
	mov r1, #0xA
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r7, #0
	mov r1, #0xA
	bl __udivsi3
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	add r1, r4, #0
	add r1, #0x30
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r2, r8
	sub r0, r2, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	mov r4, #0x1F
	and r2, r4
	mov r0, sl
	str r0, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #0x48]
	str r4, [sp, #8]
	ldr r0, [sp, #0xC]
	ldr r4, [sp, #0x10]
	lsr r3, r4, #0x10
	bl PutMapChar
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, r9
	bcc _08079754
	b _08079824
_080797A2:
	cmp r7, #0
	bne _080797C0
	mov r0, #0x1F
	and r2, r0
	and r3, r0
	mov r7, sl
	str r7, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #0x48]
	str r0, [sp, #8]
	ldr r0, [sp, #0xC]
	mov r1, #0x30
	bl PutMapChar
	b _08079824
_080797C0:
	mov r5, #0
	cmp r5, r9
	bcs _08079824
	add r0, r3, #0
	mov r1, #0x1F
	and r0, r1
	lsl r0, r0, #0x10
	str r0, [sp, #0x14]
_080797D0:
	add r0, r7, #0
	mov r1, #0xA
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r7, #0
	mov r1, #0xA
	bl __udivsi3
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r4, #0
	bne _080797F0
	cmp r7, #0
	beq _08079824
_080797F0:
	add r1, r4, #0
	add r1, #0x30
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r2, r8
	sub r0, r2, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	mov r4, #0x1F
	and r2, r4
	mov r0, sl
	str r0, [sp, #0]
	str r6, [sp, #4]
	ldr r4, [sp, #0x48]
	str r4, [sp, #8]
	ldr r0, [sp, #0xC]
	ldr r4, [sp, #0x14]
	lsr r3, r4, #0x10
	bl PutMapChar
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, r9
	bcc _080797D0
_08079824:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end PutMapNumber

