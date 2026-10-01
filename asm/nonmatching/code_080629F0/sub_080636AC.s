	thumb_func_start sub_080636AC
sub_080636AC: @ 0x080636AC
	push {r4, r5, r6, lr}
	ldr r1, _080636C8 @ =0x03000040
	ldr r2, _080636CC @ =0x0000485A
	add r0, r1, r2
	ldrb r0, [r0]
	add r2, r1, #0
	cmp r0, #0xB
	bls _080636BE
	b _08063A20
_080636BE:
	lsl r0, r0, #2
	ldr r1, _080636D0 @ =0x080636D4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080636C8: .4byte 0x03000040
_080636CC: .4byte 0x0000485A
_080636D0: .4byte 0x080636D4
_080636D4:
	.4byte _08063704
	.4byte _08063934
	.4byte _08063954
	.4byte _0806395A
	.4byte _080639EC
	.4byte _08063A20
	.4byte _08063A20
	.4byte _08063A20
	.4byte _08063A20
	.4byte _08063A20
	.4byte _080639EC
	.4byte _08063A10
_08063704:
	ldr r4, _08063754 @ =0x04000208
	mov r5, #0
	strh r5, [r4]
	ldr r2, _08063758 @ =0x04000200
	ldrh r3, [r2]
	ldr r1, _0806375C @ =0x0000FFFD
	add r0, r1, #0
	and r0, r3
	strh r0, [r2]
	mov r3, #1
	strh r3, [r4]
	strh r5, [r4]
	ldrh r0, [r2]
	and r1, r0
	strh r1, [r2]
	ldr r1, _08063760 @ =0x03000000
	mov r0, #0
	str r0, [r1, #4]
	strh r3, [r4]
	bl sub_08006878
	ldr r1, _08063764 @ =0x02015160
	mov r3, #0x8A
	lsl r3, r3, #1
	add r0, r1, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1C
	mov r4, #0x81
	lsl r4, r4, #1
	add r1, r1, r4
	add r0, r0, r1
	ldrh r1, [r0]
	add r2, r1, #0
	ldr r0, _08063768 @ =0x0000FFFF
	cmp r1, r0
	bne _0806376C
	mov r0, #0
	b _0806379A
	.align 2, 0
_08063754: .4byte 0x04000208
_08063758: .4byte 0x04000200
_0806375C: .4byte 0x0000FFFD
_08063760: .4byte 0x03000000
_08063764: .4byte 0x02015160
_08063768: .4byte 0x0000FFFF
_0806376C:
	ldr r0, _08063780 @ =0x000007CF
	cmp r1, r0
	bhi _08063788
	add r0, #0x30
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08063784 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0806379A
_08063780: .4byte 0x000007CF
_08063784: .4byte gUnk_08623DF4
_08063788:
	ldr r3, _080637C4 @ =0xFFFFF830
	add r0, r2, r3
	ldr r1, _080637C8 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r4, _080637CC @ =0x08623DF4
	add r0, r0, r4
	ldrh r0, [r0]
	add r0, #1
_0806379A:
	ldr r2, _080637D0 @ =0x02013D90
	strh r0, [r2, #2]
	ldr r3, _080637D4 @ =0x02015160
	mov r1, #0x8A
	lsl r1, r1, #1
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1C
	mov r4, #0x81
	lsl r4, r4, #1
	add r1, r3, r4
	add r0, r0, r1
	ldrh r1, [r0]
	add r4, r1, #0
	ldr r0, _080637D8 @ =0x0000FFFF
	add r5, r3, #0
	cmp r1, r0
	bne _080637DC
	mov r0, #0
	b _0806380A
_080637C4: .4byte 0xFFFFF830
_080637C8: .4byte 0x000007FF
_080637CC: .4byte gUnk_08623DF4
_080637D0: .4byte 0x02013D90
_080637D4: .4byte 0x02015160
_080637D8: .4byte 0x0000FFFF
_080637DC:
	ldr r0, _080637F0 @ =0x000007CF
	cmp r1, r0
	bhi _080637F8
	add r0, #0x30
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _080637F4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0806380A
_080637F0: .4byte 0x000007CF
_080637F4: .4byte gUnk_08623DF4
_080637F8:
	ldr r3, _08063830 @ =0xFFFFF830
	add r0, r4, r3
	ldr r1, _08063834 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r4, _08063838 @ =0x08623DF4
	add r0, r0, r4
	ldrh r0, [r0]
	add r0, #1
_0806380A:
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r0, _08063834 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0806383C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0806384A
	cmp r0, #0x17
	ble _08063840
	cmp r0, #0x18
	beq _08063844
	b _0806384A
_08063830: .4byte 0xFFFFF830
_08063834: .4byte 0x000007FF
_08063838: .4byte gUnk_08623DF4
_0806383C: .4byte gUnk_08621DE0
_08063840:
	mov r0, #0
	b _08063860
_08063844:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08063860
_0806384A:
	ldr r0, _08063884 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r3, _08063888 @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08063860:
	str r0, [r2, #0x2C]
	mov r4, #0x8A
	lsl r4, r4, #1
	add r0, r5, r4
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1C
	mov r3, #0x81
	lsl r3, r3, #1
	add r1, r5, r3
	add r0, r0, r1
	ldrh r1, [r0]
	add r3, r1, #0
	ldr r0, _0806388C @ =0x0000FFFF
	cmp r1, r0
	bne _08063890
	mov r0, #0
	b _080638BE
_08063884: .4byte 0x000007FF
_08063888: .4byte gUnk_08621DE0
_0806388C: .4byte 0x0000FFFF
_08063890:
	ldr r0, _080638A4 @ =0x000007CF
	cmp r1, r0
	bhi _080638AC
	add r0, #0x30
	and r1, r0
	lsl r0, r1, #1
	ldr r4, _080638A8 @ =0x08623DF4
	add r0, r0, r4
	ldrh r0, [r0]
	b _080638BE
_080638A4: .4byte 0x000007CF
_080638A8: .4byte gUnk_08623DF4
_080638AC:
	ldr r1, _080638E4 @ =0xFFFFF830
	add r0, r3, r1
	ldr r1, _080638E8 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _080638EC @ =0x08623DF4
	add r0, r0, r3
	ldrh r0, [r0]
	add r0, #1
_080638BE:
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r0, _080638E8 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r4, _080638F0 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080638FE
	cmp r0, #0x17
	ble _080638F4
	cmp r0, #0x18
	beq _080638F8
	b _080638FE
_080638E4: .4byte 0xFFFFF830
_080638E8: .4byte 0x000007FF
_080638EC: .4byte gUnk_08623DF4
_080638F0: .4byte gUnk_08621DE0
_080638F4:
	mov r0, #0
	b _08063914
_080638F8:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08063914
_080638FE:
	ldr r0, _08063920 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08063924 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08063928 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08063914:
	str r0, [r2, #0x30]
	ldr r0, _0806392C @ =0x03000040
	ldr r2, _08063930 @ =0x0000485A
	add r0, r0, r2
	b _080639FC
	.align 2, 0
_08063920: .4byte 0x000007FF
_08063924: .4byte gUnk_08621DE0
_08063928: .4byte 0x000001FF
_0806392C: .4byte 0x03000040
_08063930: .4byte 0x0000485A
_08063934:
	bl sub_0800696C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063A02
	bl sub_08006B80
	ldr r0, _0806394C @ =0x03000040
	ldr r3, _08063950 @ =0x0000485A
	add r0, r0, r3
	b _080639FC
	.align 2, 0
_0806394C: .4byte 0x03000040
_08063950: .4byte 0x0000485A
_08063954:
	bl sub_08006A98
	b _080639F0
_0806395A:
	bl sub_08006AE8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063970
	ldr r0, _080639E0 @ =0x03000040
	ldr r1, _080639E4 @ =0x0000485A
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_08063970:
	ldr r6, _080639E0 @ =0x03000040
	mov r0, #0x10
	ldrh r2, [r6, #6]
	and r0, r2
	cmp r0, #0
	beq _080639A8
	ldr r4, _080639E8 @ =0x02015160
	mov r3, #0x8A
	lsl r3, r3, #1
	add r4, r4, r3
	ldrb r5, [r4]
	lsl r0, r5, #0x1D
	lsr r0, r0, #0x1D
	add r0, #1
	mov r1, #5
	bl __modsi3
	mov r1, #7
	and r0, r1
	mov r1, #8
	neg r1, r1
	and r1, r5
	orr r1, r0
	strb r1, [r4]
	ldr r4, _080639E4 @ =0x0000485A
	add r1, r6, r4
	mov r0, #0xA
	strb r0, [r1]
_080639A8:
	mov r0, #0x20
	ldrh r1, [r6, #6]
	and r0, r1
	cmp r0, #0
	beq _08063A02
	ldr r4, _080639E8 @ =0x02015160
	mov r2, #0x8A
	lsl r2, r2, #1
	add r4, r4, r2
	ldrb r5, [r4]
	lsl r0, r5, #0x1D
	lsr r0, r0, #0x1D
	add r0, #4
	mov r1, #5
	bl __modsi3
	mov r1, #7
	and r0, r1
	mov r1, #8
	neg r1, r1
	and r1, r5
	orr r1, r0
	strb r1, [r4]
	ldr r3, _080639E4 @ =0x0000485A
	add r1, r6, r3
	mov r0, #0xA
	strb r0, [r1]
	b _08063A02
_080639E0: .4byte 0x03000040
_080639E4: .4byte 0x0000485A
_080639E8: .4byte 0x02015160
_080639EC:
	bl sub_08006ABC
_080639F0:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063A02
	ldr r0, _08063A08 @ =0x03000040
	ldr r4, _08063A0C @ =0x0000485A
	add r0, r0, r4
_080639FC:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_08063A02:
	mov r0, #0
	b _08063A22
	.align 2, 0
_08063A08: .4byte 0x03000040
_08063A0C: .4byte 0x0000485A
_08063A10:
	ldr r0, _08063A1C @ =0x0000485A
	add r1, r2, r0
	mov r0, #0
	strb r0, [r1]
	b _08063A22
	.align 2, 0
_08063A1C: .4byte 0x0000485A
_08063A20:
	mov r0, #1
_08063A22:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_080636AC

