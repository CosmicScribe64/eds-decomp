	thumb_func_start sub_08036150
sub_08036150: @ 0x08036150
	push {r4, r5, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _080361C8
	ldr r0, _08036184 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08036188
	cmp r0, #0x80
	bne _080361C8
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #6
	mov r2, #0
	mov r3, #0
	bl sub_08022678
	mov r0, #0x7F
	b _080361CA
_08036184: .4byte 0x02017A40
_08036188:
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r4, #1
	sub r0, r4, r0
	ldr r1, _080361BC @ =0x020192E0
	ldr r2, _080361C0 @ =0x00001B64
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #1
	bl sub_080193B0
	ldrb r5, [r5, #2]
	and r4, r5
	mov r0, #0x60
	cmp r4, #0
	bne _080361AC
	ldr r0, _080361C4 @ =0x00008060
_080361AC:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x64
	b _080361CA
	.align 2, 0
_080361BC: .4byte 0x020192E0
_080361C0: .4byte 0x00001B64
_080361C4: .4byte 0x00008060
_080361C8:
	mov r0, #0
_080361CA:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08036150

