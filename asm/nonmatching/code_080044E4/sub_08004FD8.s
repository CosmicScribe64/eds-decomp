	thumb_func_start sub_08004FD8
sub_08004FD8: @ 0x08004FD8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _0800520C @ =0x05000200
	ldr r1, _08005210 @ =0x0822C300
	mov sl, r1
	mov r2, #0x20
	bl sub_080752B0
	mov r0, #0x20
	mov r1, #0x10
	bl sub_08074B08
	ldr r0, _08005214 @ =0x0000100F
	mov r9, r0
	ldr r4, _08005218 @ =0x08081408
	mov r0, #9
	mov r1, #9
	mov r2, r9
	add r3, r4, #0
	bl sub_0807501C
	ldr r1, _0800521C @ =0x00001007
	mov r8, r1
	mov r0, #8
	mov r1, #8
	mov r2, r8
	add r3, r4, #0
	bl sub_0807501C
	ldr r6, _08005220 @ =0x00000C01
	mov r0, #0x73
	mov r1, #0xB
	add r2, r6, #0
	add r3, r4, #0
	bl sub_0807501C
	ldr r5, _08005224 @ =0x00000C0D
	mov r0, #0x72
	mov r1, #0xA
	add r2, r5, #0
	add r3, r4, #0
	bl sub_0807501C
	ldr r4, _08005228 @ =0x08081414
	mov r0, #1
	mov r1, #0x29
	mov r2, r9
	add r3, r4, #0
	bl sub_0807501C
	mov r0, #0
	mov r1, #0x28
	mov r2, r8
	add r3, r4, #0
	bl sub_0807501C
	mov r0, #0x69
	mov r1, #0x2B
	add r2, r6, #0
	add r3, r4, #0
	bl sub_0807501C
	mov r0, #0x68
	mov r1, #0x2A
	add r2, r5, #0
	add r3, r4, #0
	bl sub_0807501C
	ldr r0, _0800522C @ =0x06014000
	mov r1, #0
	bl sub_08075114
	mov r4, #0xA0
	lsl r4, r4, #0x13
	add r0, r4, #0
	mov r1, sl
	mov r2, #0x20
	bl sub_08075294
	mov r0, #0
	strh r0, [r4]
	ldr r3, _08005230 @ =0x087BDAA8
	mov r0, #0x20
	mov r1, #0x10
	mov r2, #0x10
	bl sub_08072FAC
	ldr r0, _08005234 @ =0x00000409
	mov r2, #0xAE
	lsl r2, r2, #2
	ldr r3, _08005238 @ =0x087C1DCC
	mov r1, #0xA0
	bl sub_0807326C
	ldr r0, _0800523C @ =0x00000809
	mov r2, #0xC4
	lsl r2, r2, #2
	ldr r3, _08005240 @ =0x087C0CD4
	mov r1, #0xB0
	bl sub_0807326C
	mov r0, #0xC0
	lsl r0, r0, #4
	mov r2, #0xE2
	lsl r2, r2, #2
	ldr r3, _08005244 @ =0x0867DFCC
	mov r1, #0xC0
	bl sub_0807326C
	mov r5, #0
	ldr r0, _08005248 @ =0x03004876
	mov r8, r0
	ldr r1, _0800524C @ =0x08198830
	mov r9, r1
	ldr r4, _08005250 @ =0xFFFFD3E6
	add r4, r8
	ldr r7, _08005254 @ =0x0000C388
_080050C8:
	mov r6, #0
	lsl r3, r5, #5
	add r5, #4
_080050CE:
	add r2, r3, r6
	lsl r0, r2, #1
	add r0, r0, r4
	strh r7, [r0]
	add r0, r2, #1
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r1, _08005258 @ =0x0000C389
	strh r1, [r0]
	add r0, r2, #2
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #3
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x20
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x21
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x22
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x23
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x41
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x42
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x43
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x60
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x61
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x62
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r0, r2, #0
	add r0, #0x63
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, #1
	strh r1, [r0]
	add r6, #4
	cmp r6, #0x1F
	ble _080050CE
	cmp r5, #0x1F
	ble _080050C8
	mov r5, #0
	mov r7, #0x1F
	mov r6, #0xF8
	lsl r6, r6, #2
	mov r4, #0xF8
	lsl r4, r4, #7
_0800519A:
	lsl r3, r5, #1
	ldr r1, _0800525C @ =0x05000180
	add r3, r3, r1
	ldrh r1, [r3]
	mov r2, #0x1F
	and r2, r1
	add r0, r1, #0
	and r0, r6
	and r1, r4
	lsr r2, r2, #1
	and r2, r7
	lsr r0, r0, #1
	and r0, r6
	lsr r1, r1, #1
	and r1, r4
	orr r2, r0
	orr r1, r2
	strh r1, [r3]
	add r5, #1
	cmp r5, #0xF
	ble _0800519A
	mov r0, r8
	mov r1, r9
	mov r2, #0x20
	bl sub_08075294
	ldr r1, _08005260 @ =0xFFFFBBDE
	add r1, r8
	ldr r0, _08005264 @ =0x08004F09
	str r0, [r1]
	ldr r3, _08005268 @ =0x04000208
	mov r5, #0
	strh r5, [r3]
	ldr r2, _0800526C @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08005270 @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	ldr r1, _08005274 @ =0x03000000
	ldr r0, _08005278 @ =0x08004ABD
	str r0, [r1, #4]
	mov r4, #1
	strh r4, [r3]
	strh r5, [r3]
	ldrh r0, [r2]
	mov r1, #2
	orr r0, r1
	strh r0, [r2]
	strh r4, [r3]
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800520C: .4byte 0x05000200
_08005210: .4byte gUnk_0822C300
_08005214: .4byte 0x0000100F
_08005218: .4byte gUnk_08081408
_0800521C: .4byte 0x00001007
_08005220: .4byte 0x00000C01
_08005224: .4byte 0x00000C0D
_08005228: .4byte gUnk_08081414
_0800522C: .4byte 0x06014000
_08005230: .4byte gUnk_087BDAA8
_08005234: .4byte 0x00000409
_08005238: .4byte gUnk_087C1DCC
_0800523C: .4byte 0x00000809
_08005240: .4byte gUnk_087C0CD4
_08005244: .4byte gUnk_0867DFCC
_08005248: .4byte 0x03004876
_0800524C: .4byte gUnk_08198830
_08005250: .4byte 0xFFFFD3E6
_08005254: .4byte 0x0000C388
_08005258: .4byte 0x0000C389
_0800525C: .4byte 0x05000180
_08005260: .4byte 0xFFFFBBDE
_08005264: .4byte sub_08004F08
_08005268: .4byte 0x04000208
_0800526C: .4byte 0x04000200
_08005270: .4byte 0x0000FFFD
_08005274: .4byte 0x03000000
_08005278: .4byte sub_08004ABC
	thumb_func_end sub_08004FD8

