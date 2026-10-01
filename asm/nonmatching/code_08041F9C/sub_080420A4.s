	thumb_func_start sub_080420A4
sub_080420A4: @ 0x080420A4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	mov r5, #5
	mov r0, #1
	and r0, r7
	ldr r1, _08042100 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_080420B8:
	mov r0, #0x94
	mul r0, r5
	add r0, r0, r6
	ldr r1, _08042104 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080420E8
	bl sub_0802CD28
	add r4, r0, #0
	ldr r0, _08042108 @ =0x02017EE8
	add r1, r7, #0
	add r2, r5, #0
	bl sub_08041DC4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r4, #1
	ble _080420E8
	cmp r0, #0
	bne _08042114
_080420E8:
	add r5, #1
	cmp r5, #9
	ble _080420B8
	ldr r1, _0804210C @ =0x020192E0
	ldr r2, _08042110 @ =0x00001B12
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	cmp r0, r7
	beq _08042118
	b _08042174
_08042100: .4byte 0x00000D64
_08042104: .4byte 0x0201930C
_08042108: .4byte 0x02017EE8
_0804210C: .4byte 0x020192E0
_08042110: .4byte 0x00001B12
_08042114:
	mov r0, #1
	b _08042176
_08042118:
	mov r5, #0
	add r3, r1, #4
	mov r1, #1
	and r1, r7
	ldr r2, _08042180 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	add r0, r0, r3
	ldrb r0, [r0, #2]
	cmp r5, r0
	bge _08042174
	add r6, r1, #0
	mov r8, r3
_08042132:
	lsl r1, r5, #2
	add r0, r6, #0
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08042184 @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08042164
	bl sub_0802CD28
	add r4, r0, #0
	ldr r0, _08042188 @ =0x02017EE8
	add r1, r7, #0
	add r2, r5, #0
	bl sub_08041F9C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r4, #1
	ble _08042164
	cmp r0, #0
	bne _08042114
_08042164:
	add r5, #1
	ldr r2, _08042180 @ =0x00000D64
	add r0, r6, #0
	mul r0, r2
	add r0, r8
	ldrb r0, [r0, #2]
	cmp r5, r0
	blt _08042132
_08042174:
	mov r0, #0
_08042176:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08042180: .4byte 0x00000D64
_08042184: .4byte 0x02019968
_08042188: .4byte 0x02017EE8
	thumb_func_end sub_080420A4

