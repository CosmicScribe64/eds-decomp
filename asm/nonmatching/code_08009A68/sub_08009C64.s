	thumb_func_start sub_08009C64
sub_08009C64: @ 0x08009C64
	push {r4, r5, r6, lr}
	add r5, r1, #0
	mov r3, #0
	ldr r4, _08009C90 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009C94 @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #4]
	cmp r3, r2
	bge _08009CA4
	ldr r6, _08009C98 @ =0x00000904
	add r0, r4, r6
	add r4, r2, #0
	ldr r2, [r5]
	add r1, r1, r0
_08009C86:
	ldr r0, [r1]
	cmp r2, r0
	bne _08009C9C
	mov r0, #1
	b _08009CA6
_08009C90: .4byte 0x020192E4
_08009C94: .4byte 0x00000D64
_08009C98: .4byte 0x00000904
_08009C9C:
	add r1, #4
	add r3, #1
	cmp r3, r4
	blt _08009C86
_08009CA4:
	mov r0, #0
_08009CA6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08009C64

