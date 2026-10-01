	thumb_func_start sub_0807C1D0
sub_0807C1D0: @ 0x0807C1D0
	push {r4, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r0, _0807C1F8 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0807C1FC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0807C210
	cmp r0, #0x16
	bgt _0807C200
	cmp r0, #0x15
	beq _0807C206
	b _0807C220
_0807C1F8: .4byte 0x000007FF
_0807C1FC: .4byte gUnk_08621DE0
_0807C200:
	cmp r0, #0x17
	beq _0807C218
	b _0807C220
_0807C206:
	ldr r3, _0807C20C @ =0x08631558
	b _0807C2DE
	.align 2, 0
_0807C20C: .4byte gUnk_08631558
_0807C210:
	ldr r3, _0807C214 @ =0x0862EEC0
	b _0807C2DE
_0807C214: .4byte gUnk_0862EEC0
_0807C218:
	ldr r3, _0807C21C @ =0x08633BF0
	b _0807C2DE
_0807C21C: .4byte gUnk_08633BF0
_0807C220:
	ldr r0, _0807C238 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _0807C23C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0807C240 @ =0x00000776
	cmp r1, r0
	bne _0807C244
	mov r0, #3
	b _0807C2A6
	.align 2, 0
_0807C238: .4byte 0x000007FF
_0807C23C: .4byte gUnk_08622AB4
_0807C240: .4byte 0x00000776
_0807C244:
	cmp r1, r0
	blt _0807C254
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0807C254
	mov r0, #1
	b _0807C2A6
_0807C254:
	ldr r0, _0807C278 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0807C27C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0807C286
	cmp r0, #0x16
	bgt _0807C280
	cmp r0, #0x15
	beq _0807C28A
	b _0807C292
	.align 2, 0
_0807C278: .4byte 0x000007FF
_0807C27C: .4byte gUnk_08621DE0
_0807C280:
	cmp r0, #0x17
	beq _0807C28E
	b _0807C292
_0807C286:
	mov r0, #7
	b _0807C2A6
_0807C28A:
	mov r0, #8
	b _0807C2A6
_0807C28E:
	mov r0, #9
	b _0807C2A6
_0807C292:
	ldr r0, _0807C2B4 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0807C2B8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0807C2A6:
	cmp r0, #2
	beq _0807C2CC
	cmp r0, #2
	bgt _0807C2BC
	cmp r0, #1
	beq _0807C2C2
	b _0807C2DC
_0807C2B4: .4byte 0x000007FF
_0807C2B8: .4byte gUnk_08621DE0
_0807C2BC:
	cmp r0, #3
	beq _0807C2D4
	b _0807C2DC
_0807C2C2:
	ldr r3, _0807C2C8 @ =0x08627AF8
	b _0807C2DE
	.align 2, 0
_0807C2C8: .4byte gUnk_08627AF8
_0807C2CC:
	ldr r3, _0807C2D0 @ =0x0862A190
	b _0807C2DE
_0807C2D0: .4byte gUnk_0862A190
_0807C2D4:
	ldr r3, _0807C2D8 @ =0x0862C828
	b _0807C2DE
_0807C2D8: .4byte gUnk_0862C828
_0807C2DC:
	ldr r3, _0807C300 @ =0x08625460
_0807C2DE:
	mov r0, #0x84
	lsl r0, r0, #3
	mov r1, #0x20
	mov r2, #0x80
	lsl r2, r2, #1
	bl sub_08072EB0
	mov r3, #0xC0
	lsl r3, r3, #2
	mov r0, #2
	mov r1, #0xA2
	add r2, r4, #0
	bl sub_0807C058
	pop {r4}
	pop {r0}
	bx r0
_0807C300: .4byte gUnk_08625460
	thumb_func_end sub_0807C1D0

