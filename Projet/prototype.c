#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <locale.h>
#include "prototype.h"

// Fonction pour vérifier les informations d'identification

void viderTamponEntree()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int Authentifier(char *username, char *password, int *isAdmin, int *idUtilisateurConnecte)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return -1; // Erreur lors de l'ouverture du fichier
    }

    char buffer[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 10]; // +10 pour stocker aussi ID_User
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char fileUsername[MAX_USERNAME_LENGTH], filePassword[MAX_PASSWORD_LENGTH];
        int fileIsAdmin, userID;
        if (sscanf(buffer, "%s %s %d %d", fileUsername, filePassword, &fileIsAdmin, &userID) == 4)
        {
            // Vérifier si les informations d'identification correspondent
            if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0)
            {
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
void masquerMotDePasse(char *password)
{
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
    while ((c = getchar()) != '\n' && i < MAX_PASSWORD_LENGTH - 1)
    {
        if (c == 127 && i > 0)
        {                    // Vérifie la touche "backspace"
            printf("\b \b"); // Efface le caractère précédent et l'espace
            i--;
        }
        else if (c != 127)
        { // Si ce n'est pas la touche "backspace"
            password[i++] = c;
            printf("*");
        }
    }

    password[i] = '\0';

    // Restaurer les paramètres du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void menuAdministration()
{
    puts("========================== ADMINISTRATION ===========================");
    puts("= \t\t1. Gestion des Etudiants\t\t\t=");
    puts("= \t\t2. Génération de fichiers\t\t\t=");
    puts("= \t\t3. Marquer les présences\t\t\t=");
    puts("= \t\t4. Envoyer un message\t\t\t\t=");
    puts("= \t\t5. Deconnexion\t\t\t\t\t=");
    puts("====================================================================");
    puts("Veuillez choisir une option : ");
}
void menuEtudiant()
{

    puts("========================== ETUDIANT =============================");
    puts("= \t\t1. Marquer ma présences\t\t\t\t=");
    puts("= \t\t2. Nombre de minute d'abscence\t\t\t=");
    puts("= \t\t3. Mes message (0)\t\t\t\t=");
    puts("= \t\t4. Deconnexion\t\t\t\t\t=");
    puts("=================================================================");
    puts("Veuillez choisir une option : ");
}

void menuGenerationdeFichier()
{

    puts("========================== Génétion de fichier ==================");
    puts("= \t\t1. Générer par dates\t\t\t\t=");
    puts("= \t\t2. Générer par date donnée\t\t\t=");
    puts("= \t\t3. Quitter\t\t\t\t\t=");
    puts("=================================================================");
    puts("Veuillez choisir une option : ");
}

int lireDonneesEtudiants(Etudiant etudiants[], int maxEtudiants)
{
    FILE *fichier = fopen("etudiants.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des étudiants.\n");
        exit(1);
    }

    int numEtudiants = 0;
    while (fscanf(fichier, "%d %s %s %s %d", &etudiants[numEtudiants].id, etudiants[numEtudiants].prenom, etudiants[numEtudiants].nom, etudiants[numEtudiants].classe, &etudiants[numEtudiants].present) == 5)
    {
        numEtudiants++;
        if (numEtudiants >= maxEtudiants)
        {
            printf("Nombre maximal d'étudiants atteint.\n");
            break;
        }
    }

    fclose(fichier);
    return numEtudiants;
}

void marquerPresence(Etudiant etudiants[], int numEtudiants)
{
    int idEtudiant;
    time_t heureActuelle;
    struct tm *infoTemps;

    // Ouvrir le fichier pour l'ajout de données
    FILE *fichier = fopen("listeDePresence.txt", "a");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier de liste de présence.\n");
        exit(1);
    }
    // Obtenir la date et l'heure actuelles
    time(&heureActuelle);
    infoTemps = localtime(&heureActuelle);

    setlocale(LC_TIME, "fr_FR.utf8");

    // Boucle pour marquer la présence des étudiants
    while (1)
    {
        int idEtudiant;
        char choix[10]; // Chaîne pour stocker l'entrée de l'utilisateur

        // Demander à l'utilisateur de saisir l'ID de l'étudiant présent (ou 'q' pour quitter)
        printf("Entrez l'ID de l'étudiant présent (ou 'q' pour quitter) : ");
        scanf("%s", choix); // Lire l'entrée de l'utilisateur comme une chaîne de caractères

        // Vérifier si l'utilisateur a entré 'q' pour quitter
        if (tolower(choix[0]) == 'q')
        {
            fclose(fichier); // Fermer le fichier
            return;          // Sortir de la fonction
        }

        // Convertir la chaîne en entier
        idEtudiant = atoi(choix);

        // Recherche de l'étudiant dans le tableau
        int etudiantTrouve = 0;
        for (int i = 0; i < numEtudiants; i++)
        {
            if (etudiants[i].id == idEtudiant)
            {
                // Vérifier si l'étudiant a déjà été marqué présent aujourd'hui
                if (etudiants[i].dernierePresence.tm_mday == infoTemps->tm_mday && etudiants[i].dernierePresence.tm_mon == infoTemps->tm_mon && etudiants[i].dernierePresence.tm_year == infoTemps->tm_year)
                {
                    printf("L'étudiant a déjà été marqué présent aujourd'hui.\n");
                }
                else
                {
                    // Marquer l'étudiant présent
                    etudiants[i].present = 1;
                    etudiants[i].dernierePresence = *infoTemps; // Mettre à jour la date de la dernière présence

                    // Formater la date et l'heure en français
                    char dateHeure[100];
                    strftime(dateHeure, sizeof(dateHeure), "%A %d %B %Y %H:%M:%S", infoTemps);

                    // Enregistrer la date et l'heure d'arrivée dans le fichier de présence
                    fprintf(fichier, "ID: %d - Prénom: %s - Nom: %s - Classe: %s - Date et heure d'arrivée: %s\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom, etudiants[i].classe, dateHeure);
                    printf("Présence de l'étudiant %s %s marquée.\n", etudiants[i].prenom, etudiants[i].nom);
                }
                etudiantTrouve = 1;
                break;
            }
        }

        if (!etudiantTrouve)
        {
            printf("Erreur : étudiant non trouvé. Veuillez saisir un ID valide.\n");
        }
    }
    fclose(fichier);
}

void sauvegarderDonneesEtudiants(Etudiant etudiants[], int numEtudiants)
{
    FILE *fichier = fopen("etudiants.txt", "w");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier des étudiants.\n");
        exit(1);
    }

    for (int i = 0; i < numEtudiants; i++)
    {
        fprintf(fichier, "%d %s %s %s %d\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom, etudiants[i].classe, etudiants[i].present);
        puts("");
    }

    fclose(fichier);
}

void marquerPresenceEtudiant(Etudiant etudiants[], int numEtudiants, int idEtudiantConnecte) {
    // Obtenir la date et l'heure actuelles
    time_t heureActuelle;
    struct tm *infoTemps;
    time(&heureActuelle);
    infoTemps = localtime(&heureActuelle);
    setlocale(LC_TIME, "fr_FR.utf8");

    // Vérifier si l'ID de l'étudiant connecté est présent dans la liste des étudiants
    int etudiantTrouve = 0;
    for (int i = 0; i < numEtudiants; i++) {
        if (etudiants[i].id == idEtudiantConnecte) {
            etudiantTrouve = 1;
            break;
        }
    }

    // Si l'ID de l'étudiant connecté est trouvé dans la liste
    if (etudiantTrouve) {
        // Recherche de l'étudiant par son ID
        for (int i = 0; i < numEtudiants; i++) {
            if (etudiants[i].id == idEtudiantConnecte) {
                // Vérifier si l'étudiant a déjà été marqué présent aujourd'hui
                if (etudiants[i].dernierePresence.tm_mday == infoTemps->tm_mday && 
                    etudiants[i].dernierePresence.tm_mon == infoTemps->tm_mon && 
                    etudiants[i].dernierePresence.tm_year == infoTemps->tm_year) {
                    printf("Vous avez déjà été marqué présent aujourd'hui.\n");
                    return;
                } else {
                    // Marquer l'étudiant présent
                    etudiants[i].present = 1;
                    etudiants[i].dernierePresence = *infoTemps; // Mettre à jour la date de la dernière présence

                    // Enregistrer la présence dans listeDePresence.txt
                    FILE *listePresence = fopen("listeDePresence.txt", "a");
                    if (listePresence == NULL) {
                        printf("Erreur lors de l'ouverture du fichier de liste de présence.\n");
                        return;
                    }
                    char dateHeure[100];
                    strftime(dateHeure, sizeof(dateHeure), "%A %d %B %Y %H:%M:%S", infoTemps);

                    fprintf(listePresence, "ID: %d - Prénom: %s - Nom: %s - Classe: %s - Date et heure d'arrivée: %s\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom, etudiants[i].classe, dateHeure);
                    fclose(listePresence);

                    // Mettre à jour le fichier des étudiants avec le statut de présence
                    FILE *fichierEtudiants = fopen("etudiants.txt", "w");
                    if (fichierEtudiants == NULL) {
                        printf("Erreur lors de l'ouverture du fichier des étudiants pour la mise à jour du statut de présence.\n");
                        return;
                    }
                    for (int j = 0; j < numEtudiants; j++) {
                        fprintf(fichierEtudiants, "%d %s %s %s %d\n", etudiants[j].id, etudiants[j].prenom, etudiants[j].nom, etudiants[j].classe, etudiants[j].present);
                    }
                    fclose(fichierEtudiants);

                    // Afficher la présence de l'étudiant
                    printf("Votre présence a été marquée.\n");
                    return;
                }
            }
        }
    } else {
        printf("Votre ID étudiant n'a pas été trouvé dans la liste des étudiants.\n");
    }
}

void genererNomMois(int mois, char *nomMois)
{
    switch (mois)
    {
    case 1:
        strcpy(nomMois, "janvier");
        break;
    case 2:
        strcpy(nomMois, "février");
        break;
    case 3:
        strcpy(nomMois, "mars");
        break;
    case 4:
        strcpy(nomMois, "avril");
        break;
    case 5:
        strcpy(nomMois, "mai");
        break;
    case 6:
        strcpy(nomMois, "juin");
        break;
    case 7:
        strcpy(nomMois, "juillet");
        break;
    case 8:
        strcpy(nomMois, "août");
        break;
    case 9:
        strcpy(nomMois, "septembre");
        break;
    case 10:
        strcpy(nomMois, "octobre");
        break;
    case 11:
        strcpy(nomMois, "novembre");
        break;
    case 12:
        strcpy(nomMois, "decembre");
        break;
    default:
        strcpy(nomMois, "mois_invalide");
    }
}

void genererFichierListePresence(Etudiant etudiants[], int numEtudiants)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    char filename[100];
    strftime(filename, sizeof(filename), "Liste_de_presence_%d_%b_%Y.txt", tm_info);

    FILE *fichier = fopen(filename, "w");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier de liste de présence.\n");
        return;
    }

    // En-tête du tableau
    fprintf(fichier, "| %-5s | %-20s | %-20s | %-15s | %-7s |\n", "ID", "Prénom", "Nom", "Classe", "Présent");
    fprintf(fichier, "+-------+----------------------+----------------------+-----------------+---------+\n");

    // Déterminer la largeur de chaque colonne
    int largeurID = 5;
    int largeurPrenom = 20;
    int largeurNom = 20;
    int largeurClasse = 15;
    int largeurPresent = 7;

    for (int i = 0; i < numEtudiants; i++)
    {
        // Mettre à jour la largeur si nécessaire
        if (strlen(etudiants[i].prenom) > largeurPrenom)
            largeurPrenom = strlen(etudiants[i].prenom);
        if (strlen(etudiants[i].nom) > largeurNom)
            largeurNom = strlen(etudiants[i].nom);
        if (strlen(etudiants[i].classe) > largeurClasse)
            largeurClasse = strlen(etudiants[i].classe);
    }

    // Contenu du tableau
    for (int i = 0; i < numEtudiants; i++)
    {
        fprintf(fichier, "| %-*d | %-*s | %-*s | %-*s | %-*d |\n", largeurID, etudiants[i].id,
                largeurPrenom, etudiants[i].prenom, largeurNom, etudiants[i].nom,
                largeurClasse, etudiants[i].classe, largeurPresent, etudiants[i].present);
    }

    fclose(fichier);

    printf("Fichier de liste de présence généré avec succès : %s\n", filename);
}

int validerFormatDate(const char *date) {
    int jour, mois, annee;
    if (sscanf(date, "%d/%d/%d", &jour, &mois, &annee) != 3) {
        return 0; // Format invalide
    }
    // Vérifier si le jour, le mois et l'année sont valides
    if (jour < 1 || jour > 31 || mois < 1 || mois > 12 || annee < 0) {
        return 0; // Date invalide
    }
    // Vérifier si le nombre de jours est correct pour le mois spécifié
    int joursParMois[] = {31, 28 + (annee % 4 == 0 && (annee % 100 != 0 || annee % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (jour > joursParMois[mois - 1]) {
        return 0; // Jour invalide pour le mois spécifié
    }
    return 1; // Format et date valides
}

// Fonction pour générer un fichier de liste de présence pour une date spécifiée
void genererFichierListePresenceParDate(const char *nomFichierListePresence, const char *dateEntree) {
    FILE *fichierEntree = NULL;
    FILE *fichierSortie = NULL;
    char nomFichierSortie[100]; // Pour stocker le nom du fichier de sortie
    struct tm dateSpecifiee = {0};

    // Valider le format de la date saisie
    if (!validerFormatDate(dateEntree)) {
        printf("Format de date invalide. Assurez-vous de saisir la date au format jj/mm/aaaa.\n");
        return;
    }

    // Convertir la date de l'entrée en structure tm
    sscanf(dateEntree, "%d/%d/%d", &dateSpecifiee.tm_mday, &dateSpecifiee.tm_mon, &dateSpecifiee.tm_year);
    dateSpecifiee.tm_mon--; // Les mois commencent à 0 dans la structure tm
    dateSpecifiee.tm_year -= 1900; // L'année est comptée à partir de 1900 dans la structure tm

    // Ouvrir le fichier d'entrée en lecture
    fichierEntree = fopen(nomFichierListePresence, "r");
    if (fichierEntree == NULL) {
        printf("Erreur lors de l'ouverture du fichier de présence.\n");
        return;
    }

    // Construire le nom du fichier de sortie avec la date spécifiée
    snprintf(nomFichierSortie, sizeof(nomFichierSortie), "ListePresenceDuDate_%s.txt", dateEntree);

    // Ouvrir le fichier de sortie en écriture
    fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        printf("Erreur lors de la création du fichier de sortie.\n");
        fclose(fichierEntree);
        return;
    }

    // Lire les présences du fichier d'entrée et les écrire dans le fichier de sortie
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichierEntree) != NULL) {
        struct tm datePresence = {0};
        int id;
        if (sscanf(ligne, "ID: %d - Prénom: %*s - Nom: %*s - Classe: %*s - Date: %d/%d/%d", &id, &datePresence.tm_mday, &datePresence.tm_mon, &datePresence.tm_year) == 4) {
            datePresence.tm_mon--; // Les mois commencent à 0 dans la structure tm
            datePresence.tm_year -= 1900; // L'année est comptée à partir de 1900 dans la structure tm
            if (difftime(mktime(&datePresence), mktime(&dateSpecifiee)) == 0) {
                fprintf(fichierSortie, "%s", ligne);
            }
        }
    }

    // Fermer les fichiers
    fclose(fichierEntree);
    fclose(fichierSortie);

    printf("Fichier de présence généré avec succès pour la date %s.\n", dateEntree);
}