/*
 *      iir.c
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include "iir.h"
#include "fir.h"
#include "fichiers.h"

void initiir(param_iir* myIIR){
    myIIR->x_1.acr = 0;
    myIIR->x_1.acir = 0;
    myIIR->x_1.dcr = 0;
    myIIR->x_1.dcir =0;

    myIIR->y_1.acr = 0;
    myIIR->y_1.acir = 0;
    myIIR->y_1.dcr = 0;
    myIIR->y_1.dcir = 0;
}
absorp irr(absorp x, param_iir* myIRR){
    absorp y;
    //on filtre notre absorp avec le formule donnÃ©e dans les slides
    y.acr = x.acr - myIRR->x_1.acr + 0.992*myIRR->y_1.acr;
    y.acir = x.acir - myIRR->x_1.acir + 0.992*myIRR->y_1.acir;

    y.dcr = x.dcr;
    y.dcir = x.dcir;

    //update buffer
    myIRR->y_1.acr = y.acr;
    myIRR->y_1.acir = y.acir;

    myIRR->x_1.acr = x.acr;
    myIRR->x_1.acir = x.acir;

    return y;
}
absorp iirTest(char* filename){
    absorp	myAbsorp;
    int etat = 0;
    FILE* fp = initFichier(filename);

    param_iir myIRR;
    initiir(&myIRR);

    while(etat!=EOF){
        absorp temp = myAbsorp;
        myAbsorp = lireFichier(fp,&etat);
        if(etat != -1){
            myAbsorp = irr(myAbsorp, &myIRR);
            //printf("acr = %f, dcr = %f, acir =  %f, dcir = %f\n", myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir);
        } else{
            myAbsorp = temp;
        }
    }
    finFichier(fp);
    return myAbsorp;
}
