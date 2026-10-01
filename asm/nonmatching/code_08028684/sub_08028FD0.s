	thumb_func_start sub_08028FD0
sub_08028FD0: @ 0x08028FD0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x10
	ldr r6, _08029024 @ =0x02020310
	ldr r0, _08029028 @ =0x00000B1C
	add r0, r0, r6
	mov r8, r0
	ldr r1, _0802902C @ =0x00000B0D
	add r0, r6, r1
	ldrb r2, [r0]
	cmp r2, #0
	beq _08028FEC
	b _08029142
_08028FEC:
	mov r3, #0xAB
	lsl r3, r3, #4
	add r0, r6, r3
	ldrb r0, [r0]
	cmp r0, #0x30
	beq _08028FFA
	b _08029142
_08028FFA:
	ldr r1, _08029030 @ =0x03000040
	mov r0, #1
	ldrh r3, [r1, #6]
	and r0, r3
	cmp r0, #0
	beq _080290C0
	ldr r3, _08029034 @ =0x00000ABE
	add r0, r6, r3
	ldrb r0, [r0]
	cmp r0, #2
	bne _08029068
	ldr r1, _08029038 @ =0x00000B0E
	add r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #1
	bne _08029040
	ldr r2, _0802903C @ =0x00000B1D
	add r1, r6, r2
	mov r0, #1
	b _08029056
	.align 2, 0
_08029024: .4byte 0x02020310
_08029028: .4byte 0x00000B1C
_0802902C: .4byte 0x00000B0D
_08029030: .4byte 0x03000040
_08029034: .4byte 0x00000ABE
_08029038: .4byte 0x00000B0E
_0802903C: .4byte 0x00000B1D
_08029040:
	ldr r3, _0802905C @ =0x00000AB1
	add r1, r6, r3
	mov r0, #0xFC
	strb r0, [r1]
	ldr r1, _08029060 @ =0x00000ABD
	add r0, r6, r1
	strb r2, [r0]
	ldr r2, _08029064 @ =0x00000AF5
	add r1, r6, r2
	ldrb r0, [r1]
	sub r0, #1
_08029056:
	strb r0, [r1]
	b _080290AA
	.align 2, 0
_0802905C: .4byte 0x00000AB1
_08029060: .4byte 0x00000ABD
_08029064: .4byte 0x00000AF5
_08029068:
	cmp r0, #0
	bne _080290C0
	ldr r3, _080290B4 @ =0x00000ACC
	add r2, r6, r3
	mov r4, #0
	strh r0, [r2]
	mov r0, #0xAC
	lsl r0, r0, #4
	add r1, r6, r0
	mov r0, #7
	strh r0, [r1]
	add r3, #0x29
	add r1, r6, r3
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	ldr r1, _080290B8 @ =0x04000052
	mov r0, #0x10
	strh r0, [r1]
	add r1, #2
	mov r0, #8
	strh r0, [r1]
	sub r1, #4
	mov r3, #0x88
	lsl r3, r3, #3
	add r0, r3, #0
	strh r0, [r1]
	ldrh r0, [r2]
	bl sub_0807B4A8
	ldr r1, _080290BC @ =0x00000ABF
	add r0, r6, r1
	strb r4, [r0]
_080290AA:
	mov r0, #1
	bl sub_08077AEC
	b _08029142
	.align 2, 0
_080290B4: .4byte 0x00000ACC
_080290B8: .4byte 0x04000052
_080290BC: .4byte 0x00000ABF
_080290C0:
	ldr r4, _080290E8 @ =0x02020310
	ldr r2, _080290EC @ =0x00000ABE
	add r0, r4, r2
	ldrb r2, [r0]
	cmp r2, #1
	bne _08029142
	ldr r3, _080290F0 @ =0x00000B0E
	add r0, r4, r3
	ldrb r0, [r0]
	cmp r0, #1
	bne _080290F8
	ldr r1, _080290F4 @ =0x00000B0D
	add r0, r4, r1
	strb r2, [r0]
	mov r2, #0xB1
	lsl r2, r2, #4
	add r0, r6, r2
	bl sub_0807BCF4
	b _08029120
_080290E8: .4byte 0x02020310
_080290EC: .4byte 0x00000ABE
_080290F0: .4byte 0x00000B0E
_080290F4: .4byte 0x00000B0D
_080290F8:
	ldrh r1, [r1, #6]
	and r2, r1
	cmp r2, #0
	beq _08029120
	ldr r3, _080291F4 @ =0x00000AF4
	add r0, r4, r3
	ldrb r0, [r0]
	bl sub_080289DC
	ldr r2, _080291F8 @ =0x00000ABF
	add r1, r4, r2
	strb r0, [r1]
	ldr r3, _080291FC @ =0x00000AF5
	add r1, r4, r3
	ldrb r0, [r1]
	add r0, #2
	strb r0, [r1]
	mov r0, #1
	bl sub_08077AEC
_08029120:
	mov r0, #3
	str r0, [sp, #0]
	mov r0, #1
	str r0, [sp, #4]
	ldr r4, _08029200 @ =0x02020DEC
	str r4, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xC]
	mov r1, #0
	mov r2, #0x40
	mov r3, #0xF
	bl sub_0807B9D4
	add r4, #0x19
	add r0, r4, #0
	bl sub_08029458
_08029142:
	ldr r4, _08029204 @ =0x02020310
	ldr r0, _08029208 @ =0x00000B0D
	add r5, r4, r0
	ldrb r1, [r5]
	cmp r1, #1
	bne _0802918A
	bl sub_08027C90
	ldr r2, _0802920C @ =0x00000AAE
	add r0, r6, r2
	ldrb r1, [r0]
	mov r3, #0xB1
	lsl r3, r3, #4
	add r2, r6, r3
	mov r0, #0x52
	bl sub_0807BCFC
	cmp r0, #0
	beq _0802918A
	ldr r0, _08029210 @ =0x00000B16
	add r1, r4, r0
	mov r0, #1
	ldrb r1, [r1]
	eor r0, r1
	ldr r2, _080291F8 @ =0x00000ABF
	add r1, r4, r2
	mov r2, #0
	strb r0, [r1]
	ldr r3, _080291FC @ =0x00000AF5
	add r1, r4, r3
	ldrb r0, [r1]
	add r0, #2
	strb r0, [r1]
	strb r2, [r5]
	bl sub_08027CA4
_0802918A:
	ldr r5, _08029204 @ =0x02020310
	ldr r1, _08029214 @ =0x00000B0E
	add r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #1
	bne _08029238
	ldr r2, _08029218 @ =0x00000ABE
	add r0, r5, r2
	ldrb r7, [r0]
	cmp r7, #2
	bne _08029298
	mov r3, r8
	ldrb r1, [r3]
	mov r0, #0xB1
	lsl r0, r0, #4
	add r4, r6, r0
	mov r0, #0x53
	add r2, r4, #0
	bl sub_0807BCFC
	cmp r0, #0
	beq _08029238
	ldr r1, _08029210 @ =0x00000B16
	add r0, r6, r1
	ldrh r0, [r0]
	cmp r0, #2
	beq _080291C8
	mov r2, r8
	ldrb r2, [r2]
	cmp r2, #2
	bne _08029224
_080291C8:
	ldr r3, _0802921C @ =0x00000AB1
	add r1, r5, r3
	mov r2, #0
	mov r0, #0xFC
	strb r0, [r1]
	ldr r1, _08029220 @ =0x00000ABD
	add r0, r5, r1
	strb r2, [r0]
	add r3, #0x44
	add r1, r5, r3
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	ldr r1, _08029208 @ =0x00000B0D
	add r0, r5, r1
	strb r2, [r0]
	mov r3, r8
	strb r2, [r3]
	add r1, #0x10
	add r0, r6, r1
	strb r2, [r0]
	b _08029238
_080291F4: .4byte 0x00000AF4
_080291F8: .4byte 0x00000ABF
_080291FC: .4byte 0x00000AF5
_08029200: .4byte 0x02020DEC
_08029204: .4byte 0x02020310
_08029208: .4byte 0x00000B0D
_0802920C: .4byte 0x00000AAE
_08029210: .4byte 0x00000B16
_08029214: .4byte 0x00000B0E
_08029218: .4byte 0x00000ABE
_0802921C: .4byte 0x00000AB1
_08029220: .4byte 0x00000ABD
_08029224:
	add r0, r4, #0
	bl sub_0807BCF4
	ldr r2, _080292A8 @ =0x00000B1D
	add r0, r6, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08029238
	mov r3, r8
	strb r7, [r3]
_08029238:
	ldr r5, _080292AC @ =0x02020310
	ldr r1, _080292B0 @ =0x00000ABE
	add r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #2
	bne _08029298
	ldr r3, _080292B4 @ =0x00000B1E
	add r2, r5, r3
	ldrb r1, [r2]
	add r1, #1
	strb r1, [r2]
	mov r3, #0xFF
	lsl r0, r1, #0x18
	lsr r4, r0, #0x18
	cmp r4, #0
	bne _08029298
	orr r1, r3
	strb r1, [r2]
	mov r0, r8
	ldrb r1, [r0]
	mov r3, #0xB1
	lsl r3, r3, #4
	add r2, r6, r3
	mov r0, #0x53
	bl sub_0807BCFC
	cmp r0, #0
	beq _08029298
	ldr r0, _080292B8 @ =0x00000AB1
	add r1, r5, r0
	mov r0, #0xFC
	strb r0, [r1]
	ldr r1, _080292BC @ =0x00000ABD
	add r0, r5, r1
	strb r4, [r0]
	ldr r2, _080292C0 @ =0x00000AF5
	add r1, r5, r2
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	ldr r3, _080292C4 @ =0x00000B0D
	add r0, r5, r3
	strb r4, [r0]
	mov r0, r8
	strb r4, [r0]
	ldr r1, _080292A8 @ =0x00000B1D
	add r0, r6, r1
	strb r4, [r0]
_08029298:
	mov r0, #0
	add sp, #0x10
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080292A8: .4byte 0x00000B1D
_080292AC: .4byte 0x02020310
_080292B0: .4byte 0x00000ABE
_080292B4: .4byte 0x00000B1E
_080292B8: .4byte 0x00000AB1
_080292BC: .4byte 0x00000ABD
_080292C0: .4byte 0x00000AF5
_080292C4: .4byte 0x00000B0D
	thumb_func_end sub_08028FD0

