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
	# Ensemble des executables à produire

generateur_genotypes: main_generateur_genotypes.o fonctions_generateur_genotypes.o
	$(CC) $^ $(LDFLAGS) -o $@
	# Edition de lien a partir des fichiers objets


main_generateur_genotypes.o: main_generateur_genotypes.c fonctions_generateur_genotypes.h
	$(CC) -c $< $(CFLAGS) -o $@

fonctions_generateur_genotypes.o: fonctions_generateur_genotypes.c fonctions_generateur_genotypes.h
	$(CC) -c $< $(CFLAGS) -o $@ 


##################### INSTRUCTIONS DE NETTOYAGE ########################

.PHONY: clean mrproper
	# PHONY = Dependances systematiquement reconstruites

clean:
	rm -f main_generateur_genotypes.o fonctions_generateur_genotypes.o
	# Supprimer tous les fichiers intermédiaires

mrproper: clean
	rm -rf $(BIN) $(OBJ)
	# Supprimer tout ce qui peut être régénéré et reconstruit complètement