	thumb_func_start sub_0805232C
sub_0805232C: @ 0x0805232C
	push {r4, lr}
	ldr r2, _08052348 @ =0x020192E0
	ldr r0, _0805234C @ =0x00001B62
	add r4, r2, r0
	ldrb r0, [r4]
	cmp r0, #0
	beq _08052358
	ldr r0, _08052350 @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _08052354 @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
	mov r0, #1
	b _08052376
_08052348: .4byte 0x020192E0
_0805234C: .4byte 0x00001B62
_08052350: .4byte 0x0201AE60
_08052354: .4byte 0x00001B64
_08052358:
	ldr r0, _0805237C @ =0x00000206
	ldr r1, _08052380 @ =0x0000040F
	ldr r3, _08052384 @ =0x08086210
	mov r2, #0xB
	bl sub_080602A4
	ldr r1, _08052388 @ =0x08052191
	ldr r2, _0805238C @ =0x080522C1
	mov r0, #5
	bl sub_08060308
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_08052376:
	pop {r4}
	pop {r1}
	bx r1
_0805237C: .4byte 0x00000206
_08052380: .4byte 0x0000040F
_08052384: .4byte gUnk_08086210
_08052388: .4byte sub_08052190
_0805238C: .4byte sub_080522C0
	thumb_func_end sub_0805232C

