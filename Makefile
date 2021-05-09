CC := tcc
test:
	@$(CC) *.c -o biblec
	@valgrind ./biblec
