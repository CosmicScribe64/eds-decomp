	thumb_func_start sub_08007CE0
sub_08007CE0: @ 0x08007CE0
	push {r4, lr}
	mov r2, #1
	and r2, r0
	ldr r0, _08007D0C @ =0x00000D64
	add r4, r2, #0
	mul r4, r0
	ldr r2, _08007D10 @ =0x02019AA8
	add r0, r4, r2
	ldr r3, _08007D14 @ =0xFFFFF83C
	add r2, r2, r3
	add r4, r4, r2
	ldrb r3, [r4, #3]
	lsl r2, r3, #2
	add r0, r0, r2
	bl sub_08007558
	ldrb r0, [r4, #3]
	add r0, #1
	strb r0, [r4, #3]
	pop {r4}
	pop {r0}
	bx r0
_08007D0C: .4byte 0x00000D64
_08007D10: .4byte 0x02019AA8
_08007D14: .4byte 0xFFFFF83C
	thumb_func_end sub_08007CE0

