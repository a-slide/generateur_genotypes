#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fonctions_generateur_genotypes.h"

/**** USAGE ***********************************************************/
// Procédure de verification de l'usage de la fonction
void usage (char* prog_name)
{
	fprintf (stderr, "\nUsage:\t%s [nbr_genotypes] [taille] [nbr_pos_ambigues_max]\n\n", prog_name);
	fprintf (stderr, "\t[nbr_genotypes]\n\tLe nombre de genotypes à generer doit être au moins égal à 25\n");
	fprintf (stderr, "\t[taille]\n\tValeur conseillée = superieure à 50\n\n");
	fprintf (stderr, "\t[taille]\n\tLa taille des génotypes doit être au moins égale à 5\n");
	fprintf (stderr, "\t[taille]\n\tValeur conseillée = superieure à 10\n\n");
	fprintf (stderr, "\t[nbr_pos_ambigues_max]\n\tLe nombre maximal de positions ambigues doit être d'au moins 1\n");
	fprintf (stderr, "\tet inferieur à la taille des genotypes\n");
	fprintf (stderr, "\tValeur conseillée = 1/3 à 1/2 de taille\n\n");
	exit (EXIT_FAILURE);
}

/**** CREATE_INT_MAT **************************************************/
// creation d'un tableau d'entiers 2D dynamique vide
int** create_int_mat (int col, int line)
{
	int i;
	int** matrice;
	matrice = malloc (sizeof (int*) * col); // creation collones
	if (matrice == NULL)	// Verification de l'allocation
	{ 
		fprintf (stderr, "Allocation impossible\n\n");
		exit (EXIT_FAILURE);
	}
	
	for ( i = 0; i < col; i++ ) {
		matrice[i] = malloc (sizeof (int) * line); // creation lignes
			if (matrice[i] == NULL)
			{
				fprintf (stderr, "Allocation impossible\n\n");
				exit (EXIT_FAILURE);
			}
		}
	return matrice;
}

/**** CREATE_ZERO_MAT *************************************************/
// creation d'un tableau d'entiers 2D dynamique rempli avec des 0
int** create_zero_mat (int col, int line)
{
	int i;
	int** matrice;
	matrice = calloc (sizeof (int*), col); // creation collones
	if (matrice == NULL)	// Verification de l'allocation
	{ 
		fprintf (stderr, "Allocation impossible\n\n");
		exit (EXIT_FAILURE);
	}
	
	for ( i = 0; i < col; i++ ) {
		matrice[i] = calloc (sizeof (int), line); // creation lignes
			if (matrice[i] == NULL)
			{
				fprintf (stderr, "Allocation impossible\n\n");
				exit (EXIT_FAILURE);
			}
		}
	return matrice;
}

/**** PRINT_INT_MAT ***************************************************/
// lecture d'un tableau 2D dynamique avec indices en lignes uniquement
void print_int_mat (int** tab, int line, int col)
{
	int i,j;
	
	for (i = 0 ; i < line ; i++) 
	{
		printf("#%d\t", i); // Impression des indices de lignes
		for (j = 0 ; j < col ; j++) // Impression des valeurs du tableau
		{
			printf("%d", tab [i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/**** PRINT_INT_MAT_INDICES *******************************************/
// lecture d'un tableau 2D dynamique avec indices en lignes et colonnes
void print_int_mat_indice (int** tab, int line, int col)
{
	int i, j;
	
	printf("\t"); // Impression des indices de colonne
	for (j = 0 ; j < col ; j++)
		{
		if (j < 10) printf("#%d  ", j);
		else if (j < 100) printf("#%d ", j);
		else printf("#%d", j);
		}
	printf("\n");
	
	for (i = 0 ; i < line ; i++) 
	{
		printf("#%d\t", i); // Impression des indices de ligne
		for (j = 0 ; j < col ; j++) // Impression des valeurs du tableau
		{
			if (tab [i][j] < 10) printf("%d   ", tab [i][j]);
			else if (tab [i][j] < 100) printf("%d  ", tab [i][j]);
			else printf("%d", tab [i][j]);
		}
		printf("\n");
	}
	printf("\n");
}



/**** CREATE_HAPLO ****************************************************/
// creation aléatoire d'haplotype.
int** create_haplo (int nbr_haplo, int taille)
{
	int i, j;
	int** tab_haplo ;
	
	tab_haplo = create_int_mat (nbr_haplo, taille);
	
	// boucle de remplissage de la matrice
	for (i = 0 ; i < nbr_haplo ; i++)
	{
		for (j = 0 ; j < taille ; j++)
		{
			tab_haplo [i][j] = (rand () % 2); // génere un int aleatoire entre 0 et 1
		}
	}
	
	printf ("\nHaplotypes générés\n\n");
	print_int_mat (tab_haplo, nbr_haplo, taille); // Pour verifier visuellement les haplotypes générés
	return tab_haplo;
}

/***** CREATE_GENO ****************************************************/
// Creation de genotypes aléatoires à partir des haplotypes
int** create_geno (int** tab_haplo, int nbr_geno, int nbr_haplo, int taille, int amb_max) {
	
	int i, j = 0;
	int** tab_geno;
	int amb_max_mem = amb_max; // memorisation du nombre initial d'ambiguités en cas d'ajustement automatique
	
	tab_geno = create_int_mat (nbr_geno, 2);
	printf("\nCréation des genotypes\n\n ");

/* ETAPE 1 INTIALISATION ET AUTO-AJUSTEMENT */

	do {
		tab_geno [0][0] = (rand () % nbr_haplo);
		tab_geno [0][1] = (rand () % nbr_haplo);
		if (++j > 1000) // Condition de contrôle de de la stringence pour ne pas être bloqué dans une boucle infinie 
		{
			amb_max++; // augmentation de la tolérence en position ambigues
			j = 0; // reinitialisation du compteur de tentatives
			printf ("Paramètres d'entrée trop stringents\n");
			printf ("Ajustement automatique du nombre de positions ambigues tolérées à %d \n\n", amb_max);
		} 
	} while (ambiguity ( tab_haplo, amb_max, tab_geno [0][0], tab_geno [0][1], taille));  // test de compatibilité
	printf("Genotype 0 :\t\t%d Essai(s) necessaire(s)\n", j);

/* ETAPE 2 AMPLIFICATION */
 
	for (i = 1; i < nbr_geno ; i++) {
		j = 0;
		do {
			tab_geno [i][0] = tab_geno [i-1][(rand () % 2)];	// Selection aléatoire d'un haplotype du genotype n-1 ...
			tab_geno [i][1] = (rand () % nbr_haplo); 			// ... Et d'un second haplotype aléatoire
			j++;
		} while ( ambiguity ( tab_haplo, amb_max, tab_geno [i][0], tab_geno [i][1], taille ) ); // test de compatibilité
		printf("Genotype %d :\t\t%d Essai(s) necessaire(s)\n", i, j);
	}
	if (amb_max > amb_max_mem)
	{
		printf ("\nATTENTION! L'algorithme a ajusté automatiquement le nombre de positions ambigues tolérées à %d\n\n", amb_max);
	}
	
	printf ("\nListe des combinaisons d'haplotype générées\n\n");
	print_int_mat (tab_geno, nbr_geno, 2); // Pour verifier visuellement les genotypes générés
	return tab_geno;
}

/***** AMBIGUITY ******************************************************/
int ambiguity (int** tab_haplo, int amb_max, int h1, int h2, int taille)
{
	int i, amb = 0;

// incrementation d'une variable à chaque ambiguité constatée
	for (i = 0; i < taille ; i++)
	{
		if (tab_haplo [h1][i] != tab_haplo [h2][i]) amb++;
	}
//	printf("Nbr pos Amb = %d\n", amb); //Pour afficher toutes les tentatives
	
	if (amb > amb_max || amb == 0) return 1; // ATTENTION ces paramètres eliminent les homozygotes.
//	if (amb > amb_max) return 1; // ATTENTION ces paramètres autorisent les homozygotes.
	else return 0;
}


/***** EXPORT_GENO ****************************************************/
void export_geno (int** tab_haplo, int** tab_geno, int taille, int nbr_geno)
{
	int i, j;
	
	FILE* f_haplo_geno = init_file_ptr ("haplotypes_genotypes.txt");
	FILE* f_geno = init_file_ptr ("genotypes.txt");
	
	for (i = 0; i < nbr_geno ; i++)
	{
		fprintf(f_haplo_geno, "Genotype %d :\t", i);
		for (j = 0; j < taille ; j++) {
			fprintf(f_haplo_geno, "%d", (tab_haplo [tab_geno[i][0]][j] + tab_haplo [tab_geno[i][1]][j]));
			fprintf(f_geno, "%d", (tab_haplo [tab_geno[i][0]][j] + tab_haplo [tab_geno[i][1]][j]));
		}
		fprintf(f_haplo_geno, "\n");
		
		fprintf(f_haplo_geno, "Haplotype %d :\t", tab_geno[i][0]);
		for (j = 0; j < taille ; j++) {
			fprintf(f_haplo_geno, "%d", tab_haplo [tab_geno[i][0]][j]);
		}
		fprintf(f_haplo_geno, "\n");
		
		fprintf(f_haplo_geno, "Haplotype %d :\t", tab_geno[i][1]);
		for (j = 0; j < taille ; j++) {
			fprintf(f_haplo_geno, "%d", tab_haplo [tab_geno[i][1]][j]);
		}
		fprintf(f_haplo_geno, "\n\n");
		fprintf(f_geno, "\n");
	}
	
	fclose(f_haplo_geno);
	fclose(f_geno);
}

/***** INIT_FILE_PTR **************************************************/
FILE* init_file_ptr (char* name)
{
	FILE* file = NULL;
	file = fopen(name, "w"); // ouverture en mode ajout
	
	if (file == NULL)
	{
		fprintf (stderr, "Impossible de creer %s\n\n", name );
		fprintf (stderr, "Erreur critique");
		exit (EXIT_FAILURE);
	}
	
	return file;
}

/**** COUNT_HAPLO_PAIRS ***********************************************/
void count_haplo_pairs (int** tab_geno, int nbr_haplo, int nbr_geno)
{
	int i, j;
	int** tab_count ;
	
	tab_count = create_zero_mat (nbr_haplo, nbr_haplo);
	
	printf ("Matrice de couple d'haplotype représenté\n\n");
	
	for (i = 0 ; i < nbr_haplo ; i++) // Boucle pour parcourir les numéros d'haplotype
	{
		for (j = 0 ; j < nbr_geno ; j++) // Boucle pour parcourir la première collone du tableau de couple de génotype
		{
			if (tab_geno [j][0] == i) // Si l'haplo en question est trouvé dans tab geno
			{
				if (tab_geno [j][0] == tab_geno [j][1]) // Cas ou homozygote
				{
					tab_count [i][i]++;
				}
				else  // Cas ou heterozygote
				{
					tab_count [i][(tab_geno [j][1])]++;
					tab_count [(tab_geno [j][1])][i]++;
				}
			}
		}
	}
	print_int_mat_indice (tab_count, nbr_haplo, nbr_haplo);
}
