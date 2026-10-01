	thumb_func_start sub_0803C550
sub_0803C550: @ 0x0803C550
	push {r4, r5, lr}
	add r3, r0, #0
	mov r2, #7
	ldrb r0, [r3, #0xA]
	and r2, r0
	cmp r2, #1
	bne _0803C590
	ldrb r4, [r3, #0xC]
	ldrh r0, [r3, #0xC]
	lsr r5, r0, #8
	and r2, r4
	mov r0, #0x94
	mul r0, r5
	ldr r1, _0803C598 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803C59C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803C590
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r4, r0
	beq _0803C590
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #1
	bl sub_08018AE8
_0803C590:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
_0803C598: .4byte 0x00000D64
_0803C59C: .4byte 0x0201930C
	thumb_func_end sub_0803C550

