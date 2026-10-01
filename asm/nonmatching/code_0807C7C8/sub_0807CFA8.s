	thumb_func_start sub_0807CFA8
sub_0807CFA8: @ 0x0807CFA8
	push {r4, r5, lr}
	ldr r5, _0807CFDC @ =0x0201F780
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r3, [r5, #3]
	lsl r2, r3, #0x1B
	lsr r2, r2, #0x1C
	bl sub_0807CCAC
	ldr r4, _0807CFE0 @ =0x03000040
	ldrh r1, [r4, #6]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _0807CFE8
	mov r0, #2
	bl sub_08077AEC
	ldr r0, _0807CFE4 @ =0x00004859
	add r1, r4, r0
	mov r0, #4
	strb r0, [r1]
	b _0807D056
_0807CFDC: .4byte 0x0201F780
_0807CFE0: .4byte 0x03000040
_0807CFE4: .4byte 0x00004859
_0807CFE8:
	mov r2, #1
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _0807D01C
	mov r0, #3
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #3
	beq _0807D008
	ldr r3, _0807D004 @ =0x00004859
	add r1, r4, r3
	mov r0, #6
	b _0807D00E
_0807D004: .4byte 0x00004859
_0807D008:
	ldr r0, _0807D018 @ =0x00004859
	add r1, r4, r0
	mov r0, #8
_0807D00E:
	strb r0, [r1]
	mov r0, #1
	bl sub_08077AEC
	b _0807D056
_0807D018: .4byte 0x00004859
_0807D01C:
	mov r0, #0xC0
	and r0, r1
	cmp r0, #0
	beq _0807D056
	add r0, r2, #0
	ldrb r1, [r5, #2]
	and r0, r1
	cmp r0, #0
	beq _0807D050
	mov r0, #0
	bl sub_08077AEC
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #2]
	b _0807D056
_0807D050:
	mov r0, #3
	bl sub_08077AEC
_0807D056:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0807CFA8
	.align 2, 0

