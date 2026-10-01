	thumb_func_start sub_08007D18
sub_08007D18: @ 0x08007D18
	push {r4, lr}
	ldr r3, _08007D44 @ =0x020192E4
	mov r2, #1
	and r2, r0
	ldr r0, _08007D48 @ =0x00000D64
	mul r0, r2
	add r4, r0, r3
	ldr r2, _08007D4C @ =0x00000A44
	add r3, r3, r2
	add r0, r0, r3
	ldrb r3, [r4, #5]
	lsl r2, r3, #2
	add r0, r0, r2
	bl sub_08007558
	ldrb r0, [r4, #5]
	add r0, #1
	strb r0, [r4, #5]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08007D44: .4byte 0x020192E4
_08007D48: .4byte 0x00000D64
_08007D4C: .4byte 0x00000A44
	thumb_func_end sub_08007D18

