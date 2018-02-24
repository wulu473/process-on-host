
all:
	mpicxx main.c -o process-on-host

clean:
	rm -f process-on-host
