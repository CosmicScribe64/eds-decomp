	thumb_func_start sub_0800D7D4
sub_0800D7D4: @ 0x0800D7D4
	push {r4, lr}
	ldr r4, _0800D810 @ =0x020185C0
	ldrh r0, [r4]
	lsr r2, r0, #0xF
	mov r0, #0x94
	ldrh r3, [r4, #2]
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0800D814 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0800D818 @ =0x0201930C
	add r1, r1, r0
	ldr r0, _0800D81C @ =0xFFFFF000
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	bl sub_080611AC
	ldr r3, _0800D820 @ =0x0000080D
	add r4, r4, r3
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800D810: .4byte 0x020185C0
_0800D814: .4byte 0x00000D64
_0800D818: .4byte 0x0201930C
_0800D81C: .4byte 0xFFFFF000
_0800D820: .4byte 0x0000080D
	thumb_func_end sub_0800D7D4

