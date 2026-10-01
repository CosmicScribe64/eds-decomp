	thumb_func_start sub_0803A17C
sub_0803A17C: @ 0x0803A17C
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803A1B4
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r2, [r4, #2]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x1A
	bl sub_08018C3C
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _0803A1A8
	ldr r1, _0803A1BC @ =0x00008060
_0803A1A8:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_0803A1B4:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
_0803A1BC: .4byte 0x00008060
	thumb_func_end sub_0803A17C

