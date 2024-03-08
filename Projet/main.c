#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h> 
#include "prototype.h"

int main() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int choix;
    
    // Demander à l'utilisateur de saisir un nom d'utilisateur
    printf("Entrez votre nom d'utilisateur : ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strcspn(username, "\n")] = '\0'; // Supprimer le retour à la ligne
    
    // Demander à l'utilisateur de saisir un mot de passe (masqué)
   
    masquerMotDePasse(password);

    // Vérifier les informations d'identification
    int isAdmin;
    int userID = Authentifier(username, password, &isAdmin);
    if (userID != -1) {
        if (isAdmin == 1) {
            printf("\nConnexion réussie en tant qu'administrateur. ID_User: %d\n", userID);
            menuAdministration();
            scanf("%d",&choix);
            system("clear");
            switch (choix) {
                case 1:
                    puts("Gestion des Etudiants");
                    break;
                case 2:
                    puts("Génération de fichiers");
                    break;
                case 3:
                    puts("Marquer les présences");
                    break;
                case 4:
                   puts("Envoyer un message");
                    break;
                case 5:
                    puts("Au revoir !");
                    break;
                default:
                  puts("Choix invalide");
                break;
            }
        } else {
            printf("\nConnexion réussie en tant qu'étudiant. ID_User: %d\n", userID);
            menuEtudiant();
            scanf("%d",&choix);
            system("clear");
            switch (choix) {
                case 1:
                    puts("Marquer ma présence");
                    break;
                case 2:
                    puts("Nombre de minutes d'abscence");
                    break;
                case 3:
                    puts("Mes Messages (0)");
                    break;
                case 4:
                   puts("Au revoir!");
                    break;
                default:
                  puts("Choix invalide");
                break;
            }
        }
    } else {
        printf("\nNom d'utilisateur ou mot de passe incorrect.\n");
    }
    
    return 0;
}
