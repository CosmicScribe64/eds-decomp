	thumb_func_start sub_0802E42C
sub_0802E42C: @ 0x0802E42C
	push {r4, r5, r6, r7, lr}
	mov r2, #0
	ldr r4, _0802E480 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	ldr r5, _0802E484 @ =0x00000D64
	mul r0, r5
	add r0, r0, r4
	ldrb r0, [r0, #2]
	cmp r2, r0
	bge _0802E4A8
	add r3, r1, #0
	mov r6, #1
	ldr r0, _0802E488 @ =0x00000684
	add r0, r0, r4
	mov ip, r0
	add r7, r4, #0
	ldr r4, _0802E48C @ =0x000007FF
_0802E452:
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	lsl r0, r2, #2
	mul r1, r5
	add r0, r0, r1
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802E490 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0802E494
	mov r0, #1
	b _0802E4AA
_0802E480: .4byte 0x020192E4
_0802E484: .4byte 0x00000D64
_0802E488: .4byte 0x00000684
_0802E48C: .4byte 0x000007FF
_0802E490: .4byte gUnk_08621DE0
_0802E494:
	add r2, #1
	lsr r0, r3, #0x1F
	add r1, r6, #0
	and r1, r0
	add r0, r1, #0
	mul r0, r5
	add r0, r0, r7
	ldrb r0, [r0, #2]
	cmp r2, r0
	blt _0802E452
_0802E4A8:
	mov r0, #0
_0802E4AA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802E42C

