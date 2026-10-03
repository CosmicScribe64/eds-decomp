	thumb_func_start CountHandMonsters
CountHandMonsters: @ 0x08009E4C
	push {r4, r5, r6, lr}
	mov r3, #0
	ldr r4, _08009E98 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009E9C @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #2]
	cmp r3, r2
	bge _08009E90
	ldr r5, _08009EA0 @ =0x00000684
	add r0, r4, r5
	ldr r5, _08009EA4 @ =0x000007FF
	add r1, r1, r0
	mov r4, #0xF8
	lsl r4, r4, #0x11
_08009E6E:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #2
	ldr r6, _08009EA8 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08009E88
	add r3, #1
_08009E88:
	add r1, #4
	sub r2, #1
	cmp r2, #0
	bne _08009E6E
_08009E90:
	add r0, r3, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08009E98: .4byte 0x020192E4
_08009E9C: .4byte 0x00000D64
_08009EA0: .4byte 0x00000684
_08009EA4: .4byte 0x000007FF
_08009EA8: .4byte gCardStats
	thumb_func_end CountHandMonsters

