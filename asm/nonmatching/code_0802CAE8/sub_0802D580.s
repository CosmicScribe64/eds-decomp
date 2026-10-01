	thumb_func_start sub_0802D580
sub_0802D580: @ 0x0802D580
	push {r4, r5, r6, lr}
	mov r2, #0
	ldr r6, _0802D5A8 @ =0x0201930C
	ldrb r0, [r0, #2]
	lsl r4, r0, #0x1F
	mov r3, #1
	ldr r5, _0802D5AC @ =0x00000D64
	mov r1, #0
_0802D590:
	lsr r0, r4, #0x1F
	sub r0, r3, r0
	and r0, r3
	mul r0, r5
	add r0, r1, r0
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802D5B0
	mov r0, #1
	b _0802D5BA
_0802D5A8: .4byte 0x0201930C
_0802D5AC: .4byte 0x00000D64
_0802D5B0:
	add r1, #0x94
	add r2, #1
	cmp r2, #4
	ble _0802D590
	mov r0, #0
_0802D5BA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802D580

