	thumb_func_start sub_08056E68
sub_08056E68: @ 0x08056E68
	push {r4, r5, r6, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #0
	ldr r4, _08056EA4 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08056EA8 @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #5]
	cmp r3, r2
	bge _08056EC0
	ldr r6, _08056EAC @ =0x00000A44
	add r0, r4, r6
	add r1, r1, r0
	ldr r6, _08056EB0 @ =0x000007FF
	ldr r4, _08056EB4 @ =0x08622AB4
_08056E8C:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r5
	bne _08056EB8
	add r0, r3, #0
	b _08056EC4
	.align 2, 0
_08056EA4: .4byte 0x020192E4
_08056EA8: .4byte 0x00000D64
_08056EAC: .4byte 0x00000A44
_08056EB0: .4byte 0x000007FF
_08056EB4: .4byte gUnk_08622AB4
_08056EB8:
	add r1, #4
	add r3, #1
	cmp r3, r2
	blt _08056E8C
_08056EC0:
	mov r0, #1
	neg r0, r0
_08056EC4:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08056E68
	.align 2, 0

