	thumb_func_start sub_08051730
sub_08051730: @ 0x08051730
	push {r4, lr}
	ldr r0, _08051748 @ =0x0201AE60
	add r4, r0, #0
	add r4, #0x23
	ldrb r0, [r4]
	cmp r0, #1
	beq _08051780
	cmp r0, #1
	bgt _0805174C
	cmp r0, #0
	beq _08051752
	b _080517B2
_08051748: .4byte 0x0201AE60
_0805174C:
	cmp r0, #2
	beq _0805178C
	b _080517B2
_08051752:
	ldr r0, _08051778 @ =0x020192E0
	ldr r1, _0805177C @ =0x00001B54
	add r0, r0, r1
	ldrh r2, [r0]
	mov r0, #1
	and r0, r2
	mov r1, #2
	and r1, r2
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	bl sub_0805163C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080517B2
_08051770:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _080517B2
_08051778: .4byte 0x020192E0
_0805177C: .4byte 0x00001B54
_08051780:
	mov r0, #0
	mov r1, #0xB
	mov r2, #0
	bl sub_08024134
	b _08051770
_0805178C:
	ldr r0, _080517A4 @ =0x02017A40
	ldr r1, _080517A8 @ =0x000004FD
	add r0, r0, r1
	ldrb r1, [r0]
	sub r1, #1
	strb r1, [r0]
	lsl r1, r1, #0x18
	cmp r1, #0
	bne _080517AC
	mov r0, #1
	b _080517B4
	.align 2, 0
_080517A4: .4byte 0x02017A40
_080517A8: .4byte 0x000004FD
_080517AC:
	mov r0, #0
	strb r0, [r4]
	b _080517B4
_080517B2:
	mov r0, #0
_080517B4:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08051730
	.align 2, 0

