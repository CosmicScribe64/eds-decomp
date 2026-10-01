	thumb_func_start sub_0801B640
sub_0801B640: @ 0x0801B640
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r3, #0
	mov r4, #0
	ldr r0, _0801B668 @ =0x02011C20
	mov r8, r0
	ldr r1, _0801B66C @ =0x0000FFFF
	mov ip, r1
	ldr r7, _0801B670 @ =0x000007CF
	ldr r5, _0801B674 @ =0x000007FF
	ldr r2, _0801B678 @ =0x08081A6C
_0801B65C:
	ldrh r0, [r2]
	add r1, r0, #0
	cmp r0, ip
	bne _0801B67C
	mov r0, #0
	b _0801B6A0
_0801B668: .4byte 0x02011C20
_0801B66C: .4byte 0x0000FFFF
_0801B670: .4byte 0x000007CF
_0801B674: .4byte 0x000007FF
_0801B678: .4byte gUnk_08081A6C
_0801B67C:
	cmp r0, r7
	bhi _0801B690
	and r0, r5
	lsl r0, r0, #1
	ldr r6, _0801B68C @ =0x08623DF4
	add r0, r0, r6
	ldrh r0, [r0]
	b _0801B6A0
_0801B68C: .4byte gUnk_08623DF4
_0801B690:
	ldr r6, _0801B6CC @ =0xFFFFF830
	add r0, r1, r6
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _0801B6D0 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_0801B6A0:
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r8
	ldrh r0, [r0, #8]
	lsl r0, r0, #0x16
	cmp r0, #0
	beq _0801B6B0
	add r4, #1
_0801B6B0:
	add r2, #2
	add r3, #1
	cmp r3, #0x3B
	bls _0801B65C
	mov r0, #0
	cmp r4, r9
	blt _0801B6C0
	mov r0, #1
_0801B6C0:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0801B6CC: .4byte 0xFFFFF830
_0801B6D0: .4byte gUnk_08623DF4
	thumb_func_end sub_0801B640

