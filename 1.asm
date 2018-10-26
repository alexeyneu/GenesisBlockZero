.model small
.stack
.data
  put db "yr5u7",'$'
.code
main   proc
  mov ax, @data
  mov ds, ax
  lea ax, put ;write input message on the screen
  mov dx,ax
  mov ah, 09h
  int 21h
  mov ax, 4c00h
  int 21h
main   endp
end main