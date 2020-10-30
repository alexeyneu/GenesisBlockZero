.model large
.586
stackle segment stack
there	dw 5
		dw 7
		dw 17
basket  equ $ - there				
		db 100h-basket dup(0) 
stackle ends 
.data
	put   db "boat", 0ah, 0dh,'$'
	outpost   db "out of",'$'
.code
	main   proc	
	mov ax, @data
	mov ds, ax
	lea ax, put 
	mov dx, ax

	mov sp, 0
	pop cx
multiply:
	pop ax
	imul cx, ax
	cmp sp, basket
	jl multiply

	mov bx, dx
	cmp cx, 595
	jne essa
	mov al, 'B'
	mov [bx], al
essa:
	mov ah, 09h
	int 21h
	mov ax, 4c00h
	int 21h

	main   endp
	end main

