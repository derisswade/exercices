#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <time.h>
#include <ctype.h>
#include <stddef.h>


#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_LONGUEUR_PRENOM 50
#define MAX_LONGUEUR_NOM 50
#define MAX_LONGUEUR_CLASSE 20
#define MAX_ETUDIANTS 100
#define MAX_LONGUEUR_USERNAME 50
#define MAX_LONGUEUR_ID 5
#define MAX_LENGHT_MESSAGE 255
#define MAX_MESSAGES_RECEIVED 100

typedef struct {
    int id;
    char nom[MAX_LONGUEUR_NOM];
    char prenom[MAX_LONGUEUR_NOM];
    char classe[MAX_LONGUEUR_CLASSE];
    int present;
    struct tm dernierePresence;
    char nomUtilisateur[MAX_LONGUEUR_USERNAME];
    char messageNonLu[MAX_MESSAGES_RECEIVED][MAX_LENGHT_MESSAGE]; // Champ pour stocker les messages non lus
    int nbMessagesNonLus; 
} Etudiant;

typedef struct {
    int id; // ID du message
    char contenu[MAX_LENGHT_MESSAGE]; // Contenu du message
} Message;

void viderTamponEntree();

// Prototype de la fonction de vérification des informations d'identification
int Authentifier(char *username, char *password, int *isAdmin ,int *idUtilisateurConnecte);

// Fonction pour masquer les caractères saisis par des étoiles
void masquerMotDePasse(char *password);

void menuAdministration ();
void menuEtudiant();
void menuGenerationdeFichier();
void marquerPresence(Etudiant etudiants[], int numEtudiants);
int lireDonneesEtudiants(Etudiant etudiants[], int maxEtudiants);
void sauvegarderDonneesEtudiants(Etudiant etudiants[], int numEtudiants);
void marquerPresenceEtudiant(Etudiant etudiants[], int numEtudiants, int idEtudiant);
void genererNomMois(int mois, char *nomMois);
void genererFichierListePresence(Etudiant etudiants[], int numEtudiants);
int validerFormatDate(const char *date);
void genererFichierListePresenceParDate(const char *nomFichierListePresence, const char *dateEntree);


#endif /* PROTOTYPE_H */
