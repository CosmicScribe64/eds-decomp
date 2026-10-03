	thumb_func_start DestroyFaceUpCardsByNumber
DestroyFaceUpCardsByNumber: @ 0x080184D8
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r4, #0
	mov r0, #1
	and r0, r5
	ldr r1, _08018534 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_080184EC:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08018538 @ =0x0201930C
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08018526
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08018526
	ldr r2, _0801853C @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08018540 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _08018526
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_08018526:
	add r4, #1
	cmp r4, #0xA
	ble _080184EC
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08018534: .4byte 0x00000D64
_08018538: .4byte 0x0201930C
_0801853C: .4byte 0x000007FF
_08018540: .4byte gCardIdToNumber
	thumb_func_end DestroyFaceUpCardsByNumber

