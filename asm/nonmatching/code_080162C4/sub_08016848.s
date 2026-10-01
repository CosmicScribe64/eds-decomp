	thumb_func_start sub_08016848
sub_08016848: @ 0x08016848
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	ldr r6, _08016864 @ =0x020185C0
	ldr r0, _08016868 @ =0x0000080A
	add r7, r6, r0
	ldrb r2, [r7]
	lsl r0, r2, #0x19
	lsr r5, r0, #0x19
	cmp r5, #0
	beq _0801686C
	cmp r5, #1
	beq _080168C4
	b _080169CC
	.align 2, 0
_08016864: .4byte 0x020185C0
_08016868: .4byte 0x0000080A
_0801686C:
	ldr r0, _080168AC @ =0x050003E0
	ldr r1, _080168B0 @ =0x0867F01C
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _080168B4 @ =0x06016C80
	lsl r1, r4, #9
	ldr r2, _080168B8 @ =0x0867F03C
	add r1, r1, r2
	mov r2, #0x80
	lsl r2, r2, #2
	bl sub_080752B0
	ldr r3, _080168BC @ =0x0000080C
	add r1, r6, r3
	ldr r0, _080168C0 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _08016A00
	.align 2, 0
_080168AC: .4byte 0x050003E0
_080168B0: .4byte gUnk_0867F01C
_080168B4: .4byte 0x06016C80
_080168B8: .4byte gUnk_0867F03C
_080168BC: .4byte 0x0000080C
_080168C0: .4byte 0xFFFFF01F
_080168C4:
	ldr r3, _08016994 @ =0x0000080C
	add r6, r6, r3
	ldrh r1, [r6]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x5F
	bgt _080169C0
	mov r2, #0
	add r3, r1, #0
	cmp r0, #0xF
	bgt _080168F8
	ldr r1, _08016998 @ =0x04000050
	mov r2, #0xF4
	lsl r2, r2, #4
	add r0, r2, #0
	strh r0, [r1]
	ldr r2, _0801699C @ =0x04000052
	lsl r1, r3, #0x14
	lsr r1, r1, #0x19
	mov r0, #0x10
	sub r0, r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	strh r1, [r2]
	mov r2, #1
_080168F8:
	lsl r0, r3, #0x14
	lsr r3, r0, #0x19
	cmp r3, #0x4F
	ble _08016922
	ldr r1, _08016998 @ =0x04000050
	mov r2, #0xF4
	lsl r2, r2, #4
	add r0, r2, #0
	strh r0, [r1]
	ldr r2, _0801699C @ =0x04000052
	mov r1, #0x60
	sub r1, r1, r3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r3, #0
	sub r0, #0x50
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	strh r1, [r2]
	mov r2, #1
_08016922:
	cmp r2, #0
	bne _0801692E
	ldr r0, _08016998 @ =0x04000050
	strh r2, [r0]
	add r0, #2
	strh r2, [r0]
_0801692E:
	ldr r0, _080169A0 @ =0x00400058
	mov r4, #0x81
	lsl r4, r4, #7
	ldr r2, _080169A4 @ =0x0000F364
	add r1, r4, #0
	bl sub_0807625C
	ldr r0, _080169A8 @ =0x00400078
	ldr r2, _080169AC @ =0x0000F36C
	add r1, r4, #0
	bl sub_0807625C
	ldr r1, _080169B0 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801695C
	ldr r0, _080169B4 @ =0x0201CFB0
	ldrb r0, [r0]
	and r5, r0
	cmp r5, #0
	beq _08016976
_0801695C:
	ldrh r2, [r6]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x57
	bgt _08016976
	add r0, #7
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #5
	ldr r1, _080169B8 @ =0xFFFFF01F
	and r1, r2
	orr r1, r0
	strh r1, [r6]
_08016976:
	ldr r2, _080169BC @ =0x020185C0
	ldr r3, _08016994 @ =0x0000080C
	add r2, r2, r3
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080169B8 @ =0xFFFFF01F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	b _08016A00
_08016994: .4byte 0x0000080C
_08016998: .4byte 0x04000050
_0801699C: .4byte 0x04000052
_080169A0: .4byte 0x00400058
_080169A4: .4byte 0x0000F364
_080169A8: .4byte 0x00400078
_080169AC: .4byte 0x0000F36C
_080169B0: .4byte 0x03000040
_080169B4: .4byte 0x0201CFB0
_080169B8: .4byte 0xFFFFF01F
_080169BC: .4byte 0x020185C0
_080169C0:
	mov r1, #2
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
_080169CC:
	ldr r2, _08016A08 @ =0x020192E0
	ldr r0, _08016A0C @ =0x00001B12
	add r2, r2, r0
	mov r0, #7
	and r4, r0
	lsl r0, r4, #2
	mov r1, #0x1D
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1B
	lsr r1, r1, #0x1D
	bl sub_08060964
	ldr r1, _08016A10 @ =0x020185C0
	ldr r0, _08016A14 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08016A00:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08016A08: .4byte 0x020192E0
_08016A0C: .4byte 0x00001B12
_08016A10: .4byte 0x020185C0
_08016A14: .4byte 0x0000080D
	thumb_func_end sub_08016848

