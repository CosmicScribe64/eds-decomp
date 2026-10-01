	thumb_func_start sub_08003920
sub_08003920: @ 0x08003920
	push {r4, r5, lr}
	ldr r4, _08003958 @ =0x03000040
	ldr r0, _0800395C @ =0x0000485A
	add r5, r4, r0
	ldrb r1, [r5]
	cmp r1, #0
	beq _08003974
	cmp r1, #1
	beq _0800398C
	ldr r0, _08003960 @ =0x05000200
	ldr r1, _08003964 @ =0x087DE858
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _08003968 @ =0x06010000
	ldr r1, _0800396C @ =0x087DE878
	mov r2, #0x80
	lsl r2, r2, #7
	bl sub_080752B0
	ldr r3, _08003970 @ =0x087D4B24
	mov r0, #0
	mov r1, #0
	mov r2, #0x20
	bl sub_08072FAC
	mov r0, #1
	b _080039B2
_08003958: .4byte 0x03000040
_0800395C: .4byte 0x0000485A
_08003960: .4byte 0x05000200
_08003964: .4byte gUnk_087DE858
_08003968: .4byte 0x06010000
_0800396C: .4byte gUnk_087DE878
_08003970: .4byte gUnk_087D4B24
_08003974:
	mov r0, #0x80
	lsl r0, r0, #0x13
	strh r1, [r0]
	ldr r4, _08003988 @ =0x02015ED8
	ldrh r0, [r4]
	mov r1, #7
	bl __umodsi3
	strh r0, [r4]
	b _080039AA
_08003988: .4byte 0x02015ED8
_0800398C:
	bl sub_080759F4
	bl sub_08073574
	bl sub_080757AC
	ldr r1, _080039B8 @ =0x0400000A
	mov r0, #0x84
	strh r0, [r1]
	ldr r0, _080039BC @ =0x0000040E
	add r1, r4, r0
	mov r0, #3
	strh r0, [r1]
	bl sub_08077B54
_080039AA:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
_080039B2:
	pop {r4, r5}
	pop {r1}
	bx r1
_080039B8: .4byte 0x0400000A
_080039BC: .4byte 0x0000040E
	thumb_func_end sub_08003920

