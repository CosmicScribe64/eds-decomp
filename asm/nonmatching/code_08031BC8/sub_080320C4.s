	thumb_func_start sub_080320C4
sub_080320C4: @ 0x080320C4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	ldr r2, _08032184 @ =0x020192E4
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	eor r0, r1
	ldr r1, _08032188 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	mov r9, r0
	mov r0, #4
	ldrb r3, [r5, #4]
	and r0, r3
	cmp r0, #0
	bne _080321B6
	mov r0, #0
	mov r8, r0
	mov r7, #1
	mov sl, r1
_080320F8:
	ldrb r2, [r5, #2]
	lsl r1, r2, #0x1F
	lsr r0, r1, #0x1F
	sub r0, r7, r0
	and r0, r7
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	ldr r3, _08032184 @ =0x020192E4
	add r0, r0, r3
	ldrb r0, [r0, #3]
	cmp r8, r0
	bge _080321AC
	lsr r0, r1, #0x1F
	sub r0, r7, r0
	and r0, r7
	mov r3, r8
	lsl r1, r3, #2
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	add r1, r1, r0
	ldr r0, _0803218C @ =0x02019AA8
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r6, r4, #0
	add r0, r7, #0
	and r0, r2
	mov r1, #0x61
	cmp r0, #0
	bne _0803213C
	ldr r1, _08032190 @ =0x00008061
_0803213C:
	add r0, r1, #0
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r3, _08032194 @ =0x000007FF
	add r1, r3, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08032198 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0803219C
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl sub_08019800
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r7, r0
	mov r1, r9
	mov r2, #1
	mov r3, #1
	bl sub_080193D4
	b _080321AC
_08032184: .4byte 0x020192E4
_08032188: .4byte 0x00000D64
_0803218C: .4byte 0x02019AA8
_08032190: .4byte 0x00008061
_08032194: .4byte 0x000007FF
_08032198: .4byte gUnk_08621DE0
_0803219C:
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r6, #0
	bl sub_08019840
	mov r0, #1
	add r9, r0
_080321AC:
	mov r1, #1
	add r8, r1
	mov r3, r8
	cmp r3, #2
	ble _080320F8
_080321B6:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_080320C4
	.align 2, 0

