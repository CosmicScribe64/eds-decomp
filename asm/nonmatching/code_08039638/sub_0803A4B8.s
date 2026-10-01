	thumb_func_start sub_0803A4B8
sub_0803A4B8: @ 0x0803A4B8
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r2, [r4, #4]
	and r0, r2
	cmp r0, #0
	beq _0803A4CA
	mov r0, #0
	b _0803A526
_0803A4CA:
	mov r0, #0xFC
	ldrb r2, [r4, #3]
	and r0, r2
	cmp r0, #0x40
	bne _0803A51C
	ldrb r1, [r4, #2]
	mov r5, #1
	add r0, r5, #0
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0803A4E4
	ldr r3, _0803A514 @ =0x00008008
_0803A4E4:
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrh r0, [r4, #0xC]
	lsr r2, r0, #8
	lsl r2, r2, #8
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0x38
	cmp r0, #0
	beq _0803A504
	ldr r2, _0803A518 @ =0x00008038
_0803A504:
	ldrh r1, [r4, #0xC]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0
	b _0803A526
_0803A514: .4byte 0x00008008
_0803A518: .4byte 0x00008038
_0803A51C:
	add r0, r4, #0
	bl sub_08036030
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_0803A526:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0803A4B8

