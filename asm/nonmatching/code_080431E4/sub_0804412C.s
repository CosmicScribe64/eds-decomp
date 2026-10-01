	thumb_func_start sub_0804412C
sub_0804412C: @ 0x0804412C
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r0, #1
	and r0, r4
	ldr r1, _08044164 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	ldr r3, _08044168 @ =0x02019BE8
	add r1, r2, r3
	lsl r0, r5, #2
	add r6, r1, r0
	add r0, r0, r2
	add r0, r0, r3
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r0, _0804416C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _08044170 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08044174 @ =0x00000776
	cmp r1, r0
	bne _08044178
	mov r0, #3
	b _080441DA
_08044164: .4byte 0x00000D64
_08044168: .4byte 0x02019BE8
_0804416C: .4byte 0x000007FF
_08044170: .4byte gUnk_08622AB4
_08044174: .4byte 0x00000776
_08044178:
	cmp r1, r0
	blt _08044188
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08044188
	mov r0, #1
	b _080441DA
_08044188:
	ldr r0, _080441AC @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _080441B0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _080441BA
	cmp r0, #0x16
	bgt _080441B4
	cmp r0, #0x15
	beq _080441BE
	b _080441C6
	.align 2, 0
_080441AC: .4byte 0x000007FF
_080441B0: .4byte gUnk_08621DE0
_080441B4:
	cmp r0, #0x17
	beq _080441C2
	b _080441C6
_080441BA:
	mov r0, #7
	b _080441DA
_080441BE:
	mov r0, #8
	b _080441DA
_080441C2:
	mov r0, #9
	b _080441DA
_080441C6:
	ldr r0, _08044210 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _08044214 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_080441DA:
	cmp r0, #0
	beq _0804421C
	cmp r0, #0
	blt _080441EA
	cmp r0, #3
	bgt _080441EA
	cmp r0, #2
	bge _08044206
_080441EA:
	mov r1, #1
	and r1, r4
	lsl r0, r5, #2
	ldr r2, _08044218 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	add r0, r0, r3
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl sub_08007834
	cmp r0, #0
	beq _0804421C
_08044206:
	ldr r0, [r6]
	lsl r0, r0, #0x11
	lsr r0, r0, #0x1F
	b _0804421E
	.align 2, 0
_08044210: .4byte 0x000007FF
_08044214: .4byte gUnk_08621DE0
_08044218: .4byte 0x00000D64
_0804421C:
	mov r0, #1
_0804421E:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0804412C

