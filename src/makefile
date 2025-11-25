# Nome do executável
TARGET = ted

# Nome do aluno (usado na regra pack)
ALUNO = vitorms

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -O0 -g -ggdb

# Diretório de código-fonte
SRC_DIR = .

# Diretório de cabeçalhos
INC_DIR = .

# Diretório de bibliotecas
LIB_DIR = ./lib

# Bibliotecas adicionais (use -lm para a biblioteca matemática, por exemplo)
LIBS = -lm

# Lista de arquivos de origem (*.c no diretório SRC_DIR)
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Lista de arquivos de objeto
OBJS = $(SRCS:.c=.o)

# Incluir diretórios
INC_FLAGS = -I$(INC_DIR)

# Diretórios de bibliotecas
LIB_FLAGS = -L$(LIB_DIR)

# Regra principal (default)
all: $(TARGET)

# Regra de compilação dos arquivos de objeto (CONFERIR SE O ESPAÇO É UM TAB)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Regra de compilação do executável (CONFERIR SE O ESPAÇO É UM TAB)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_FLAGS) $(LIBS) -o $(TARGET)

# Limpar arquivos intermediários e o executável (CONFERIR SE O ESPAÇO É UM TAB)
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Falsos alvos para evitar conflitos com arquivos de mesmo nome
.PHONY: all clean pack

# Regra para criar um pacote ZIP (CONFERIR SE O ESPAÇO É UM TAB)
pack: $(TARGET)
	rm -f ../$(ALUNO).zip
	echo $(ALUNO)
	date >> .entrega
	cd ..; zip -r $(ALUNO).zip $(SRC_DIR)/*.c $(SRC_DIR)/*.h Makefile LEIA-ME.txt .entrega
	rm .entrega