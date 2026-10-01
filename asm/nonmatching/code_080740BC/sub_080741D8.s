	thumb_func_start sub_080741D8
sub_080741D8: @ 0x080741D8
	ldr r1, _08074204 @ =0x03005B60
	ldr r2, _08074208 @ =0x00000A1F
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08074202
	add r2, #5
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08074202
	ldr r2, _0807420C @ =0x04000128
	ldr r0, _08074210 @ =0x0000FEFE
	strh r0, [r2, #2]
	ldrh r0, [r2]
	mov r1, #0x80
	orr r0, r1
	strh r0, [r2]
	ldr r1, _08074214 @ =0x0400010E
	mov r0, #0xC0
	strh r0, [r1]
_08074202:
	bx lr
_08074204: .4byte 0x03005B60
_08074208: .4byte 0x00000A1F
_0807420C: .4byte 0x04000128
_08074210: .4byte 0x0000FEFE
_08074214: .4byte 0x0400010E
	thumb_func_end sub_080741D8

