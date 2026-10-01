	thumb_func_start sub_08009DEC
sub_08009DEC: @ 0x08009DEC
	push {r4, r5, r6, lr}
	mov r3, #0
	ldr r4, _08009E38 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009E3C @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #4]
	cmp r3, r2
	bge _08009E30
	ldr r5, _08009E40 @ =0x00000904
	add r0, r4, r5
	ldr r5, _08009E44 @ =0x000007FF
	add r1, r1, r0
	mov r4, #0xF8
	lsl r4, r4, #0x11
_08009E0E:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #2
	ldr r6, _08009E48 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08009E28
	add r3, #1
_08009E28:
	add r1, #4
	sub r2, #1
	cmp r2, #0
	bne _08009E0E
_08009E30:
	add r0, r3, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08009E38: .4byte 0x020192E4
_08009E3C: .4byte 0x00000D64
_08009E40: .4byte 0x00000904
_08009E44: .4byte 0x000007FF
_08009E48: .4byte gUnk_08621DE0
	thumb_func_end sub_08009DEC

