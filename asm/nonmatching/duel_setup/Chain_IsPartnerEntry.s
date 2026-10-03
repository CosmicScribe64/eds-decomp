	thumb_func_start Chain_IsPartnerEntry
Chain_IsPartnerEntry: @ 0x0801FA48
	add r2, r0, #0
	ldr r1, _0801FA7C @ =0x02015EE8
	mov r3, #1
	add r0, r3, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0801FA8C
	add r0, r3, #0
	ldrb r1, [r2, #2]
	and r0, r1
	cmp r0, #0
	beq _0801FA8C
	ldr r0, _0801FA80 @ =0x000007FF
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _0801FA84 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0801FA88 @ =0x000003B6
	ldrh r0, [r0]
	cmp r0, r1
	beq _0801FA8C
	mov r0, #1
	b _0801FA8E
	.align 2, 0
_0801FA7C: .4byte 0x02015EE8
_0801FA80: .4byte 0x000007FF
_0801FA84: .4byte gCardIdToNumber
_0801FA88: .4byte 0x000003B6
_0801FA8C:
	mov r0, #0
_0801FA8E:
	bx lr
	thumb_func_end Chain_IsPartnerEntry

