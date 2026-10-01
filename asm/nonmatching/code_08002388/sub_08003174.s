	thumb_func_start sub_08003174
sub_08003174: @ 0x08003174
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	mov r1, #0
	ldr r0, _08003230 @ =0x0201F7E0
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1D
	cmp r0, #3
	bls _08003190
	mov r1, #1
_08003190:
	add r7, r1, #0
	cmp r7, #4
	bgt _08003222
	mov r0, #0x84
	lsl r0, r0, #1
	mov r9, r0
_0800319C:
	ldr r0, _08003230 @ =0x0201F7E0
	ldrb r0, [r0]
	lsl r1, r0, #0x1D
	lsr r1, r1, #0x1D
	lsl r0, r1, #2
	add r0, r0, r1
	add r0, r0, r7
	lsl r0, r0, #1
	ldr r1, _08003234 @ =0x0819834C
	add r0, r0, r1
	ldrh r0, [r0]
	bl sub_08063DAC
	cmp r0, #0
	bne _0800321C
	ldr r0, _08003238 @ =0x081983AC
	lsl r1, r7, #2
	add r1, r1, r0
	mov r2, #0
	ldsh r5, [r1, r2]
	mov r2, sl
	lsl r0, r2, #1
	sub r5, r5, r0
	mov r0, #2
	ldsh r6, [r1, r0]
	lsl r4, r6, #0x10
	add r0, r5, #0
	orr r0, r4
	mov r1, #0x80
	mov r2, r9
	mov r3, #0
	bl sub_08076348
	mov r1, #0x20
	add r1, r1, r5
	mov r8, r1
	orr r4, r1
	add r0, r4, #0
	mov r1, #0x80
	mov r2, r9
	mov r3, #0x80
	lsl r3, r3, #5
	bl sub_08076348
	add r6, #0x20
	lsl r6, r6, #0x10
	orr r5, r6
	add r0, r5, #0
	mov r1, #0x80
	mov r2, r9
	mov r3, #0x80
	lsl r3, r3, #6
	bl sub_08076348
	mov r2, r8
	orr r2, r6
	mov r8, r2
	mov r0, r8
	mov r1, #0x80
	mov r2, r9
	mov r3, #0xC0
	lsl r3, r3, #6
	bl sub_08076348
_0800321C:
	add r7, #1
	cmp r7, #4
	ble _0800319C
_08003222:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08003230: .4byte 0x0201F7E0
_08003234: .4byte gUnk_0819834C
_08003238: .4byte gUnk_081983AC
	thumb_func_end sub_08003174

