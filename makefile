CC=gcc
CFLAGS=-O3 -std=c11 -c -Wall -pedantic -W -g
LDFLAGS= -lpthread

RM=rm -f

CLI=cli
SRV=srv
BIN=bin

CSRC_CLI=$(wildcard ./src/cli/*.c)
HSRC_CLI=$(wildcard ./src/cli/*.h)

CSRC_SRV=$(wildcard ./src/srv/*.c)
HSRC_SRV=$(wildcard ./src/srv/*.h)

OBJ_CLI=$(subst .c,.o,$(subst src/cli,obj/cli,$(CSRC_CLI)))
OBJ_SRV=$(subst .c,.o,$(subst src/srv,obj/srv,$(CSRC_SRV)))

DIR:= $(shell pwd)

IP=127.0.0.1
PORTA=9090


####### ALL ########
all: cliente servidor
	
clean:
	@rm -rf obj/ bin/

run:
	@xfce4-terminal --default-working-directory=$(DIR)/$(BIN) --initial-title="Servidor" --command="./$(SRV) $(PORTA)" -H
	@sleep 1s
	@xfce4-terminal --default-working-directory=$(DIR)/$(BIN) --initial-title="Cliente"  --command="./$(CLI) $(IP) $(PORTA)" -H 

####### Usado pelas 2 regras #######
bin:
	@mkdir -p bin/

####### CLIENTE ########
cliente: bin obj_cliente $(CLI)

$(CLI):$(OBJ_CLI)
	@$(CC) $^ -o $(BIN)/$@ $(LDFLAGS)

#gerar arquivos .o
./obj/cli/%.o: ./src/cli/%.c ./src/cli/%.h
	@$(CC) $< $(CFLAGS) -o $@

#./obj/cli/main.o: ./src/cli/main.c $(HSRC_CLI)
#	@$(CC) $< $(CFLAGS) -o $@

obj_cliente:
	@mkdir -p obj/cli/

limpar_cliente:
	@rm -rf obj/cli/ $(BIN)/$(CLI)

executar_cliente:
	@./$(BIN)/$(CLI) $(IP) $(PORTA)



####### SERVIDOR ########
servidor: bin obj_servidor $(SRV)

$(SRV):$(OBJ_SRV)
	@$(CC) $^ -o $(BIN)/$@ $(LDFLAGS)

#gerar arquivos .o
./obj/srv/%.o: ./src/srv/%.c ./src/srv/%.h
	@$(CC) $< $(CFLAGS) -o $@

#./obj/srv/main.o: ./src/srv/main.c $(HSRC_CLI)
#	@$(CC) $< $(CFLAGS) -o $@

obj_servidor:
	@mkdir -p obj/srv/

limpar_servidor:
	@rm -rf obj/srv/ $(BIN)/$(SRV)

executar_servidor:
	@./$(BIN)/$(SRV) $(PORTA)


#alvo falso
.PHONY: cliente   limpar_cliente   executar_cliente  \
		servidor  limpar_servidor  executar_servidor \
		all       clean            run 
        

