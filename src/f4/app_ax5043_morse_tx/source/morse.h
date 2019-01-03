
#ifndef _MORSE_H_
#define _MORSE_H_


#define SPACE " "
uint16_t ditLength;

#define LETTER_SPACE_MS (ditLength*3)
#define WORD_SPACE_MS (ditLength*7)
#define DIT_MS (ditLength)
#define DASH_MS (ditLength*3)



void SendDot(void);
void SendDash(void);
void LetterSleep(void);
void WordSleep(void);
void ElementSleep(void);
void SetWpm(int wpm);
void SetMessage(char *message);
void SendMessage(void);

#endif
