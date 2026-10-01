	thumb_func_start sub_08017B04
sub_08017B04: @ 0x08017B04
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov sl, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	add r6, r5, #0
	lsl r2, r2, #0x10
	lsr r3, r2, #0x10
	str r3, [sp, #0]
	lsl r0, r5, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #4]
	lsr r1, r1, #0x18
	str r1, [sp, #8]
	lsl r0, r3, #0x18
	lsr r7, r0, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0xC]
	mov r0, #1
	mov r8, r0
	add r1, r7, #0
	and r1, r0
	mov r4, #0x94
	add r0, r2, #0
	mul r0, r4
	ldr r2, _08017BCC @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r4, _08017BD0 @ =0x0201930C
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	mov r9, r4
	mov r0, #0x83
	mov r1, sl
	cmp r1, #0
	beq _08017B5A
	ldr r0, _08017BD4 @ =0x00008083
_08017B5A:
	add r1, r5, #0
	add r2, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r5, _08017BD8 @ =0x000007FF
	and r4, r5
	lsl r0, r4, #1
	ldr r4, _08017BDC @ =0x08622AB4
	add r0, r0, r4
	ldr r1, _08017BE0 @ =0x00000547
	ldrh r0, [r0]
	cmp r0, r1
	bne _08017BE8
	mov r0, #0x73
	cmp r7, #0
	beq _08017B7E
	ldr r0, _08017BE4 @ =0x00008073
_08017B7E:
	mov r1, r9
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	mov r2, #1
	bl sub_08018544
	ldr r1, [sp, #4]
	mov r2, r8
	and r1, r2
	ldr r3, [sp, #8]
	mov r2, #0x94
	add r0, r3, #0
	mul r0, r2
	ldr r3, _08017BCC @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r1, _08017BD0 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r4
	mov r1, #0x91
	lsl r1, r1, #3
	ldrh r0, [r0]
	cmp r0, r1
	bne _08017BFC
	add r0, r7, #0
	ldr r1, [sp, #0xC]
	mov r2, #1
	bl sub_08018544
	b _08017BFC
_08017BCC: .4byte 0x00000D64
_08017BD0: .4byte 0x0201930C
_08017BD4: .4byte 0x00008083
_08017BD8: .4byte 0x000007FF
_08017BDC: .4byte gUnk_08622AB4
_08017BE0: .4byte 0x00000547
_08017BE4: .4byte 0x00008073
_08017BE8:
	mov r2, r8
	mov r3, sl
	sub r0, r2, r3
	ldr r4, [sp, #0]
	lsl r1, r4, #0x10
	orr r6, r1
	mov r1, #0x19
	add r2, r6, #0
	bl sub_08042AB0
_08017BFC:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08017B04

