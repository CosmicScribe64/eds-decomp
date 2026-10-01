	thumb_func_start sub_0803A654
sub_0803A654: @ 0x0803A654
	push {r4, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _0803A69C
	mov r2, #7
	ldrb r0, [r1, #0xA]
	and r2, r0
	cmp r2, #1
	bne _0803A69C
	ldrb r4, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r3, r1, #8
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _0803A6A4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803A6A8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803A69C
	mov r0, #0xA9
	cmp r4, #0
	beq _0803A692
	ldr r0, _0803A6AC @ =0x000080A9
_0803A692:
	add r1, r3, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
_0803A69C:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_0803A6A4: .4byte 0x00000D64
_0803A6A8: .4byte 0x0201930C
_0803A6AC: .4byte 0x000080A9
	thumb_func_end sub_0803A654

