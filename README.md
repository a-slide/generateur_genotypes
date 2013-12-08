generateur_genotype
===================

USAGE:generateur_genotype [-n <entier> -m <entier> -t <entier> -a <entier> -H -h]
DESCRIPTION
Ce programme crée un fichier texte de génotypes à partir d'haplotypes générés aleatoirement. Afin d'obtenir une redondance de l'information necessaire pour utiliser un programme d'inference, les génotypes sont créés semi aléatoirement à partir des haplotypes. L'un des haplotypes provient du génotype crée precedement et le second est tiré aléatoirement dans la liste d'haplotypes. Avant d'être assemblées en un genotype les paires d'haplotypes sont testés pour leur respect des contraintes d'ambiguités. Si le nombre d'ambiguités est trop elevé un nouveau tirage est réalisé. Pour la première paire si le nombre de tirage excède 1000, alors le nombre maximal d'ambiguités tolerées est ajusté automatiquement et l'utilisateur est informé de cette mise à jour.
La stratégie d'encodage des positions adoptée est la suivante:
	0 si les 2 haplotypes possèdent l'allèle mineure
	2 si les 2 haplotypes possèdent l'allèle majeure
	1 si 1 haplotype possède l'allèle majeure et l'autre l'allèle mineure
Par exemple si le genotype G est constitué des haplotypes Ha = aBCd et Hb = abCd ou les minuscules sont les allèles mineures et les majuscules les allèles majeures, alors les Ha, Hb et G seront respectivement codés O110, 0010 et 0120
	
DETAILS DES OPTIONS
	-n	Nombre de génotypes à créer. Minimum = 30 (par défaut 50)
	-m	Nombre de haplotype à créer. Minimum = 10 (par défaut 16)
		Si l'option n'est pas utilisée 1/3 du nombre de génotype sont générés
	-t	Taille des génotypes à créer. Minimum = 5 (par défaut 15)
	-a	Nombre maximal d’ambiguïtés dans les génotypes. 
		Minimum = 1 / Maximum = Taille des génotypes (par défaut 5)
		Si l'option n'est pas utilisée ce nombre correspondra à 1/3 de la taille 
	-H	Autorise la création de génotypes homozygotes (par défaut non autorisé)
	-h	Affiche l'écran d'aide


EXEMPLES\n\n\
	generateur_genotype -H -t 30 -n 200
	generateur_genotype -t 30 -n 200 -a 20
