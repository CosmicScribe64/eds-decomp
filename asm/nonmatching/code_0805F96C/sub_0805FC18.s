	thumb_func_start sub_0805FC18
sub_0805FC18: @ 0x0805FC18
	push {r4, r5, lr}
	ldr r4, _0805FC48 @ =0x0201AE60
	add r1, r4, #0
	add r1, #0x22
	ldrb r0, [r1]
	add r2, r0, #0
	cmp r2, #1
	beq _0805FC50
	cmp r2, #2
	beq _0805FC92
	ldr r1, _0805FC4C @ =0x03000040
	mov r0, #0x80
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0805FC96
	mov r0, #0
	bl sub_08077AEC
	mov r0, #1
	ldrh r1, [r4, #0x14]
	sub r0, r0, r1
	strh r0, [r4, #0x14]
	b _0805FC96
_0805FC48: .4byte 0x0201AE60
_0805FC4C: .4byte 0x03000040
_0805FC50:
	add r5, r4, #0
	add r5, #0x23
	ldrb r3, [r5]
	cmp r3, #0x3B
	bhi _0805FC8C
	add r4, r3, #1
	strb r4, [r5]
	ldr r1, _0805FC84 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0805FC74
	ldr r0, _0805FC88 @ =0x0201CFB0
	ldrb r0, [r0]
	and r2, r0
	cmp r2, #0
	beq _0805FCE2
_0805FC74:
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x33
	bhi _0805FCE2
	add r0, r3, #0
	add r0, #8
	strb r0, [r5]
	b _0805FCE2
_0805FC84: .4byte 0x03000040
_0805FC88: .4byte 0x0201CFB0
_0805FC8C:
	add r0, #1
	strb r0, [r1]
	b _0805FCE2
_0805FC92:
	mov r0, #1
	b _0805FCE4
_0805FC96:
	ldr r4, _0805FCEC @ =0x03000040
	mov r0, #0x40
	ldrh r2, [r4, #6]
	and r0, r2
	cmp r0, #0
	beq _0805FCB2
	mov r0, #0
	bl sub_08077AEC
	ldr r1, _0805FCF0 @ =0x0201AE60
	mov r0, #1
	ldrh r2, [r1, #0x14]
	sub r0, r0, r2
	strh r0, [r1, #0x14]
_0805FCB2:
	mov r0, #1
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0805FCD2
	mov r0, #1
	bl sub_08077AEC
	ldr r0, _0805FCF0 @ =0x0201AE60
	add r3, r0, #0
	add r3, #0x22
	mov r2, #0
	mov r1, #1
	strb r1, [r3]
	add r0, #0x23
	strb r2, [r0]
_0805FCD2:
	mov r0, #2
	ldrh r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	beq _0805FCE2
	mov r0, #3
	bl sub_08077AEC
_0805FCE2:
	mov r0, #0
_0805FCE4:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0805FCEC: .4byte 0x03000040
_0805FCF0: .4byte 0x0201AE60
	thumb_func_end sub_0805FC18

