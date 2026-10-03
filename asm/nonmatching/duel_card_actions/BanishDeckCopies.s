	thumb_func_start BanishDeckCopies
BanishDeckCopies: @ 0x0801A010
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	mov r4, #0
	ldr r2, _0801A088 @ =0x020192E4
	mov r1, #1
	and r1, r6
	ldr r3, _0801A08C @ =0x00000D64
	add r0, r1, #0
	mul r0, r3
	add r0, r0, r2
	ldrb r0, [r0, #3]
	cmp r4, r0
	bge _0801A07E
	add r5, r1, #0
	ldr r0, _0801A090 @ =0x000007C4
	add r7, r2, r0
_0801A03A:
	lsl r2, r4, #2
	add r1, r5, #0
	mul r1, r3
	add r0, r2, r1
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r3, _0801A094 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, r8
	bne _0801A06C
	add r0, r1, r7
	add r0, r0, r2
	mov r3, #0x68
	cmp r6, #0
	beq _0801A060
	ldr r3, _0801A098 @ =0x00008068
_0801A060:
	ldrh r1, [r0]
	ldrh r2, [r0, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
_0801A06C:
	add r4, #1
	ldr r1, _0801A088 @ =0x020192E4
	ldr r3, _0801A08C @ =0x00000D64
	add r0, r5, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r4, r0
	blt _0801A03A
_0801A07E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801A088: .4byte 0x020192E4
_0801A08C: .4byte 0x00000D64
_0801A090: .4byte 0x000007C4
_0801A094: .4byte gCardIdToNumber
_0801A098: .4byte 0x00008068
	thumb_func_end BanishDeckCopies

