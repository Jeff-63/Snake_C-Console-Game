#include <conio.h>	// pour _kbhit() et _getch()
#include <time.h>	// pour time(int)
#include <stdlib.h>	// pour srand(int) et rand()
#include "RockUtiles.h"	// pour la gestion de l'�cran

// Partie I
// --------
void afficherTerrain(int nbLignes, int nbColonnes);
int recupererTouche();
int calculerDirectionTouche(int touche);
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY);
void deplacerSerpentI(int direction, int &posX, int &posY);

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
bool testerCollisionMurs(int posX, int posY);
void deplacerSouris(int sourisX[], int sourisY[], int nbSouris);
void placerMurs(int nbLignes, int nbColonnes, int nbMurs);

//  fonction principale
//  -------------------
void main()
{
	srand(time(NULL));
	int const NB_LIGNES = 30;
	int const NB_COLONES = 70;
	int const NB_ELEMENTS_SERPENT = 21;
	int const NB_ELEMENTS_SOURIS = 21;
	int const NB_ELEMENTS_MURS = 11;
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
	bool gagnez = true;
	//on demande de saisir le niveau de difficult�/le nb de souris sur le terrain.
	int niveau = saisirNiveau();
	nbSouris = niveau;
	nbMurs = niveau / 2;
	system("cls");
	//on affiche le cadre
	afficherTerrain(NB_LIGNES, NB_COLONES);
	//on initialise aleatoirement la position du serpent
	positionAleatoire(NB_LIGNES, NB_COLONES, serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1]);
	curseurVisible(false);
	// on cr�� les souris suivant le niveau de difficult�
	creerSouris(NB_LIGNES, NB_COLONES, sourisX, sourisY, nbSouris);
	// on les affiches
	afficherSouris(sourisX, sourisY, nbSouris);
	// on affiche les murs aleatoires
	placerMurs(NB_LIGNES, NB_COLONES, nbMurs);

	while (nbSouris > 0 && gagnez)
	{
		touche = recupererTouche();
		if (touche != -1)
		{
			direction = calculerDirectionTouche(touche);
		}
		effacerSerpent(serpentX, serpentY, tailleSerpent);
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
		bool collisionSerpent = testerCollisionSerpent(serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1], serpentX, serpentY, tailleSerpent);
		if (collisionSerpent)
		{
			gagnez = false;
		}
		deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
		afficherSerpent(serpentX, serpentY, tailleSerpent);
		Sleep(70);
	}
	if (gagnez)
	{
		system("cls");
		cout << "Bravo, vous avez gagne !" << endl << endl;
		system("pause");
	}
	else
	{
		system("cls");
		cout << "Vous avez perdu. Meilleure chance la prochaine fois !" << endl << endl;
		system("pause");
	}
}

//--------------------------------------------------------------/
/*
T�che: afficher le contour du terrain
Param�tres: le nombre de lignes et de colonnes du terrain rectangulaire
*/
void afficherTerrain(int nbLignes, int nbColonnes)
{
	cadre(1, 1, nbColonnes, nbLignes, 10);
	setDimensionFenetre(1, 1, nbColonnes, nbLignes);
}


/*
T�che: calculer une position al�atoire sur le terrain
Param�tres: les dimensions du terrain en entr�e et les coordonn�es de la position al�atoire en sortie
*/
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY)
{
	posX = ((rand() % (nbColonnes - 2)) + 2);
	posY = ((rand() % (nbLignes - 2)) + 2);
}

/*
T�che: tester si le joueur a appuy� sur une touche
Retour: retourner le code ASCII de la touche press�e, -1 sinon
*/
int recupererTouche()
{
	//on verifie qu'une touche a �t� press�e avec _kbhit()
	if (_kbhit() == NULL)
	{
		return -1;
	}
	else
	{
		// Si on a appuy� sur une touche, seul les touches de d�placement seront valides (w,a,s,d)
		char caractere;
		do
		{
			caractere = toupper(_getch());
		} while ((caractere != 'W') && (caractere != 'A') && (caractere != 'S') && (caractere != 'D'));

		//on retourne la valeur ASCII de la touche sur laquelle on a appuy�
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
T�che: calculer la direction correspondant � une touche ou -1
Param�tre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond � la touche
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
T�che: d�placer le serpent dans la direction
Param�tres: la direction du serpent en entr�e, et la position du serpent en entr�e / sortie.
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
T�che: lire le niveau de difficult� avec tests de validation d'entr�e
Retour: le niveau (= le nombre de souris initialement sur le terrain)
*/
int saisirNiveau()
{
	int niveau;
	cout << "Saisir le niveau de difficulte (de 1 a 20) : ";
	cin >> niveau;
	while (cin.fail() || cin.peek() != 10 || niveau < 0 || niveau > 20)
	{
		cin.clear();
		cin.ignore(512, '\n');
		cout << "Erreur de saisie ! Veuillez saisir un nombre entre 1 et 20 : ";
		cin >> niveau;
	}

	return niveau;
}

/*
T�che: afficher les souris
Param�tres: les tableaux de coordonn�es et le nombre de souris
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
T�che: g�n�rer les nbSouris al�atoirement sur le terrain
Param�tres: les dimensions du terrain, les tableaux de coordonn�es et le
nombre de souris
*/
void creerSouris(int nbLignes, int nbColonnes, int sourisX[], int sourisY[], int nbSouris)
{
	for (int i = 0; i < nbSouris; i++)
	{
		sourisX[i] = ((rand() % (nbColonnes - 2)) + 2);
		sourisY[i] = ((rand() % (nbLignes - 2)) + 2);
	}
}

/*
T�che: d�placer le serpent dans la direction
Param�tres: en entr�e : 	la direction du serpent,
en entr�e/sortie :	les tableaux de coordonn�es du serpent et
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
T�che: tester si (posX, posY) est la position d'une souris. Si oui, retirer
la souris de la population de souris
Param�tres: la position de la t�te du serpent, les tableaux de coordonn�es et
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

/*
On teste si le serpent se mord lui-m�me
*/
bool testerCollisionSerpent(int posX, int posY, int serpentX[], int serpentY[], int tailleSerpent)
{
	for (int i = 0; i < tailleSerpent-1; i++)
	{
		if (serpentX[i] == posX && serpentY[i] == posY)
		{
			return true;
		}
	}
	return false;
}

//Affichage du serpent
void afficherSerpent(int serpentX[], int serpentY[], int tailleSerpent)
{
	for (int i = 0; i < tailleSerpent; i++)
	{
		gotoXY(serpentX[i], serpentY[i]);
		cout << '@';
	}
}

//Effacement de l'affichage
void effacerSerpent(int serpentX[], int serpentY[], int tailleSerpent)
{
	for (int i = 0; i < tailleSerpent; i++)
	{
		gotoXY(serpentX[i], serpentY[i]);
		cout << ' ';
	}
}

//Test de collision avec un mur
bool testerCollisionMurs(int posX, int posY)
{
	bool collisionMur = false;
	if (getCharXY(posX, posY) == '|')
	{
		return true;
	}
	return collisionMur;
}
/*
T�che: d�placer les souris (al�atoirement ou intelligemment)
Param�tres: les tableaux de coordonn�es et le nombre de souris
*/
void deplacerSouris(int sourisX[], int sourisY[], int nbSouris)
{

}
/*
T�che: placer des murs al�atoirement sur le terrain de jeu
Param�tres: les dimensions du terrain et le nombre de murs � placer
*/
void placerMurs(int nbLignes, int nbColonnes, int nbMurs)
{
	for (int i = 0; i < nbMurs; i++)
	{
		gotoXY(((rand() % (nbColonnes - 2)) + 2), ((rand() % (nbLignes - 2)) + 2));
		cout << '|';
	}
}


