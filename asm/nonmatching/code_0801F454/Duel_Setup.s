	thumb_func_start Duel_Setup
Duel_Setup: @ 0x0801F744
	push {r4, r5, lr}
	ldr r0, _0801F7D8 @ =0x02015EE8
	mov r1, #8
	bl MemClear16
	ldr r4, _0801F7DC @ =0x020192E0
	ldr r1, _0801F7E0 @ =0x00001B78
	add r0, r4, #0
	bl MemClear16
	ldr r5, _0801F7E4 @ =0x0201CFB0
	mov r1, #0x86
	lsl r1, r1, #4
	add r0, r5, #0
	bl MemClear16
	ldr r0, _0801F7E8 @ =0x0201D810
	mov r1, #0xC4
	lsl r1, r1, #2
	bl MemClear16
	ldr r0, _0801F7EC @ =0x0201AE60
	ldr r1, _0801F7F0 @ =0x00002124
	bl MemClear16
	ldr r0, _0801F7F4 @ =0x020185C0
	mov r1, #0xD2
	lsl r1, r1, #4
	bl MemClear16
	ldr r0, _0801F7F8 @ =0x0201CF90
	mov r1, #0x14
	bl MemClear16
	ldr r0, _0801F7FC @ =0x02017A40
	ldr r1, _0801F800 @ =0x0000056C
	bl MemClear16
	ldr r0, _0801F804 @ =0x02017A30
	mov r1, #0x10
	bl MemClear16
	ldr r0, _0801F808 @ =0x02017FB0
	ldr r1, _0801F80C @ =0x00000494
	bl MemClear16
	ldr r0, _0801F810 @ =0x02018450
	mov r1, #0xB0
	lsl r1, r1, #1
	bl MemClear16
	ldr r0, _0801F814 @ =0x00001B12
	add r4, r4, r0
	mov r0, #0xC0
	ldrb r1, [r4]
	orr r0, r1
	strb r0, [r4]
	bl FadeOutBGM
	ldr r1, _0801F818 @ =0x03000040
	mov r0, #0xC0
	lsl r0, r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _0801F7D0
	mov r0, #1
	ldrb r1, [r5]
	orr r0, r1
	strb r0, [r5]
_0801F7D0:
	mov r0, #1
	pop {r4, r5}
	pop {r1}
	bx r1
_0801F7D8: .4byte 0x02015EE8
_0801F7DC: .4byte 0x020192E0
_0801F7E0: .4byte 0x00001B78
_0801F7E4: .4byte 0x0201CFB0
_0801F7E8: .4byte 0x0201D810
_0801F7EC: .4byte 0x0201AE60
_0801F7F0: .4byte 0x00002124
_0801F7F4: .4byte 0x020185C0
_0801F7F8: .4byte 0x0201CF90
_0801F7FC: .4byte 0x02017A40
_0801F800: .4byte 0x0000056C
_0801F804: .4byte 0x02017A30
_0801F808: .4byte 0x02017FB0
_0801F80C: .4byte 0x00000494
_0801F810: .4byte 0x02018450
_0801F814: .4byte 0x00001B12
_0801F818: .4byte 0x03000040
	thumb_func_end Duel_Setup

