	thumb_func_start LinkSioCheckRecvData
LinkSioCheckRecvData: @ 0x08074260
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov sl, r0
	ldr r4, _0807431C @ =0x04000208
	mov r3, #0
	strh r3, [r4]
	ldr r5, _08074320 @ =0x03005B60
	ldr r0, _08074324 @ =0x00000B08
	add r0, r0, r5
	mov r8, r0
	ldr r1, _08074328 @ =0x00000A38
	add r6, r5, r1
	ldr r2, [r6]
	str r2, [r0]
	ldr r7, _0807432C @ =0x00000A34
	add r1, r5, r7
	ldr r0, [r1]
	str r0, [r6]
	str r2, [r1]
	ldr r0, _08074330 @ =0x00000A21
	add r1, r5, r0
	ldrb r2, [r1]
	add r7, #0xDC
	add r0, r5, r7
	mov r7, #0
	strh r2, [r0]
	strb r3, [r1]
	mov r0, #1
	strh r0, [r4]
	ldr r0, _08074334 @ =0x00000A22
	add r1, r5, r0
	strh r7, [r1]
	cmp r2, #0
	beq _08074372
	ldr r2, _08074338 @ =0x00000AFC
	add r0, r5, r2
	str r7, [r0]
	mov r9, r8
	add r4, r0, #0
	ldr r3, _0807433C @ =0x00000B12
	add r7, r5, r3
	mov r0, #0
	mov r8, r0
	add r2, #4
	add r6, r5, r2
	add r5, r1, #0
_080742C4:
	ldr r0, [r4]
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	ldr r3, _08074340 @ =0x03006598
	ldr r0, [r3]
	add r0, r0, r1
	mov r1, r9
	str r0, [r1]
	mov r2, r8
	strh r2, [r7]
	mov r3, r8
	str r3, [r6]
	add r2, r0, #0
_080742E0:
	ldr r1, [r6]
	lsl r0, r1, #1
	add r0, r0, r2
	ldrh r3, [r7]
	ldrh r0, [r0]
	add r0, r3, r0
	strh r0, [r7]
	add r1, #1
	str r1, [r6]
	cmp r1, #9
	bls _080742E0
	lsl r1, r0, #0x10
	ldr r0, _08074344 @ =0xFFFF0000
	cmp r1, r0
	bne _08074348
	mov r1, r9
	ldr r0, [r1]
	add r0, #4
	ldr r1, [r4]
	lsl r1, r1, #4
	add r1, sl
	mov r2, #8
	bl CpuSet
	ldr r1, [r4]
	mov r0, #1
	lsl r0, r1
	ldrh r2, [r5]
	orr r0, r2
	b _08074354
_0807431C: .4byte 0x04000208
_08074320: .4byte 0x03005B60
_08074324: .4byte 0x00000B08
_08074328: .4byte 0x00000A38
_0807432C: .4byte 0x00000A34
_08074330: .4byte 0x00000A21
_08074334: .4byte 0x00000A22
_08074338: .4byte 0x00000AFC
_0807433C: .4byte 0x00000B12
_08074340: .4byte 0x03006598
_08074344: .4byte 0xFFFF0000
_08074348:
	ldr r1, [r4]
	add r1, #4
	mov r0, #1
	lsl r0, r1
	ldrh r3, [r5]
	orr r0, r3
_08074354:
	strh r0, [r5]
	mov r0, r8
	str r0, [sp, #0]
	mov r2, r9
	ldr r1, [r2]
	add r1, #4
	mov r0, sp
	ldr r2, _08074398 @ =0x05000004
	bl CpuSet
	ldr r0, [r4]
	add r0, #1
	str r0, [r4]
	cmp r0, #1
	ble _080742C4
_08074372:
	ldr r0, _0807439C @ =0x03005B60
	mov r3, #0xA2
	lsl r3, r3, #4
	add r2, r0, r3
	ldr r7, _080743A0 @ =0x00000A22
	add r0, r0, r7
	ldrb r1, [r2]
	ldrb r3, [r0]
	orr r1, r3
	strb r1, [r2]
	ldrh r0, [r0]
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08074398: .4byte 0x05000004
_0807439C: .4byte 0x03005B60
_080743A0: .4byte 0x00000A22
	thumb_func_end LinkSioCheckRecvData

