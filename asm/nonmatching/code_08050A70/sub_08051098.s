	thumb_func_start sub_08051098
sub_08051098: @ 0x08051098
	push {r4, r5, lr}
	ldr r1, _080510B0 @ =0x02017FB0
	ldr r0, _080510B4 @ =0x0000048E
	add r5, r1, r0
	ldrb r0, [r5]
	cmp r0, #0
	beq _080510B8
	cmp r0, #1
	beq _08051110
_080510AA:
	mov r0, #1
	b _08051132
	.align 2, 0
_080510B0: .4byte 0x02017FB0
_080510B4: .4byte 0x0000048E
_080510B8:
	ldr r2, _080510F8 @ =0x0000045C
	add r0, r1, r2
	ldrh r0, [r0]
	bl sub_08047058
	ldr r4, _080510FC @ =0x02017A40
	ldr r3, _08051100 @ =0x000003D6
	add r1, r4, r3
	strh r0, [r1]
	lsl r0, r0, #0x10
	cmp r0, #0
	blt _080510AA
	ldr r0, _08051104 @ =0x00000484
	add r3, r4, r0
	ldr r2, _08051108 @ =0x0819A9D4
	mov r0, #0
	ldsh r1, [r1, r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r2, #0x14
	add r0, r0, r2
	ldr r0, [r0]
	str r0, [r3]
	cmp r0, #0
	beq _080510AA
	ldr r1, _0805110C @ =0x000003E5
	add r0, r4, r1
	mov r1, #0
	strb r1, [r0]
	b _0805112A
	.align 2, 0
_080510F8: .4byte 0x0000045C
_080510FC: .4byte 0x02017A40
_08051100: .4byte 0x000003D6
_08051104: .4byte 0x00000484
_08051108: .4byte gUnk_0819A9D4
_0805110C: .4byte 0x000003E5
_08051110:
	ldr r2, _08051138 @ =0x02017A40
	ldr r3, _0805113C @ =0x00000484
	add r2, r2, r3
	sub r3, #0x28
	add r0, r1, r3
	add r3, #0x14
	add r1, r1, r3
	ldr r2, [r2]
	bl _call_via_r2
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08051130
_0805112A:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_08051130:
	mov r0, #0
_08051132:
	pop {r4, r5}
	pop {r1}
	bx r1
_08051138: .4byte 0x02017A40
_0805113C: .4byte 0x00000484
	thumb_func_end sub_08051098

