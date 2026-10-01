	thumb_func_start sub_08037E94
sub_08037E94: @ 0x08037E94
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08037ECC
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r4, #0xC]
	bl sub_08019800
	mov r0, #1
	ldrb r1, [r4, #2]
	and r0, r1
	mov r3, #0x8E
	cmp r0, #0
	beq _08037EBC
	ldr r3, _08037ED4 @ =0x0000808E
_08037EBC:
	ldrh r0, [r4, #2]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x1A
	ldrh r2, [r4, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
_08037ECC:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_08037ED4: .4byte 0x0000808E
	thumb_func_end sub_08037E94

