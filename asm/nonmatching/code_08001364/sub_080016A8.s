	thumb_func_start sub_080016A8
sub_080016A8: @ 0x080016A8
	push {r4, r5, r6, lr}
	sub sp, #4
	mov r5, #1
	ldr r1, _080016F0 @ =0xFFFFFE80
	ldr r4, _080016F4 @ =0x020150CC
	mov r0, #0
	mov r2, #0
	add r3, r4, #0
	bl sub_080787F4
	sub r0, r4, #1
	ldrb r0, [r0]
	cmp r0, #0x23
	bgt _08001704
	cmp r0, #0x20
	blt _08001704
	bl sub_08001C78
	add r1, r4, #0
	add r1, #0x80
	ldrh r2, [r1]
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #6
	add r1, r1, r2
	lsl r1, r1, #2
	ldr r2, _080016F8 @ =0x0813ADF8
	add r1, r1, r2
	ldr r3, _080016FC @ =0xFFFFF6C0
	add r2, r4, r3
	ldr r6, _08001700 @ =0xFFFFFDD4
	add r3, r4, r6
	str r5, [sp, #0]
	bl sub_08000570
	b _08001736
_080016F0: .4byte 0xFFFFFE80
_080016F4: .4byte 0x020150CC
_080016F8: .4byte gUnk_0813ADF8
_080016FC: .4byte 0xFFFFF6C0
_08001700: .4byte 0xFFFFFDD4
_08001704:
	ldr r4, _08001754 @ =0x02013DE0
	ldr r1, _08001758 @ =0x000012EB
	add r0, r4, r1
	ldrb r0, [r0]
	bl sub_08001C78
	ldr r2, _0800175C @ =0x0000136C
	add r1, r4, r2
	ldrh r2, [r1]
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #6
	add r1, r1, r2
	lsl r1, r1, #2
	ldr r2, _08001760 @ =0x0813ADF8
	add r1, r1, r2
	ldr r3, _08001764 @ =0x000009AC
	add r2, r4, r3
	mov r6, #0x86
	lsl r6, r6, #5
	add r4, r4, r6
	str r5, [sp, #0]
	add r3, r4, #0
	bl sub_08000420
_08001736:
	bl sub_08000A28
	ldr r0, _08001768 @ =0x0201478C
	bl sub_08000944
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _0800176C @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #1
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08001754: .4byte 0x02013DE0
_08001758: .4byte 0x000012EB
_0800175C: .4byte 0x0000136C
_08001760: .4byte gUnk_0813ADF8
_08001764: .4byte 0x000009AC
_08001768: .4byte 0x0201478C
_0800176C: .4byte 0x00001F04
	thumb_func_end sub_080016A8

