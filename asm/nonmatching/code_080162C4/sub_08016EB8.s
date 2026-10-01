	thumb_func_start sub_08016EB8
sub_08016EB8: @ 0x08016EB8
	push {r4, r5, lr}
	ldr r4, _08016EEC @ =0x020185C0
	ldr r0, _08016EF0 @ =0x0000080A
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _08016F00
	cmp r0, #1
	beq _08016F58
	ldr r1, _08016EF4 @ =0x020192E0
	ldr r2, _08016EF8 @ =0x00001B12
	add r1, r1, r2
	mov r0, #1
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r0, _08016EFC @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _08017000
_08016EEC: .4byte 0x020185C0
_08016EF0: .4byte 0x0000080A
_08016EF4: .4byte 0x020192E0
_08016EF8: .4byte 0x00001B12
_08016EFC: .4byte 0x0000080D
_08016F00:
	mov r0, #0xB
	bl sub_08077AEC
	ldr r0, _08016F40 @ =0x050003E0
	ldr r1, _08016F44 @ =0x08687B9C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _08016F48 @ =0x06016C80
	ldr r1, _08016F4C @ =0x08687BBC
	mov r2, #0x80
	lsl r2, r2, #3
	bl sub_080752B0
	ldr r0, _08016F50 @ =0x0000080C
	add r1, r4, r0
	ldr r0, _08016F54 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldrb r2, [r5]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	b _08017000
_08016F40: .4byte 0x050003E0
_08016F44: .4byte gUnk_08687B9C
_08016F48: .4byte 0x06016C80
_08016F4C: .4byte gUnk_08687BBC
_08016F50: .4byte 0x0000080C
_08016F54: .4byte 0xFFFFF01F
_08016F58:
	ldr r1, _08017008 @ =0x0000080C
	add r0, r4, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _08016F7C
	cmp r0, #0xF
	bgt _08016F7C
	lsl r3, r0, #0x14
	ldr r0, _0801700C @ =0x00300058
	ldr r1, _08017010 @ =0x000040C0
	ldr r2, _08017014 @ =0x0000F364
	mov r4, #0x80
	lsl r4, r4, #0xD
	add r3, r3, r4
	bl sub_08076714
_08016F7C:
	ldr r0, _08017018 @ =0x020185C0
	ldr r1, _08017008 @ =0x0000080C
	add r4, r0, r1
	ldrh r2, [r4]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	sub r0, #0x10
	cmp r0, #0x50
	bhi _08016F98
	ldr r0, _0801700C @ =0x00300058
	ldr r1, _08017010 @ =0x000040C0
	ldr r2, _08017014 @ =0x0000F364
	bl sub_080761F0
_08016F98:
	ldrh r1, [r4]
	lsl r0, r1, #0x14
	lsr r3, r0, #0x19
	add r0, r3, #0
	sub r0, #0x61
	cmp r0, #0x1E
	bhi _08016FC2
	sub r3, #0x60
	ldr r0, _0801700C @ =0x00300058
	ldr r1, _08017010 @ =0x000040C0
	ldr r2, _08017014 @ =0x0000F364
	lsl r3, r3, #0x18
	bl sub_08076714
	ldrh r4, [r4]
	lsl r0, r4, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x7F
	bne _08016FC2
	bl sub_0801EBA8
_08016FC2:
	ldr r4, _08017018 @ =0x020185C0
	ldr r2, _08017008 @ =0x0000080C
	add r3, r4, r2
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _0801701C @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0
	bne _08017000
	ldr r0, _08017020 @ =0x0000080A
	add r3, r4, r0
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_08017000:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08017008: .4byte 0x0000080C
_0801700C: .4byte 0x00300058
_08017010: .4byte 0x000040C0
_08017014: .4byte 0x0000F364
_08017018: .4byte 0x020185C0
_0801701C: .4byte 0xFFFFF01F
_08017020: .4byte 0x0000080A
	thumb_func_end sub_08016EB8

