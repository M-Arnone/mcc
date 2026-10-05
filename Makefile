mcc: mcc.c
	@gcc mcc.c -o mcc
clean:
	@rm -f mcc *.i *.s return
