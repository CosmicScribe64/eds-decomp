	thumb_func_start sub_08031410
sub_08031410: @ 0x08031410
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08031476
	mov r5, #0
	mov r4, #1
	ldr r0, _08031484 @ =0x00000D64
	mov r8, r0
	ldr r7, _08031488 @ =0x0201930C
_0803142C:
	ldrb r1, [r6, #2]
	lsl r2, r1, #0x1F
	lsr r1, r2, #0x1F
	sub r1, r4, r1
	and r1, r4
	mov r0, #0x94
	add r3, r5, #0
	mul r3, r0
	mov r0, r8
	mul r0, r1
	add r0, r3, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08031470
	lsr r0, r2, #0x1F
	sub r0, r4, r0
	and r0, r4
	mov r1, r8
	mul r1, r0
	add r1, r3, r1
	add r1, r1, r7
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08031470
	lsr r0, r2, #0x1F
	sub r0, r4, r0
	add r1, r5, #0
	mov r2, #1
	bl sub_08018DC8
_08031470:
	add r5, #1
	cmp r5, #4
	ble _0803142C
_08031476:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08031484: .4byte 0x00000D64
_08031488: .4byte 0x0201930C
	thumb_func_end sub_08031410

