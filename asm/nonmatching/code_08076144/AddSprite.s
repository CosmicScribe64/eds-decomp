	thumb_func_start AddSprite
AddSprite: @ 0x080761F0
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov ip, r2
	lsl r2, r0, #0x10
	lsr r6, r2, #0x10
	lsr r3, r0, #0x10
	mov r2, #0xFF
	lsl r2, r2, #8
	and r2, r1
	lsl r1, r1, #8
	ldr r4, _08076248 @ =0xFFFFFE00
	add r0, r4, #0
	and r1, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	ldr r0, _0807624C @ =0x03000040
	ldr r7, _08076250 @ =0x00004830
	add r4, r0, r7
	ldrb r1, [r4]
	cmp r1, #0x80
	beq _08076242
	lsl r1, r1, #3
	ldr r7, _08076254 @ =0x00004430
	add r0, r0, r7
	add r1, r1, r0
	mov r0, #0xFF
	and r3, r0
	orr r2, r3
	strh r2, [r1]
	ldr r0, _08076258 @ =0x000001FF
	and r0, r6
	orr r0, r5
	strh r0, [r1, #2]
	mov r0, ip
	strh r0, [r1, #4]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08076242:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08076248: .4byte 0xFFFFFE00
_0807624C: .4byte 0x03000040
_08076250: .4byte 0x00004830
_08076254: .4byte 0x00004430
_08076258: .4byte 0x000001FF
	thumb_func_end AddSprite

