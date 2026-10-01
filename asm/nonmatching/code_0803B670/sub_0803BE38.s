	thumb_func_start sub_0803BE38
sub_0803BE38: @ 0x0803BE38
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803BEC8
	ldr r0, _0803BE68 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _0803BE78
	cmp r0, #0x80
	bne _0803BEC8
	ldr r0, _0803BE6C @ =0x00000206
	ldr r1, _0803BE70 @ =0x00000712
	ldr r3, _0803BE74 @ =0x08083A14
	mov r2, #0xB
	bl sub_080602A4
_0803BE64:
	mov r0, #0x7F
	b _0803BECA
_0803BE68: .4byte 0x02017A40
_0803BE6C: .4byte 0x00000206
_0803BE70: .4byte 0x00000712
_0803BE74: .4byte gUnk_08083A14
_0803BE78:
	mov r0, #0xE0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0803BE64
	mov r6, #1
	add r0, r6, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r2, #0xA1
	cmp r0, #0
	bne _0803BE94
	ldr r2, _0803BEBC @ =0x000080A1
_0803BE94:
	ldr r4, _0803BEC0 @ =0x0201CFB0
	ldr r0, _0803BEC4 @ =0x0000082C
	add r4, r4, r0
	ldrh r1, [r4]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r6, r0
	ldr r1, [r4]
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
	mov r0, #0x7E
	b _0803BECA
_0803BEBC: .4byte 0x000080A1
_0803BEC0: .4byte 0x0201CFB0
_0803BEC4: .4byte 0x0000082C
_0803BEC8:
	mov r0, #0
_0803BECA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0803BE38

