	thumb_func_start sub_0803C060
sub_0803C060: @ 0x0803C060
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _0803C0C0
	ldrb r1, [r6, #0xA]
	mov r0, #7
	and r0, r1
	cmp r0, #2
	bne _0803C0C0
	lsl r0, r1, #0x1D
	mov r5, #0
	cmp r0, #0
	beq _0803C0C0
_0803C080:
	lsl r1, r5, #1
	add r0, r6, #0
	add r0, #0xC
	add r0, r0, r1
	ldrb r4, [r0]
	ldrh r0, [r0]
	lsr r3, r0, #8
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0803C0C8 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803C0CC @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803C0B4
	add r0, r4, #0
	add r1, r3, #0
	mov r2, #1
	bl sub_08018AE8
_0803C0B4:
	add r5, #1
	ldrb r1, [r6, #0xA]
	lsl r0, r1, #0x1D
	lsr r0, r0, #0x1D
	cmp r5, r0
	blt _0803C080
_0803C0C0:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0803C0C8: .4byte 0x00000D64
_0803C0CC: .4byte 0x0201930C
	thumb_func_end sub_0803C060

