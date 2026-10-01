	thumb_func_start sub_08047058
sub_08047058: @ 0x08047058
	push {r4, r5, r6, lr}
	mov r5, #0
	ldr r4, _08047088 @ =0x000001A9
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0804708C @ =0x08622AB4
	add r0, r0, r1
	ldrh r3, [r0]
	ldr r6, _08047090 @ =0x0819A9D4
_0804706A:
	add r0, r5, r4
	lsr r1, r0, #0x1F
	add r0, r0, r1
	asr r2, r0, #1
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #3
	add r0, r0, r6
	ldrh r0, [r0]
	add r1, r0, #0
	cmp r3, r0
	bne _08047094
	add r0, r2, #0
	b _080470BA
	.align 2, 0
_08047088: .4byte 0x000001A9
_0804708C: .4byte gUnk_08622AB4
_08047090: .4byte gUnk_0819A9D4
_08047094:
	cmp r5, r4
	bne _0804709E
	mov r0, #1
	neg r0, r0
	b _080470BA
_0804709E:
	cmp r3, r0
	bls _080470A4
	add r5, r2, #0
_080470A4:
	cmp r3, r1
	bcs _080470AA
	add r4, r2, #0
_080470AA:
	add r0, r5, r4
	lsr r1, r0, #0x1F
	add r0, r0, r1
	asr r0, r0, #1
	cmp r0, r2
	bne _0804706A
	add r5, r4, #0
	b _0804706A
_080470BA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08047058

