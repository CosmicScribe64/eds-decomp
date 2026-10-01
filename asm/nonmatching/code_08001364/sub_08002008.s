	thumb_func_start sub_08002008
sub_08002008: @ 0x08002008
	push {lr}
	ldr r1, _08002024 @ =0x0201F7D0
	mov r0, #8
	ldrb r1, [r1, #8]
	and r0, r1
	cmp r0, #0
	beq _08002030
	ldr r0, _08002028 @ =0x06007080
	ldr r1, _0800202C @ =0x087F1798
	mov r2, #0x96
	lsl r2, r2, #6
	bl sub_08075294
	b _0800203C
_08002024: .4byte 0x0201F7D0
_08002028: .4byte 0x06007080
_0800202C: .4byte gUnk_087F1798
_08002030:
	ldr r0, _08002040 @ =0x06011080
	ldr r1, _08002044 @ =0x087F1798
	mov r2, #0x96
	lsl r2, r2, #6
	bl sub_08075294
_0800203C:
	pop {r0}
	bx r0
_08002040: .4byte 0x06011080
_08002044: .4byte gUnk_087F1798
	thumb_func_end sub_08002008

