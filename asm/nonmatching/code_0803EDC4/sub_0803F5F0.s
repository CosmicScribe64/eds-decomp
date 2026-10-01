	thumb_func_start sub_0803F5F0
sub_0803F5F0: @ 0x0803F5F0
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _0803F630 @ =0x02017A40
	ldr r1, _0803F634 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _0803F644
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	mov r2, #0
	bl sub_080088A4
	cmp r0, #0
	beq _0803F688
	ldr r0, _0803F638 @ =0x00000206
	ldr r1, _0803F63C @ =0x00000712
	ldr r3, _0803F640 @ =0x08084290
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _0803F694
_0803F630: .4byte 0x02017A40
_0803F634: .4byte 0x000003E5
_0803F638: .4byte 0x00000206
_0803F63C: .4byte 0x00000712
_0803F640: .4byte gUnk_08084290
_0803F644:
	ldr r1, _0803F658 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803F65C
	mov r0, #0
	strb r0, [r5]
	b _0803F696
	.align 2, 0
_0803F658: .4byte 0x03000040
_0803F65C:
	mov r0, #0xE0
	bl sub_08052F38
	cmp r0, #0
	beq _0803F694
	ldr r0, _0803F68C @ =0x0201CFB0
	ldr r3, _0803F690 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803F694
_0803F688:
	mov r0, #1
	b _0803F696
_0803F68C: .4byte 0x0201CFB0
_0803F690: .4byte 0x00000824
_0803F694:
	mov r0, #0
_0803F696:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0803F5F0

