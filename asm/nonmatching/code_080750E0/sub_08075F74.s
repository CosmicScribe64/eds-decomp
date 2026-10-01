	thumb_func_start sub_08075F74
sub_08075F74: @ 0x08075F74
	push {r4, r5, r6, r7, lr}
	ldr r2, _08075FA0 @ =0x03006644
	ldr r0, _08075FA4 @ =0x04000120
	ldr r1, [r0, #4]
	ldr r0, [r0]
	str r0, [r2]
	str r1, [r2, #4]
	ldr r0, _08075FA8 @ =0x0000FEFE
	ldrh r1, [r2]
	cmp r1, r0
	bne _08075FB0
	add r1, r2, #0
	sub r1, #0xB8
	ldr r0, [r1]
	cmp r0, #9
	ble _08075FB0
	mov r0, #3
	neg r0, r0
	str r0, [r1]
	ldr r4, _08075FAC @ =0xFFFFF51C
	add r5, r2, r4
	b _08076022
_08075FA0: .4byte 0x03006644
_08075FA4: .4byte 0x04000120
_08075FA8: .4byte 0x0000FEFE
_08075FAC: .4byte 0xFFFFF51C
_08075FB0:
	ldr r0, _08076078 @ =0x03005B60
	ldr r1, _0807607C @ =0x00000A2C
	add r2, r0, r1
	ldr r1, [r2]
	add r5, r0, #0
	cmp r1, #0
	blt _08076022
	ldr r4, _08076080 @ =0x00000AFC
	add r1, r5, r4
	mov r0, #0
	str r0, [r1]
	add r4, r1, #0
	mov ip, r2
	mov r0, #0xA3
	lsl r0, r0, #4
	add r7, r5, r0
	ldr r1, _08076084 @ =0x00000AE4
	add r6, r5, r1
_08075FD4:
	ldr r0, [r4]
	mov r1, ip
	ldr r2, [r1]
	ldr r3, [r7]
	lsl r2, r2, #1
	lsl r1, r0, #1
	add r0, r1, r0
	lsl r0, r0, #3
	add r0, r0, r3
	add r2, r2, r0
	add r1, r1, r6
	ldrh r0, [r1]
	strh r0, [r2]
	ldr r0, [r4]
	add r0, #1
	str r0, [r4]
	cmp r0, #1
	ble _08075FD4
	ldr r4, _0807607C @ =0x00000A2C
	add r0, r5, r4
	ldr r0, [r0]
	cmp r0, #9
	bne _08076022
	ldr r1, _08076088 @ =0x00000AEC
	add r0, r5, r1
	add r4, #8
	add r2, r5, r4
	ldr r3, [r2]
	str r3, [r0]
	mov r0, #0xA3
	lsl r0, r0, #4
	add r1, r5, r0
	ldr r0, [r1]
	str r0, [r2]
	str r3, [r1]
	sub r4, #0x13
	add r1, r5, r4
	mov r0, #1
	strb r0, [r1]
_08076022:
	add r1, r5, #0
	ldr r0, _0807607C @ =0x00000A2C
	add r2, r1, r0
	ldr r0, [r2]
	cmp r0, #0xA
	bgt _08076032
	add r0, #1
	str r0, [r2]
_08076032:
	ldr r4, _0807608C @ =0x00000A1E
	add r3, r1, r4
	ldrb r0, [r3]
	cmp r0, #0
	beq _08076042
	ldr r1, _08076090 @ =0x0400010E
	mov r0, #0
	strh r0, [r1]
_08076042:
	ldr r0, [r2]
	cmp r0, #9
	bgt _08076070
	cmp r0, #0
	blt _0807605A
	ldr r2, _08076094 @ =0x04000128
	lsl r0, r0, #1
	ldr r4, _08076098 @ =0x00000A3C
	add r1, r5, r4
	add r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2, #2]
_0807605A:
	ldrb r0, [r3]
	cmp r0, #0
	beq _08076070
	ldr r2, _08076094 @ =0x04000128
	ldrh r0, [r2]
	mov r1, #0x80
	orr r0, r1
	strh r0, [r2]
	ldr r1, _08076090 @ =0x0400010E
	mov r0, #0xC0
	strh r0, [r1]
_08076070:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08076078: .4byte 0x03005B60
_0807607C: .4byte 0x00000A2C
_08076080: .4byte 0x00000AFC
_08076084: .4byte 0x00000AE4
_08076088: .4byte 0x00000AEC
_0807608C: .4byte 0x00000A1E
_08076090: .4byte 0x0400010E
_08076094: .4byte 0x04000128
_08076098: .4byte 0x00000A3C
	thumb_func_end sub_08075F74

