	thumb_func_start sub_0803B104
sub_0803B104: @ 0x0803B104
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803B1E4
	ldr r0, _0803B148 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _0803B16C
	cmp r0, #0x80
	bne _0803B1E4
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #0
	bne _0803B158
	ldr r0, _0803B14C @ =0x00000206
	ldr r1, _0803B150 @ =0x00000613
	ldr r3, _0803B154 @ =0x08083558
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	b _0803B164
	.align 2, 0
_0803B148: .4byte 0x02017A40
_0803B14C: .4byte 0x00000206
_0803B150: .4byte 0x00000613
_0803B154: .4byte gUnk_08083558
_0803B158:
	bl sub_08076F9C
	ldr r2, _0803B168 @ =0x0201AE60
	mov r1, #1
	and r0, r1
	strh r0, [r2, #0x14]
_0803B164:
	mov r0, #0x7F
	b _0803B1E6
_0803B168: .4byte 0x0201AE60
_0803B16C:
	bl sub_08076F9C
	add r4, r0, #0
	mov r0, #1
	and r4, r0
	mov r6, #1
	add r0, r6, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r2, #0xE0
	cmp r0, #0
	beq _0803B186
	ldr r2, _0803B1D4 @ =0x000080E0
_0803B186:
	ldr r7, _0803B1D8 @ =0x0201AE60
	ldrh r1, [r7, #0x14]
	add r0, r2, #0
	add r2, r4, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r6, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r1, #0x12
	cmp r0, #0
	beq _0803B1A2
	ldr r1, _0803B1DC @ =0x00008012
_0803B1A2:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldrh r7, [r7, #0x14]
	cmp r4, r7
	bne _0803B1CE
	add r0, r6, #0
	ldrb r5, [r5, #2]
	and r0, r5
	mov r1, #0x3A
	cmp r0, #0
	beq _0803B1C2
	ldr r1, _0803B1E0 @ =0x0000803A
_0803B1C2:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_0803B1CE:
	mov r0, #0xA
	b _0803B1E6
	.align 2, 0
_0803B1D4: .4byte 0x000080E0
_0803B1D8: .4byte 0x0201AE60
_0803B1DC: .4byte 0x00008012
_0803B1E0: .4byte 0x0000803A
_0803B1E4:
	mov r0, #0
_0803B1E6:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803B104

