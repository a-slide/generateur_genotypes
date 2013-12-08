############################# VARIABLES ################################

CC = gcc
	# Compilateur
	
CFLAGS = -g -Wall -W -Wno-unused-parameter
	# Options de compilation
	# -Wall -W et -Wno-unused-parameter pour apporter des informations

LDFLAGS = 
	# Options d'edition de lien

BIN = generateur_genotypes
	# Nom des executables a creer


#################### INSTRUCTIONS DE COMPILATION #######################
# $@ =  Cible # $^ = liste des dépendances # $< Première dépendance #

all: $(BIN)

generateur_genotypes: generateur_genotypes.o ptr_allocation.o
	$(CC) $^ $(LDFLAGS) -o $@
	# Edition de lien a partir des fichiers objets

generateur_genotypes.o: generateur_genotypes.c generateur_genotypes.h ptr_allocation.h
	$(CC) -c $< $(CFLAGS) -o $@
	# Compilation de generateur_genotypes.c
	
ptr_allocation.o: ptr_allocation.c ptr_allocation.h
	$(CC) -c $< $(CFLAGS) -o $@ 
	# Compilation de ptr_allocation.c

##################### INSTRUCTIONS DE NETTOYAGE ########################

.PHONY: clean mrproper
	# PHONY = Dependances systematiquement reconstruites

clean:
	rm -f ptr_allocation.o generateur_genotypes.o
	# Supprimer tous les fichiers intermédiaires

mrproper: clean
	rm -rf $(BIN) $(OBJ)
	# Supprimer tout ce qui peut être régénéré et reconstruit complètement
