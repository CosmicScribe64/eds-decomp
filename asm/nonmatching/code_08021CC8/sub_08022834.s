	thumb_func_start sub_08022834
sub_08022834: @ 0x08022834
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r5, #0
	ldr r1, _080228A4 @ =0x020192E4
	mov r2, #1
	and r2, r6
	ldr r3, _080228A8 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r5, r0
	bge _080228FE
	add r7, r2, #0
	ldr r0, _080228AC @ =0x000007FF
	mov r8, r0
_08022858:
	lsl r1, r5, #2
	add r0, r7, #0
	mul r0, r3
	add r1, r1, r0
	ldr r0, _080228B0 @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r6, #0
	add r1, r4, #0
	bl sub_08054398
	cmp r0, #0
	beq _080228EC
	add r0, r4, #0
	bl sub_08007834
	cmp r0, #0
	bne _080228EC
	add r0, r4, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080228B4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080228C0
	cmp r0, #0x17
	ble _080228B8
	cmp r0, #0x18
	beq _080228BC
	b _080228C0
_080228A4: .4byte 0x020192E4
_080228A8: .4byte 0x00000D64
_080228AC: .4byte 0x000007FF
_080228B0: .4byte 0x02019968
_080228B4: .4byte gUnk_08621DE0
_080228B8:
	mov r0, #0
	b _080228D4
_080228BC:
	mov r0, #0xA
	b _080228D4
_080228C0:
	mov r0, r8
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _080228E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080228D4:
	cmp r0, #4
	bhi _080228EC
	add r0, r6, #0
	mov r1, #0xD
	mov r2, #0
	mov r3, #0
	bl sub_08022678
	mov r0, #1
	b _08022900
_080228E8: .4byte gUnk_08621DE0
_080228EC:
	add r5, #1
	ldr r1, _0802290C @ =0x020192E4
	ldr r3, _08022910 @ =0x00000D64
	add r0, r7, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r5, r0
	blt _08022858
_080228FE:
	mov r0, #0
_08022900:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802290C: .4byte 0x020192E4
_08022910: .4byte 0x00000D64
	thumb_func_end sub_08022834

