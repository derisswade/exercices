#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

// Prototype de la fonction de vérification des informations d'identification
int Authentifier(char *username, char *password, int *isAdmin);

// Fonction pour masquer les caractères saisis par des étoiles
void masquerMotDePasse(char *password);

#endif /* PROTOTYPE_H */
