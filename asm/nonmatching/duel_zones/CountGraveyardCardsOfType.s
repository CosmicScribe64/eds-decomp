	thumb_func_start CountGraveyardCardsOfType
CountGraveyardCardsOfType: @ 0x08009150
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #0
	ldr r4, _080091A0 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _080091A4 @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #4]
	cmp r3, r2
	bge _08009198
	ldr r6, _080091A8 @ =0x00000904
	add r0, r4, r6
	ldr r6, _080091AC @ =0x000007FF
	add r1, r1, r0
	mov r4, #0xF8
	lsl r4, r4, #0x11
_08009176:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r7, _080091B0 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, r5
	bne _08009190
	add r3, #1
_08009190:
	add r1, #4
	sub r2, #1
	cmp r2, #0
	bne _08009176
_08009198:
	add r0, r3, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080091A0: .4byte 0x020192E4
_080091A4: .4byte 0x00000D64
_080091A8: .4byte 0x00000904
_080091AC: .4byte 0x000007FF
_080091B0: .4byte gCardStats
	thumb_func_end CountGraveyardCardsOfType

