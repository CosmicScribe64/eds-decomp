	thumb_func_start sub_080086CC
sub_080086CC: @ 0x080086CC
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	mov r5, #0
	mov r4, #0
	ldr r1, _08008720 @ =0x0201930C
	mov ip, r1
	mov r1, #1
	and r1, r0
	ldr r0, _08008724 @ =0x00000D64
	mul r1, r0
	ldr r7, _08008728 @ =0x000007FF
_080086E4:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r1
	mov r2, ip
	add r3, r0, r2
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08008712
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _08008712
	and r2, r7
	lsl r0, r2, #1
	ldr r2, _0800872C @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r6
	bne _08008712
	add r5, #1
_08008712:
	add r4, #1
	cmp r4, #4
	ble _080086E4
	add r0, r5, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08008720: .4byte 0x0201930C
_08008724: .4byte 0x00000D64
_08008728: .4byte 0x000007FF
_0800872C: .4byte gUnk_08622AB4
	thumb_func_end sub_080086CC

