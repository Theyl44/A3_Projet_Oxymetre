/*
 *      lecture.c
 *      by Théo LOPEZ & Tristan ROUX
 *      CIR3/CIR3
 *      9 avril 2021
 */

#include <string.h>
#include "lecture.h"

//Fonction de lecture du fichier d'entré binaire
absorp lecture(FILE* file_pf, int* file_state){

	absorp myAbsorp;
    char nextLine[25];  //Servira à passer la ligne


    myAbsorp.acir =0;
    myAbsorp.acr = 0;
    myAbsorp.dcr = 0;
    myAbsorp.dcir = 0;

    float acr = -1;        //Variables temporaires des différents trucs qu'on veut mettre dans la struct
    float dcr = -1;
    float acir = -1;
    float dcir = -1;

    if(file_pf != NULL){        //On regarde si on peut acceder au fichier
        if(feof(file_pf)){      //On regarde si on est à la fin du fichier
            *file_state = EOF;
        } else {
            int test = fscanf(file_pf,"%f,%f,%f,%f\n",&acr,&dcr,&acir,&dcir);   //On prend la prochaine ligne
                while(test != 4){           //On test cette ligne jusqu'a ce qu'elle nous conviennen et on drop les anciennes
                    fgets(nextLine, 25, file_pf);       //On utilise fgets pour passer à la ligne suivante
                    printf("Ligne fausse\n");
                    test = fscanf(file_pf,"%f,%f,%f,%f\n",&acr,&dcr,&acir,&dcir);
                }

                myAbsorp.acir = acir - 2048;    //On recadre les valeurs acir et acr autour de 0 (valeurs comprisent entre 0 et 4096 de base)
                myAbsorp.acr = acr - 2048;
                myAbsorp.dcr = dcr;
                myAbsorp.dcir = dcir;
                //printf("ACR : %f \n", myAbsorp.acr);
                //printf("DCR : %f \n", myAbsorp.dcr);
                //printf("ACIR : %f \n", myAbsorp.acir);
                //printf("DCIR : %f  \n", myAbsorp.dcir);
                *file_state = (int ) ftell(file_pf);    //On met file state à l'endroit ou on en est dans le fichier
            }
    }else {
        printf("Le fichier est absent / l'ouverture a buggé"); //On arrive pas à ouvrir le fichier :(
        *file_state = EOF;
    }
	return myAbsorp; //On return la struct avec les bonnes valeurs dedant
}