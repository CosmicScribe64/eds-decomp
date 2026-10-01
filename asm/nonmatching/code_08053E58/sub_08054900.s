	thumb_func_start sub_08054900
sub_08054900: @ 0x08054900
	push {r4, r5, r6, lr}
	ldr r4, _08054918 @ =0x0201CF90
	ldrh r1, [r4, #0xE]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #1
	beq _08054998
	cmp r0, #1
	bgt _0805491C
	cmp r0, #0
	beq _08054922
	b _08054B58
_08054918: .4byte 0x0201CF90
_0805491C:
	cmp r0, #2
	beq _08054A00
	b _08054B58
_08054922:
	ldrb r1, [r4, #3]
	lsl r0, r1, #0x1E
	cmp r0, #0
	bge _08054938
	lsl r0, r1, #0x1B
	lsr r0, r0, #0x1F
	ldrb r2, [r4, #2]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	bl sub_08017FF4
_08054938:
	ldrb r1, [r4, #3]
	lsl r0, r1, #0x1D
	cmp r0, #0
	bge _0805494E
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1F
	ldrb r2, [r4, #2]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1D
	bl sub_08017FF4
_0805494E:
	ldrb r5, [r4]
	lsl r0, r5, #0x1F
	mov r6, #0xC4
	cmp r0, #0
	beq _0805495A
	ldr r6, _08054990 @ =0x000080C4
_0805495A:
	ldrb r1, [r4, #3]
	lsr r0, r1, #7
	ldr r1, _08054994 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	ldrh r2, [r4]
	lsr r0, r2, #6
	mov r3, #0xF
	add r2, r3, #0
	and r2, r0
	lsl r2, r2, #4
	lsl r0, r5, #0x1A
	lsr r0, r0, #0x1B
	and r3, r0
	orr r2, r3
	ldrb r3, [r4, #1]
	lsl r0, r3, #0x19
	lsr r0, r0, #0x1F
	lsr r3, r3, #7
	lsl r3, r3, #1
	orr r0, r3
	lsl r0, r0, #8
	orr r2, r0
	add r0, r6, #0
	b _080549D6
_08054990: .4byte 0x000080C4
_08054994: .4byte 0x00007FFF
_08054998:
	ldrb r2, [r4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0
	bl sub_08024134
	ldrb r1, [r4, #1]
	lsl r0, r1, #0x19
	cmp r0, #0
	blt _080549C4
	ldr r0, _080549C0 @ =0xFFFFF01F
	ldrh r2, [r4, #0xE]
	and r0, r2
	mov r2, #0xA0
	lsl r2, r2, #1
	add r1, r2, #0
	b _080549EE
	.align 2, 0
_080549C0: .4byte 0xFFFFF01F
_080549C4:
	ldrb r1, [r4, #3]
	lsr r0, r1, #7
	ldr r1, _080549F8 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	mov r0, #0x71
	mov r2, #1
_080549D6:
	mov r3, #0
	bl sub_0801EC58
	ldrh r2, [r4, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080549FC @ =0xFFFFF01F
	and r0, r2
_080549EE:
	orr r0, r1
	strh r0, [r4, #0xE]
	mov r0, #0
	b _08054B5A
	.align 2, 0
_080549F8: .4byte 0x00007FFF
_080549FC: .4byte 0xFFFFF01F
_08054A00:
	ldrb r0, [r4]
	lsl r1, r0, #0x1F
	lsr r3, r1, #0x1F
	lsl r0, r0, #0x1A
	lsr r2, r0, #0x1B
	mov r0, #0x94
	mul r0, r2
	ldr r1, _08054A74 @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r1, _08054A78 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08054A22
	b _08054B58
_08054A22:
	mov r0, #0x90
	cmp r3, #0
	beq _08054A2A
	ldr r0, _08054A7C @ =0x00008090
_08054A2A:
	add r1, r2, #0
	ldrh r2, [r4, #0xC]
	mov r3, #0
	bl sub_0801EC58
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_080467B0
	ldrb r2, [r4, #3]
	lsr r1, r2, #7
	ldr r0, _08054A80 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r0, r2
	lsl r0, r0, #1
	orr r0, r1
	ldr r1, _08054A84 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08054A88 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08054A8C @ =0x00000462
	cmp r1, r0
	beq _08054AD0
	cmp r1, r0
	bgt _08054AA4
	mov r0, #0xC7
	lsl r0, r0, #2
	cmp r1, r0
	beq _08054B0C
	cmp r1, r0
	bgt _08054A94
	ldr r0, _08054A90 @ =0x000001F3
	b _08054AB0
	.align 2, 0
_08054A74: .4byte 0x00000D64
_08054A78: .4byte 0x0201930C
_08054A7C: .4byte 0x00008090
_08054A80: .4byte 0x00007FFF
_08054A84: .4byte 0x000007FF
_08054A88: .4byte gUnk_08622AB4
_08054A8C: .4byte 0x00000462
_08054A90: .4byte 0x000001F3
_08054A94:
	ldr r0, _08054AA0 @ =0x00000455
	cmp r1, r0
	beq _08054AD0
	add r0, #9
	b _08054AC4
	.align 2, 0
_08054AA0: .4byte 0x00000455
_08054AA4:
	ldr r0, _08054AB8 @ =0x000004DE
	cmp r1, r0
	beq _08054AD0
	cmp r1, r0
	bgt _08054ABC
	sub r0, #6
_08054AB0:
	cmp r1, r0
	beq _08054AD0
	b _08054B32
	.align 2, 0
_08054AB8: .4byte 0x000004DE
_08054ABC:
	ldr r0, _08054ACC @ =0x00000534
	cmp r1, r0
	beq _08054AD0
	add r0, #0x51
_08054AC4:
	cmp r1, r0
	beq _08054B20
	b _08054B32
	.align 2, 0
_08054ACC: .4byte 0x00000534
_08054AD0:
	ldr r5, _08054B04 @ =0x0201CF90
	ldrb r4, [r5]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	lsl r0, r1, #0x1F
	lsl r4, r4, #0x1A
	lsr r4, r4, #0x1B
	lsl r2, r4, #0x10
	mov r3, #0xA4
	lsl r3, r3, #0x14
	orr r2, r3
	orr r0, r2
	ldrb r2, [r5, #3]
	lsr r3, r2, #7
	ldr r2, _08054B08 @ =0x00007FFF
	ldrh r5, [r5, #4]
	and r2, r5
	lsl r2, r2, #1
	orr r2, r3
	orr r0, r2
	lsl r4, r4, #8
	orr r1, r4
	bl sub_0801FBCC
	b _08054B32
	.align 2, 0
_08054B04: .4byte 0x0201CF90
_08054B08: .4byte 0x00007FFF
_08054B0C:
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
	b _08054B32
_08054B20:
	ldr r0, _08054B50 @ =0x0201CF90
	ldrb r1, [r0]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	mov r2, #1
	bl sub_08018544
_08054B32:
	ldr r3, _08054B50 @ =0x0201CF90
	ldrh r2, [r3, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _08054B54 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3, #0xE]
	mov r0, #0
	b _08054B5A
	.align 2, 0
_08054B50: .4byte 0x0201CF90
_08054B54: .4byte 0xFFFFF01F
_08054B58:
	mov r0, #1
_08054B5A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08054900

