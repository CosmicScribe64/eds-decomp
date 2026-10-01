	thumb_func_start sub_0803E22C
sub_0803E22C: @ 0x0803E22C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldrb r1, [r5, #2]
	mov r4, #1
	add r2, r4, #0
	and r2, r1
	cmp r2, #0
	beq _0803E274
	mov r0, #8
	neg r0, r0
	ldrb r1, [r5, #0xA]
	and r0, r1
	strb r0, [r5, #0xA]
	mov r0, #0
	bl sub_08008860
	cmp r0, #0
	bgt _0803E252
	b _0803E39E
_0803E252:
	sub r4, #2
	mov r0, #0
	add r1, r4, #0
	mov r2, #1
	mov r3, #1
	bl sub_0805748C
	add r2, r0, #0
	cmp r2, r4
	bgt _0803E268
	b _0803E39E
_0803E268:
	add r0, r5, #0
	mov r1, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	b _0803E39E
_0803E274:
	ldr r0, _0803E2BC @ =0x02017A40
	ldr r6, _0803E2C0 @ =0x000003E5
	add r3, r0, r6
	ldrb r0, [r3]
	cmp r0, #0
	bne _0803E35C
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r4, r0
	bl sub_08008860
	cmp r0, #0
	bne _0803E29A
	b _0803E39E
_0803E29A:
	ldr r0, _0803E2C4 @ =0x000007FF
	ldrh r5, [r5]
	and r0, r5
	lsl r0, r0, #1
	ldr r3, _0803E2C8 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0803E2CC @ =0x000002DA
	cmp r1, r0
	beq _0803E310
	cmp r1, r0
	bgt _0803E2D0
	cmp r1, #0x5E
	beq _0803E2E8
	cmp r1, #0x77
	beq _0803E2FC
	b _0803E32C
_0803E2BC: .4byte 0x02017A40
_0803E2C0: .4byte 0x000003E5
_0803E2C4: .4byte 0x000007FF
_0803E2C8: .4byte gUnk_08622AB4
_0803E2CC: .4byte 0x000002DA
_0803E2D0:
	ldr r0, _0803E2E0 @ =0x000002E6
	cmp r1, r0
	beq _0803E2FC
	ldr r0, _0803E2E4 @ =0x00000536
	cmp r1, r0
	beq _0803E310
	b _0803E32C
	.align 2, 0
_0803E2E0: .4byte 0x000002E6
_0803E2E4: .4byte 0x00000536
_0803E2E8:
	ldr r0, _0803E2F0 @ =0x00000206
	ldr r1, _0803E2F4 @ =0x00000712
	ldr r3, _0803E2F8 @ =0x08083CF8
	b _0803E316
_0803E2F0: .4byte 0x00000206
_0803E2F4: .4byte 0x00000712
_0803E2F8: .4byte gUnk_08083CF8
_0803E2FC:
	ldr r0, _0803E304 @ =0x00000206
	ldr r1, _0803E308 @ =0x00000712
	ldr r3, _0803E30C @ =0x08083D50
	b _0803E316
_0803E304: .4byte 0x00000206
_0803E308: .4byte 0x00000712
_0803E30C: .4byte gUnk_08083D50
_0803E310:
	ldr r0, _0803E320 @ =0x00000206
	ldr r1, _0803E324 @ =0x00000712
	ldr r3, _0803E328 @ =0x08083D90
_0803E316:
	mov r2, #0xB
	bl sub_080602A4
	b _0803E338
	.align 2, 0
_0803E320: .4byte 0x00000206
_0803E324: .4byte 0x00000712
_0803E328: .4byte gUnk_08083D90
_0803E32C:
	ldr r0, _0803E348 @ =0x00000206
	ldr r1, _0803E34C @ =0x00000712
	ldr r3, _0803E350 @ =0x08083DCC
	mov r2, #0xB
	bl sub_080602A4
_0803E338:
	ldr r0, _0803E354 @ =0x02017A40
	ldr r6, _0803E358 @ =0x000003E5
	add r0, r0, r6
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803E3BA
	.align 2, 0
_0803E348: .4byte 0x00000206
_0803E34C: .4byte 0x00000712
_0803E350: .4byte gUnk_08083DCC
_0803E354: .4byte 0x02017A40
_0803E358: .4byte 0x000003E5
_0803E35C:
	ldr r1, _0803E36C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803E370
	strb r2, [r3]
	b _0803E3BA
_0803E36C: .4byte 0x03000040
_0803E370:
	mov r0, #0xF0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0803E3BA
	ldr r0, _0803E3A4 @ =0x0201CFB0
	ldr r2, _0803E3A8 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803E3AC @ =0x00000828
	add r2, r0, r3
	ldr r6, _0803E3B0 @ =0x0000082C
	add r0, r0, r6
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r5, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803E3B4
_0803E39E:
	mov r0, #1
	b _0803E3BC
	.align 2, 0
_0803E3A4: .4byte 0x0201CFB0
_0803E3A8: .4byte 0x00000824
_0803E3AC: .4byte 0x00000828
_0803E3B0: .4byte 0x0000082C
_0803E3B4:
	mov r0, #3
	bl sub_08077AEC
_0803E3BA:
	mov r0, #0
_0803E3BC:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0803E22C
	.align 2, 0

