	thumb_func_start RemoveCardFromBanished
RemoveCardFromBanished: @ 0x08009D8C
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r7, r1, #0
	mov r3, #0
	ldr r2, _08009DD0 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _08009DD4 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r5, [r1, #6]
	cmp r3, r5
	bge _08009DE4
	ldr r5, _08009DD8 @ =0x00000B84
	add r5, r5, r2
	mov ip, r5
	add r6, r0, #0
	add r5, r1, #0
_08009DB0:
	mov r0, ip
	add r1, r6, r0
	lsl r0, r3, #2
	add r1, r1, r0
	ldr r2, [r7]
	ldr r0, [r1]
	cmp r2, r0
	bne _08009DDC
	add r0, r4, #0
	add r1, r3, #0
	bl RemoveBanishedCardAt
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08009DE6
	.align 2, 0
_08009DD0: .4byte 0x020192E4
_08009DD4: .4byte 0x00000D64
_08009DD8: .4byte 0x00000B84
_08009DDC:
	add r3, #1
	ldrb r0, [r5, #6]
	cmp r3, r0
	blt _08009DB0
_08009DE4:
	mov r0, #0
_08009DE6:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end RemoveCardFromBanished

