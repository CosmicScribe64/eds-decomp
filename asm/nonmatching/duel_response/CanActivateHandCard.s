	thumb_func_start CanActivateHandCard
CanActivateHandCard: @ 0x08041F9C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	add r6, r1, #0
	mov r0, #1
	and r0, r6
	lsl r2, r2, #2
	ldr r1, _08042008 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r7, _0804200C @ =0x02019968
	add r2, r2, r7
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldr r5, _08042010 @ =0x000007FF
	and r5, r4
	add r0, r6, #0
	add r1, r4, #0
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	beq _08042004
	lsl r0, r5, #2
	ldr r1, _08042014 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08042004
	add r0, r4, #0
	bl GetCardSpellSpeed
	cmp r0, #1
	bgt _0804201C
	ldr r1, _08042018 @ =0x0000148A
	add r0, r7, r1
	ldrb r1, [r0]
	lsl r0, r1, #0x1B
	lsr r0, r0, #0x1D
	cmp r0, #2
	beq _08041FFC
	cmp r0, #4
	bne _08042004
_08041FFC:
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	cmp r0, r6
	beq _0804201C
_08042004:
	mov r0, #0
	b _0804205E
_08042008: .4byte 0x00000D64
_0804200C: .4byte 0x02019968
_08042010: .4byte 0x000007FF
_08042014: .4byte gCardStats
_08042018: .4byte 0x0000148A
_0804201C:
	mov r0, r8
	strh r4, [r0]
	ldr r0, _08042068 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _0804206C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08042050
	cmp r0, #0x15
	blt _08042050
	ldr r2, _08042070 @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08042074 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #7]
	lsr r0, r0, #6
	cmp r0, #0
	bne _08042004
_08042050:
	mov r0, r8
	mov r1, #0
	mov r2, #1
	bl CanActivateEffect
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_0804205E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08042068: .4byte 0x000007FF
_0804206C: .4byte gCardStats
_08042070: .4byte 0x020192E4
_08042074: .4byte 0x00000D64
	thumb_func_end CanActivateHandCard

