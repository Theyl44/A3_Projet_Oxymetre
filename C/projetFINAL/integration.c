/*
 *      integration.c
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "define.h"
#include "fir.h"
#include "iir.h"
#include "mesure.h"
#include "fichiers.h"
#include "lecture.h"
#include "affichage.h"

//Fonction d'intégration qui va permettre d'executé toutes les autres fonctions à la suite
void integrationTest(char* filename){
    int etat = 0 ;
    absorp myAbsorp;
    oxy myOxy;
    FILE* fp = initFichier(filename);

    //INIT des parametres des filtres
    param_fir myFIR;
    initfir(&myFIR);

    param_iir myIRR;
    initiir(&myIRR);

    param_mesure myMES;
    init_mesure(&myMES);

    while(etat!=EOF){   //Tant qu'on a pas fini le fichié d'entré on execute toutes les fonction "au fil de l'eau"
        myAbsorp = lecture(fp,&etat);
        if(etat != EOF){
            myAbsorp = fir(myAbsorp, &myFIR);
            myAbsorp = irr(myAbsorp, &myIRR);
            myOxy = mesure(myAbsorp,&myMES);
            affichage(myOxy);
        }
    }
    finFichier(fp);
}