	thumb_func_start sub_0806AF1C
sub_0806AF1C: @ 0x0806AF1C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	mov r4, #0
	ldr r2, _0806AF6C @ =0x0201DB20
	ldr r0, _0806AF70 @ =0x00001C1C
	add r6, r2, r0
	ldrb r3, [r6]
	mov r1, #0xA5
	lsl r1, r1, #5
	add r5, r2, r1
	add r1, r3, r5
	ldrb r7, [r1]
	lsl r0, r7, #1
	add r1, r7, #0
	add r0, r0, r1
	add r0, r0, r3
	lsl r0, r0, #1
	ldr r1, _0806AF74 @ =0x00001494
	add r2, r2, r1
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r4, r0
	bcs _0806AF94
	mov r8, r2
_0806AF56:
	ldrb r0, [r6]
	add r1, r0, r5
	ldrb r1, [r1]
	add r2, r4, #0
	bl sub_08068D1C
	cmp r0, r9
	bne _0806AF78
	add r0, r4, #0
	b _0806AF96
	.align 2, 0
_0806AF6C: .4byte 0x0201DB20
_0806AF70: .4byte 0x00001C1C
_0806AF74: .4byte 0x00001494
_0806AF78:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldrb r2, [r6]
	add r1, r2, r5
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r0, r3
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r8
	ldrh r0, [r0]
	cmp r4, r0
	bcc _0806AF56
_0806AF94:
	mov r0, #0
_0806AF96:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0806AF1C
	.align 2, 0

