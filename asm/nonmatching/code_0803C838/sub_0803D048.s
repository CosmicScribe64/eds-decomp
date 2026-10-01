	thumb_func_start sub_0803D048
sub_0803D048: @ 0x0803D048
	push {r4, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r3, r1, #0
	mov r0, #0x80
	lsl r0, r0, #8
	and r0, r1
	cmp r0, #0
	beq _0803D07C
	mov r2, #1
	and r2, r4
	ldr r0, _0803D070 @ =0x00000FFF
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0803D074 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803D078 @ =0x02019968
	b _0803D09E
_0803D070: .4byte 0x00000FFF
_0803D074: .4byte 0x00000D64
_0803D078: .4byte 0x02019968
_0803D07C:
	mov r0, #0x80
	lsl r0, r0, #7
	and r0, r1
	cmp r0, #0
	bne _0803D08A
	mov r0, #0
	b _0803D0A6
_0803D08A:
	mov r2, #1
	and r2, r4
	ldr r0, _0803D0AC @ =0x00000FFF
	and r3, r0
	mov r0, #0x94
	mul r0, r3
	ldr r1, _0803D0B0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803D0B4 @ =0x0201930C
_0803D09E:
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
_0803D0A6:
	pop {r4}
	pop {r1}
	bx r1
_0803D0AC: .4byte 0x00000FFF
_0803D0B0: .4byte 0x00000D64
_0803D0B4: .4byte 0x0201930C
	thumb_func_end sub_0803D048

