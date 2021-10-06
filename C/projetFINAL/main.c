/*
 *      main.c
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include <stdio.h>
#include "lecture.h"
#include "fichiers.h"
#include "affichage.h"
#include "mesure.h"
#include "fir.h"
#include "iir.h"
#include "integration.h"

int main() {
    ////test fonctions
    //integrationTest("src/record1_bin.dat");//pour demo avec debian
    integrationTest("../src/record1_bin.dat");//pour debugg clion
    //firTest("../src/record1.dat");
    //iirTest("../src/record1_fir.dat");
    //mesureTest("../src/record1_iir.dat");


    ////test lecture
    //int fileState = 0;
    //FILE* file = initFichier("../src/record1_bin.dat");
    //for(int i = 1; i < 6; i++){
    //    printf("\n itération %d \n", i);
    //    lecture(file, &fileState);
    //    //fclose(file);
    //}
    //fclose(file);
    ////test affichage
    //oxy myoxy;
    //myoxy.pouls = 100;
    //myoxy.spo2 = 50;
//
    //for(int i = 0; i<100; i++){
    //    affichage(myoxy);
    //    myoxy.pouls ++;
    //    myoxy.spo2 ++;
    //    sleep(1);
    //}


    return 0;
}
