	thumb_func_start sub_08053EF8
sub_08053EF8: @ 0x08053EF8
	push {r4, r5, lr}
	ldr r4, _08053F24 @ =0x0201AE60
	add r5, r4, #0
	add r5, #0x22
	ldrb r0, [r5]
	cmp r0, #0
	bne _08053F30
	ldr r0, _08053F28 @ =0x020192E0
	ldrh r4, [r4, #0x14]
	lsl r1, r4, #1
	ldr r2, _08053F2C @ =0x00001B52
	add r0, r0, r2
	add r1, r1, r0
	ldrh r0, [r1]
	mov r1, #1
	bl sub_0805F074
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _08053F92
_08053F24: .4byte 0x0201AE60
_08053F28: .4byte 0x020192E0
_08053F2C: .4byte 0x00001B52
_08053F30:
	ldr r0, _08053F44 @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	beq _08053F48
	ldrh r0, [r4, #0x14]
	add r0, #4
	b _08053F54
	.align 2, 0
_08053F44: .4byte 0x03000040
_08053F48:
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	beq _08053F84
	ldrh r0, [r4, #0x14]
	add r0, #1
_08053F54:
	strh r0, [r4, #0x14]
	ldrh r0, [r4, #0x14]
	mov r1, #5
	bl __umodsi3
	strh r0, [r4, #0x14]
	bl sub_0805ED9C
	ldr r0, _08053F7C @ =0x020192E0
	ldrh r4, [r4, #0x14]
	lsl r1, r4, #1
	ldr r2, _08053F80 @ =0x00001B52
	add r0, r0, r2
	add r1, r1, r0
	ldrh r0, [r1]
	mov r1, #1
	bl sub_0805F074
	mov r0, #0
	b _08053F92
_08053F7C: .4byte 0x020192E0
_08053F80: .4byte 0x00001B52
_08053F84:
	mov r0, #1
	and r0, r1
	cmp r0, #0
	bne _08053F90
	mov r0, #0
	b _08053F92
_08053F90:
	mov r0, #1
_08053F92:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08053EF8

