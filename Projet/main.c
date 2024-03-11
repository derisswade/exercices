#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <locale.h>
#include "prototype.h"

int main()
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int choix, choix2, idEtudiant;
    int idUtilisateurConnecte;
    int connecte = 0;
    int isAdmin;
    int userID;
    Etudiant etudiants[MAX_ETUDIANTS];
    int numEtudiants = lireDonneesEtudiants(etudiants, MAX_ETUDIANTS);
    char dateEntree[30];

    do
    {
        int userID = Authentifier(username, password, &isAdmin, &idUtilisateurConnecte);
        if (!connecte)
        {
            // Demander à l'utilisateur de saisir un nom d'utilisateur
            printf("Entrez votre nom d'utilisateur : ");
            fgets(username, MAX_USERNAME_LENGTH, stdin);
            username[strcspn(username, "\n")] = '\0'; // Supprimer le retour à la ligne

            // Demander à l'utilisateur de saisir un mot de passe (masqué)

            masquerMotDePasse(password);

            // Vérifier les informations d'identification

            int userID = Authentifier(username, password, &isAdmin, &idUtilisateurConnecte);
            if (userID != -1)
            {

                connecte = 1;
                printf("\nConnexion réussie en tant que %s. ID_User: %d\n", isAdmin ? "administrateur" : "étudiant", userID);
            }
            else
            {
                printf("\nNom d'utilisateur ou mot de passe incorrect.\n");
            }
        }
        else
        {
            if (isAdmin == 1)
            {
                do
                {
                    menuAdministration();
                    scanf("%d", &choix);
                    /* system("clear"); */
                    switch (choix)
                    {
                    case 1:
                        puts("Gestion des Etudiants");
                        break;
                    case 2:
                        puts("Génération de fichiers");
                        do
                        {
                            menuGenerationdeFichier();
                            scanf("%d", &choix2);
                            switch (choix2)
                            {
                            case 1:
                                genererFichierListePresence(etudiants, numEtudiants);
                                break;
                            case 2:
                                puts("Génération de fichiers par date donneé");
                                printf("Entrez la date au format jj/mm/aaaa : ");
                                scanf("%s", dateEntree);
                                genererFichierListePresenceParDate("listeDePresence.txt", dateEntree);
                                break;
                            case 3:
                                puts("Au revoir");
                                break;
                            default:
                                printf("Choix invalide\n");
                                break;
                            }
                            if (choix2 == 3)
                            {
                                break;
                            }

                        } while (1);

                        break;
                    case 3:
                        puts("Marquer les présences");
                        puts(" ");
                        Etudiant etudiants[MAX_ETUDIANTS];
                        int numEtudiants = lireDonneesEtudiants(etudiants, MAX_ETUDIANTS);

                        // Affichage des étudiants non marqués présents
                        printf("Liste des étudiants non marqués présents :\n");
                        printf("ID\tPrénom\t\tNom\t\tClasse\n");
                        printf("---------------------------------------------\n");
                        for (int i = 0; i < numEtudiants; i++)
                        {
                            if (!etudiants[i].present)
                            {
                                printf("%d\t%s\t\t%s\t\t%s\n", etudiants[i].id, etudiants[i].prenom, etudiants[i].nom, etudiants[i].classe);
                            }
                        }

                        // Marquage de la présence des étudiants
                        marquerPresence(etudiants, numEtudiants);

                        // Enregistrement des données mises à jour dans le fichier
                        sauvegarderDonneesEtudiants(etudiants, numEtudiants);

                        break;
                    case 4:
                        /* Etudiant etudiants[MAX_ETUDIANTS];
                        int numEtudiants = lireDonneesEtudiants(etudiants, MAX_ETUDIANTS);
                        char message [MAX_LENGHT_MESSAGE]; */
                        puts("Envoyer un message");
                        /* envoyerMessage(etudiants , numEtudiants);
                        envoyerBroadcast(message , etudiants ,numEtudiants);
                        break; */
                    case 5:
                        puts("Au revoir !");
                        connecte = 0;
                        viderTamponEntree();
                        break;
                    default:
                        puts("Choix invalide");

                        break;
                    }
                } while (choix != 5);
            }
            else
            {
                do
                {
                    menuEtudiant();
                    scanf("%d", &choix);

                    switch (choix)
                    {
                    case 1:
                        Etudiant etudiants[MAX_ETUDIANTS];
                        int numEtudiants = lireDonneesEtudiants(etudiants, MAX_ETUDIANTS);

                        int idEtudiant;
                        char choix1[MAX_LONGUEUR_ID]; // Chaîne pour stocker l'entrée de l'utilisateur
                        while (1)
                        {
                            // Demander à l'utilisateur de saisir son propre ID étudiant (ou 'q' pour quitter)
                            printf("Entrez votre ID étudiant pour marquer votre présence (ou 'q' pour quitter) : ");
                            scanf("%s", choix1); // Lire l'entrée de l'utilisateur comme une chaîne de caractères
                            /* choix1[strcspn(choix1, "\n")] = '\0'; */

                            // Vérifier si l'utilisateur a entré 'q' pour quitter
                            if (tolower(choix1[0]) == 'q')
                            {

                                break; // Quitter le programme
                            }

                            //* / Convertir la chaîne en entier
                            idEtudiant = atoi(choix1);
                            printf("la valeur de idEtudiant est : %d\n", idEtudiant);
                            printf("la valeur de userID est : %d\n", userID);

                            // Vérifier si l'ID étudiant appartient à l'utilisateur connecté
                            if (userID == idEtudiant)
                            {
                                // Appeler la fonction pour marquer la présence de l'étudiant
                                marquerPresenceEtudiant(etudiants, numEtudiants, idEtudiant);
                                break;
                            }
                            else
                            {
                                printf("L'ID entré ne correspond pas à celui de l'étudiant connecté.\n");
                            }
                        }

                        break;
                    case 2:
                        puts("Nombre de minutes d'abscence");
                        break;
                    case 3:
                        puts("Mes Messages (0)");
                        break;
                    case 4:
                        puts("Au revoir!");
                        connecte = 0;
                        viderTamponEntree();
                        break;
                    default:
                        puts("Choix invalide");
                        break;
                    }

                } while (choix != 4);
            }
        }

    } while (1);
    reinitialiserPresenceEtudiants("etudiants.txt");
    return 0;
}
