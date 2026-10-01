	thumb_func_start sub_08060160
sub_08060160: @ 0x08060160
	push {r4, r5, r6, lr}
	ldr r1, _08060170 @ =0x0201AE60
	ldrh r0, [r1, #4]
	cmp r0, #0
	beq _08060174
	cmp r0, #1
	beq _080601C4
	b _08060294
_08060170: .4byte 0x0201AE60
_08060174:
	ldrh r2, [r1, #0xC]
	ldrh r3, [r1, #8]
	add r0, r2, r3
	sub r0, #2
	lsl r5, r0, #3
	add r0, r1, #0
	add r0, #0x21
	ldrb r0, [r0]
	sub r0, #4
	lsl r0, r0, #0x13
	orr r5, r0
	ldr r2, _080601B4 @ =0x081A423C
	ldr r0, _080601B8 @ =0x03000040
	ldr r1, _080601BC @ =0x0000485E
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r0, r0, #2
	mov r1, #0x1F
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r2, r0, #0x12
	ldr r3, _080601C0 @ =0x42E40000
	add r2, r2, r3
	lsr r2, r2, #0x10
	add r0, r5, #0
	mov r1, #0x40
	bl sub_080761F0
	b _08060294
	.align 2, 0
_080601B4: .4byte gUnk_081A423C
_080601B8: .4byte 0x03000040
_080601BC: .4byte 0x0000485E
_080601C0: .4byte 0x42E40000
_080601C4:
	ldrh r2, [r1, #0xC]
	lsr r0, r2, #1
	ldrh r3, [r1, #8]
	add r0, r3, r0
	lsl r5, r0, #3
	sub r5, #0x30
	add r0, r1, #0
	add r0, #0x21
	ldrb r0, [r0]
	sub r0, #4
	lsl r6, r0, #3
	ldrh r0, [r1, #0x12]
	cmp r0, #0
	bne _08060204
	ldr r2, _080601F8 @ =0x081A4424
	ldr r0, _080601FC @ =0x03000040
	ldr r1, _08060200 @ =0x0000485E
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r0, r0, #2
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r3, [r0]
	b _08060208
_080601F8: .4byte gUnk_081A4424
_080601FC: .4byte 0x03000040
_08060200: .4byte 0x0000485E
_08060204:
	mov r3, #0x80
	lsl r3, r3, #1
_08060208:
	lsl r4, r6, #0x10
	orr r5, r4
	mov r1, #0x81
	lsl r1, r1, #7
	ldr r2, _08060248 @ =0x0000430C
	lsl r3, r3, #0x10
	add r0, r5, #0
	bl sub_08076714
	ldr r1, _0806024C @ =0x0201AE60
	ldrh r2, [r1, #0xC]
	lsr r0, r2, #1
	ldrh r3, [r1, #8]
	add r0, r3, r0
	lsl r5, r0, #3
	add r5, #0x10
	ldrh r1, [r1, #0x12]
	cmp r1, #1
	bne _0806025C
	ldr r2, _08060250 @ =0x081A4424
	ldr r0, _08060254 @ =0x03000040
	ldr r1, _08060258 @ =0x0000485E
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r0, r0, #2
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r3, [r0]
	b _08060260
	.align 2, 0
_08060248: .4byte 0x0000430C
_0806024C: .4byte 0x0201AE60
_08060250: .4byte gUnk_081A4424
_08060254: .4byte 0x03000040
_08060258: .4byte 0x0000485E
_0806025C:
	mov r3, #0x80
	lsl r3, r3, #1
_08060260:
	orr r5, r4
	mov r1, #0x81
	lsl r1, r1, #7
	ldr r2, _0806029C @ =0x00004314
	lsl r3, r3, #0x10
	add r0, r5, #0
	bl sub_08076714
	ldr r1, _080602A0 @ =0x0201AE60
	ldrh r2, [r1, #0xC]
	lsr r0, r2, #1
	ldrh r3, [r1, #8]
	add r0, r3, r0
	lsl r5, r0, #3
	sub r5, #0x30
	ldrh r1, [r1, #0x12]
	lsl r0, r1, #6
	add r5, r5, r0
	add r6, #8
	lsl r0, r6, #0x10
	orr r5, r0
	add r0, r5, #0
	mov r1, #0x80
	mov r2, #0
	bl sub_080761F0
_08060294:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0806029C: .4byte 0x00004314
_080602A0: .4byte 0x0201AE60
	thumb_func_end sub_08060160

