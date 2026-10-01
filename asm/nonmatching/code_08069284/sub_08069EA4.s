	thumb_func_start sub_08069EA4
sub_08069EA4: @ 0x08069EA4
	push {r4, r5, lr}
	add r4, r0, #0
	add r3, r1, #0
	lsl r2, r2, #0x18
	lsr r5, r2, #0x18
	mov r2, #0
_08069EB0:
	sub r0, r5, #1
	cmp r0, #7
	bhi _08069F30
	lsl r0, r0, #2
	ldr r1, _08069EC0 @ =0x08069EC4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08069EC0: .4byte 0x08069EC4
_08069EC4:
	.4byte _08069EE4
	.4byte _08069EE8
	.4byte _08069EEE
	.4byte _08069EFC
	.4byte _08069F02
	.4byte _08069F0E
	.4byte _08069F16
	.4byte _08069F28
_08069EE4:
	mov r0, #0xF
	b _08069EF0
_08069EE8:
	ldrb r0, [r4]
	strh r0, [r3]
	b _08069F30
_08069EEE:
	ldr r0, _08069EF8 @ =0x00000FFF
_08069EF0:
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r3]
	b _08069F30
_08069EF8: .4byte 0x00000FFF
_08069EFC:
	ldrh r0, [r4]
	strh r0, [r3]
	b _08069F30
_08069F02:
	ldrh r0, [r4, #2]
	strh r0, [r3]
	mov r0, #0xF
	ldrh r1, [r4]
	and r0, r1
	b _08069F2E
_08069F0E:
	ldrh r0, [r4, #2]
	strh r0, [r3]
	ldrb r0, [r4]
	b _08069F2E
_08069F16:
	ldrh r0, [r4, #2]
	strh r0, [r3]
	ldr r0, _08069F24 @ =0x00000FFF
	ldrh r1, [r4]
	and r0, r1
	b _08069F2E
	.align 2, 0
_08069F24: .4byte 0x00000FFF
_08069F28:
	ldrh r0, [r4, #2]
	strh r0, [r3]
	ldrh r0, [r4]
_08069F2E:
	strh r0, [r3, #2]
_08069F30:
	add r3, #4
	add r4, #4
	add r2, #1
	cmp r2, #7
	ble _08069EB0
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08069EA4

