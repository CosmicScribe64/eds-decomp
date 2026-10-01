	thumb_func_start sub_08068B90
sub_08068B90: @ 0x08068B90
	push {r4, r5, r6, lr}
	sub sp, #8
	ldr r5, _08068BC0 @ =0x0201DB20
	ldr r1, _08068BC4 @ =0x00001C1C
	add r0, r5, r1
	ldrb r2, [r0]
	mov r6, #0xA5
	lsl r6, r6, #5
	add r0, r5, r6
	add r0, r2, r0
	ldrb r3, [r0]
	lsl r0, r2, #1
	mov r6, #0xC4
	lsl r6, r6, #3
	add r1, r5, r6
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r2, #1
	beq _08068BE0
	cmp r2, #1
	bgt _08068BC8
	cmp r2, #0
	beq _08068BCE
	b _08068C06
_08068BC0: .4byte 0x0201DB20
_08068BC4: .4byte 0x00001C1C
_08068BC8:
	cmp r2, #2
	beq _08068BF4
	b _08068C06
_08068BCE:
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r2, _08068BDC @ =0x00000644
	b _08068C00
_08068BDC: .4byte 0x00000644
_08068BE0:
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r6, _08068BF0 @ =0x00000CAE
	add r1, r5, r6
	b _08068C02
_08068BF0: .4byte 0x00000CAE
_08068BF4:
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r2, _08068C38 @ =0x00000D4E
_08068C00:
	add r1, r5, r2
_08068C02:
	add r0, r0, r1
	ldrh r4, [r0]
_08068C06:
	lsl r0, r4, #0x10
	lsr r0, r0, #0xA
	ldr r6, _08068C3C @ =0x0822C720
	add r0, r0, r6
	ldr r2, _08068C40 @ =0x0201F775
	ldrh r1, [r0]
	ldrh r0, [r2]
	cmp r1, r0
	beq _08068C2E
	strh r1, [r2]
	add r0, sp, #4
	mov r2, #0
	strh r1, [r0]
	strh r2, [r0, #2]
	ldr r1, _08068C44 @ =0x06012FE0
	str r2, [sp, #0]
	mov r2, #1
	mov r3, #0
	bl sub_080791F4
_08068C2E:
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08068C38: .4byte 0x00000D4E
_08068C3C: .4byte gUnk_0822C720
_08068C40: .4byte 0x0201F775
_08068C44: .4byte 0x06012FE0
	thumb_func_end sub_08068B90

