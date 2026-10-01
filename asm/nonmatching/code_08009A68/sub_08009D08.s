	thumb_func_start sub_08009D08
sub_08009D08: @ 0x08009D08
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r2, r1, #0
	ldr r7, _08009D6C @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009D70 @ =0x00000D64
	add r6, r1, #0
	mul r6, r0
	add r3, r6, r7
	ldrb r0, [r3, #6]
	cmp r2, r0
	bge _08009D80
	sub r0, #1
	strb r0, [r3, #6]
	add r5, r2, #0
	cmp r5, r0
	bge _08009D68
	ldr r0, _08009D74 @ =0x00000B84
	add r2, r7, r0
	lsl r1, r5, #1
	ldr r4, _08009D78 @ =0x00000CC4
	add r0, r7, r4
	add r0, r6, r0
	add r4, r1, r0
	add r2, r6, r2
	mov r8, r3
	lsl r1, r5, #2
	ldr r3, _08009D7C @ =0x00000B88
	add r0, r7, r3
	add r0, r6, r0
	add r7, r1, r0
	add r6, r1, r2
_08009D4C:
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08007558
	ldrh r0, [r4, #2]
	strh r0, [r4]
	add r4, #2
	add r7, #4
	add r6, #4
	add r5, #1
	mov r0, r8
	ldrb r0, [r0, #6]
	cmp r5, r0
	blt _08009D4C
_08009D68:
	mov r0, #1
	b _08009D82
_08009D6C: .4byte 0x020192E4
_08009D70: .4byte 0x00000D64
_08009D74: .4byte 0x00000B84
_08009D78: .4byte 0x00000CC4
_08009D7C: .4byte 0x00000B88
_08009D80:
	mov r0, #0
_08009D82:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08009D08

