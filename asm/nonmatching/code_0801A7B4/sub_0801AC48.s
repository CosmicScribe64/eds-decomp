	thumb_func_start sub_0801AC48
sub_0801AC48: @ 0x0801AC48
	push {lr}
	ldr r1, _0801AC74 @ =0x03000040
	ldr r0, _0801AC78 @ =0x0000485E
	add r1, r1, r0
	mov r0, #7
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801AC68
	mov r0, #0xEE
	lsl r0, r0, #8
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
_0801AC68:
	bl sub_08001AE4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
_0801AC74: .4byte 0x03000040
_0801AC78: .4byte 0x0000485E
	thumb_func_end sub_0801AC48

