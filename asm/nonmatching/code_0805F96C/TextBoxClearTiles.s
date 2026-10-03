	thumb_func_start TextBoxClearTiles
TextBoxClearTiles: @ 0x0805FCF4
	push {r4, r5, lr}
	ldr r5, _0805FD1C @ =0x0201AE84
	mov r4, #0xD7
_0805FCFA:
	add r0, r5, #0
	ldr r1, _0805FD20 @ =0x06009A40
	mov r2, #0x20
	bl CopyDoubleWords
	add r5, #0x20
	sub r4, #1
	cmp r4, #0
	bge _0805FCFA
	ldr r1, _0805FD24 @ =0x0201AE60
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
_0805FD1C: .4byte 0x0201AE84
_0805FD20: .4byte 0x06009A40
_0805FD24: .4byte 0x0201AE60
	thumb_func_end TextBoxClearTiles

