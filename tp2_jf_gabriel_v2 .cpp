#include <conio.h>	// pour _kbhit() et _getch()
#include <time.h>	// pour time(int)
#include <stdlib.h>	// pour srand(int) et rand()
#include "RockUtiles.h"	// pour la gestion de l'écran

// Partie I
// --------
void afficherTerrain(int nbLignes, int nbColonnes);
int recupererTouche();
int calculerDirectionTouche(int touche);
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY);
void deplacerSerpentI(int direction, int &posX, int &posY);
char oui_non();
void affichageReglesJeu();

// Partie II
// --------
int saisirNiveau();
void afficherSouris(int sourisX[], int sourisY[], int nbSouris);
void deplacerSerpentII(int direction, int serpentX[], int serpentY[], int &tailleSerpent, int sourisX[], int sourisY[], int &nbSouris);
bool testerCollision(int posX, int posY, int sourisX[], int sourisY[], int &nbSouris);
void creerSouris(int nbLignes, int nbColonnes, int sourisX[], int sourisY[], int nbSouris);
void effacerSerpent(int serpentX[], int serpentY[], int tailleSerpent);
void afficherSerpent(int serpentX[], int serpentY[], int tailleSerpent);
bool testerCollisionSerpent(int posX, int posY, int serpentX[], int serpentY[], int tailleSerpent);

// Partie III (Bonus)
// --------
void creerMurs(int nbLignes, int nbColonnes, int mursX[], int mursY[], int nbMurs);
void placerMurs(int mursX[], int mursY[], int nbMurs);
bool testerCollisionMurs(int posX, int posY, int mursX[], int mursY[], int nbMurs);


//  fonction principale
//  -------------------
void main()
{
	srand(time(NULL));
	int const NB_LIGNES = 30;
	int const NB_COLONES = 70;
	int const NB_ELEMENTS_SERPENT = 21;
	int const NB_ELEMENTS_SOURIS = 20;
	int const NB_ELEMENTS_MURS = 30;
	char continuer;
	setDimensionFenetre(0, 0, 70, 20);
	affichageReglesJeu();
	system("pause");
	system("cls");
	setDimensionFenetre(0, 0, NB_COLONES + 3, NB_LIGNES + 3);
	do
	{
		int serpentX[NB_ELEMENTS_SERPENT] = {};
		int serpentY[NB_ELEMENTS_SERPENT] = {};
		int sourisX[NB_ELEMENTS_SOURIS] = {};
		int sourisY[NB_ELEMENTS_SOURIS] = {};
		int mursX[NB_ELEMENTS_MURS] = {};
		int mursY[NB_ELEMENTS_MURS] = {};
		int touche = 0;
		int direction = -1;
		int nbSouris = 0;
		int tailleSerpent = 1;
		int nbMurs = 0;
		bool perdu = false;

		//on demande de saisir le niveau de difficulté/le nb de souris sur le terrain.
		int niveau = saisirNiveau();
		nbSouris = niveau;
		nbMurs = niveau * 1.5;
		system("cls");

		//on affiche le cadre
		afficherTerrain(NB_LIGNES, NB_COLONES);
		//on initialise aleatoirement la position du serpent
		positionAleatoire(NB_LIGNES, NB_COLONES, serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1]);
		curseurVisible(false);
		// on créé les souris suivant le niveau de difficulté
		creerSouris(NB_LIGNES, NB_COLONES, sourisX, sourisY, nbSouris);
		// on les affiches
		afficherSouris(sourisX, sourisY, nbSouris);
		// on créé les murs avant de les afficher;
		creerMurs(NB_LIGNES, NB_COLONES, mursX, mursY, nbMurs);
		// on affiche les murs aleatoires
		placerMurs(mursX, mursY, nbMurs);

		while (nbSouris > 0 && !perdu)
		{
			// Verifier si une touche est appuyer
			touche = recupererTouche();
			// Calculez la direction du serpent dans le cas ou une touche est appuyer
			if (touche != -1)
			{
				direction = calculerDirectionTouche(touche);
			}
			// Efface l'affichage du serpent dans sa position actuel
			effacerSerpent(serpentX, serpentY, tailleSerpent);
			// Test de collision avec les souris
			bool collision = testerCollision(serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1], sourisX, sourisY, nbSouris);
			if (collision)
			{
				tailleSerpent++;
				for (int i = tailleSerpent - 1; i >= 0; i--)
				{
					serpentX[i] = serpentX[i - 1];
					serpentY[i] = serpentY[i - 1];
				}
				serpentX[0] = whereX();
				serpentY[0] = whereY();
			}
			// Test de collision sur lui-même (le serpent)
			bool collisionSerpent = testerCollisionSerpent(serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1], serpentX, serpentY, tailleSerpent);
			if (collisionSerpent)
			{
				perdu = true;
			}
			// Test de collision avec un mur
			bool collisionMur = testerCollisionMurs(serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1], mursX, mursY, nbMurs);
			if (collisionMur)
			{
				perdu = true;
			}
			// Deplacement des coordonées du serpent dans les tableaux
			deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
			// Affichage du serpent avec les nouvelles coordonnées
			afficherSerpent(serpentX, serpentY, tailleSerpent);

			Sleep(100);
		}

		if (perdu)
		{
			system("cls");
			cout << "Vous perdez. Meilleure chance la prochaine fois !" << endl;
			system("pause");
		}
		else
		{
			system("cls");
			cout << "Bravo, vous avez gagne !" << endl;
			system("pause");
		}

		// Verifier si l'utilisateur veut recommencer la partie
		system("cls");
		cout << "Voulez vous faire une autre partie? (oui = O, non = N)";
		continuer = oui_non();
		system("cls");
	} while (continuer == 'O');
}

//--------------------------------------------------------------/
/*
Tâche: afficher le contour du terrain
Paramètres: le nombre de lignes et de colonnes du terrain rectangulaire
*/
void afficherTerrain(int nbLignes, int nbColonnes)
{
	cadre(1, 1, nbColonnes, nbLignes, 10);
}

/*
Tâche: calculer une position aléatoire sur le terrain
Paramètres: les dimensions du terrain en entrée et les coordonnées de la position aléatoire en sortie
*/
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY)
{
	posX = ((rand() % (nbColonnes - 2)) + 2);
	posY = ((rand() % (nbLignes - 2)) + 2);
}

/*
Tâche: tester si le joueur a appuyé sur une touche
Retour: retourner le code ASCII de la touche pressée, -1 sinon
*/
int recupererTouche()
{
	//on verifie qu'une touche a été pressée avec _kbhit()
	if (_kbhit() == NULL)
	{
		return -1;
	}
	else
	{
		// Si on a appuyé sur une touche, seul les touches de déplacement seront valides (w,a,s,d)
		char caractere;
		do
		{
			caractere = toupper(_getch());
		} while ((caractere != 'W') && (caractere != 'A') && (caractere != 'S') && (caractere != 'D'));

		//on retourne la valeur ASCII de la touche sur laquelle on a appuyé
		if (caractere == 'W')
			return 87;
		else if (caractere == 'A')
			return 65;
		else if (caractere == 'S')
			return 83;
		else
			return 68;
	}
}


/*
Tâche: calculer la direction correspondant à une touche ou -1
Paramètre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond à la touche
(0: droite, 1: gauche, 2: haut, 3: bas)
*/
int calculerDirectionTouche(int touche)
{
	if (touche == 87)
	{
		return 2; // w --> Haut
	}
	else if (touche == 65)
	{
		return 1; // a --> gauche
	}
	else if (touche == 83)
	{
		return 3; // s --> bas
	}
	else if (touche == 68)
	{
		return 0; // d --> droite
	}
}

/*
Tâche: déplacer le serpent dans la direction
Paramètres: la direction du serpent en entrée, et la position du serpent en entrée / sortie.
*/
void deplacerSerpentI(int direction, int &posX, int &posY)
{
	switch (direction)
	{
	case 0: //deplacement vers la droite
	{
		// si la position en X du serpent n'est pas a la limite
		if (posX < 69)
		{
			//on supprime le serpent de sa position actuelle et on l'affiche une case vers la droite
			posX++;
		}
		else // sinon : il sort du terrain
		{
			//on supprime le serpent de sa position actuelle et on l'affiche au debut de la ligne
			posX = 2;
		}
		break;
	}
	case 1: //deplacement vers la gauche
	{
		// si la position en X du serpent n'est pas a la limite
		if (posX > 2)
		{
			//on supprime le serpent de sa position actuelle et on l'affiche une case vers la gauche
			posX--;
		}
		else // sinon : il sort du terrain
		{
			//on supprime le serpent de sa position actuelle et on l'affiche au debut de la ligne
			posX = 69;
		}
		break;
	}
	case 2: //deplacement vers le haut
	{
		// si la position en Y du serpent n'est pas a la limite
		if (posY > 2)
		{
			//on supprime le serpent de sa position actuelle et on l'affiche une case vers le haut
			posY--;
		}
		else // sinon : il sort du terrain
		{
			//on supprime le serpent de sa position actuelle et on l'affiche au debut de la colone
			posY = 29;
		}
		break;
	}
	case 3: //deplacement vers le bas
	{
		// si la position en Y du serpent n'est pas a la limite
		if (posY < 29)
		{
			//on supprime le serpent de sa position actuelle et on l'affiche une case vers le bas
			posY++;
		}
		else // sinon : il sort du terrain
		{
			//on supprime le serpent de sa position actuelle et on l'affiche au debut de la colone
			posY = 2;
		}
		break;
	}
	}
}

/*
Retourner un caractere lu au clavier en n'acceptant
que 'O', 'N', 'o', 'n'.
*/
char oui_non()
{
	char caractere;

	do
	{
		caractere = toupper(_getch());
	} while ((caractere != 'O') && (caractere != 'N'));

	return caractere;
}


/*
Tâche: lire le niveau de difficulté avec tests de validation d'entrée
Retour: le niveau (= le nombre de souris initialement sur le terrain)
*/
int saisirNiveau()
{
	int niveau;
	cout << "Saisir le niveau de difficulte (de 1 a 20) : ";
	cin >> niveau;
	while (cin.fail() || cin.peek() != 10 || niveau < 1 || niveau > 20)
	{
		cin.clear();
		cin.ignore(512, '\n');
		cout << "Erreur de saisie ! Veuillez saisir un nombre entre 1 et 20 : ";
		cin >> niveau;
	}

	return niveau;
}

/*
Tâche: afficher les souris
Paramètres: les tableaux de coordonnées et le nombre de souris
*/
void afficherSouris(int sourisX[], int sourisY[], int nbSouris)
{
	for (int i = 0; i < nbSouris; i++)
	{
		gotoXY(sourisX[i], sourisY[i]);
		cout << "*";
	}
}

/*
Tâche: générer les nbSouris aléatoirement sur le terrain
Paramètres: les dimensions du terrain, les tableaux de coordonnées et le
nombre de souris
*/
void creerSouris(int nbLignes, int nbColonnes, int sourisX[], int sourisY[], int nbSouris)
{
	for (int i = 0; i < nbSouris; i++)
	{
		bool existe;
		int sourisActuelleX;
		int sourisActuelleY;
		do
		{
			existe = false;

			sourisActuelleX = ((rand() % (nbColonnes - 2)) + 2);
			sourisActuelleY = ((rand() % (nbLignes - 2)) + 2);

			for (int j = 0; j < nbSouris && existe == false; j++)
			{
				if (sourisX[j] == sourisActuelleX && sourisY[j] == sourisActuelleY)
				{
					existe = true;
				}
			}

		} while (existe);

		sourisX[i] = sourisActuelleX;
		sourisY[i] = sourisActuelleY;
	}
}

/*
Tâche: déplacer le serpent dans la direction
Paramètres: en entrée : 	la direction du serpent,
en entrée/sortie :	les tableaux de coordonnées du serpent et
la taille du serpent.
*/
void deplacerSerpentII(int direction, int serpentX[], int serpentY[], int &tailleSerpent, int sourisX[], int sourisY[], int &nbSouris)
{
	for (int i = 0; i < tailleSerpent - 1; i++)
	{
		serpentX[i] = serpentX[i + 1];
		serpentY[i] = serpentY[i + 1];
	}
	deplacerSerpentI(direction, serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1]);
}

/*
Tâche: tester si (posX, posY) est la position d'une souris. Si oui, retirer
la souris de la population de souris
Paramètres: la position de la tête du serpent, les tableaux de coordonnées et
le nombre de souris
Retour: true si collision, false sinon
*/
bool testerCollision(int posX, int posY, int sourisX[], int sourisY[], int &nbSouris)
{
	bool collision = false;
	for (int i = 0; i < nbSouris; i++)
	{
		if (sourisX[i] == posX && sourisY[i] == posY)
		{
			collision = true;
			sourisX[i] = -1;
			sourisY[i] = -1;
			nbSouris--;
		}

		if (sourisX[i] == -1)
		{
			sourisX[i] = sourisX[i + 1];
			sourisY[i] = sourisY[i + 1];

			sourisX[i + 1] = -1;
			sourisY[i + 1] = -1;
		}
	}
	return collision;
}

// Afficher le serpent
void afficherSerpent(int serpentX[], int serpentY[], int tailleSerpent)
{
	for (int i = 0; i < tailleSerpent; i++)
	{
		gotoXY(serpentX[i], serpentY[i]);
		cout << '@';
	}
}

// Effacer le serpent
void effacerSerpent(int serpentX[], int serpentY[], int tailleSerpent)
{
	for (int i = 0; i < tailleSerpent; i++)
	{
		gotoXY(serpentX[i], serpentY[i]);
		cout << ' ';
	}
}

// Tester la collision du serpent avec lui-même
bool testerCollisionSerpent(int posX, int posY, int serpentX[], int serpentY[], int tailleSerpent)
{
	for (int i = 0; i < tailleSerpent - 1; i++)
	{
		if (serpentX[i] == posX && serpentY[i] == posY)
		{
			return true;
		}
	}
	return false;
}

// Tester la collision du serpent avec les murs
bool testerCollisionMurs(int posX, int posY, int mursX[], int mursY[], int nbMurs)
{
	bool collisionMur = false;
	for (int i = 0; i < nbMurs; i++)
	{
		if (mursX[i] == posX && mursY[i] == posY)
		{
			return true;
		}
	}
	return collisionMur;
}

/*
Tâche: placer des murs aléatoirement sur le terrain de jeu
Paramètres: les dimensions du terrain et le nombre de murs à placer
*/
void creerMurs(int nbLignes, int nbColonnes, int mursX[], int mursY[], int nbMurs)
{
	for (int i = 0; i < nbMurs; i++)
	{
		bool existe;
		int mursActuelleX;
		int mursActuelleY;
		do
		{
			existe = false;

			mursActuelleX = ((rand() % (nbColonnes - 2)) + 2);
			mursActuelleY = ((rand() % (nbLignes - 2)) + 2);

			for (int j = 0; j < nbMurs && existe == false; j++)
			{
				if (mursX[j] == mursActuelleX && mursY[j] == mursActuelleY)
				{
					existe = true;
				}
			}

		} while (existe);

		mursX[i] = mursActuelleX;
		mursY[i] = mursActuelleY;
	}
}
void placerMurs(int mursX[], int mursY[], int nbMurs)
{
	for (int i = 0; i < nbMurs; i++)
	{
		gotoXY(mursX[i], mursY[i]);
		cout << "|";
	}
}

//Affichage des règles du jeu
void affichageReglesJeu()
{
	cout << "		  |	LE SERPENT	|" << endl;
	cout << "		   ---------------------" << endl << endl << endl;
	cout << "			Regle du jeu :				" << endl << endl;
	cout << "  Deplacer le serpent et manger toutes les souris pour gagner." << endl << endl;
	cout << "			ATTENTION :					" << endl << endl;
	cout << "  Vous perdez si vous touchez un des murs dissemine sur la grille !" << endl << endl;
	cout << "			   OU" << endl << endl;
	cout << "	   Vous perdez si vous vous mordez !" << endl << endl;
	cout << "		     Bonne chance !" << endl<<endl;

}
