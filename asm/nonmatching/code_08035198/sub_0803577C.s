	thumb_func_start sub_0803577C
sub_0803577C: @ 0x0803577C
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r6, r1, #0
	ldr r1, _080357B8 @ =0x0201D810
	ldrb r0, [r1, #5]
	lsl r3, r0, #0x1E
	lsr r0, r3, #0x1E
	ldrh r2, [r1, #6]
	add r0, r0, r2
	lsl r0, r0, #2
	add r7, r1, #0
	add r7, #0xC
	add r5, r0, r7
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08035860
	ldr r0, _080357BC @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7E
	beq _08035824
	cmp r0, #0x7E
	bgt _080357C0
	cmp r0, #0x7D
	beq _0803583E
	b _08035860
_080357B8: .4byte 0x0201D810
_080357BC: .4byte 0x02017A40
_080357C0:
	cmp r0, #0x7F
	beq _08035800
	cmp r0, #0x80
	bne _08035860
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0
	bl sub_0802E6F8
	cmp r0, #0
	beq _08035860
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _080357F8 @ =0x000007FF
	ldrh r4, [r4]
	and r2, r4
	lsl r2, r2, #1
	ldr r3, _080357FC @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7F
	b _08035862
_080357F8: .4byte 0x000007FF
_080357FC: .4byte gUnk_08622AB4
_08035800:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0x65
	cmp r0, #0
	beq _0803580E
	ldr r3, _08035820 @ =0x00008065
_0803580E:
	ldrh r1, [r5]
	ldrh r2, [r5, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x7E
	b _08035862
	.align 2, 0
_08035820: .4byte 0x00008065
_08035824:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	lsr r1, r3, #0x1E
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r7
	mov r2, #1
	mov r3, #0
	bl sub_08056094
	mov r0, #0x7D
	b _08035862
_0803583E:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _0803584C
	ldr r1, _0803585C @ =0x00008060
_0803584C:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x64
	b _08035862
_0803585C: .4byte 0x00008060
_08035860:
	mov r0, #0
_08035862:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803577C

