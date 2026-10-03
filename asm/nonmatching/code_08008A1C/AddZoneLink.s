	thumb_func_start AddZoneLink
AddZoneLink: @ 0x0800935C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	lsl r1, r0, #8
	lsr r1, r1, #0x18
	mov r8, r1
	lsr r0, r0, #0x18
	mov ip, r0
	mov r1, #1
	mov r0, r8
	and r1, r0
	mov r0, #0x94
	mov r6, ip
	mul r6, r0
	ldr r0, _080093C8 @ =0x00000D64
	add r4, r1, #0
	mul r4, r0
	add r0, r6, r4
	ldr r1, _080093CC @ =0x0201930C
	add r5, r0, r1
	add r0, r5, #0
	add r0, #0x8A
	ldrh r3, [r0]
	mov sl, r1
	cmp r2, #0xA
	beq _080093DA
	mov r2, #0
	cmp r2, r3
	bge _080093DA
	add r0, r4, #0
	add r0, #0x4A
	add r0, r6, r0
	add r1, r0, r1
	add r0, r5, #0
	add r0, #0xA
_080093B2:
	ldrh r4, [r0]
	cmp r4, r7
	bne _080093D0
	ldrh r2, [r1]
	lsr r0, r2, #8
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	ldrb r4, [r1]
	orr r0, r4
	b _0800940E
_080093C8: .4byte 0x00000D64
_080093CC: .4byte 0x0201930C
_080093D0:
	add r1, #2
	add r0, #2
	add r2, #1
	cmp r2, r3
	blt _080093B2
_080093DA:
	mov r2, #1
	mov r0, r8
	and r2, r0
	mov r0, #0x94
	mov r1, ip
	mul r1, r0
	add r0, r1, #0
	ldr r1, _08009420 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	mov r4, sl
	add r2, r0, r4
	lsl r1, r3, #1
	add r0, r2, #0
	add r0, #0xA
	add r0, r0, r1
	strh r7, [r0]
	add r0, r2, #0
	add r0, #0x4A
	add r0, r0, r1
	mov r1, r9
	strh r1, [r0]
	add r1, r2, #0
	add r1, #0x8A
	ldrh r0, [r1]
	add r0, #1
_0800940E:
	strh r0, [r1]
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08009420: .4byte 0x00000D64
	thumb_func_end AddZoneLink

