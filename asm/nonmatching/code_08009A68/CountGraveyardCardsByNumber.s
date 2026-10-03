	thumb_func_start CountGraveyardCardsByNumber
CountGraveyardCardsByNumber: @ 0x08009CAC
	push {r4, r5, r6, lr}
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #0
	ldr r4, _08009CF4 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009CF8 @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #4]
	cmp r3, r2
	bge _08009CEC
	ldr r6, _08009CFC @ =0x00000904
	add r0, r4, r6
	add r1, r1, r0
	ldr r6, _08009D00 @ =0x000007FF
	ldr r4, _08009D04 @ =0x08622AB4
_08009CD0:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r5
	bne _08009CE4
	add r3, #1
_08009CE4:
	add r1, #4
	sub r2, #1
	cmp r2, #0
	bne _08009CD0
_08009CEC:
	add r0, r3, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08009CF4: .4byte 0x020192E4
_08009CF8: .4byte 0x00000D64
_08009CFC: .4byte 0x00000904
_08009D00: .4byte 0x000007FF
_08009D04: .4byte gCardIdToNumber
	thumb_func_end CountGraveyardCardsByNumber

