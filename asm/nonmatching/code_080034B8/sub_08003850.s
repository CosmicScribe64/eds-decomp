	thumb_func_start sub_08003850
sub_08003850: @ 0x08003850
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r0, _0800390C @ =0x000A0038
	mov r4, #0x81
	lsl r4, r4, #7
	add r1, r4, #0
	mov r2, #0
	bl sub_080761F0
	ldr r0, _08003910 @ =0x000A0058
	add r1, r4, #0
	mov r2, #4
	bl sub_080761F0
	ldr r0, _08003914 @ =0x000A0078
	add r1, r4, #0
	mov r2, #8
	bl sub_080761F0
	ldr r0, _08003918 @ =0x000A0098
	add r1, r4, #0
	mov r2, #0xC
	bl sub_080761F0
	mov r7, #0
	mov r0, #0x38
	mov r9, r0
	add r6, r4, #0
	mov r5, #0xE8
	lsl r5, r5, #0xD
	mov r1, #0x80
	lsl r1, r1, #0xF
	mov r8, r1
_08003896:
	mov r0, r8
	lsr r4, r0, #0x10
	ldr r0, _0800391C @ =0x02015ED8
	ldrh r0, [r0]
	cmp r0, r7
	bne _080038AA
	add r0, r4, #0
	add r0, #0x10
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_080038AA:
	add r0, r5, #0
	mov r1, r9
	orr r0, r1
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080761F0
	mov r0, #0x58
	orr r0, r5
	add r2, r4, #4
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r1, r6, #0
	bl sub_080761F0
	mov r0, #0x78
	orr r0, r5
	add r2, r4, #0
	add r2, #8
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r1, r6, #0
	bl sub_080761F0
	mov r0, #0x98
	orr r0, r5
	add r2, r4, #0
	add r2, #0xC
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r1, r6, #0
	bl sub_080761F0
	mov r0, #0x80
	lsl r0, r0, #0xD
	add r5, r5, r0
	mov r1, #0x80
	lsl r1, r1, #0xF
	add r8, r1
	add r7, #1
	cmp r7, #6
	ble _08003896
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800390C: .4byte 0x000A0038
_08003910: .4byte 0x000A0058
_08003914: .4byte 0x000A0078
_08003918: .4byte 0x000A0098
_0800391C: .4byte 0x02015ED8
	thumb_func_end sub_08003850

