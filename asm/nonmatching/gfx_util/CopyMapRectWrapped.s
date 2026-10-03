	thumb_func_start CopyMapRectWrapped
CopyMapRectWrapped: @ 0x0807AAE4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r6, r0, #0
	add r5, r1, #0
	ldr r0, [sp, #0x28]
	ldr r4, [sp, #0x2C]
	ldr r1, [sp, #0x30]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r0, #0
	lsr r3, r3, #0x18
	cmp r0, r3
	bcs _0807AB7C
	lsl r2, r2, #0x10
	mov r8, r2
	lsr r2, r2, #0x10
	str r2, [sp, #0]
	lsl r1, r1, #1
	str r1, [sp, #4]
	mov sl, r3
_0807AB24:
	mov r2, #0
	add r4, #1
	add r0, #1
	mov ip, r0
	ldr r0, [sp, #0]
	cmp r2, r0
	bcs _0807AB56
	mov r7, #0x1F
	mov r1, r8
	lsr r3, r1, #0x10
_0807AB38:
	mov r0, r9
	add r1, r0, r2
	add r0, r1, #0
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r5
	lsl r1, r1, #1
	add r1, r1, r6
	ldrh r1, [r1]
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, r3
	bcc _0807AB38
_0807AB56:
	ldr r1, [sp, #4]
	add r6, r6, r1
	lsl r0, r4, #0x18
	lsr r4, r0, #0x18
	cmp r4, #0x20
	bne _0807AB70
	ldr r2, _0807AB6C @ =0xFFFFF840
	add r5, r5, r2
	mov r4, #0
	b _0807AB72
	.align 2, 0
_0807AB6C: .4byte 0xFFFFF840
_0807AB70:
	add r5, #0x40
_0807AB72:
	mov r1, ip
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	cmp r0, sl
	bcc _0807AB24
_0807AB7C:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CopyMapRectWrapped

