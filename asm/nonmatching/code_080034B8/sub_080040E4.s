	thumb_func_start sub_080040E4
sub_080040E4: @ 0x080040E4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1C
	str r0, [sp, #4]
	add r5, r1, #0
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #0x16
	lsr r7, r0, #0x10
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0
	mov r2, #0x20
	mov r3, #0x20
	bl sub_08073500
	add r4, r5, #3
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	mov r1, #0
	mov r2, #0x20
	mov r3, #0x20
	bl sub_08073500
	add r3, r7, #0
	add r3, #0xA0
	ldr r0, [sp, #4]
	cmp r0, #3
	bgt _08004130
	ldr r0, _0800412C @ =0x087E52A4
	b _08004132
	.align 2, 0
_0800412C: .4byte gUnk_087E52A4
_08004130:
	ldr r0, _08004208 @ =0x087E5CF4
_08004132:
	str r0, [sp, #0]
	add r0, r4, #0
	mov r1, #0x63
	mov r2, #0x30
	bl sub_0807332C
	add r6, r5, #1
	add r3, r7, #0
	add r3, #0xC0
	ldr r0, _0800420C @ =0x08198604
	ldr r1, [sp, #4]
	lsl r4, r1, #2
	add r0, r4, r0
	ldr r0, [r0]
	str r0, [sp, #0]
	add r0, r6, #0
	mov r1, #0x63
	mov r2, #0x40
	bl sub_0807332C
	mov r2, #4
	str r2, [sp, #0xC]
	str r4, [sp, #0x14]
	ldr r0, [sp, #4]
	cmp r0, #3
	bgt _0800416A
	mov r1, #5
	str r1, [sp, #0xC]
_0800416A:
	mov r2, #0
	str r2, [sp, #8]
	ldr r0, [sp, #0xC]
	cmp r2, r0
	bge _0800426C
	lsl r0, r5, #2
	add r0, r0, r5
	add r0, #5
	lsl r1, r6, #0x10
	str r1, [sp, #0x10]
	mov r2, #0xC6
	lsl r2, r2, #0xF
	str r2, [sp, #0x18]
	add r7, #0xE0
	mov r8, r7
	mov r1, #0x80
	lsl r1, r1, #0xB
	mov sl, r1
	mov r2, #5
	mov r9, r2
	lsl r7, r0, #4
_08004194:
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #4]
	add r0, r1, r2
	ldr r1, [sp, #8]
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r4, r5, #1
	add r0, r4, #0
	bl sub_08063DAC
	cmp r0, #0
	beq _08004220
	mov r2, sl
	lsr r1, r2, #0x10
	lsl r1, r1, #5
	add r1, #4
	ldr r2, _08004210 @ =0x081985A0
	lsl r0, r5, #2
	add r0, r0, r2
	ldr r0, [r0]
	str r0, [sp, #0]
	add r0, r6, #0
	add r2, r7, #0
	mov r3, r8
	bl sub_0807332C
	ldr r0, _08004214 @ =0x02011C20
	lsl r4, r4, #2
	add r4, r4, r0
	ldr r0, _08004218 @ =0x000020D0
	add r5, r4, r0
	ldrh r1, [r5]
	lsl r3, r1, #0x15
	lsr r3, r3, #0x15
	add r0, r6, #0
	mov r1, #0xD
	mov r2, r9
	bl sub_0800407C
	ldr r2, _0800421C @ =0x000020D2
	add r4, r4, r2
	ldrh r4, [r4]
	lsr r3, r4, #6
	add r0, r6, #0
	mov r1, #0x13
	mov r2, r9
	bl sub_0800407C
	ldr r3, [r5]
	lsl r3, r3, #0xA
	lsr r3, r3, #0x15
	add r0, r6, #0
	mov r1, #0x19
	mov r2, r9
	bl sub_0800407C
	b _08004246
_08004208: .4byte gUnk_087E5CF4
_0800420C: .4byte gUnk_08198604
_08004210: .4byte gUnk_081985A0
_08004214: .4byte 0x02011C20
_08004218: .4byte 0x000020D0
_0800421C: .4byte 0x000020D2
_08004220:
	mov r0, sl
	lsr r1, r0, #0x10
	lsl r1, r1, #5
	add r1, #4
	ldr r0, _0800427C @ =0x087E77B4
	str r0, [sp, #0]
	add r0, r6, #0
	add r2, r7, #0
	mov r3, r8
	bl sub_0807332C
	ldr r2, [sp, #0x18]
	lsr r1, r2, #0x10
	ldr r2, [sp, #0x10]
	lsr r0, r2, #0x10
	mov r2, #8
	mov r3, #1
	bl sub_08073500
_08004246:
	ldr r0, [sp, #0x18]
	mov r1, #0xC0
	lsl r1, r1, #0xF
	add r0, r0, r1
	str r0, [sp, #0x18]
	mov r2, #0xC
	add r8, r2
	mov r0, #0xC0
	lsl r0, r0, #0xA
	add sl, r0
	mov r1, #3
	add r9, r1
	add r7, #0x10
	ldr r2, [sp, #8]
	add r2, #1
	str r2, [sp, #8]
	ldr r0, [sp, #0xC]
	cmp r2, r0
	blt _08004194
_0800426C:
	add sp, #0x1C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800427C: .4byte gUnk_087E77B4
	thumb_func_end sub_080040E4

