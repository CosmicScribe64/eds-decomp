	thumb_func_start FormatStr
FormatStr: @ 0x080753F4
	push {r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	add r1, r2, #0
	b _08075426
_080753FE:
	cmp r0, #0x25
	bne _0807541E
	ldrb r0, [r4, #1]
	cmp r0, #0x73
	bne _0807541E
	mov r0, #0
	strb r0, [r5]
	add r4, #2
	add r0, r5, #0
	bl StrCat
	add r0, r5, #0
	add r1, r4, #0
	bl StrCat
	b _0807542C
_0807541E:
	ldrb r0, [r4]
	strb r0, [r5]
	add r4, #1
	add r5, #1
_08075426:
	ldrb r0, [r4]
	cmp r0, #0
	bne _080753FE
_0807542C:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end FormatStr
	.align 2, 0

