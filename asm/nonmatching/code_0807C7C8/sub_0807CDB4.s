	thumb_func_start sub_0807CDB4
sub_0807CDB4: @ 0x0807CDB4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r0, _0807CF08 @ =0x03000040
	mov r9, r0
	ldr r1, _0807CF0C @ =0x0000040E
	add r1, r9
	mov r5, #0
	mov r0, #3
	strh r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #0x13
	strh r5, [r0]
	ldr r1, _0807CF10 @ =0x04000008
	mov r0, #4
	strh r0, [r1]
	add r1, #2
	ldr r2, _0807CF14 @ =0x00000105
	add r0, r2, #0
	strh r0, [r1]
	bl sub_08073574
	bl sub_08073498
	bl sub_08075630
	ldr r0, _0807CF18 @ =0x0400004C
	strh r5, [r0]
	bl sub_080759F4
	bl sub_080757AC
	ldr r0, _0807CF1C @ =0x00000414
	add r0, r9
	str r5, [r0]
	ldr r4, _0807CF20 @ =0x04000208
	strh r5, [r4]
	ldr r2, _0807CF24 @ =0x04000200
	ldrh r3, [r2]
	ldr r1, _0807CF28 @ =0x0000FFFD
	add r0, r1, #0
	and r0, r3
	strh r0, [r2]
	mov r3, #1
	strh r3, [r4]
	strh r5, [r4]
	ldrh r0, [r2]
	and r1, r0
	strh r1, [r2]
	ldr r0, _0807CF2C @ =0x03000000
	str r5, [r0, #4]
	strh r3, [r4]
	ldr r0, _0807CF30 @ =0x05000200
	ldr r1, _0807CF34 @ =0x0870B5E0
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _0807CF38 @ =0x05000220
	ldr r1, _0807CF3C @ =0x0870B600
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _0807CF40 @ =0x05000240
	ldr r1, _0807CF44 @ =0x0870C620
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _0807CF48 @ =0x087095E0
	mov r1, #0
	mov r2, #0x10
	mov r3, #8
	bl sub_0807CC78
	ldr r0, _0807CF4C @ =0x0870A5E0
	mov r1, #0x10
	mov r2, #0x10
	mov r3, #8
	bl sub_0807CC78
	ldr r0, _0807CF50 @ =0x0870B620
	mov r1, #0x80
	lsl r1, r1, #1
	mov r2, #0x10
	mov r3, #8
	bl sub_0807CC78
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r1, _0807CF54 @ =0x0822C300
	mov r2, #0x20
	bl sub_08075294
	mov r2, #0x80
	lsl r2, r2, #2
	ldr r3, _0807CF58 @ =0x08707B28
	mov r0, #0
	mov r1, #0x10
	bl sub_080731D0
	ldr r7, _0807CF5C @ =0x0201F780
	ldrh r0, [r7]
	cmp r0, #0
	beq _0807CEFA
	ldrh r3, [r7]
	lsl r0, r3, #6
	ldr r1, _0807CF60 @ =0x0822C720
	mov r8, r1
	add r0, r8
	bl sub_080753E0
	add r4, r0, #0
	mov r5, #0xC
	cmp r4, #0x12
	ble _0807CE9C
	mov r5, #0xA
_0807CE9C:
	mov r0, #0x20
	mov r1, #3
	bl sub_08074B08
	mul r4, r5
	asr r4, r4, #1
	mov r0, #0x78
	sub r0, r0, r4
	lsr r6, r5, #1
	mov r1, #0xD
	sub r1, r1, r6
	lsl r5, r5, #8
	mov r3, #8
	add r2, r5, #0
	orr r2, r3
	ldrh r3, [r7]
	lsl r3, r3, #6
	mov ip, r3
	add r3, r8
	bl sub_0807501C
	mov r0, #0x77
	sub r0, r0, r4
	mov r1, #0xC
	sub r1, r1, r6
	mov r2, #7
	orr r5, r2
	ldrh r7, [r7]
	lsl r3, r7, #6
	add r3, r8
	add r2, r5, #0
	bl sub_0807501C
	ldr r0, _0807CF64 @ =0x06005000
	mov r1, #0
	bl sub_08075114
	mov r2, #0
	ldr r1, _0807CF68 @ =0x0000085C
	add r1, r9
_0807CEEC:
	add r0, r2, #0
	add r0, #0x80
	strh r0, [r1]
	add r1, #2
	add r2, #1
	cmp r2, #0x5F
	ble _0807CEEC
_0807CEFA:
	mov r0, #1
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0807CF08: .4byte 0x03000040
_0807CF0C: .4byte 0x0000040E
_0807CF10: .4byte 0x04000008
_0807CF14: .4byte 0x00000105
_0807CF18: .4byte 0x0400004C
_0807CF1C: .4byte 0x00000414
_0807CF20: .4byte 0x04000208
_0807CF24: .4byte 0x04000200
_0807CF28: .4byte 0x0000FFFD
_0807CF2C: .4byte 0x03000000
_0807CF30: .4byte 0x05000200
_0807CF34: .4byte gUnk_0870B5E0
_0807CF38: .4byte 0x05000220
_0807CF3C: .4byte gUnk_0870B600
_0807CF40: .4byte 0x05000240
_0807CF44: .4byte gUnk_0870C620
_0807CF48: .4byte gUnk_087095E0
_0807CF4C: .4byte gUnk_0870A5E0
_0807CF50: .4byte gUnk_0870B620
_0807CF54: .4byte gUnk_0822C300
_0807CF58: .4byte gUnk_08707B28
_0807CF5C: .4byte 0x0201F780
_0807CF60: .4byte gUnk_0822C720
_0807CF64: .4byte 0x06005000
_0807CF68: .4byte 0x0000085C
	thumb_func_end sub_0807CDB4

