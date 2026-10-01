	thumb_func_start sub_080030FC
sub_080030FC: @ 0x080030FC
	push {r4, lr}
	ldr r4, _08003130 @ =0x0201F7E0
	mov r0, #7
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _08003118
	mov r0, #0xC0
	lsl r0, r0, #0xF
	mov r2, #0x96
	lsl r2, r2, #1
	mov r1, #0x40
	bl sub_080762D0
_08003118:
	mov r1, #0
	ldrb r4, [r4]
	lsl r0, r4, #0x1D
	lsr r0, r0, #0x1D
	cmp r0, #1
	beq _08003144
	cmp r0, #1
	bgt _08003134
	cmp r0, #0
	beq _0800313E
	b _08003158
	.align 2, 0
_08003130: .4byte 0x0201F7E0
_08003134:
	cmp r0, #2
	beq _0800314A
	cmp r0, #3
	beq _08003150
	b _08003158
_0800313E:
	bl sub_08063BAC
	b _08003154
_08003144:
	bl sub_08063C14
	b _08003154
_0800314A:
	bl sub_08063C7C
	b _08003154
_08003150:
	bl sub_08063CE4
_08003154:
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08003158:
	cmp r1, #0
	beq _08003168
	ldr r0, _08003170 @ =0x006000E0
	mov r2, #0x97
	lsl r2, r2, #1
	mov r1, #0x40
	bl sub_080762D0
_08003168:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08003170: .4byte 0x006000E0
	thumb_func_end sub_080030FC

