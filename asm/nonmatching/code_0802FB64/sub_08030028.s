	thumb_func_start sub_08030028
sub_08030028: @ 0x08030028
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r5
	ldr r1, _08030068 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803006C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _080300C2
	ldr r0, _08030070 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08030074 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08030078 @ =0x0000023D
	cmp r1, r0
	beq _08030088
	cmp r1, r0
	bgt _0803007C
	cmp r1, #0x2F
	beq _08030088
	b _080300B8
	.align 2, 0
_08030068: .4byte 0x00000D64
_0803006C: .4byte 0x0201930C
_08030070: .4byte 0x000007FF
_08030074: .4byte gUnk_08622AB4
_08030078: .4byte 0x0000023D
_0803007C:
	ldr r0, _080300C8 @ =0x000004D9
	cmp r1, r0
	beq _08030088
	add r0, #0x10
	cmp r1, r0
	bne _080300B8
_08030088:
	mov r0, #0x90
	cmp r4, #0
	beq _08030090
	ldr r0, _080300CC @ =0x00008090
_08030090:
	lsl r1, r5, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _080300D0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _080300D4 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x40
	ldrb r2, [r1, #1]
	orr r0, r2
	strb r0, [r1, #1]
_080300B8:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #1
	bl sub_08018544
_080300C2:
	pop {r4, r5}
	pop {r0}
	bx r0
_080300C8: .4byte 0x000004D9
_080300CC: .4byte 0x00008090
_080300D0: .4byte 0x00000D64
_080300D4: .4byte 0x0201930C
	thumb_func_end sub_08030028

