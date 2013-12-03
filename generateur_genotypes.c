#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "generateur_genotypes.h"
#include "ptr_allocation.h"
 
/***********************************************************************
 * main
 **********************************************************************/
 int main(int argc, char** argv)
{
	/******** Declaration et initialisation des variables *********/
	
	int** tab_haplo = NULL;
	int** tab_geno = NULL;
	
	/******** Parsing des options avec Getopt *********/
	
	int optch;
    extern int opterr;
    char format[] = "o:n:m:t:a:hH";
	opterr = 1;
	
    while ((optch = getopt(argc, argv, format)) != -1)
    switch (optch)
    {
        case 'o':
            OUTPUT_NAME = optarg;
            break;
        case 'n':
            NBR_GENO = atoi(optarg);
             if (NBR_GENO < 30) usage(argv[0]);
            break;
		case 'm':
            NBR_HAPLO = atoi(optarg);
            if (NBR_HAPLO < 10) usage(argv[0]);
            break;
		case 't':
            TAILLE = atoi(optarg);
            if (TAILLE < 5) usage(argv[0]);
            break;
        case 'a':
            AMB_MAX = atoi(optarg);
            if (AMB_MAX < 1) usage(argv[0]);
            if (AMB_MAX < TAILLE) usage(argv[0]);
			break;
		case 'H':
            HOMOZYGOTE = 1;
            break;
        case 'h':
			usage(argv[0]);
            break;
    }

	/******** Parametrage *********/
	
	// Si NBR_HAPLO et AMBIGUITE non initialisé dans les options = calcul automatique
	if (NBR_HAPLO == 0) NBR_HAPLO = NBR_GENO/3;
	if (AMB_MAX == 0) AMB_MAX = TAILLE/3;
	AMB_MAX_INIT = AMB_MAX;
	if (OUTPUT_NAME == NULL) usage(argv[0]);
	
	encadre ("PARAMETRES D'ENTREE ET REGLAGES");
	printf ("\tLes génotypes homozygotes sont %s.\n", ((HOMOZYGOTE == 1) ? "autorisés" : "exclus"));
	printf ("\tNombre d'haplotypes à créer aléatoirement = %d\n", NBR_HAPLO);
	printf ("\tNombre de genotypes à créer à partir des haplotypes = %d\n", NBR_GENO);
	printf ("\tTaille des genotypes/haplotypes = %d\n", TAILLE);
	printf ("\tNombre maximal d'ambiguités tolérées = %d\n", AMB_MAX);
	printf ("\tPrefixe du nom de sortie = %s\n", OUTPUT_NAME);

	/******** Appels fonctions *********/
	
	// seed value pour rand utilisé par plusieurs programmes appelés
	srand (time(NULL));
	
	do {
		// Creation aleatoire des haplotypes et stockage de leur séquence dans tab_haplo
		encadre("GENERATION ALEATOIRE DES HAPLOTYPES");
		tab_haplo = create_haplo ();
		
		// Creation d'une liste de paires d'haplotypes compatibles et stockage de leurs indices dans tab_geno
		encadre("ASSOCIATION DES HAPLOTYPES EN GENOTYPE COMPATIBLES");
		tab_geno = create_geno (tab_haplo);
		
		// Creation d'une matrice d'association des haplotypes et affichage pour verification visuelle
		encadre("MATRICE D'ASSOCIATION");
		count_haplo_pairs (tab_geno);
		
	 } while (!validation());
	
	encadre("EXPORT DES GENOTYPES");
	// Association des paires d'haplotypes et export des genotypes ainsi créés dans un fichier texte.
	export_geno (tab_haplo, tab_geno);

	return (EXIT_SUCCESS);
}

/***********************************************************************
 * usage = Affichage de l'usage de la fonction
 **********************************************************************/
void usage (char* prog_name)
{
	fprintf (stderr, "\nUSAGE:\t%s -o <prefixe_fichier_sortie> [-n <entier> -m <entier> -t <entier> -a <entier> -H -h]\n\n", prog_name);
	fprintf (stderr, "\nDESCRIPTION\n\n\
	Ce programme crée un fichier texte de génotypes à partir d'haplotypes générés aleatoirement\n\
	Afin d'obtenir une redondance de l'information necessaire pour utiliser un programme d'inference,\n\
	les génotypes sont créés semi aléatoirement à partir des haplotypes. L'un des haplotypes\n\
	provient du génotype crée precedement et le second est tiré aléatoirement dans la liste d'haplotypes.\n\
	Avant d'être assemblées en un genotype les paires d'haplotypes sont testés pour leur respect des\n\
	contraintes d'ambiguités. Si le nombre d'ambiguités est trop elevé un nouveau tirage est réalisé.\n\
	Pour la première paire si le nombre de tirage excède 1000, alors le nombre maximal d'ambiguités\n\
	tolerées est ajusté automatiquement et l'utilisateur est informé de cette mise à jour.\n\
	La stratégie d'encodage des positions adoptée est la suivante:\n\
		0 si les 2 haplotypes possèdent l'allèle mineure\n\
		2 si les 2 haplotypes possèdent l'allèle majeure\n\
		1 si 1 haplotype possède l'allèle majeure et l'autre l'allèle mineure\n\
	Par exemple si le genotype G est constitué des haplotypes Ha = aBCd et Hb = abCd ou les minuscules\n\
	sont les allèles minneures et les majuscules les allèles majeures, alors les Ha, Hb et G\n\
	seront respectivement codés O110, 0010 et 0120.\n\n");
	
	fprintf (stderr, "DETAILS DES OPTIONS\n\n\
	-o	Prefixe du nom du fichier de sortie\n\
		Il s'agit de la seule option obligatoire du programme\n\n\
	-n	Nombre de genotypes à créer. Minimun = 30 (par defaut 50)\n\n\
	-m	Nombre de haplotype à créer. Minimun = 10 (par defaut 16)\n\n\
		Si l'option n'est pas utilisé 1/3 du nombre de genotype sont generé\n\
		Il n'est pas conseillé de modifier cette valeur\n\n\
	-t	Taille des genotypes à créer. Minimun = 5 (par defaut 15)\n\n\
	-a	Nombre maximal d'ambiguités dans les genotypes. Minimun = 1 / Maximum = Taille des genotypes\n\
		Si l'option n'est pas utilisé ce nombre correspondra à 1/3 de la taille (par defaut 5)\n\
		Il n'est pas conseillé de modifier cette valeur\n\n\
	-H	Autorise la création de genotypes homozygotes (par defaut non autorisé)\n\n\
	-h	Affiche cet ecran d'aide\n\n");

	fprintf (stderr, "EXEMPLES\n\n\
	%s -o test -H -t 30 -n 200 \n\
	%s -o test -H -t 30 -n 200 \n\n", prog_name, prog_name );
	
	exit (EXIT_FAILURE);
}

/***********************************************************************
 * create_haplo = creation aléatoire d'haplotype.
 **********************************************************************/
int** create_haplo (void)
{
	int i, j;
	int** tab_haplo ;
	
	tab_haplo = malloc_int_mat (NBR_HAPLO, TAILLE);
	
	// boucle de remplissage de la matrice
	for (i = 0 ; i < NBR_HAPLO ; i++)
	{
		for (j = 0 ; j < TAILLE ; j++)
		{
			tab_haplo [i][j] = (rand () % 2); // génere un int aleatoire entre 0 et 1
		}
	}
	
	printf ("\nHaplotypes générés\n\n");
	print_int_mat (tab_haplo, NBR_HAPLO, TAILLE); // Pour verifier visuellement les haplotypes générés
	return tab_haplo;
}

/***********************************************************************
 * create_geno = Creation de genotypes semi-aléatoires à partir des haplotypes
 **********************************************************************/
int** create_geno (int** tab_haplo)
{
	int i, j = 0;
	int** tab_geno;
	
	tab_geno = malloc_int_mat (NBR_GENO, 2);
	printf("\nCréation des genotypes\n\n ");

/* ETAPE 1 INTIALISATION ET AUTO-AJUSTEMENT */

	do {
		tab_geno [0][0] = (rand () % NBR_HAPLO);
		tab_geno [0][1] = (rand () % NBR_HAPLO);
		if (++j > 1000) // Condition de contrôle de de la stringence pour ne pas être bloqué dans une boucle infinie 
		{
			AMB_MAX++; // Augmentation de la tolérence en position ambigues
			j = 0; // reinitialisation du compteur de tentatives
			printf ("Paramètres d'entrée trop stringents\n");
			printf ("Ajustement automatique du nombre de positions ambigues tolérées à %d\n", AMB_MAX);
		} 
	} while (ambiguity ( tab_haplo, tab_geno [0][0], tab_geno [0][1]));  // test de compatibilité
	printf("\nGenotype # 0 :\t\t%d Essai%snecessaire%s\tConstitué des haplotypes %d %d\n",\
	j, ((j>1)?"s ":" "), ((j>1)?"s ":" "), tab_geno [0][0], tab_geno [0][1]);

/* ETAPE 2 AMPLIFICATION */
 
	for (i = 1; i < NBR_GENO ; i++) {
		j = 0;
		do {
			tab_geno [i][0] = tab_geno [i-1][(rand () % 2)];	// Selection aléatoire d'un haplotype du genotype n-1 ...
			tab_geno [i][1] = (rand () % NBR_HAPLO); 			// ... Et d'un second haplotype tiré aléatoirement dans la liste
			j++;
		} while ( ambiguity ( tab_haplo, tab_geno [i][0], tab_geno [i][1])); // test de compatibilité
		
		printf("Génotype # %d:\t\t%d Essai%snecessaire%s\tConstitué des haplotypes %d %d\n",\
		i, j, ((j>1)?"s ":" "), ((j>1)?"s ":" "), tab_geno [i][0], tab_geno [i][1]);
	}
	return tab_geno;
}

/***********************************************************************
 * ambiguity = retourne 1 si le nbr d'ambiguité est inferieur à AMB_MAX
 **********************************************************************/
 
int ambiguity (int** tab_haplo, int h1, int h2)
{
	int i;
	int amb = 0;

// Incrementation de amb à chaque ambiguité constatée
	for (i = 0; i < TAILLE ; i++)
		if (tab_haplo [h1][i] != tab_haplo [h2][i]) amb++;
	
	if (HOMOZYGOTE == 0)
	{
		if (amb > AMB_MAX || amb == 0) 
			return 1;
	}
	
	else // HOMOZYGOTE == 1
	{
		if (amb > AMB_MAX)
			return 1;
	}
	
	return 0; // Si ambiguité trop elevé
}

/***********************************************************************
 * count_haplo_pairs = cree une matrice de la distribution des paires d'haplotypes
 **********************************************************************/
void count_haplo_pairs (int** tab_geno)
{
	int i, j;
	int** tab_count ;
	
	// creation d'une matrice d'int initialisé à 0
	tab_count = calloc_int_mat (NBR_HAPLO, NBR_HAPLO);
	
	for (i = 0 ; i < NBR_HAPLO ; i++) // Boucle pour parcourir les numéros d'haplotype
	{
		for (j = 0 ; j < NBR_GENO ; j++) // Boucle pour parcourir la première colonne du tableau de couple de génotype
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
	print_count_mat (tab_count);
	if (AMB_MAX > AMB_MAX_INIT)
	{
		printf ("\nATTENTION Paramètres d'entrée trop stringents");
		printf ("\nLe Nombre de positions ambigues tolérées (%d) a été ajusté automatiquement à %d\n", AMB_MAX_INIT, AMB_MAX);
	}
	return;
}

/***********************************************************************
 * print_count_mat = affiche la matrice generée par count_haplo_pairs
 **********************************************************************/
void print_count_mat (int** tab)
{
	int i, j;
	int geno_uniq = 0;
	
	printf ("Matrice de couples d'haplotypes représentés\n\n");
	
	for (i = 0 ; i < NBR_HAPLO ; i++) 
	{
		// Impression des indices de ligne
		printf("#%d\t", i);
		
		// Impression des valeurs du tableau en supprimant 1 colonne à chaque tour
		for (j = 0 ; j < 1+i ; j++) 
		{
			if (tab [i][j] < 10)
				printf("%d   ", tab [i][j]);
			else if (tab [i][j] < 100)
				printf("%d  ", tab [i][j]);
			else
				printf("%d", tab [i][j]);
		
			if (tab [i][j] != 0) geno_uniq++;
		}
		printf("\n");
	}
	
	printf("\t");
	// Impression des indices de colonne
	for (j = 0 ; j < NBR_HAPLO ; j++)
		{
		if (j < 10) printf("#%d  ", j);
		else if (j < 100) printf("#%d ", j);
		else printf("#%d", j);
		}
	printf("\n\nLa matrice contient %d genotype%sunique%s\n", geno_uniq, ((geno_uniq > 1) ? "s " : " "), ((geno_uniq > 1) ? "s " : " "));
	return;
}

/***********************************************************************
 * validation
 **********************************************************************/
int validation (void)
{
	int valid = 1;
	
	printf ("\nVoulez vous (G)enerer le fichier texte de genotypes à partir de ces associations,\n");
	printf ("(R)elancer un tirage d'haplotypes avec les mêmes paramètres\n");
	printf ("(Q)uitter le programme pour entrer de nouveaux paramètres,\n");
	
	do
	{
		switch (getchar())
		 {
			case 'G':
			case 'g':
				printf ("\n\nExport des genotypes fichier texte de genotype\n");
				return 1;
				break;
			case 'R':
			case 'r':
				printf ("\n\nNouveau tirage d'haplotypes avec les mêmes paramètres\n");
				return 0;
				break;
			case 'Q':
			case 'q':
				printf ("\n\nSortie du programme\n");
				exit (EXIT_FAILURE);
				break;
			default:
				printf ("\n\nSaisie non valide, veuillez entrer G, R ou Q\n");
				valid = 0;
				break;
		  }				
	} while (valid == 0);
	
	return 0;
}

/***********************************************************************
 * export_geno = exporte les genotypes tirés dans 2 fichiers texte
 **********************************************************************/
void export_geno (int** tab_haplo, int** tab_geno)
{
	int i, j;
	char* name1 = NULL;
	char* name2 = NULL;
	
	// création des noms des fichiers de sortie
	name1 = malloc_char_string(strlen(OUTPUT_NAME)+1);
	name2 = malloc_char_string(strlen(OUTPUT_NAME)+1);
	strcpy (name1, OUTPUT_NAME);
	strcat (name1, "_haplo_geno.txt");
	strcpy (name2, OUTPUT_NAME);
	strcat (name2, "_geno.txt");
	
	FILE* f_haplo_geno = init_file_ptr (name1, "w");
	FILE* f_geno = init_file_ptr (name2, "w");
	
	for (i = 0; i < NBR_GENO ; i++)
	{
		fprintf(f_haplo_geno, "Genotype %d :\t", i);
		for (j = 0; j < TAILLE ; j++) {
			fprintf(f_haplo_geno, "%d", (tab_haplo [tab_geno[i][0]][j] + tab_haplo [tab_geno[i][1]][j]));
			fprintf(f_geno, "%d", (tab_haplo [tab_geno[i][0]][j] + tab_haplo [tab_geno[i][1]][j]));
		}
		fprintf(f_haplo_geno, "\n");
		
		fprintf(f_haplo_geno, "Haplotype %d :\t", tab_geno[i][0]);
		for (j = 0; j < TAILLE ; j++) {
			fprintf(f_haplo_geno, "%d", tab_haplo [tab_geno[i][0]][j]);
		}
		fprintf(f_haplo_geno, "\n");
		
		fprintf(f_haplo_geno, "Haplotype %d :\t", tab_geno[i][1]);
		for (j = 0; j < TAILLE ; j++) {
			fprintf(f_haplo_geno, "%d", tab_haplo [tab_geno[i][1]][j]);
		}
		fprintf(f_haplo_geno, "\n\n");
		fprintf(f_geno, "\n");
	}
	
	fclose(f_haplo_geno);
	fclose(f_geno);
	return;
}
