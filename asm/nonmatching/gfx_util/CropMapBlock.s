	thumb_func_start CropMapBlock
CropMapBlock: @ 0x0807ADE8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r9, r0
	add r6, r2, #0
	ldr r0, [sp, #0x28]
	ldr r2, [sp, #0x2C]
	mov r8, r2
	ldr r4, [sp, #0x30]
	ldr r5, [sp, #0x34]
	ldr r2, [sp, #0x38]
	lsl r1, r1, #0x10
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r3, r8
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	lsl r4, r4, #0x18
	lsr r7, r4, #0x18
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	mov sl, r5
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsr r1, r1, #0xF
	add r9, r1
	ldr r3, [sp, #0]
	add r1, r6, #0
	mul r1, r3
	lsl r1, r1, #1
	mov r3, r9
	add r5, r3, r1
	mov r1, r8
	bl GetTilemapOffset
	ldr r1, _0807AE80 @ =0x0000FFFE
	and r1, r0
	ldr r0, [sp, #0x24]
	add r6, r0, r1
	mov r4, #0
	cmp r4, sl
	bcs _0807AE70
	ldr r1, _0807AE84 @ =0x001FFFFF
	mov r8, r1
_0807AE52:
	add r0, r5, #0
	add r1, r6, #0
	mov r2, r8
	and r2, r7
	bl CpuSet
	ldr r2, [sp, #0]
	lsl r0, r2, #1
	add r5, r5, r0
	add r6, #0x40
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, sl
	bcc _0807AE52
_0807AE70:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0807AE80: .4byte 0x0000FFFE
_0807AE84: .4byte 0x001FFFFF
	thumb_func_end CropMapBlock

