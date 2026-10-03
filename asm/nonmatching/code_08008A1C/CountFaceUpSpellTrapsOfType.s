	thumb_func_start CountFaceUpSpellTrapsOfType
CountFaceUpSpellTrapsOfType: @ 0x080090E0
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	mov r4, #0
	mov r3, #5
	ldr r1, _08009140 @ =0x0201930C
	mov ip, r1
	mov r1, #1
	and r1, r0
	ldr r0, _08009144 @ =0x00000D64
	add r5, r1, #0
	mul r5, r0
	ldr r7, _08009148 @ =0x000007FF
_080090FA:
	mov r0, #0x94
	mul r0, r3
	add r0, r0, r5
	mov r1, ip
	add r2, r0, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _08009130
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08009130
	and r1, r7
	lsl r0, r1, #2
	ldr r1, _0800914C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, r6
	bne _08009130
	add r4, #1
_08009130:
	add r3, #1
	cmp r3, #9
	ble _080090FA
	add r0, r4, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08009140: .4byte 0x0201930C
_08009144: .4byte 0x00000D64
_08009148: .4byte 0x000007FF
_0800914C: .4byte gCardStats
	thumb_func_end CountFaceUpSpellTrapsOfType

