	thumb_func_start sub_08008D3C
sub_08008D3C: @ 0x08008D3C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sl, r0
	mov r9, r1
	mov r7, #0
	ldr r2, _08008DA8 @ =0x0201930C
_08008D50:
	mov r6, #0
	add r0, r7, #1
	str r0, [sp, #0]
_08008D56:
	mov r4, #0
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	mul r0, r6
	ldr r5, _08008DAC @ =0x00000D64
	mul r1, r5
	add r0, r0, r1
	add r3, r2, #0
	add r0, r0, r2
	add r0, #0x8A
	add r1, r6, #1
	mov r8, r1
	ldrh r0, [r0]
	cmp r4, r0
	bge _08008E1E
_08008D76:
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	add r0, r2, #0
	mul r0, r5
	add r1, r1, r0
	add r1, r1, r3
	lsl r2, r4, #1
	add r0, r1, #0
	add r0, #0xA
	add r0, r0, r2
	ldrh r3, [r0]
	add r1, #0x4A
	add r1, r1, r2
	ldrb r0, [r1]
	sub r0, #1
	cmp r0, #9
	bhi _08008DFA
	lsl r0, r0, #2
	ldr r1, _08008DB0 @ =0x08008DB4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08008DA8: .4byte 0x0201930C
_08008DAC: .4byte 0x00000D64
_08008DB0: .4byte 0x08008DB4
_08008DB4:
	.4byte _08008DDC
	.4byte _08008DDC
	.4byte _08008DFA
	.4byte _08008DFA
	.4byte _08008DDC
	.4byte _08008DFA
	.4byte _08008DDC
	.4byte _08008DFA
	.4byte _08008DFA
	.4byte _08008DDC
_08008DDC:
	mov r2, sl
	lsl r0, r2, #0x18
	mov r2, r9
	lsl r1, r2, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	cmp r3, r0
	bne _08008DFC
	add r0, r7, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080082A0
	b _08008DFC
_08008DFA:
	add r4, #1
_08008DFC:
	add r4, #1
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r5, _08008E3C @ =0x00000D64
	add r0, r2, #0
	mul r0, r5
	add r1, r1, r0
	ldr r3, _08008E40 @ =0x0201930C
	add r1, r1, r3
	add r1, #0x8A
	add r2, r3, #0
	ldrh r1, [r1]
	cmp r4, r1
	blt _08008D76
_08008E1E:
	mov r6, r8
	cmp r6, #4
	ble _08008D56
	ldr r7, [sp, #0]
	cmp r7, #1
	ble _08008D50
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08008E3C: .4byte 0x00000D64
_08008E40: .4byte 0x0201930C
	thumb_func_end sub_08008D3C

