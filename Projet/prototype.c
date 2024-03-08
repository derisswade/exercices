#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "prototype.h"

// Fonction pour vérifier les informations d'identification
int Authentifier(char *username, char *password, int *isAdmin) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return -1; // Erreur lors de l'ouverture du fichier
    }

    char buffer[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 10]; // +10 pour stocker aussi ID_User
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char fileUsername[MAX_USERNAME_LENGTH], filePassword[MAX_PASSWORD_LENGTH];
        int fileIsAdmin, userID;
        if (sscanf(buffer, "%s %s %d %d", fileUsername, filePassword, &fileIsAdmin, &userID) == 4) {
            // Vérifier si les informations d'identification correspondent
            if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0) {
                fclose(file);
                *isAdmin = fileIsAdmin;
                return userID;
            }
        }
    }
    fclose(file);
    return -1; // Utilisateur non trouvé
}

// Fonction pour masquer les caractères saisis par des étoiles
void masquerMotDePasse(char *password) {
    int i = 0;
    char c;
    struct termios oldt, newt;

    // Désactiver l'écho des caractères saisis
    printf("Entrez votre mot de passe : ");
    fflush(stdout); // Pour vider le buffer de sortie
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Désactiver l'écho des caractères saisis
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Lire les caractères un par un et les masquer
    while ((c = getchar()) != '\n' && i < MAX_PASSWORD_LENGTH - 1) {
        if (c == '\b' && i > 0) { // Gestion de la touche backspace
            printf("\b \b");
            i--;
        } else {
            password[i++] = c;
            printf("*");
        }
    }
    password[i] = '\0';

    // Restaurer les paramètres du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
void menuAdministration (){
      puts("========================== ADMINISTRATION ===========================");
      puts("= \t\t1. Gestion des Etudiants\t\t=");
      puts("= \t\t2. Génération de fichiers\t\t=");
      puts("= \t\t3. Marquer les présences\t\t=");
      puts("= \t\t4. Envoyer un message\t\t\t=");
      puts("= \t\t5. Quitter\t\t\t\t=");
      puts("===================================================================="); 
      puts("Veuillez choisir une option : ");  

}
void menuEtudiant (){
    puts("========================== ETUDIANT ===========================");
    puts("= \t\t1. Marquer ma présences\t\t=");
    puts("= \t\t2. Nombre de minute d'abscence\t\t=");
    puts("= \t\t3. Mes message (0)\t\t\t=");
    puts("= \t\t4. Quitter\t\t\t=");
    puts("===================================================================="); 
    puts("Veuillez choisir une option : ");
}