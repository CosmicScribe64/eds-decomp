	thumb_func_start sub_08019078
sub_08019078: @ 0x08019078
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r1, r1, #0x10
	lsr r0, r1, #0x10
	mov sl, r0
	lsl r2, r2, #0x10
	lsr r0, r2, #0x10
	mov r9, r0
	mov r3, sl
	lsl r0, r3, #0x18
	lsr r5, r0, #0x18
	lsr r1, r1, #0x18
	mov r3, r9
	lsl r0, r3, #0x18
	lsr r6, r0, #0x18
	lsr r2, r2, #0x18
	mov r8, r2
	mov r0, #1
	mov ip, r0
	add r0, r5, #0
	mov r2, ip
	and r0, r2
	mov r7, #0x94
	mul r1, r7
	ldr r3, _08019110 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r2, _08019114 @ =0x0201930C
	add r4, r1, r2
	ldr r0, [r4]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08019186
	add r1, r6, #0
	mov r0, ip
	and r1, r0
	mov r0, r8
	mul r0, r7
	mul r1, r3
	add r0, r0, r1
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08019186
	mov r0, #0x82
	cmp r5, #0
	beq _080190E0
	ldr r0, _08019118 @ =0x00008082
_080190E0:
	mov r1, sl
	mov r2, r9
	mov r3, #0
	bl sub_0801EC58
	cmp r5, r6
	beq _08019186
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	ldr r0, _0801911C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08019120 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _08019124 @ =0x000001E3
	cmp r2, r0
	beq _08019128
	add r0, #0x3F
	cmp r2, r0
	beq _0801915C
	b _08019186
	.align 2, 0
_08019110: .4byte 0x00000D64
_08019114: .4byte 0x0201930C
_08019118: .4byte 0x00008082
_0801911C: .4byte 0x000007FF
_08019120: .4byte gUnk_08622AB4
_08019124: .4byte 0x000001E3
_08019128:
	mov r0, #0x20
	ldrb r4, [r4, #7]
	and r0, r4
	cmp r0, #0
	beq _08019186
	add r0, r5, #0
	bl sub_080197C0
	mov r1, #0xFA
	lsl r1, r1, #3
	add r0, r6, #0
	bl sub_08019860
	mov r0, #0x92
	cmp r6, #0
	beq _0801914A
	ldr r0, _08019158 @ =0x00008092
_0801914A:
	mov r1, r8
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	b _08019186
	.align 2, 0
_08019158: .4byte 0x00008092
_0801915C:
	mov r0, #0x20
	ldrb r4, [r4, #7]
	and r0, r4
	cmp r0, #0
	beq _08019186
	add r0, r5, #0
	bl sub_080197C0
	ldr r1, _08019194 @ =0x00000BB8
	add r0, r5, #0
	bl sub_08019980
	mov r0, #0x92
	cmp r6, #0
	beq _0801917C
	ldr r0, _08019198 @ =0x00008092
_0801917C:
	mov r1, r8
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08019186:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08019194: .4byte 0x00000BB8
_08019198: .4byte 0x00008092
	thumb_func_end sub_08019078

