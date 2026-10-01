	thumb_func_start sub_0801A09C
sub_0801A09C: @ 0x0801A09C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov ip, r1
	mov r3, #0
	ldr r4, _0801A100 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _0801A104 @ =0x00000D64
	mul r0, r1
	add r1, r0, r4
	ldrb r6, [r1, #3]
	cmp r3, r6
	bge _0801A120
	ldr r7, _0801A108 @ =0x000007C4
	add r6, r4, r7
	add r4, r0, #0
	lsl r2, r2, #0x10
	mov r8, r2
	mov r9, r1
	add r1, r4, r6
_0801A0CE:
	lsl r2, r3, #2
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r7, _0801A10C @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	cmp r0, ip
	bne _0801A114
	add r0, r4, r6
	add r0, r0, r2
	mov r3, #0x66
	cmp r5, #0
	beq _0801A0EC
	ldr r3, _0801A110 @ =0x00008066
_0801A0EC:
	ldrh r1, [r0]
	ldrh r2, [r0, #2]
	add r0, r3, #0
	mov r4, r8
	lsr r3, r4, #0x10
	bl sub_0801EC58
	mov r0, #1
	b _0801A122
	.align 2, 0
_0801A100: .4byte 0x020192E4
_0801A104: .4byte 0x00000D64
_0801A108: .4byte 0x000007C4
_0801A10C: .4byte gUnk_08622AB4
_0801A110: .4byte 0x00008066
_0801A114:
	add r1, #4
	add r3, #1
	mov r7, r9
	ldrb r7, [r7, #3]
	cmp r3, r7
	blt _0801A0CE
_0801A120:
	mov r0, #0
_0801A122:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0801A09C
	.align 2, 0

