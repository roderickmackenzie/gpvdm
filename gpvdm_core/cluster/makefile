CC=gcc
OBJS=state.o debug.o node.o head.o util.o inp.o rx_file.c tx_file.c jobs.o nodes.o command.o rx_loop.o cal_path.o run_jobs.o ip.o register.o killall.o sleep.o poweroff.o get_data.o clean.o progress.o load.o quit.o encrypt.o sync.o packet.o message.o jobs_save_load.o log.o kill_pid.o

main: main.c $(OBJS)
	gcc main.c $(OBJS) -g -o cluster.o -lzip -lpthread -lssl -lcrypto -lz -I./

.c.o : 
	$(CC) -c $< -g -o $@  -I./ -Wall

clean:
	rm *.o -f
