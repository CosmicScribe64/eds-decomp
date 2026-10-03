	thumb_func_start EffectCeasefirePrepare
EffectCeasefirePrepare: @ 0x0802EEAC
	push {r4, r5, r6, r7, lr}
	mov r5, #0
	mov r0, #1
	mov ip, r0
	mov r4, #0
	ldr r7, _0802EEE0 @ =0x00000D64
	ldr r6, _0802EEE4 @ =0x0201930C
_0802EEBA:
	mov r2, #0
	add r3, r4, #0
_0802EEBE:
	add r0, r2, #0
	mov r1, ip
	and r0, r1
	mul r0, r7
	add r0, r3, r0
	add r1, r0, r6
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802EEE8
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0802EEE8
	mov r0, #1
	b _0802EEF8
_0802EEE0: .4byte 0x00000D64
_0802EEE4: .4byte 0x0201930C
_0802EEE8:
	add r2, #1
	cmp r2, #1
	ble _0802EEBE
	add r4, #0x94
	add r5, #1
	cmp r5, #4
	ble _0802EEBA
	mov r0, #0
_0802EEF8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectCeasefirePrepare
	.align 2, 0

