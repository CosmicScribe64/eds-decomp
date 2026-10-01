	thumb_func_start sub_08004EAC
sub_08004EAC: @ 0x08004EAC
	push {r4, r5, lr}
	ldr r1, _08004EF0 @ =0x0819879C
	ldr r5, _08004EF4 @ =0x03000040
	ldr r0, _08004EF8 @ =0x00004878
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08004F00
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004EEC
	ldrb r0, [r4]
	add r0, #1
	mov r1, #0
	strb r0, [r4]
	ldr r2, _08004EFC @ =0x00004858
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_08004EEC:
	mov r0, #0
	b _08004F02
_08004EF0: .4byte gUnk_0819879C
_08004EF4: .4byte 0x03000040
_08004EF8: .4byte 0x00004878
_08004EFC: .4byte 0x00004858
_08004F00:
	mov r0, #1
_08004F02:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08004EAC

