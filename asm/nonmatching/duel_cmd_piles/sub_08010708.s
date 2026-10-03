	thumb_func_start sub_08010708
sub_08010708: @ 0x08010708
	push {r4, r5, r6, r7, lr}
	ldr r0, _08010758 @ =0x020185C0
	ldrh r1, [r0, #4]
	lsl r5, r1, #0x10
	ldrh r2, [r0, #2]
	orr r5, r2
	mov r4, #0
	ldr r6, _0801075C @ =0x020192E4
	ldrh r1, [r0]
	lsr r2, r1, #0xF
	ldr r1, _08010760 @ =0x00000D64
	mul r2, r1
	add r3, r2, r6
	mov ip, r0
	ldrb r0, [r3, #4]
	cmp r4, r0
	bge _0801077C
	ldr r1, _08010764 @ =0x00000904
	add r0, r6, r1
	ldr r6, _08010768 @ =0x0000080D
	add r6, ip
	add r1, r3, #0
	mov r7, #0x21
	neg r7, r7
	add r2, r2, r0
_0801073A:
	ldr r0, [r2]
	cmp r0, r5
	bne _08010770
	ldr r2, _0801076C @ =0x00000906
	add r0, r3, r2
	mov r1, #0x80
	ldrb r2, [r0]
	orr r1, r2
	strb r1, [r0]
	add r0, r7, #0
	ldrb r1, [r6]
	and r0, r1
	strb r0, [r6]
	b _0801078A
	.align 2, 0
_08010758: .4byte 0x020185C0
_0801075C: .4byte 0x020192E4
_08010760: .4byte 0x00000D64
_08010764: .4byte 0x00000904
_08010768: .4byte 0x0000080D
_0801076C: .4byte 0x00000906
_08010770:
	add r3, #4
	add r2, #4
	add r4, #1
	ldrb r0, [r1, #4]
	cmp r4, r0
	blt _0801073A
_0801077C:
	ldr r1, _08010790 @ =0x0000080D
	add r1, ip
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801078A:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08010790: .4byte 0x0000080D
	thumb_func_end sub_08010708

