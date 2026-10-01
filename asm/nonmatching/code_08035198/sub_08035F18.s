	thumb_func_start sub_08035F18
sub_08035F18: @ 0x08035F18
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08035F78
	ldr r0, _08035F4C @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08035F50
	cmp r0, #0x80
	bne _08035F78
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #6
	mov r2, #0
	mov r3, #0
	bl sub_08022678
	mov r0, #0x7F
	b _08035F7A
_08035F4C: .4byte 0x02017A40
_08035F50:
	ldrb r1, [r1, #2]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	ldr r1, _08035F70 @ =0x020192E0
	ldr r2, _08035F74 @ =0x00001B64
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #1
	mov r3, #1
	bl sub_080193D4
	mov r0, #0x64
	b _08035F7A
	.align 2, 0
_08035F70: .4byte 0x020192E0
_08035F74: .4byte 0x00001B64
_08035F78:
	mov r0, #0
_08035F7A:
	pop {r1}
	bx r1
	thumb_func_end sub_08035F18
	.align 2, 0

