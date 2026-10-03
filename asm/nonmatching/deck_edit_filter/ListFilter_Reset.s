	thumb_func_start ListFilter_Reset
ListFilter_Reset: @ 0x08069A90
	push {r4, lr}
	ldr r1, _08069ACC @ =0x0201DB20
	ldr r2, _08069AD0 @ =0x00001C4E
	add r0, r1, r2
	mov r3, #0
	mov r2, #0
	strh r2, [r0]
	ldr r4, _08069AD4 @ =0x00001C4C
	add r0, r1, r4
	strh r2, [r0]
	ldr r2, _08069AD8 @ =0x00001C49
	add r0, r1, r2
	strb r3, [r0]
	sub r4, #2
	add r0, r1, r4
	strb r3, [r0]
	add r2, #9
	add r0, r1, r2
	strb r3, [r0]
	add r4, #9
	add r0, r1, r4
	strb r3, [r0]
	ldr r0, _08069ADC @ =0x00001C4B
	add r1, r1, r0
	strb r3, [r1]
	mov r0, #1
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08069ACC: .4byte 0x0201DB20
_08069AD0: .4byte 0x00001C4E
_08069AD4: .4byte 0x00001C4C
_08069AD8: .4byte 0x00001C49
_08069ADC: .4byte 0x00001C4B
	thumb_func_end ListFilter_Reset

