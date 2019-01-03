/*
The code here refers to logic and snippets from Adam Parker - KD5OOL
*/

#include <string.h>
#include <ctype.h>


#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "morse.h"


#define MAX_MESSAGE_SIZE 512
char beaconMessage[MAX_MESSAGE_SIZE];

static const char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};

static const char *num[] = {
    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9
};

void SendDot(void)
{

    chThdSleepMicroseconds(DIT_MS);

}

void SendDash(void)
{

    chThdSleepMicroseconds(DASH_MS);

}

void LetterSleep(void)
{
    chThdSleepMicroseconds(LETTER_SPACE_MS);
}

void WordSleep(void)
{
    chThdSleepMicroseconds(WORD_SPACE_MS);
}

void ElementSleep(void)
{
    chThdSleepMicroseconds(DIT_MS);
}

//sets ditLength in ms
void SetWpm(int wpm)
{
    ditLength = 1200/wpm;
}

void SetMessage(char *message)
{
    memset(beaconMessage, 0, MAX_MESSAGE_SIZE);
    int len = strlen(message);
    if (strlen(message) >= MAX_MESSAGE_SIZE)
        len = MAX_MESSAGE_SIZE-1;
    memcpy(beaconMessage, message, len);
}

const char *AsciiToMorse(char letter)
{
    letter = tolower(letter);

    if (isalpha(letter))
    {
        return alpha[letter-'a'];
    }
    else if (isdigit(letter))
    {
        return num[letter-'0'];
    }

    return SPACE;
}

void SendMessage(void)
{
    int element;
    int index = 0;
    const char *morse;
    while (beaconMessage[index] != '\0')
    {
        morse = AsciiToMorse(beaconMessage[index]);

        element = 0;
        while (morse[element] != '\0')
        {
            switch(morse[element])
            {
            case '-':
                SendDash();
                break;
            case '.':
                SendDot();
            }

            if (morse[element] == ' ')
            {
                WordSleep();
            }
            else if (morse[element+1] != '\0')
            {
                ElementSleep();
            }
            else if (morse[element+1] == '\0')
            {
                LetterSleep();
            }
            element++;
        }

        index++;
    }
}

