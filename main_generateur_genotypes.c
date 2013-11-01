#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fonctions_generateur_genotypes.h"


/****MAIN**************************************************************/

int main(int argc, char** argv)
{
	int nbr_geno, taille, amb_max, nbr_haplo;
	int** tab_haplo;
	int** tab_geno;
	
	if (argc != 4) usage(argv[0]); // Affiche usage et sort si nombre de paramètres incorects
	
	nbr_geno = atoi (argv[1]); // Reccupération des arguments dans variables locale
	taille = atoi (argv[2]);
	amb_max = atoi (argv[3]);
	
	// test si les valeurs des arguments sont adaptées
	if ((nbr_geno < 25) || (taille < 5) || (amb_max < 1) || (taille <= amb_max)) usage (argv[0]);
	
	//printf ("\nParamètrage corect. Les paramètres suivants seront utilisés\n");
	//printf ("nbr_genotypes = %d, taille = %d, nbr_pos_ambigues_max %d\n\n", nbr_geno, taille, amb_max);
	
	srand (time(NULL)); // seed value pour rand utilisé par plusieurs programmes appelés
	
	nbr_haplo = (nbr_geno/3); // A adapter si besoin mais ça à l'air plutôt bien
	tab_haplo = create_haplo (nbr_haplo, taille); // Programme qui crée de haplotypes aléatoire
	
	tab_geno = create_geno (tab_haplo, nbr_geno, nbr_haplo, taille, amb_max);

	count_haplo_pairs (tab_geno, nbr_haplo, nbr_geno); // Pour verifier visuellement la fréquence des couples d'haplo generés
	export_geno (tab_haplo, tab_geno, taille, nbr_geno);

	return (EXIT_SUCCESS);
}

