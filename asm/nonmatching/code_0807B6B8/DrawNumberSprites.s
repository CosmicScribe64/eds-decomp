	thumb_func_start DrawNumberSprites
DrawNumberSprites: @ 0x0807B864
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x34
	ldr r4, [sp, #0x54]
	ldr r5, [sp, #0x60]
	ldr r6, [sp, #0x64]
	ldr r7, [sp, #0x68]
	mov r8, r7
	ldr r7, [sp, #0x6C]
	mov r9, r7
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov sl, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0x20]
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	str r4, [sp, #0x24]
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	str r5, [sp, #0x28]
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	str r6, [sp, #0x2C]
	mov r0, r8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x30]
	mov r1, r9
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	mov r6, #0
	cmp r2, #0
	beq _0807B8C0
	cmp r2, #1
	beq _0807B928
	b _0807B9C4
_0807B8C0:
	mov r5, #0
	cmp r5, sl
	bcs _0807B9C4
_0807B8C6:
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
	lsl r0, r4, #3
	ldr r1, [sp, #0x58]
	add r0, r1, r0
	add r2, r6, #0
	add r1, r2, #1
	lsl r1, r1, #0x18
	lsr r6, r1, #0x18
	ldr r1, [sp, #0x28]
	add r3, r2, #0
	mul r3, r1
	ldr r1, [sp, #0x20]
	sub r3, r1, r3
	ldr r1, [sp, #0x24]
	str r1, [sp, #0]
	mov r1, #2
	str r1, [sp, #4]
	mov r1, r8
	str r1, [sp, #8]
	ldr r1, [sp, #0x2C]
	str r1, [sp, #0xC]
	ldr r1, [sp, #0x30]
	str r1, [sp, #0x10]
	mov r1, #0
	str r1, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r1, [sp, #0x70]
	str r1, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	bl OamListAddSpriteGroup
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, sl
	bcc _0807B8C6
	b _0807B9C4
_0807B928:
	cmp r7, #0
	bne _0807B956
	ldr r5, [sp, #0x24]
	str r5, [sp, #0]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, r8
	str r0, [sp, #8]
	ldr r1, [sp, #0x2C]
	str r1, [sp, #0xC]
	ldr r5, [sp, #0x30]
	str r5, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	ldr r7, [sp, #0x70]
	str r7, [sp, #0x1C]
	ldr r0, [sp, #0x58]
	mov r1, #0
	mov r2, #1
	ldr r3, [sp, #0x20]
	bl OamListAddSpriteGroup
	b _0807B9C4
_0807B956:
	mov r5, #0
	cmp r5, sl
	bcs _0807B9C4
_0807B95C:
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
	bne _0807B97C
	cmp r7, #0
	beq _0807B9C4
_0807B97C:
	lsl r0, r4, #3
	ldr r1, [sp, #0x58]
	add r0, r1, r0
	add r2, r6, #0
	add r1, r2, #1
	lsl r1, r1, #0x18
	lsr r6, r1, #0x18
	ldr r1, [sp, #0x28]
	add r3, r2, #0
	mul r3, r1
	ldr r1, [sp, #0x20]
	sub r3, r1, r3
	ldr r1, [sp, #0x24]
	str r1, [sp, #0]
	mov r1, #2
	str r1, [sp, #4]
	mov r1, r8
	str r1, [sp, #8]
	ldr r1, [sp, #0x2C]
	str r1, [sp, #0xC]
	ldr r1, [sp, #0x30]
	str r1, [sp, #0x10]
	mov r1, #0
	str r1, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r1, [sp, #0x70]
	str r1, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	bl OamListAddSpriteGroup
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, sl
	bcc _0807B95C
_0807B9C4:
	add sp, #0x34
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DrawNumberSprites

