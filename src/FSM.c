/*!
 * \file       FSM.c
 * \author     Horak, Jurena
 * \date       2019.6
 * \brief      Implementation of function.h header file
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/* Includes ------------------------------------------------------------------*/
#include "FSM.h"

/* Private types -------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
/* Exported functions definitions --------------------------------------------*/

void FSM_RemoveNotes(FILE* input, FILE* output) {

    if(input == NULL || output == NULL)
    {
        return;
    }

    int znak;
    enum{start,slash,oneLine,multiLine,maybeEND,uvozovka,apostrophe,backSlash,backSlashApostrophe}stav = start;
    int inComment = 0; //flag to track whether inside a comment

    while((znak = fgetc(input)) != EOF) //read characters from input file
    {
        switch (stav)
        {
        case start:
            if (znak == '/') {
                stav = slash;
            }
            else{
                if (znak == '"')
                {
                    stav = uvozovka;
                }
                else if (znak == '\'')
                {
                    stav = apostrophe;
                }
                if (!inComment) { //output only if not inside a comment
                    fputc(znak, output);
                }
            }
            break;

        case slash:
            if (znak == '/') {
                stav = oneLine;
            }
            else if (znak == '*') {
                stav = multiLine;
                inComment = 1; //set flag to indicate inside a comment
            }else {
                stav = start;
                fputc('/', output);
                fputc(znak, output);
            }
            break;

        case oneLine:
            if (znak == '\n') {
                stav = start;
                inComment = 0; //reset flag when comment ends
                fputc(znak, output);
            }
            break;

        case multiLine:
            if (znak == '*') {
                stav = maybeEND;
            }
            break;

        case maybeEND:
            if (znak == '/') {
                stav = start;
                inComment = 0; //reset flag when comment ends
            }
            else {
                stav = multiLine;
            }
            break;

        case uvozovka:
            if (znak == '"') {
                stav = start;
                fputc(znak, output);
            } else if (znak == '\\') {
                stav = backSlash;
                fputc('\\', output);
            } else {
                fputc(znak, output);
            }
            break;

        case backSlash:
            stav = uvozovka;
            fputc(znak, output);
            break;

        case apostrophe:
            if (znak == '\\') {
                stav = backSlashApostrophe;
                fputc(znak, output);
            } else if (znak == '\'') {
                stav = start;
                fputc(znak, output);
            } else {
                fputc(znak, output);
            }
            break;

        case backSlashApostrophe:
            stav = apostrophe;
            fputc(znak, output);
            break;
        }
    }

}



/* Private function definitions ----------------------------------------------*/
