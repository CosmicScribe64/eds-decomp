	thumb_func_start OamListAddSprite
OamListAddSprite: @ 0x0807B6B8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r4, [sp, #0x2C]
	ldr r5, [sp, #0x30]
	ldr r6, [sp, #0x34]
	mov ip, r6
	ldr r6, [sp, #0x38]
	ldr r7, [sp, #0x40]
	mov r8, r7
	ldr r7, [sp, #0x44]
	mov sl, r7
	ldr r7, [sp, #0x48]
	mov r9, r7
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0]
	lsl r4, r4, #0x18
	lsr r7, r4, #0x18
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	mov r4, ip
	lsl r1, r4, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #4]
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	str r6, [sp, #8]
	mov r6, r8
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	mov r8, r6
	mov r1, sl
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov sl, r1
	mov r4, r9
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r9, r4
	lsl r2, r2, #0x17
	lsr r6, r2, #0x17
	lsl r3, r3, #0x10
	mov r1, #0xFF
	lsl r1, r1, #0x10
	and r1, r3
	lsr r4, r1, #0x10
	ldr r1, [sp, #0x4C]
	bl OamListAlloc
	add r2, r0, #0
	mov r0, sl
	lsl r1, r0, #0x19
	ldr r3, [sp, #4]
	cmp r3, #8
	bne _0807B73E
	mov r0, #0x80
	lsl r0, r0, #6
	orr r1, r0
	mov r0, r8
	orr r1, r0
	b _0807B742
_0807B73E:
	mov r3, r8
	orr r1, r3
_0807B742:
	str r1, [r2]
	cmp r7, #0x10
	beq _0807B792
	cmp r7, #0x10
	bgt _0807B752
	cmp r7, #8
	beq _0807B75C
	b _0807B840
_0807B752:
	cmp r7, #0x20
	beq _0807B7C6
	cmp r7, #0x40
	beq _0807B808
	b _0807B840
_0807B75C:
	cmp r5, #0x10
	beq _0807B77C
	cmp r5, #0x10
	bgt _0807B76A
	cmp r5, #8
	beq _0807B774
	b _0807B840
_0807B76A:
	cmp r5, #0x20
	beq _0807B784
	cmp r5, #0x40
	beq _0807B790
	b _0807B840
_0807B774:
	lsl r1, r6, #0x10
	orr r1, r4
	ldr r0, [r2]
	b _0807B83C
_0807B77C:
	ldr r0, [r2]
	mov r1, #0x80
	lsl r1, r1, #8
	b _0807B836
_0807B784:
	ldr r0, [r2]
	ldr r1, _0807B78C @ =0x40008000
	b _0807B836
	.align 2, 0
_0807B78C: .4byte 0x40008000
_0807B790:
	b _0807B790
_0807B792:
	cmp r5, #0x10
	beq _0807B7B2
	cmp r5, #0x10
	bgt _0807B7A0
	cmp r5, #8
	beq _0807B7AA
	b _0807B840
_0807B7A0:
	cmp r5, #0x20
	beq _0807B7BA
	cmp r5, #0x40
	beq _0807B7C4
	b _0807B840
_0807B7AA:
	ldr r0, [r2]
	mov r1, #0x80
	lsl r1, r1, #7
	b _0807B836
_0807B7B2:
	ldr r0, [r2]
	mov r1, #0x80
	lsl r1, r1, #0x17
	b _0807B836
_0807B7BA:
	ldr r0, [r2]
	ldr r1, _0807B7C0 @ =0x80008000
	b _0807B836
_0807B7C0: .4byte 0x80008000
_0807B7C4:
	b _0807B7C4
_0807B7C6:
	cmp r5, #0x10
	beq _0807B7E8
	cmp r5, #0x10
	bgt _0807B7D4
	cmp r5, #8
	beq _0807B7DE
	b _0807B840
_0807B7D4:
	cmp r5, #0x20
	beq _0807B7F4
	cmp r5, #0x40
	beq _0807B7FC
	b _0807B840
_0807B7DE:
	ldr r0, [r2]
	ldr r1, _0807B7E4 @ =0x40004000
	b _0807B836
_0807B7E4: .4byte 0x40004000
_0807B7E8:
	ldr r0, [r2]
	ldr r1, _0807B7F0 @ =0x80004000
	b _0807B836
	.align 2, 0
_0807B7F0: .4byte 0x80004000
_0807B7F4:
	ldr r0, [r2]
	mov r1, #0x80
	lsl r1, r1, #0x18
	b _0807B836
_0807B7FC:
	ldr r0, [r2]
	ldr r1, _0807B804 @ =0xC0008000
	b _0807B836
	.align 2, 0
_0807B804: .4byte 0xC0008000
_0807B808:
	cmp r5, #0x10
	beq _0807B822
	cmp r5, #0x10
	bgt _0807B816
	cmp r5, #8
	beq _0807B820
	b _0807B840
_0807B816:
	cmp r5, #0x20
	beq _0807B824
	cmp r5, #0x40
	beq _0807B830
	b _0807B840
_0807B820:
	b _0807B820
_0807B822:
	b _0807B822
_0807B824:
	ldr r0, [r2]
	ldr r1, _0807B82C @ =0xC0004000
	b _0807B836
	.align 2, 0
_0807B82C: .4byte 0xC0004000
_0807B830:
	ldr r0, [r2]
	mov r1, #0xC0
	lsl r1, r1, #0x18
_0807B836:
	orr r0, r1
	lsl r1, r6, #0x10
	orr r1, r4
_0807B83C:
	orr r0, r1
	str r0, [r2]
_0807B840:
	ldr r4, [sp, #8]
	lsl r0, r4, #0xC
	ldr r6, [sp, #0]
	orr r0, r6
	mov r7, r9
	lsl r1, r7, #0xA
	orr r0, r1
	strh r0, [r2, #4]
	add r0, r2, #0
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end OamListAddSprite
	.align 2, 0

