	thumb_func_start DrawBgFullwidthString
DrawBgFullwidthString: @ 0x08072AF8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r7, r3, #0
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r5, r2, #0x10
	ldr r0, _08072B90 @ =0x0300045C
	mov r8, r0
	lsl r0, r1, #8
	lsr r0, r0, #0x18
	str r0, [sp, #0]
	lsr r1, r1, #0x18
	str r1, [sp, #4]
	mov r9, r4
	lsl r0, r4, #1
	add r8, r0
	mov r2, #0x1F
	mov sl, r2
_08072B28:
	ldrb r0, [r7]
	cmp r0, #0
	beq _08072BA4
	bl AsciiToFullwidthSjis
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r3, r0, #0
	cmp r3, #0
	beq _08072B8C
	add r2, r4, #0
	mov r0, sl
	and r2, r0
	ldr r6, _08072B94 @ =0x03000040
	ldr r0, _08072B98 @ =0x0000441E
	add r1, r6, r0
	mov r0, sl
	ldrh r1, [r1]
	and r0, r1
	sub r0, #2
	cmp r2, r0
	blt _08072B68
	mov r0, r9
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r9, r4
	lsl r1, r4, #1
	ldr r2, _08072B9C @ =0x0000041C
	add r0, r6, r2
	add r1, r1, r0
	mov r8, r1
_08072B68:
	lsl r1, r5, #5
	ldr r0, _08072BA0 @ =0x06004000
	add r1, r1, r0
	add r0, r3, #0
	ldr r2, [sp, #0]
	ldr r3, [sp, #4]
	bl RenderSjisGlyphTile
	mov r2, r8
	strh r5, [r2]
	mov r0, #2
	add r8, r0
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
_08072B8C:
	add r7, #1
	b _08072B28
_08072B90: .4byte 0x0300045C
_08072B94: .4byte 0x03000040
_08072B98: .4byte 0x0000441E
_08072B9C: .4byte 0x0000041C
_08072BA0: .4byte 0x06004000
_08072BA4:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DrawBgFullwidthString

