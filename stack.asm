.model small
.586
stackle segment stack 
there	dw 5
		dw 7
		dw 17
		stacks equ $ - there
stackle ends 
.data
	put db "boat",'$'
.code
	main   proc	
	mov ax, @data
	mov ds, ax
	lea ax, put 
	mov dx, ax

	pop cx

	multiply:
	pop ax
	imul cx, ax
	cmp sp, stacks
	jl multiply

;	mov cx, 595

	mov bx, dx
	mov [bx], cl 

	mov ah, 09h
	int 21h
	mov ax, 4c00h
	int 21h
	main   endp
	end main