	thumb_func_start ListFilter_ShowNowFiltering
ListFilter_ShowNowFiltering: @ 0x08069E64
	push {lr}
	ldr r0, _08069E8C @ =0x086FD580
	ldr r1, _08069E90 @ =0x0600C200
	mov r2, #0x1E
	mov r3, #8
	bl CopyMapRect
	ldr r0, _08069E94 @ =0x086FD0D0
	ldr r1, _08069E98 @ =0x0600E200
	mov r2, #0x1E
	mov r3, #8
	bl CopyMapRect
	ldr r0, _08069E9C @ =0x03000040
	ldr r1, _08069EA0 @ =0x00000414
	add r0, r0, r1
	mov r1, #0
	str r1, [r0]
	pop {r0}
	bx r0
_08069E8C: .4byte gListFilterNowFilteringMap
_08069E90: .4byte 0x0600C200
_08069E94: .4byte gListFilterBarFrameMap
_08069E98: .4byte 0x0600E200
_08069E9C: .4byte 0x03000040
_08069EA0: .4byte 0x00000414
	thumb_func_end ListFilter_ShowNowFiltering

