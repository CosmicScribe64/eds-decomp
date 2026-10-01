	thumb_func_start sub_080310EC
sub_080310EC: @ 0x080310EC
	push {lr}
	add r3, r0, #0
	mov r1, #0
	mov r0, #4
	ldrb r2, [r3, #4]
	and r0, r2
	cmp r0, #0
	bne _08031178
	ldr r0, _0803111C @ =0x000007FF
	ldrh r2, [r3]
	and r0, r2
	lsl r0, r0, #1
	ldr r2, _08031120 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _08031124 @ =0x00000153
	cmp r2, r0
	beq _08031140
	cmp r2, r0
	bgt _08031128
	sub r0, #2
	cmp r2, r0
	beq _0803113C
	b _08031150
_0803111C: .4byte 0x000007FF
_08031120: .4byte gUnk_08622AB4
_08031124: .4byte 0x00000153
_08031128:
	mov r0, #0xAA
	lsl r0, r0, #1
	cmp r2, r0
	beq _08031146
	ldr r0, _08031138 @ =0x000003EE
	cmp r2, r0
	beq _0803114C
	b _08031150
_08031138: .4byte 0x000003EE
_0803113C:
	mov r1, #0xC8
	b _08031154
_08031140:
	mov r1, #0x96
	lsl r1, r1, #2
	b _08031154
_08031146:
	mov r1, #0xC8
	lsl r1, r1, #2
	b _08031154
_0803114C:
	mov r1, #0xC8
	lsl r1, r1, #1
_08031150:
	cmp r1, #0
	beq _08031178
_08031154:
	ldrh r2, [r3, #0xC]
	cmp r2, #0
	beq _08031160
	cmp r2, #1
	beq _0803116C
	b _08031178
_08031160:
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08019980
	b _08031178
_0803116C:
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r2, r0
	bl sub_08019980
_08031178:
	mov r0, #0
	pop {r1}
	bx r1
	thumb_func_end sub_080310EC
	.align 2, 0

