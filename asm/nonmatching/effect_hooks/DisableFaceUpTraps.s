	thumb_func_start DisableFaceUpTraps
DisableFaceUpTraps: @ 0x080469DC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r5, #0
	mov r0, #1
	mov r9, r0
	ldr r1, _08046A60 @ =0x00000D64
	mov r8, r1
_080469EE:
	mov r4, #5
	add r7, r5, #1
	add r0, r5, #0
	mov r2, r9
	and r0, r2
	mov r6, r8
	mul r6, r0
_080469FC:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08046A64 @ =0x0201930C
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08046A48
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08046A48
	ldr r2, _08046A68 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #2
	ldr r1, _08046A6C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08046A48
	mov r0, #0xB1
	cmp r5, #0
	beq _08046A3C
	ldr r0, _08046A70 @ =0x000080B1
_08046A3C:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08046A48:
	add r4, #1
	cmp r4, #9
	ble _080469FC
	add r5, r7, #0
	cmp r5, #1
	ble _080469EE
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08046A60: .4byte 0x00000D64
_08046A64: .4byte 0x0201930C
_08046A68: .4byte 0x000007FF
_08046A6C: .4byte gCardStats
_08046A70: .4byte 0x000080B1
	thumb_func_end DisableFaceUpTraps

