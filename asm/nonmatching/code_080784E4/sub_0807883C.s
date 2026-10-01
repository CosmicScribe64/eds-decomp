	thumb_func_start sub_0807883C
sub_0807883C: @ 0x0807883C
	push {r4, lr}
	add r2, r0, #0
	ldrb r0, [r2, #6]
	cmp r0, #1
	bne _08078898
	ldrh r0, [r2, #4]
	mov r1, #4
	ldsh r3, [r2, r1]
	cmp r3, #0
	beq _08078898
	ldrh r1, [r2, #2]
	add r0, r0, r1
	mov r4, #0
	strh r0, [r2, #2]
	cmp r3, #0
	ble _08078874
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0x80
	lsl r1, r1, #5
	cmp r0, r1
	bls _08078890
	strh r1, [r2, #2]
	strh r4, [r2, #4]
	mov r0, #2
	strb r0, [r2, #6]
	mov r0, #0x10
	b _08078888
_08078874:
	lsl r1, r0, #0x10
	mov r0, #0x80
	lsl r0, r0, #0x15
	cmp r1, r0
	bls _08078890
	strh r4, [r2, #2]
	strh r4, [r2, #4]
	mov r0, #3
	strb r0, [r2, #6]
	mov r0, #0
_08078888:
	bl sub_0807B4C0
	mov r0, #1
	b _0807889A
_08078890:
	ldrh r2, [r2, #2]
	lsr r0, r2, #8
	bl sub_0807B4C0
_08078898:
	mov r0, #0
_0807889A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0807883C

