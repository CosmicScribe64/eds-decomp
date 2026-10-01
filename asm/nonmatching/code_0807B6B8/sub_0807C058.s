	thumb_func_start sub_0807C058
sub_0807C058: @ 0x0807C058
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r7, r2, #0x10
	lsl r3, r3, #0x10
	lsr r5, r3, #0x10
	mov r2, #0xE0
	lsl r2, r2, #0xB
	and r2, r0
	lsr r2, r2, #5
	ldr r0, _0807C0B0 @ =0x0300045C
	add r4, r2, r0
	lsr r1, r1, #0xF
	add r4, r4, r1
	ldr r0, _0807C0B4 @ =0x0000FFFF
	cmp r7, r0
	bne _0807C0B8
	mov r0, #0
	mov r8, r0
	mov r1, #0
_0807C08A:
	strh r1, [r4]
	strh r1, [r4, #2]
	strh r1, [r4, #4]
	strh r1, [r4, #6]
	strh r1, [r4, #8]
	strh r1, [r4, #0xA]
	strh r1, [r4, #0xC]
	strh r1, [r4, #0xE]
	strh r1, [r4, #0x10]
	strh r1, [r4, #0x12]
	add r4, #0x40
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	cmp r0, #9
	bls _0807C08A
	b _0807C1A4
_0807C0B0: .4byte 0x0300045C
_0807C0B4: .4byte 0x0000FFFF
_0807C0B8:
	mov r1, #0
	mov r8, r1
	lsr r3, r3, #0x11
	lsl r5, r5, #5
	mov r9, r5
_0807C0C2:
	mov r5, #0
	add r6, r4, #0
	add r6, #0x40
_0807C0C8:
	lsl r2, r5, #1
	add r2, r2, r4
	add r1, r3, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	strh r1, [r2]
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	cmp r5, #8
	bls _0807C0C8
	add r4, r6, #0
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	cmp r0, #9
	bls _0807C0C2
	ldr r0, _0807C1B4 @ =0x000007FF
	and r7, r0
	ldr r0, _0807C1B8 @ =0x05000100
	lsl r1, r7, #7
	ldr r2, _0807C1BC @ =0x08608360
	add r1, r1, r2
	mov r2, #0x80
	bl sub_08075294
	lsl r0, r7, #4
	add r0, r0, r7
	lsl r0, r0, #3
	sub r0, r0, r7
	lsl r0, r0, #5
	ldr r1, _0807C1C0 @ =0x082A6500
	add r1, r1, r0
	mov ip, r1
	ldr r7, _0807C1C4 @ =0x06004000
	add r7, r9
	mov r2, #0
	mov r8, r2
	mov r0, #0x3F
	mov sl, r0
	ldr r1, _0807C1C8 @ =0x00008080
	mov r9, r1
_0807C122:
	mov r2, ip
	ldrh r4, [r2]
	ldrh r2, [r2, #2]
	mov r0, ip
	ldrh r5, [r0, #4]
	add r6, r4, #0
	mov r1, sl
	and r6, r1
	add r0, r4, #0
	mov r1, #0xFC
	lsl r1, r1, #4
	and r0, r1
	lsl r0, r0, #2
	orr r6, r0
	lsr r4, r4, #0xC
	mov r1, #3
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #4
	orr r4, r0
	mov r0, #0xFC
	and r0, r2
	lsl r0, r0, #6
	orr r4, r0
	lsr r2, r2, #8
	add r3, r2, #0
	mov r0, sl
	and r3, r0
	lsr r2, r2, #6
	mov r1, #0xF
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #2
	orr r2, r0
	lsl r2, r2, #8
	orr r3, r2
	lsr r5, r5, #4
	add r0, r5, #0
	mov r1, sl
	and r0, r1
	mov r2, #0xFC
	lsl r2, r2, #4
	and r5, r2
	lsl r5, r5, #2
	orr r0, r5
	mov r1, r9
	orr r6, r1
	strh r6, [r7]
	orr r4, r1
	strh r4, [r7, #2]
	orr r3, r1
	strh r3, [r7, #4]
	orr r0, r1
	strh r0, [r7, #6]
	mov r2, #6
	add ip, r2
	add r7, #8
	mov r0, r8
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldr r0, _0807C1CC @ =0x000002CF
	cmp r8, r0
	bls _0807C122
_0807C1A4:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807C1B4: .4byte 0x000007FF
_0807C1B8: .4byte 0x05000100
_0807C1BC: .4byte gUnk_08608360
_0807C1C0: .4byte gUnk_082A6500
_0807C1C4: .4byte 0x06004000
_0807C1C8: .4byte 0x00008080
_0807C1CC: .4byte 0x000002CF
	thumb_func_end sub_0807C058

