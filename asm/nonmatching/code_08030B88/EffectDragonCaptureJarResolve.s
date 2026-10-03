	thumb_func_start EffectDragonCaptureJarResolve
EffectDragonCaptureJarResolve: @ 0x08030F04
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r1, #4
	ldrb r0, [r0, #4]
	and r1, r0
	cmp r1, #0
	bne _08030F6E
	mov r5, #0
	mov r0, #1
	mov r9, r0
	ldr r1, _08030F7C @ =0x00000D64
	mov r8, r1
_08030F20:
	mov r4, #0
	add r7, r5, #1
	add r0, r5, #0
	mov r2, r9
	and r0, r2
	mov r6, r8
	mul r6, r0
_08030F2E:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08030F80 @ =0x0201930C
	add r1, r0, r1
	mov r0, #3
	ldrb r2, [r1, #6]
	and r0, r2
	cmp r0, #2
	bne _08030F62
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08030F62
	add r0, r5, #0
	add r1, r4, #0
	bl GetZoneCardType
	cmp r0, #1
	bne _08030F62
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl ChangeBattlePosition
_08030F62:
	add r4, #1
	cmp r4, #4
	ble _08030F2E
	add r5, r7, #0
	cmp r5, #1
	ble _08030F20
_08030F6E:
	mov r0, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08030F7C: .4byte 0x00000D64
_08030F80: .4byte 0x0201930C
	thumb_func_end EffectDragonCaptureJarResolve

