all:
	gcc server.c -o server -lws2_32
	gcc client.c -o client -lws2_32