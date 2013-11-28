generateur_genotype
===================

USAGE:generateur_genotype -o <prefixe_fichier_sortie> [-n <entier> -m <entier> -t <entier> -a <entier> -H -h]
DESCRIPTION
Ce programme crée un fichier texte de génotypes à partir d'haplotypes générés aleatoirement. Afin d'obtenir une redondance de l'information necessaire pour utiliser un programme d'inference, les génotypes sont créés semi aléatoirement à partir des haplotypes. L'un des haplotypes provient du génotype crée precedement et le second est tiré aléatoirement dans la liste d'haplotypes. Avant d'être assemblées en un genotype les paires d'haplotypes sont testés pour leur respect des contraintes d'ambiguités. Si le nombre d'ambiguités est trop elevé un nouveau tirage est réalisé. Pour la première paire si le nombre de tirage excède 1000, alors le nombre maximal d'ambiguités tolerées est ajusté automatiquement et l'utilisateur est informé de cette mise à jour.
La stratégie d'encodage des positions adoptée est la suivante:
	0 si les 2 haplotypes possèdent l'allèle mineure
	2 si les 2 haplotypes possèdent l'allèle majeure
	1 si 1 haplotype possède l'allèle majeure et l'autre l'allèle mineure
Par exemple si le genotype G est constitué des haplotypes Ha = aBCd et Hb = abCd ou les minuscules sont les allèles minneures et les majuscules les allèles majeures, alors les Ha, Hb et G seront respectivement codés O110, 0010 et 0120
	
DETAILS DES OPTIONS
	-o	Prefixe du nom du fichier de sortie
		Il s'agit de la seule option obligatoire du programme
	-n	Nombre de genotypes à créer. Minimun = 30 (par defaut 50)
	-m	Nombre de haplotype à créer. Minimun = 10 (par defaut 16)
		Si l'option n'est pas utilisé 1/3 du nombre de genotype sont generé
		Il n'est pas conseillé de modifier cette valeur
	-t	Taille des genotypes à créer. Minimun = 5 (par defaut 15)
	-a	Nombre maximal d'ambiguités dans les genotypes. Minimun = 1 / Maximum = Taille des genotypes
		Si l'option n'est pas utilisé ce nombre correspondra à 1/3 de la taille (par defaut 5)
		Il n'est pas conseillé de modifier cette valeur
	-H	Autorise la création de genotypes homozygotes (par defaut non autorisé)
	-h	Affiche cet ecran d'aide

EXEMPLES\n\n\
	generateur_genotype -o test -H -t 30 -n 200
	generateur_genotype -o test -H -t 30 -n 200
