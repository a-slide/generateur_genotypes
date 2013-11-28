#ifndef GENERATEUR_GENOTYPE_H
#define GENERATEUR_GENOTYPE_H
	
	/***********************************************************************
	 * VARIABLES GLOBALES
	 **********************************************************************/
	
	int HOMOZYGOTE = 0; 		// Autorisation ou excusion des homozigotes
	int NBR_GENO = 50;			// Nombre de genotypes à generer
	int NBR_HAPLO = 0; 			// Si pas initialisé dans getopt = mise à 1/3 NBR_GENO
	int TAILLE = 15;			// Taille des genotypes à creer 
	int AMB_MAX = 0;			// Si pas initialisé dans getopt = mise à 1/3 SIZE
	int AMB_MAX_INIT = 0;		// Pour sauvegarde de la valeur de amb max en cas d'ajustement auto par le programme
	char* OUTPUT_NAME = NULL;	// Nom de sortie du fichier texte
			
	/***********************************************************************
	 * PROTOTYPES
	 **********************************************************************/
	
	void usage (char*);
	int** create_haplo (void);
	int** create_geno (int**);
	int ambiguity (int**, int, int);
	void count_haplo_pairs (int**);
	void print_count_mat (int**);
	int validation (void);
	void export_geno (int**, int**);

#endif /* GENERATEUR_GENOTYPE_H */
