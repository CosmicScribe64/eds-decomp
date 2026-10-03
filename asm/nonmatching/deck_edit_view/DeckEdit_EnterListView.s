	thumb_func_start DeckEdit_EnterListView
DeckEdit_EnterListView: @ 0x0806DBA4
	push {lr}
	bl DeckEdit_InitListView
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end DeckEdit_EnterListView

