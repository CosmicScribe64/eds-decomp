	thumb_func_start sub_080375F8
sub_080375F8: @ 0x080375F8
	push {r4, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _0803767E
	ldr r0, _08037624 @ =0x000007FF
	ldrh r2, [r1]
	and r0, r2
	lsl r0, r0, #1
	ldr r2, _08037628 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _0803762C @ =0x00000474
	cmp r2, r0
	beq _08037630
	add r0, #0xA4
	cmp r2, r0
	beq _08037656
	b _0803767E
	.align 2, 0
_08037624: .4byte 0x000007FF
_08037628: .4byte gUnk_08622AB4
_0803762C: .4byte 0x00000474
_08037630:
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r4, r0, #0x1F
	add r0, r4, #0
	add r1, r2, #0
	bl sub_08009CAC
	lsl r1, r0, #5
	sub r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r0
	lsl r1, r1, #2
	mov r0, #0xFA
	lsl r0, r0, #2
	add r1, r1, r0
	add r0, r4, #0
	bl sub_08019980
	b _0803767E
_08037656:
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r1, r0, #0x1F
	mov r4, #1
	sub r4, r4, r1
	add r0, r1, #0
	add r1, r2, #0
	bl sub_08009CAC
	lsl r2, r0, #2
	add r2, r2, r0
	lsl r1, r2, #4
	sub r1, r1, r2
	lsl r1, r1, #2
	mov r2, #0xAF
	lsl r2, r2, #2
	add r1, r1, r2
	add r0, r4, #0
	bl sub_08019860
_0803767E:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_080375F8
	.align 2, 0

