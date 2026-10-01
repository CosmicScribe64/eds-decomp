	thumb_func_start sub_080321C8
sub_080321C8: @ 0x080321C8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08032242
	mov r3, #7
	ldrb r0, [r1, #0xA]
	and r3, r0
	cmp r3, #1
	bne _08032242
	ldrb r2, [r1, #2]
	lsl r0, r2, #0x1F
	lsr r4, r0, #0x1F
	ldrh r2, [r1, #2]
	lsl r0, r2, #0x16
	lsr r0, r0, #0x1A
	mov ip, r0
	ldrb r2, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r1, r1, #8
	mov r8, r1
	cmp r4, r2
	beq _08032242
	add r1, r4, #0
	and r1, r3
	mov r7, #0x94
	mov r0, ip
	mul r0, r7
	ldr r6, _08032250 @ =0x00000D64
	mul r1, r6
	add r0, r0, r1
	ldr r5, _08032254 @ =0x0201930C
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08032242
	and r3, r2
	mov r0, r8
	mul r0, r7
	add r1, r3, #0
	mul r1, r6
	add r0, r0, r1
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08032242
	mov r3, ip
	lsl r1, r3, #8
	orr r1, r4
	mov r3, r8
	lsl r0, r3, #8
	orr r2, r0
	add r0, r4, #0
	bl sub_0801919C
_08032242:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08032250: .4byte 0x00000D64
_08032254: .4byte 0x0201930C
	thumb_func_end sub_080321C8

