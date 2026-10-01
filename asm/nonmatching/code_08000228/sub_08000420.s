	thumb_func_start sub_08000420
sub_08000420: @ 0x08000420
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	add r7, r0, #0
	mov r8, r1
	add r6, r2, #0
	mov r9, r3
	ldr r0, [sp, #0x20]
	lsl r0, r0, #0x18
	ldr r1, _080004C4 @ =0x08139F5C
	lsr r0, r0, #0x16
	add r0, r0, r1
	ldr r1, [r0]
	add r0, r1, #4
	ldr r4, _080004C8 @ =0x0203A614
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	add r1, r4, #0
	bl sub_0807A1A8
	str r4, [r6, #0xC]
	ldr r1, _080004CC @ =0x06005A00
	mov r5, #0xF0
	lsl r5, r5, #5
	add r0, r4, #0
	add r2, r5, #0
	bl CpuSet
	ldr r1, _080004D0 @ =0x0600FA00
	add r0, r4, #0
	add r2, r5, #0
	bl CpuSet
	ldr r1, [r7]
	add r0, r1, #4
	ldr r5, _080004D4 @ =0x02031014
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	add r1, r5, #0
	bl sub_0807A1A8
	str r5, [r6]
	mov r4, #0xB4
	lsl r4, r4, #7
	mov r0, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080008A4
	mov r0, #1
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080008A4
	ldr r0, [r7, #4]
	str r0, [r6, #4]
	ldr r0, [r7, #8]
	str r0, [r6, #8]
	ldr r1, [r7, #0xC]
	cmp r1, #0
	beq _080004DC
	add r0, r1, #4
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	add r1, r5, #0
	bl sub_0807A1A8
	ldr r1, _080004D8 @ =0x06014000
	add r0, r5, #0
	mov r2, #0x10
	bl sub_08077CEC
	b _080004E8
	.align 2, 0
_080004C4: .4byte gUnk_08139F5C
_080004C8: .4byte 0x0203A614
_080004CC: .4byte 0x06005A00
_080004D0: .4byte 0x0600FA00
_080004D4: .4byte 0x02031014
_080004D8: .4byte 0x06014000
_080004DC:
	mov r0, sp
	strh r1, [r0]
	ldr r1, _08000554 @ =0x06014000
	ldr r2, _08000558 @ =0x01002000
	bl CpuSet
_080004E8:
	ldr r0, [r7, #0x10]
	str r0, [r6, #0x10]
	mov r0, r8
	str r0, [r6, #0x14]
	ldr r0, [r7, #4]
	str r0, [r6, #4]
	cmp r0, #0
	beq _08000502
	add r0, #0x20
	ldr r1, _0800055C @ =0x05000020
	mov r2, #0xF8
	bl CpuSet
_08000502:
	ldr r0, [r7, #8]
	str r0, [r6, #8]
	cmp r0, #0
	beq _08000514
	ldr r1, _08000560 @ =0x05000200
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
_08000514:
	ldr r0, [r7, #0x10]
	str r0, [r6, #0x10]
	cmp r0, #0
	beq _08000538
	mov r1, r9
	bl sub_08078670
	ldr r2, _08000564 @ =0x02013DE0
	ldr r1, _08000568 @ =0x000009A4
	add r2, r2, r1
	mov r1, #7
	and r0, r1
	mov r1, #8
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
_08000538:
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r1, _0800056C @ =0x0874E304
	mov r2, #0x20
	bl sub_08075294
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08000554: .4byte 0x06014000
_08000558: .4byte 0x01002000
_0800055C: .4byte 0x05000020
_08000560: .4byte 0x05000200
_08000564: .4byte 0x02013DE0
_08000568: .4byte 0x000009A4
_0800056C: .4byte gUnk_0874E304
	thumb_func_end sub_08000420

