	thumb_func_start sub_08059A78
sub_08059A78: @ 0x08059A78
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r4, #1
	neg r4, r4
	mov r0, #0
	add r1, r4, #0
	mov r2, #1
	mov r3, #0
	bl sub_0805748C
	add r7, r0, #0
	mov r0, #1
	add r1, r4, #0
	mov r2, #1
	mov r3, #0
	bl sub_0805748C
	mov r8, r0
	cmp r5, r4
	beq _08059AAC
	mov r0, #0
	add r1, r7, #0
	bl sub_0800C894
	b _08059AAE
_08059AAC:
	mov r0, #0
_08059AAE:
	add r5, r0, #0
	mov r0, #1
	neg r0, r0
	cmp r6, r0
	beq _08059AC2
	mov r0, #1
	mov r1, r8
	bl sub_0800C894
	b _08059AC4
_08059AC2:
	mov r0, #0
_08059AC4:
	add r6, r0, #0
	ldr r0, _08059B00 @ =0x020192E4
	ldr r1, _08059B04 @ =0x00000D64
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r5, r1
	bge _08059B08
	sub r0, r5, r6
	cmp r0, r1
	bge _08059B08
	cmp r5, r6
	bgt _08059B08
	mov r5, #0
	mov r4, #0
_08059AE0:
	mov r0, #0
	add r1, r4, #0
	bl sub_0800C894
	add r5, r5, r0
	add r4, #1
	cmp r4, #4
	ble _08059AE0
	ldr r0, _08059B00 @ =0x020192E4
	ldr r1, _08059B04 @ =0x00000D64
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r5, r0
	bgt _08059B08
	mov r0, #0
	b _08059B0A
_08059B00: .4byte 0x020192E4
_08059B04: .4byte 0x00000D64
_08059B08:
	mov r0, #1
_08059B0A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08059A78

