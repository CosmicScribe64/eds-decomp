	thumb_func_start sub_08030880
sub_08030880: @ 0x08030880
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	mov r6, #1
	lsr r2, r3, #0x1F
	ldrh r1, [r4, #2]
	lsl r5, r1, #0x16
	lsr r1, r5, #0x1A
	mov r0, #0x94
	mul r0, r1
	ldr r1, _080308C4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080308C8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803090E
	mov r2, #7
	ldrb r0, [r4, #0xA]
	and r2, r0
	cmp r2, #1
	bne _080308CC
	lsr r0, r3, #0x1F
	add r1, r0, #0
	lsr r2, r5, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	ldrh r2, [r4, #0xC]
	bl sub_08017B04
	b _0803090E
_080308C4: .4byte 0x00000D64
_080308C8: .4byte 0x0201930C
_080308CC:
	ldr r0, _08030918 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0803091C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08030920 @ =0x000003C2
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803090E
	cmp r2, #2
	bne _0803090E
	lsr r0, r3, #0x1F
	add r1, r0, #0
	lsr r2, r5, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	ldrh r2, [r4, #0xC]
	bl sub_08017B04
	ldrb r2, [r4, #2]
	and r6, r2
	mov r0, #0x87
	cmp r6, #0
	beq _08030900
	ldr r0, _08030924 @ =0x00008087
_08030900:
	ldrh r2, [r4, #2]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x1A
	ldrh r2, [r4, #0xE]
	mov r3, #0
	bl sub_0801EC58
_0803090E:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08030918: .4byte 0x000007FF
_0803091C: .4byte gUnk_08622AB4
_08030920: .4byte 0x000003C2
_08030924: .4byte 0x00008087
	thumb_func_end sub_08030880

