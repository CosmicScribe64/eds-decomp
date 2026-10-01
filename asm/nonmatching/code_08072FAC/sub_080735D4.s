	thumb_func_start sub_080735D4
sub_080735D4: @ 0x080735D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r9, r0
	mov sl, r1
	ldr r3, _08073700 @ =0x04000208
	mov r0, #0
	strh r0, [r3]
	ldr r2, _08073704 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08073708 @ =0x0000FF3F
	and r0, r1
	strh r0, [r2]
	mov r0, #1
	strh r0, [r3]
	mov r0, #0
	str r0, [sp, #0]
	ldr r4, _0807370C @ =0x03005B60
	ldr r2, _08073710 @ =0x050002CE
	mov r0, sp
	add r1, r4, #0
	bl CpuSet
	mov r0, #0xA3
	lsl r0, r0, #4
	add r1, r4, r0
	ldr r2, _08073714 @ =0x00000A54
	add r0, r4, r2
	str r0, [r1]
	ldr r6, _08073718 @ =0x00000A34
	add r1, r4, r6
	add r2, #0x30
	add r0, r4, r2
	str r0, [r1]
	add r6, #4
	add r1, r4, r6
	add r2, #0x30
	add r0, r4, r2
	str r0, [r1]
	mov r2, #0
	sub r6, #0x10
	add r6, r6, r4
	mov r8, r6
	ldr r0, _0807371C @ =0x00000A18
	add r0, r0, r4
	mov ip, r0
	mov r3, #0
	ldr r1, _08073720 @ =0x00000A1A
	add r7, r4, r1
	mov r6, #0xFF
	add r5, r4, #0
_08073640:
	mov r0, r8
	add r1, r2, r0
	ldrb r0, [r1]
	orr r0, r6
	strb r0, [r1]
	mov r1, ip
	add r0, r2, r1
	strb r3, [r0]
	add r0, r2, r7
	strb r3, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #1
	bls _08073640
	ldr r2, _08073724 @ =0x00000A26
	add r0, r5, r2
	mov r3, #0
	strh r3, [r0]
	ldr r4, _08073728 @ =0x04000134
	mov r6, #0xC0
	lsl r6, r6, #8
	add r0, r6, #0
	strh r0, [r4]
	ldr r2, _0807372C @ =0x04000128
	mov r1, #0x80
	lsl r1, r1, #5
	add r0, r1, #0
	strh r0, [r2]
	strh r3, [r2]
	mov r0, #3
	strh r0, [r2]
	ldrh r0, [r2]
	mov r6, #0x80
	lsl r6, r6, #6
	add r1, r6, #0
	orr r0, r1
	strh r0, [r2]
	strh r3, [r4]
	ldr r0, _08073730 @ =0x00000A2C
	add r1, r5, r0
	mov r0, #0xC
	str r0, [r1]
	mov r6, #0xA4
	lsl r6, r6, #4
	add r1, r5, r6
	mov r0, #0x80
	lsl r0, r0, #5
	strh r0, [r1]
	mov r0, r9
	str r0, [r5]
	mov r1, sl
	str r1, [r5, #4]
	add r4, #0xD4
	strh r3, [r4]
	ldr r3, _08073704 @ =0x04000200
	ldrh r0, [r3]
	mov r1, #0x80
	orr r0, r1
	strh r0, [r3]
	ldr r0, _08073734 @ =0x08075F75
	mov r6, r9
	str r0, [r6]
	mov r1, sl
	str r0, [r1]
	ldrh r0, [r2]
	mov r6, #0x80
	lsl r6, r6, #7
	add r1, r6, #0
	orr r0, r1
	strh r0, [r2]
	mov r2, #1
	strh r2, [r4]
	ldr r0, _08073738 @ =0x00000B0C
	add r1, r5, r0
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0
	bne _080736F0
	strh r0, [r4]
	ldrh r0, [r3]
	mov r1, #0x40
	orr r0, r1
	strh r0, [r3]
	strh r2, [r4]
_080736F0:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08073700: .4byte 0x04000208
_08073704: .4byte 0x04000200
_08073708: .4byte 0x0000FF3F
_0807370C: .4byte 0x03005B60
_08073710: .4byte 0x050002CE
_08073714: .4byte 0x00000A54
_08073718: .4byte 0x00000A34
_0807371C: .4byte 0x00000A18
_08073720: .4byte 0x00000A1A
_08073724: .4byte 0x00000A26
_08073728: .4byte 0x04000134
_0807372C: .4byte 0x04000128
_08073730: .4byte 0x00000A2C
_08073734: .4byte sub_08075F74
_08073738: .4byte 0x00000B0C
	thumb_func_end sub_080735D4

