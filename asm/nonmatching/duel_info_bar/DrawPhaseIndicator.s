	thumb_func_start DrawPhaseIndicator
DrawPhaseIndicator: @ 0x08060964
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	add r7, r1, #0
	mov r4, #0
_08060970:
	ldr r0, _080609BC @ =0x00000286
	cmp r4, #0
	beq _08060978
	sub r0, #0x13
_08060978:
	add r2, r0, #0
	mov r3, #0
	add r5, r4, #1
	ldr r0, _080609C0 @ =0x03001C5C
	mov ip, r0
	mov r0, #0x80
	lsl r0, r0, #7
	add r6, r0, #0
_08060988:
	mov r1, #0xA2
	lsl r1, r1, #2
	cmp r4, r8
	bne _08060996
	cmp r3, r7
	bne _08060996
	sub r1, #6
_08060996:
	lsl r0, r2, #1
	add r0, ip
	add r1, r1, r3
	add r1, r1, r6
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r3, #1
	cmp r3, #5
	ble _08060988
	add r4, r5, #0
	cmp r4, #1
	ble _08060970
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080609BC: .4byte 0x00000286
_080609C0: .4byte 0x03001C5C
	thumb_func_end DrawPhaseIndicator

