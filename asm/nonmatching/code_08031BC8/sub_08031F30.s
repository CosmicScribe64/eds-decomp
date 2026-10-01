	thumb_func_start sub_08031F30
sub_08031F30: @ 0x08031F30
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08031F9A
	ldrb r1, [r6, #0xA]
	mov r0, #7
	and r0, r1
	cmp r0, #2
	bne _08031F9A
	lsl r0, r1, #0x1D
	mov r4, #0
	cmp r0, #0
	beq _08031F9A
_08031F50:
	lsl r1, r4, #1
	add r0, r6, #0
	add r0, #0xC
	add r0, r0, r1
	ldrb r5, [r0]
	ldrh r0, [r0]
	lsr r3, r0, #8
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _08031FA4 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08031FA8 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08031F8E
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _08031F8E
	add r0, r5, #0
	add r1, r3, #0
	mov r2, #1
	bl sub_08018544
_08031F8E:
	add r4, #1
	ldrb r1, [r6, #0xA]
	lsl r0, r1, #0x1D
	lsr r0, r0, #0x1D
	cmp r4, r0
	blt _08031F50
_08031F9A:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08031FA4: .4byte 0x00000D64
_08031FA8: .4byte 0x0201930C
	thumb_func_end sub_08031F30

