/*
 *      affichage.c
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "affichage.h"

//Fonction qui va permettre l'affichage des données sur l'interface java
void affichage(oxy myOxy){

    if(access(".verrouData", F_OK) == -1){      //On regarde si on peut acceder au fichier verrouData
        FILE *verrouData = fopen(".verrouData", "w");   //Si verrouData n'existe pas, on le crée
        fclose(verrouData);

        FILE *dataFile = fopen("Data.txt", "w");    //On ecrit les valeurs dans dataFile pour que l'interface puisse les afficher
        fprintf(dataFile, "%d\n%d", myOxy.spo2, myOxy.pouls);
        fclose(dataFile);

        remove(".verrouData");  //On delete verrouData pour que l'interface puisse lire
    }
}
