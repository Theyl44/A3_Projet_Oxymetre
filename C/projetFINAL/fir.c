/*
 *      fir.c
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "fir.h"
#include "fichiers.h"

absorp firTest(char* filename){
    absorp myAbsorp;
    int etat = 0;
    FILE* fp = initFichier(filename);

    //INITIALISATION DES PARAMETRES : un tableau buffer qui recap les 50 dernieres valeurs de notre fichier & un tableau de constante
    param_fir myFIR;
    initfir(&myFIR);
    while(etat!=EOF){
        /*
         * A cause de la ligne en dessus on a un problème pour la derniere itération
         * du ficher donc on va memoriser la derniere valeur et lire notre fichier
         * si nous arrivons dans les "else" on sera dans ce cas sinon on va passer notre ligne dans le filtre normalement
         * sinon on restaure la valeur de myabsorp avant de le return
         */
        absorp temp = myAbsorp;
        myAbsorp = lireFichier(fp,&etat);
        if(etat != -1){
            myAbsorp = fir(myAbsorp, &myFIR);
            //printf("acr = %f, dcr = %f, acir =  %f, dcir = %f\n", myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir);
        }else{
            myAbsorp = temp;
        }
    }
    finFichier(fp);
    return myAbsorp;
}
void initfir(param_fir* myFIR){
    //c'est les valeurs qui ont été donné dans l'ENT
    //on init nos valeurs avec ca comme c'est des coef on n'y touche plus (on pourrait les mettre en variable global)
    float FIR_TAPS[51]={
            1.4774946e-019,
            1.6465231e-004,
            3.8503956e-004,
            7.0848037e-004,
            1.1840522e-003,
            1.8598621e-003,
            2.7802151e-003,
            3.9828263e-003,
            5.4962252e-003,
            7.3374938e-003,
            9.5104679e-003,
            1.2004510e-002,
            1.4793934e-002,
            1.7838135e-002,
            2.1082435e-002,
            2.4459630e-002,
            2.7892178e-002,
            3.1294938e-002,
            3.4578348e-002,
            3.7651889e-002,
            4.0427695e-002,
            4.2824111e-002,
            4.4769071e-002,
            4.6203098e-002,
            4.7081811e-002,
            4.7377805e-002,
            4.7081811e-002,
            4.6203098e-002,
            4.4769071e-002,
            4.2824111e-002,
            4.0427695e-002,
            3.7651889e-002,
            3.4578348e-002,
            3.1294938e-002,
            2.7892178e-002,
            2.4459630e-002,
            2.1082435e-002,
            1.7838135e-002,
            1.4793934e-002,
            1.2004510e-002,
            9.5104679e-003,
            7.3374938e-003,
            5.4962252e-003,
            3.9828263e-003,
            2.7802151e-003,
            1.8598621e-003,
            1.1840522e-003,
            7.0848037e-004,
            3.8503956e-004,
            1.6465231e-004,
            1.4774946e-019
    };

    int i;
    //par defaut on met toutes les valeurs du buffer a 0
    for(i = 0 ; i < 51 ; i ++){
        myFIR->buffer[i].acr = 0 ;
        myFIR->buffer[i].acir = 0 ;
        myFIR->buffer[i].dcir = 0 ;
        myFIR->buffer[i].dcr = 0 ;

        //ici on init les valeurs de nos coef avec les valeurs qu'on nous a donné dans l'ENT
        myFIR->FIR_TAPS[i] = FIR_TAPS[i];
    }
}


/*
 * en gros on va decaler toutes nos valeurs vers la gauche comme une file. on jete la derniere valeur et on
 * fait rentrer la valeur la plus recente. Comme ca on conserve au file de l'eau les 50 derniere valeur de notre fichier
 */
void updatebuffer(absorp x,param_fir* myFIR){
    int i;
    for(i = 50; i >= 0  ; i--){
        if(i == 0){
            myFIR->buffer[i].acr = x.acr;
            myFIR->buffer[i].acir = x.acir;
            myFIR->buffer[i].dcir = x.dcir;
            myFIR->buffer[i].dcr = x.dcr;

        }else{
            myFIR->buffer[i].acr = myFIR->buffer[i-1].acr ;
            myFIR->buffer[i].acir = myFIR->buffer[i-1].acir;
            myFIR->buffer[i].dcir = myFIR->buffer[i-1].dcir;
            myFIR->buffer[i].dcr = myFIR->buffer[i-1].dcr;
        }
    }
}
absorp fir(absorp x, param_fir* myFIR){
    /*
     * Comme nous avons besoin des 50 valeurs precedentes pour determiner notre nvx absorp
     * les 50 premieres valeurs ne sont pas a prendre en compte comme il faut mettre des choses dans notre buffer
     */
    updatebuffer(x, myFIR);//on actualise le buffer pour qu'il est toujours les 51 dernieres valeurs
    //faut stocker les valeurs avant de faire le filtre sinon ca marche pas gogole

    //comme on regardera pas les 50 premieres valeur on met a 0 mais ca ne change rien elle ne seront pas interessante dans tous les cas comme on a besoin d'un buffer de 50 avant
    absorp y;
    y.acr = 0 ;
    y.acir = 0 ;
    y.dcr = x.dcr;
    y.dcir = x.dcir;
    //printf("debug : acr = %f, dcr = %f, acir =  %f, dcir = %f\n", y.acr,y.dcr,y.acir,y.dcir);
    int i;
    for(i = 0 ; i <= 50 ; i++){
        //ici on applique juste la formule du prof pour faire un filtre passe bas avec les outils qu'on a vu plus haut dans le code
        y.acr += (myFIR->FIR_TAPS[i] * myFIR->buffer[50 - i].acr);
        y.acir += (myFIR->FIR_TAPS[i] * myFIR->buffer[50 - i].acir);
    }
    return y;
}