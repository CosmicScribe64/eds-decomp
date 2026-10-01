	thumb_func_start sub_08009538
sub_08009538: @ 0x08009538
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	mov ip, r1
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mov r1, ip
	mul r1, r0
	add r0, r1, #0
	ldr r1, _080095D0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080095D4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	mov r9, r1
	cmp r0, #0
	beq _08009654
	mov r6, #0
_08009566:
	mov r5, #0
_08009568:
	mov r1, #1
	and r1, r6
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r7, _080095D0 @ =0x00000D64
	add r0, r1, #0
	mul r0, r7
	add r2, r2, r0
	mov r3, r9
	add r2, r2, r3
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08009648
	mov r0, #2
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #0
	beq _08009648
	mov r4, #0
	add r0, r2, #0
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r4, r0
	bge _08009648
_0800959C:
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	add r0, r2, #0
	mul r0, r7
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
	bhi _08009628
	lsl r0, r0, #2
	ldr r1, _080095D8 @ =0x080095DC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080095D0: .4byte 0x00000D64
_080095D4: .4byte 0x0201930C
_080095D8: .4byte 0x080095DC
_080095DC:
	.4byte _08009604
	.4byte _0800960A
	.4byte _08009628
	.4byte _08009628
	.4byte _08009604
	.4byte _08009628
	.4byte _0800960A
	.4byte _08009628
	.4byte _08009628
	.4byte _08009604
_08009604:
	mov r2, r8
	lsl r0, r2, #0x18
	b _0800960E
_0800960A:
	mov r1, r8
	lsl r0, r1, #0x18
_0800960E:
	mov r2, ip
	lsl r1, r2, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	cmp r3, r0
	bne _08009628
	lsl r0, r6, #0x18
	lsl r1, r5, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	b _08009656
_08009628:
	add r4, #1
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r7, _08009664 @ =0x00000D64
	add r0, r2, #0
	mul r0, r7
	add r1, r1, r0
	mov r3, r9
	add r1, r1, r3
	add r1, #0x8A
	ldrh r1, [r1]
	cmp r4, r1
	blt _0800959C
_08009648:
	add r5, #1
	cmp r5, #4
	ble _08009568
	add r6, #1
	cmp r6, #1
	ble _08009566
_08009654:
	ldr r0, _08009668 @ =0x0000FFFF
_08009656:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08009664: .4byte 0x00000D64
_08009668: .4byte 0x0000FFFF
	thumb_func_end sub_08009538

