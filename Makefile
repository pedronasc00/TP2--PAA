# --- Variáveis ---

# Compilador C
CC = gcc

# Flags do compilador:
# -Wall = Mostra todos os avisos (Warnings)
# -g    = Inclui informações de debug (para usar com GDB)
# -std=c99 = Usa o padrão C99
CFLAGS = -Wall -g -std=c99

# Diretório dos seus arquivos .c e .h do módulo
SRCDIR = src

# Diretório de "include", onde o compilador procura os .h
# -I$(SRCDIR) diz ao gcc para "procurar em /src" por headers
INCLUDES = -I$(SRCDIR)

# O nome do seu programa executável final
TARGET = trabalho_paa

# Lista de arquivos-objeto (.o) necessários
# main.o será criado na raiz
# lerMapa.o será criado dentro de src/
OBJECTS = main.o $(SRCDIR)/lerMapa.o


# --- Regras ---

# Regra padrão (o que é executado quando você digita 'make')
# Depende do executável final
all: $(TARGET)

# Regra para "linkar" o programa final
# Depende de todos os arquivos-objeto
# Este comando une main.o e src/lerMapa.o para criar o 'trabalho_paa'
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Regra para compilar main.o
# Depende de main.c e do header src/lerMapa.h
# O -c significa "apenas compile, não linke"
main.o: main.c $(SRCDIR)/lerMapa.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c -o main.o

# Regra para compilar src/lerMapa.o
# Depende dos arquivos .c e .h em src/
$(SRCDIR)/lerMapa.o: $(SRCDIR)/lerMapa.c $(SRCDIR)/lerMapa.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCDIR)/lerMapa.c -o $(SRCDIR)/lerMapa.o

# Regra 'clean' para limpar os arquivos gerados
# Permite que você digite 'make clean' para apagar o lixo
clean:
	rm -f $(TARGET) *.o $(SRCDIR)/*.o