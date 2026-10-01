	thumb_func_start sub_080162C4
sub_080162C4: @ 0x080162C4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r7, #0
	ldr r0, _080163F0 @ =0x020185C0
	mov r8, r0
	mov r1, #0xAA
	lsl r1, r1, #4
	add r1, r8
	mov sl, r1
	mov r2, #0
	mov r9, r2
_080162E0:
	lsl r5, r7, #1
	ldr r3, _080163F4 @ =0x02018DD8
	add r2, r5, r3
	mov r0, #1
	and r0, r7
	ldr r1, _080163F8 @ =0x00000D64
	add r4, r0, #0
	mul r4, r1
	ldr r0, _080163FC @ =0x020192E4
	add r6, r4, r0
	ldrb r0, [r6, #3]
	strh r0, [r2]
	mov r0, #0x82
	lsl r0, r0, #4
	add r0, r8
	add r0, r9
	ldr r2, _08016400 @ =0x02019AA8
	add r1, r4, r2
	mov r2, #0xA0
	lsl r2, r2, #1
	bl sub_080752B0
	ldr r0, _08016404 @ =0x0000081C
	add r0, r8
	add r5, r5, r0
	ldrb r0, [r6, #5]
	strh r0, [r5]
	ldr r3, _08016408 @ =0x02019D28
	add r4, r4, r3
	mov r0, sl
	add r1, r4, #0
	mov r2, #0xA0
	lsl r2, r2, #1
	bl sub_080752B0
	mov r0, #0xA0
	lsl r0, r0, #1
	add sl, r0
	add r9, r0
	add r7, #1
	cmp r7, #1
	ble _080162E0
	ldr r0, _0801640C @ =0x02017A40
	ldr r1, _08016410 @ =0x0000056C
	bl sub_08075278
	ldr r1, _08016400 @ =0x02019AA8
	ldr r2, _08016414 @ =0xFFFFF83C
	add r4, r1, r2
	ldr r1, _08016418 @ =0x00001B0C
	add r0, r4, #0
	bl sub_08075278
	mov r7, #0
	ldr r3, _080163F4 @ =0x02018DD8
	mov r0, #0xA2
	lsl r0, r0, #2
	add r3, r3, r0
	mov r8, r3
	mov r1, #0
	mov sl, r1
_0801635A:
	mov r0, #1
	and r0, r7
	ldr r1, _080163F8 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r2, _080163FC @ =0x020192E4
	add r6, r5, r2
	lsl r4, r7, #1
	ldr r3, _080163F4 @ =0x02018DD8
	add r0, r4, r3
	ldrh r0, [r0]
	strb r0, [r6, #3]
	ldr r0, _0801641C @ =0x000007C4
	add r2, r2, r0
	mov r9, r2
	add r0, r5, r2
	ldr r2, _080163F0 @ =0x020185C0
	mov r3, #0x82
	lsl r3, r3, #4
	add r1, r2, r3
	add r1, sl
	mov r2, #0xA0
	lsl r2, r2, #1
	bl sub_080752B0
	ldr r1, _080163F0 @ =0x020185C0
	ldr r2, _08016404 @ =0x0000081C
	add r0, r1, r2
	add r4, r4, r0
	ldrh r0, [r4]
	strb r0, [r6, #5]
	ldr r3, _08016408 @ =0x02019D28
	add r5, r5, r3
	add r0, r5, #0
	mov r1, r8
	mov r2, #0xA0
	lsl r2, r2, #1
	bl sub_080752B0
	mov r0, #0xA0
	lsl r0, r0, #1
	add r8, r0
	add sl, r0
	add r7, #1
	cmp r7, #1
	ble _0801635A
	ldr r0, _08016414 @ =0xFFFFF83C
	add r0, r9
	mov r1, #0xFA
	lsl r1, r1, #5
	strh r1, [r0]
	mov r0, #0xB4
	lsl r0, r0, #3
	add r0, r9
	strh r1, [r0]
	ldr r1, _08016420 @ =0x0000134A
	add r1, r9
	mov r0, #0x1C
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r1, _080163F4 @ =0x02018DD8
	sub r1, #0xB
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080163F0: .4byte 0x020185C0
_080163F4: .4byte 0x02018DD8
_080163F8: .4byte 0x00000D64
_080163FC: .4byte 0x020192E4
_08016400: .4byte 0x02019AA8
_08016404: .4byte 0x0000081C
_08016408: .4byte 0x02019D28
_0801640C: .4byte 0x02017A40
_08016410: .4byte 0x0000056C
_08016414: .4byte 0xFFFFF83C
_08016418: .4byte 0x00001B0C
_0801641C: .4byte 0x000007C4
_08016420: .4byte 0x0000134A
	thumb_func_end sub_080162C4

