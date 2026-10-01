	thumb_func_start sub_08031334
sub_08031334: @ 0x08031334
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	bne _080313B4
	ldr r0, _080313A4 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _080313B0
	mov r6, #0
_08031352:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r2, #1
	sub r5, r2, r0
	add r1, r5, #0
	and r1, r2
	mov r0, #0x94
	add r2, r6, #0
	mul r2, r0
	ldr r0, _080313A8 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080313AC @ =0x0201930C
	add r4, r2, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08031398
	mov r0, #2
	ldrb r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	bne _08031398
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #1
	bl sub_08018DC8
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r5, #0
	bl sub_08019840
_08031398:
	add r6, #1
	cmp r6, #4
	ble _08031352
	mov r0, #0x7F
	b _080313B6
	.align 2, 0
_080313A4: .4byte 0x02017A40
_080313A8: .4byte 0x00000D64
_080313AC: .4byte 0x0201930C
_080313B0:
	bl sub_08046AD0
_080313B4:
	mov r0, #0
_080313B6:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08031334

