	thumb_func_start sub_080039E0
sub_080039E0: @ 0x080039E0
	push {r4, r5, lr}
	bl sub_08003850
	ldr r5, _08003A3C @ =0x03000040
	mov r0, #0x40
	ldrh r1, [r5, #6]
	and r0, r1
	cmp r0, #0
	beq _08003A0A
	ldr r4, _08003A40 @ =0x02015ED8
	ldrh r0, [r4]
	add r0, #6
	strh r0, [r4]
	ldrh r0, [r4]
	mov r1, #7
	bl __umodsi3
	strh r0, [r4]
	mov r0, #0
	bl sub_08077AEC
_08003A0A:
	mov r0, #0x80
	ldrh r1, [r5, #6]
	and r0, r1
	cmp r0, #0
	beq _08003A2C
	ldr r4, _08003A40 @ =0x02015ED8
	ldrh r0, [r4]
	add r0, #8
	strh r0, [r4]
	ldrh r0, [r4]
	mov r1, #7
	bl __umodsi3
	strh r0, [r4]
	mov r0, #0
	bl sub_08077AEC
_08003A2C:
	mov r0, #1
	ldrh r5, [r5, #6]
	and r0, r5
	cmp r0, #0
	bne _08003A44
	mov r0, #0
	b _08003A50
	.align 2, 0
_08003A3C: .4byte 0x03000040
_08003A40: .4byte 0x02015ED8
_08003A44:
	mov r0, #1
	bl sub_08077AEC
	bl sub_08077BCC
	mov r0, #1
_08003A50:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_080039E0
	.align 2, 0

