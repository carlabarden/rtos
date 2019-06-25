CC       = gcc
CFLAGS	 = -O3 -c #-g -W -Wall -pedantic -std=c11
LDFLAGS  = -lrt -lpthread 

CLI=cli
SRV=srv
BIN=bin

CSRC_CLI=$(wildcard ./src/cli/*.c)
HSRC_CLI=$(wildcard ./src/cli/*.h)

CSRC_SRV=$(wildcard ./src/srv/*.c)
HSRC_SRV=$(wildcard ./src/srv/*.h)

CSRC_INC=$(wildcard ./src/shr/*.c)
HSRC_INC=$(wildcard ./src/shr/*.h)

OBJ_CLI=$(subst .c,.o,$(subst src/cli,obj/cli,$(CSRC_CLI)))
OBJ_SRV=$(subst .c,.o,$(subst src/srv,obj/srv,$(CSRC_SRV)))
OBJ_INC=$(subst .c,.o,$(subst src/shr,obj/inc,$(CSRC_INC)))

IP=127.0.0.1
PORTA=6066

DIR:= $(shell pwd)


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


share: obj_inc

./obj/inc/%.o: ./src/shr/%.c ./src/shr/%.h
	@$(CC) $< $(CFLAGS) -o $@

obj_inc:
	@mkdir -p obj/inc/



####### CLIENTE ########
cliente: bin share obj_cliente $(CLI)

$(CLI): $(OBJ_CLI) $(OBJ_INC)
	@$(CC) $(OBJ_CLI) $(OBJ_INC) -o $(BIN)/$@ $(LDFLAGS)
	
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
servidor: bin share obj_servidor $(SRV)

$(SRV):$(OBJ_SRV) $(OBJ_INC)
	@$(CC) $(OBJ_SRV) $(OBJ_INC) -o $(BIN)/$@ $(LDFLAGS)

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
        

