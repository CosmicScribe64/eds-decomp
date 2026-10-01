	thumb_func_start sub_080254C8
sub_080254C8: @ 0x080254C8
	push {r4, r5, lr}
	ldr r1, _08025584 @ =0x03000040
	ldr r0, _08025588 @ =0x0000040E
	add r1, r1, r0
	mov r5, #0
	mov r2, #0
	mov r0, #1
	strh r0, [r1]
	ldr r0, _0802558C @ =0x04000016
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #6
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	add r0, #6
	strh r2, [r0]
	sub r0, #2
	strh r2, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08025590 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	ldr r4, _08025594 @ =0x0201F820
	add r0, r4, #0
	bl sub_0807A2EC
	mov r1, #0xC3
	lsl r1, r1, #3
	add r0, r4, r1
	bl sub_0807B534
	ldr r1, _08025598 @ =0x00000AC4
	add r0, r4, r1
	strb r5, [r0]
	add r1, #1
	add r0, r4, r1
	strb r5, [r0]
	add r1, #1
	add r0, r4, r1
	strb r5, [r0]
	ldr r0, _0802559C @ =0x00000ACB
	add r1, r4, r0
	mov r0, #0xFF
	strb r0, [r1]
	ldr r1, _080255A0 @ =0x00000ACC
	add r0, r4, r1
	strb r5, [r0]
	mov r0, #0xAD
	lsl r0, r0, #4
	add r3, r4, r0
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl sub_0807B0EC
	mov r1, #0x80
	lsl r1, r1, #1
	ldr r0, _080255A4 @ =0x00000AD8
	add r3, r4, r0
	mov r0, #0
	mov r2, #2
	bl sub_0807B100
	mov r1, #0xAE
	lsl r1, r1, #4
	add r3, r4, r1
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl sub_0807B0EC
	ldr r0, _080255A8 @ =0x00000AE8
	add r1, r4, r0
	mov r0, #0xB0
	lsl r0, r0, #5
	strh r0, [r1]
	ldr r0, _080255AC @ =0x00000AEC
	add r1, r4, r0
	mov r0, #0x80
	lsl r0, r0, #7
	strh r0, [r1]
	ldr r1, _080255B0 @ =0x00000AEE
	add r4, r4, r1
	mov r0, #0xAC
	lsl r0, r0, #2
	strh r0, [r4]
	mov r0, #1
	pop {r4, r5}
	pop {r1}
	bx r1
_08025584: .4byte 0x03000040
_08025588: .4byte 0x0000040E
_0802558C: .4byte 0x04000016
_08025590: .4byte 0x0000E0FF
_08025594: .4byte 0x0201F820
_08025598: .4byte 0x00000AC4
_0802559C: .4byte 0x00000ACB
_080255A0: .4byte 0x00000ACC
_080255A4: .4byte 0x00000AD8
_080255A8: .4byte 0x00000AE8
_080255AC: .4byte 0x00000AEC
_080255B0: .4byte 0x00000AEE
	thumb_func_end sub_080254C8

