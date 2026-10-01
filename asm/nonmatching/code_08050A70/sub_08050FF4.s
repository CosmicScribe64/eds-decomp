	thumb_func_start sub_08050FF4
sub_08050FF4: @ 0x08050FF4
	push {r4, r5, lr}
	ldr r1, _0805100C @ =0x02017FB0
	ldr r0, _08051010 @ =0x0000048D
	add r5, r1, r0
	ldrb r0, [r5]
	cmp r0, #0
	beq _08051014
	cmp r0, #1
	beq _08051068
_08051006:
	mov r0, #1
	b _0805108C
	.align 2, 0
_0805100C: .4byte 0x02017FB0
_08051010: .4byte 0x0000048D
_08051014:
	ldr r2, _08051058 @ =0x0000045C
	add r0, r1, r2
	ldrh r0, [r0]
	bl sub_08047058
	ldr r4, _0805105C @ =0x02017A40
	ldr r3, _08051060 @ =0x000003D6
	add r1, r4, r3
	strh r0, [r1]
	lsl r0, r0, #0x10
	cmp r0, #0
	blt _08051006
	mov r0, #0x90
	lsl r0, r0, #3
	add r3, r4, r0
	ldr r2, _08051064 @ =0x0819A9D4
	mov r0, #0
	ldsh r1, [r1, r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r2, #0x10
	add r0, r0, r2
	ldr r0, [r0]
	str r0, [r3]
	cmp r0, #0
	beq _08051006
	mov r1, #0xF9
	lsl r1, r1, #2
	add r0, r4, r1
	mov r1, #0
	strb r1, [r0]
	b _08051084
	.align 2, 0
_08051058: .4byte 0x0000045C
_0805105C: .4byte 0x02017A40
_08051060: .4byte 0x000003D6
_08051064: .4byte gUnk_0819A9D4
_08051068:
	ldr r2, _08051094 @ =0x02017A40
	mov r3, #0x90
	lsl r3, r3, #3
	add r2, r2, r3
	sub r3, #0x24
	add r0, r1, r3
	add r3, #0x14
	add r1, r1, r3
	ldr r2, [r2]
	bl _call_via_r2
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0805108A
_08051084:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_0805108A:
	mov r0, #0
_0805108C:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08051094: .4byte 0x02017A40
	thumb_func_end sub_08050FF4

