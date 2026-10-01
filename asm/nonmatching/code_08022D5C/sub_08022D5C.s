	thumb_func_start sub_08022D5C
sub_08022D5C: @ 0x08022D5C
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	ldr r1, _08022D80 @ =0x02017FB0
	ldrh r6, [r1, #2]
	ldrh r4, [r1, #4]
	ldrh r3, [r1, #6]
	ldrh r5, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #8
	add r0, r6, #0
	and r0, r2
	add r7, r1, #0
	cmp r0, #0
	beq _08022D88
	ldr r0, _08022D84 @ =0x00007FFF
	and r6, r0
	b _08022D8A
	.align 2, 0
_08022D80: .4byte 0x02017FB0
_08022D84: .4byte 0x00007FFF
_08022D88:
	orr r6, r2
_08022D8A:
	ldr r0, _08022DA0 @ =0x00000FFF
	and r0, r6
	sub r0, #4
	cmp r0, #0xDA
	bls _08022D96
	b _080231FC
_08022D96:
	lsl r0, r0, #2
	ldr r1, _08022DA4 @ =0x08022DA8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08022DA0: .4byte 0x00000FFF
_08022DA4: .4byte 0x08022DA8
_08022DA8:
	.4byte _080231E2
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023114
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023160
	.4byte _08023166
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023176
	.4byte _08023176
	.4byte _08023176
	.4byte _08023176
	.4byte _08023176
	.4byte _080231FC
	.4byte _08023176
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231A2
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023176
	.4byte _08023176
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _0802311A
	.4byte _0802311A
	.4byte _0802311A
	.4byte _0802313A
	.4byte _0802313A
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231CE
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231A2
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023176
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023176
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023176
	.4byte _08023176
	.4byte _08023176
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023176
	.4byte _080231FC
	.4byte _080231FC
	.4byte _080231FC
	.4byte _08023176
	.4byte _08023176
	.4byte _08023176
	.4byte _08023176
_08023114:
	mov r0, #1
	sub r0, r0, r4
	b _080231DC
_0802311A:
	mov r2, #1
	sub r0, r2, r4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r4, #8
	lsl r1, r1, #8
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	sub r2, r2, r3
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsr r0, r3, #8
	lsl r3, r0, #8
	orr r3, r2
	b _080231FC
_0802313A:
	mov r2, #1
	sub r0, r2, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r3, #8
	lsl r1, r1, #8
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	lsl r0, r5, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	blt _080231FC
	cmp r0, #2
	ble _0802315C
	cmp r0, #5
	bne _080231FC
_0802315C:
	sub r0, r2, r4
	b _080231D2
_08023160:
	ldrh r4, [r7, #6]
	ldrh r3, [r7, #4]
	b _080231FC
_08023166:
	ldrh r4, [r7, #6]
	ldrh r3, [r7, #4]
	ldrh r0, [r7, #8]
	lsr r1, r0, #8
	lsl r0, r0, #0x18
	lsr r5, r0, #0x10
	orr r5, r1
	b _080231FC
_08023176:
	lsl r0, r3, #0x10
	orr r0, r4
	str r0, [sp, #0]
	mov r3, sp
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #4
	ldrb r2, [r3, #1]
	mov r0, #0x11
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #1]
	ldr r1, [sp, #0]
	lsl r0, r1, #0x10
	lsr r4, r0, #0x10
	lsr r3, r1, #0x10
	b _080231FC
_080231A2:
	lsl r0, r5, #0x10
	orr r0, r3
	str r0, [sp, #4]
	add r3, sp, #4
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #4
	ldrb r2, [r3, #1]
	mov r0, #0x11
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #1]
	ldr r1, [sp, #4]
	lsl r0, r1, #0x10
	lsr r3, r0, #0x10
	lsr r5, r1, #0x10
	b _080231FC
_080231CE:
	mov r0, #1
	sub r0, r0, r4
_080231D2:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r4, #8
	lsl r1, r1, #8
	orr r0, r1
_080231DC:
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	b _080231FC
_080231E2:
	ldrh r1, [r7, #4]
	cmp r1, #1
	bhi _080231F0
	mov r0, #1
	sub r0, r0, r1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_080231F0:
	ldr r0, _0802321C @ =0x00000306
	add r1, r7, r0
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_080231FC:
	ldr r1, _08023220 @ =0x00000484
	add r0, r7, r1
	strh r6, [r0]
	ldr r2, _08023224 @ =0x00000486
	add r0, r7, r2
	strh r4, [r0]
	add r1, #4
	add r0, r7, r1
	strh r3, [r0]
	add r2, #4
	add r0, r7, r2
	strh r5, [r0]
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0802321C: .4byte 0x00000306
_08023220: .4byte 0x00000484
_08023224: .4byte 0x00000486
	thumb_func_end sub_08022D5C

