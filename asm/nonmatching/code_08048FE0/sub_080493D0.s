	thumb_func_start sub_080493D0
sub_080493D0: @ 0x080493D0
	push {lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #2
	bgt _080493FC
	cmp r0, #1
	blt _080493FC
	ldr r1, _0804943C @ =0x020192E0
	ldr r2, _08049440 @ =0x00001B33
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08049444 @ =0x00001B34
	add r1, r1, r3
	ldrh r1, [r1]
	lsl r1, r1, #0x17
	lsr r1, r1, #0x18
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
_080493FC:
	ldr r2, _0804943C @ =0x020192E0
	ldr r1, _08049440 @ =0x00001B33
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r3, r0, #0x1E
	lsr r3, r3, #0x1F
	add r1, #1
	add r0, r2, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x17
	lsr r0, r0, #0x18
	mov r1, #0x94
	mul r1, r0
	ldr r0, _08049448 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	add r0, r2, #0
	add r0, #0x2C
	add r1, r1, r0
	mov r0, #4
	ldrb r3, [r1, #7]
	orr r0, r3
	strb r0, [r1, #7]
	ldr r0, _0804944C @ =0x00001B2C
	add r2, r2, r0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	pop {r0}
	bx r0
_0804943C: .4byte 0x020192E0
_08049440: .4byte 0x00001B33
_08049444: .4byte 0x00001B34
_08049448: .4byte 0x00000D64
_0804944C: .4byte 0x00001B2C
	thumb_func_end sub_080493D0

