	thumb_func_start sub_080220D4
sub_080220D4: @ 0x080220D4
	push {r4, r5, r6, lr}
	sub sp, #0x80
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r4, r1, #0
	ldr r2, _08022124 @ =0x020192E0
	ldr r0, _08022128 @ =0x00001B62
	add r5, r2, r0
	ldrb r3, [r5]
	cmp r3, #0
	bne _08022140
	ldr r6, _0802212C @ =0x00001B64
	add r0, r2, r6
	strh r3, [r0]
	cmp r1, #0
	beq _0802214A
	ldr r1, _08022130 @ =0x08081E34
	lsl r2, r4, #6
	ldr r0, _08022134 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl sub_080753F4
	ldr r0, _08022138 @ =0x00000206
	ldr r1, _0802213C @ =0x00000713
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _0802214C
	.align 2, 0
_08022124: .4byte 0x020192E0
_08022128: .4byte 0x00001B62
_0802212C: .4byte 0x00001B64
_08022130: .4byte gUnk_08081E34
_08022134: .4byte gUnk_0822C720
_08022138: .4byte 0x00000206
_0802213C: .4byte 0x00000713
_08022140:
	ldr r0, _08022154 @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _08022158 @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
_0802214A:
	mov r0, #1
_0802214C:
	add sp, #0x80
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08022154: .4byte 0x0201AE60
_08022158: .4byte 0x00001B64
	thumb_func_end sub_080220D4

