	thumb_func_start sub_0803552C
sub_0803552C: @ 0x0803552C
	push {lr}
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r2, #4]
	and r0, r1
	cmp r0, #0
	bne _08035578
	ldr r0, _08035564 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _0803556C
	ldrh r1, [r2, #6]
	ldrb r0, [r2, #6]
	mov r2, #0x91
	cmp r0, #0
	beq _08035554
	ldr r2, _08035568 @ =0x00008091
_08035554:
	lsr r1, r1, #8
	add r0, r2, #0
	mov r2, #7
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x7F
	b _0803557A
_08035564: .4byte 0x02017A40
_08035568: .4byte 0x00008091
_0803556C:
	ldrb r0, [r2, #6]
	ldrh r2, [r2, #6]
	lsr r1, r2, #8
	mov r2, #1
	bl sub_08018544
_08035578:
	mov r0, #0
_0803557A:
	pop {r1}
	bx r1
	thumb_func_end sub_0803552C
	.align 2, 0

