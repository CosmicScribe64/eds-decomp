	thumb_func_start sub_08073B64
sub_08073B64: @ 0x08073B64
	push {r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r5, r0, #0
	add r6, r1, #0
	ldr r0, _08073BEC @ =0x04000128
	ldrh r0, [r0]
	mov r7, #0
	mov r0, sp
	bl sub_080740BC
	lsl r0, r0, #0x10
	mov r1, #0xF0
	lsl r1, r1, #0xC
	and r1, r0
	lsr r1, r1, #0x10
	asr r1, r5
	mov r0, #1
	and r1, r0
	cmp r1, #0
	beq _08073C04
	lsl r0, r5, #4
	mov r1, sp
	add r2, r1, r0
	mov r1, #0xF0
	lsl r1, r1, #8
	ldrh r3, [r2]
	and r1, r3
	mov r0, #0x80
	lsl r0, r0, #5
	cmp r1, r0
	beq _08073BD2
	mov r0, #0xC0
	lsl r0, r0, #6
	cmp r1, r0
	bne _08073BD2
	add r0, r2, #2
	lsl r4, r5, #8
	add r4, r4, r5
	lsl r4, r4, #1
	ldr r1, _08073BF0 @ =0x03005D6C
	add r4, r4, r1
	add r1, r4, #0
	mov r2, #7
	bl CpuSet
	ldr r2, _08073BF4 @ =0x000001FF
	ldrh r0, [r4]
	and r2, r0
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsr r2, r2, #0x11
	add r0, r4, #0
	add r1, r6, #0
	bl CpuSet
_08073BD2:
	ldr r1, _08073BF8 @ =0x03005B60
	lsl r2, r5, #1
	ldr r3, _08073BFC @ =0x00000AF8
	add r0, r1, r3
	add r0, r2, r0
	mov r3, #0
	strh r3, [r0]
	ldr r0, _08073C00 @ =0x00000AF4
	add r1, r1, r0
	add r2, r2, r1
	strh r3, [r2]
	add r0, r7, #0
	b _08073C06
_08073BEC: .4byte 0x04000128
_08073BF0: .4byte 0x03005D6C
_08073BF4: .4byte 0x000001FF
_08073BF8: .4byte 0x03005B60
_08073BFC: .4byte 0x00000AF8
_08073C00: .4byte 0x00000AF4
_08073C04:
	mov r0, #0
_08073C06:
	add sp, #0x20
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08073B64
	.align 2, 0

