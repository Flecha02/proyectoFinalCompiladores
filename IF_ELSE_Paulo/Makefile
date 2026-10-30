CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
BUILD = build

.PHONY: all test clean
all: $(BUILD)/easylang

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/proyectoFinal.tab.c $(BUILD)/proyectoFinal.tab.h &: proyectoFinal.y codigo.h | $(BUILD)
	bison -Wall -Werror -d -o $(BUILD)/proyectoFinal.tab.c proyectoFinal.y

$(BUILD)/lex.yy.c: proyectoFinal.l $(BUILD)/proyectoFinal.tab.h
	flex -o $@ proyectoFinal.l

$(BUILD)/easylang: $(BUILD)/proyectoFinal.tab.c $(BUILD)/lex.yy.c codigo.c codigo.h
	$(CC) $(CFLAGS) -I. -I$(BUILD) $(BUILD)/proyectoFinal.tab.c $(BUILD)/lex.yy.c codigo.c -o $@

test: all
	python3 pruebas/probar.py $(BUILD)/easylang

clean:
	rm -rf $(BUILD)
