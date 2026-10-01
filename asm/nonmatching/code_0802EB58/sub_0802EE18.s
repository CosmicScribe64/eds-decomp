	thumb_func_start sub_0802EE18
sub_0802EE18: @ 0x0802EE18
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r6, #0
	mov r2, #0
	ldr r4, _0802EE78 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	ldr r5, _0802EE7C @ =0x00000D64
	mul r0, r5
	add r0, r0, r4
	ldrb r0, [r0, #4]
	cmp r2, r0
	bge _0802EEA0
	add r3, r1, #0
	mov r7, #1
	ldr r0, _0802EE80 @ =0x00000904
	add r0, r0, r4
	mov r8, r0
	mov ip, r4
	ldr r4, _0802EE84 @ =0x000007FF
_0802EE44:
	lsr r0, r3, #0x1F
	add r1, r7, #0
	and r1, r0
	lsl r0, r2, #2
	mul r1, r5
	add r0, r0, r1
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802EE88 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0802EE70
	add r6, #1
_0802EE70:
	cmp r6, #4
	ble _0802EE8C
	mov r0, #1
	b _0802EEA2
_0802EE78: .4byte 0x020192E4
_0802EE7C: .4byte 0x00000D64
_0802EE80: .4byte 0x00000904
_0802EE84: .4byte 0x000007FF
_0802EE88: .4byte gUnk_08621DE0
_0802EE8C:
	add r2, #1
	lsr r0, r3, #0x1F
	add r1, r7, #0
	and r1, r0
	add r0, r1, #0
	mul r0, r5
	add r0, ip
	ldrb r0, [r0, #4]
	cmp r2, r0
	blt _0802EE44
_0802EEA0:
	mov r0, #0
_0802EEA2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802EE18

