/*
 *      mesure.c
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "mesure.h"
#include "fir.h"
#include "iir.h"
#include "fichiers.h"

//Fonction de test pour mesure qui va envoyer les lignes du fichiers une à une à la fonction mesure
oxy mesureTest(char* filename){
    oxy myOxy;
    absorp	myAbsorp;
    int etat = 0;
    FILE* fp = initFichier(filename);

    //INITIALISATION DES PARAMETRES
    param_mesure myMES;
    init_mesure(&myMES);

    while(etat!=EOF){       //On lit le fichier jusqu'a la fin
        absorp temp = myAbsorp;
        myAbsorp = lireFichier(fp,&etat);
        if(etat != -1){
            myOxy = mesure(myAbsorp, &myMES);
        }else{
            myAbsorp = temp;
        }
    }
    finFichier(fp);
    return myOxy;
}

//Initialisation des differents parametres pour la fonction mesure
void init_mesure(param_mesure* paramMesure){
    //init pour le pouls
    paramMesure->rithmeSave = 0;
    paramMesure->rithmeCounter = 0;

    // init buffer
    paramMesure->buffer_acir = 1;
    paramMesure->bufferPouls2 = 0;

    int i;
    for(i = 0 ; i < 2 ; i++){
        paramMesure->buffer_pouls[i] = 0;
    }

    for(i = 0 ; i < 3000 ; i++ ){
        paramMesure->buffer[i].acr = 0;
        paramMesure->buffer[i].acir = 0;
        paramMesure->buffer[i].dcr = 0;
        paramMesure->buffer[i].dcir = 0;
    }
}

//Mise à jours du buffer
void updatebuffer_tot(absorp x, param_mesure* paramMesures){
    int i;
    for(i = 2999; i >= 0 ; i--){
        if(i == 0){         //On sauvegarde que les 3000 dernieres valeurs de acr,acir...
            paramMesures->buffer[i].acr = x.acr;
            paramMesures->buffer[i].acir = x.acir;
            paramMesures->buffer[i].dcr = x.dcr;
            paramMesures->buffer[i].dcir = x.dcir;
        }else{
            paramMesures->buffer[i].acr = paramMesures->buffer[i-1].acr;
            paramMesures->buffer[i].acir = paramMesures->buffer[i-1].acir;
            paramMesures->buffer[i].dcr = paramMesures->buffer[i-1].dcr;
            paramMesures->buffer[i].dcir = paramMesures->buffer[i-1].dcir;
        }
    }
}

//Mise à jours du buffer pour la fonction mesure
void updatebuffer_mesure(float newVal, param_mesure* paramMesures){
    int i;
    for(i = 1; i >= 0 ; i--){
        if(i == 0){
            paramMesures->buffer_pouls[i] = (int)newVal;
        }else{
            paramMesures->buffer_pouls[i] = paramMesures->buffer_pouls[i - 1];
        }
    }
}

//Calcule de la moyenne du pouls à partir des 3 dernieres valeurs que nous avons trouvé
int moyenne_pouls(param_mesure* paramMesures){
    int n=0;
    int k=0;
    int i;
    int somme=0;
    for(i = 0 ; i < 2 ; i++){       //On fait juste une moyenne classique sans prendre les valeurs == 0 (qu'on drop a cause l'init)
        if(paramMesures->buffer_pouls[i] != 0){
            somme += paramMesures->buffer_pouls[i];
            n++;
        }else{
            k++;
        }
    }
    if(k==2){   //gestion d'un cas limite ou on aurait pu diviser par 0
        n = 1;
    }
    //printf("moyenne = %d\n",somme/n);
    return somme/n;
}

//Calcule du PTP à partir des 3000 dernieres valeurs
void calcule_PtP(float* tab,param_mesure* paramMesure){
    /*
     * PtP[0 : max ACIR
     * PtP[1 : min ACIR
     * PtP[2 : max ACR
     * PtP[3 : min ACR
     */
    int i;
    for(i = 0; i < 3000 ; i++){         //On calcule les plus grande et plus petites valeurs de notre buffer pour ressortir le sp02
        if(i==0){
            tab[0] = paramMesure->buffer[i].acir;
            tab[1] = paramMesure->buffer[i].acir;

            tab[2] = paramMesure->buffer[i].acr;
            tab[3] = paramMesure->buffer[i].acr;
        }else{
            if(paramMesure->buffer[i].acir > tab[0]){
                tab[0] = paramMesure->buffer[i].acir;
            }
            if(paramMesure->buffer[i].acir < tab[1]){
                tab[1] = paramMesure->buffer[i].acir;
            }
            if(paramMesure->buffer[i].acr > tab[2]){
                tab[2] = paramMesure->buffer[i].acr;
            }
            if(paramMesure->buffer[i].acr < tab[3]){
                tab[3] = paramMesure->buffer[i].acr;
            }
        }
    }
}

//Fonction principale de mesure, va revoyé la classe oxy avec les bon pouls et sp02 dedans
oxy mesure(absorp absorp1, param_mesure* paramMesure){
    oxy myOxy;

    updatebuffer_tot(absorp1, paramMesure);
    float PtP[4];
    /*
     * PtP[0 : max ACIR
     * PtP[1 : min ACIR
     * PtP[2 : max ACR
     * PtP[3 : min ACR
     * on a un tableau qui stock ces valeurs "au file de l'eau"
     */

    calcule_PtP((float *) &PtP, paramMesure);
    //Calcul de SP02
    float rsIR1 = (PtP[2] - PtP[3]) / absorp1.dcr;      //On calcule la partie haute et la partie basse de la division
    float rsIR2 = (PtP[0] - PtP[1]) / absorp1.dcir;
    if (rsIR2 != 0) {       //On verifie qu'on ne va pas faire une division par 0
        int sp02;
        float rsIR = rsIR1 / rsIR2;
        //Source des coéficiant : https://www.editions-petiteelisabeth.fr/calculs_fonctions_affines_1.php
        if (rsIR < 1) {
            sp02 = -25 * rsIR + 110;
        } else {
            sp02 = -35 * rsIR + 120;
        }
        //printf("Le pourcentage de SP02 est : %d \n", sp02);
        myOxy.spo2 = sp02;
    }
    //Calcule de pouls

    if ((absorp1.acir > 0 && paramMesure->buffer_acir < 0) || (absorp1.acir < 0 && paramMesure->buffer_acir > 0)) {
        //printf("acir: %f\n acr : %f\n", absorp1.acir, absorp1.acr);
        //update buffer acir
        paramMesure->buffer_acir = absorp1.acir;
        if (paramMesure->rithmeSave == 0) {//1er point de calcule du rythme cardiaque
            paramMesure->rithmeCounter = 0;
            paramMesure->rithmeSave++;      //Rithm Save va sauvgarder combien de fois nous sommes passé à 0, au 2nd passage il calculera le rythme cardiaque
        } else if (paramMesure->rithmeSave == 1) {
            paramMesure->rithmeSave++;
        } else if (paramMesure->rithmeSave == 2) {
            paramMesure->rithmeSave = 0;
            //Calcule du rythme cardiaque
            float temp = (float) 1 / (2 * (float) paramMesure->rithmeCounter / 1000) * 60;

            //printf("param mesure : %f\n", (float) paramMesure->rithmeCounter);
            //printf("le rythme cardiaque est : %f \n", temp);
            myOxy.pouls = moyenne_pouls(paramMesure);
            paramMesure->bufferPouls2 = myOxy.pouls;
            updatebuffer_mesure(temp,paramMesure);
            //printf("%d\n",myOxy.pouls);
        }
    }
    myOxy.pouls = paramMesure->bufferPouls2;
    paramMesure->rithmeCounter++; //On incrémente toute les 2 ms la valeur de rythm save pour pouvoir s'en reservir dans le calcul
    return myOxy;
}