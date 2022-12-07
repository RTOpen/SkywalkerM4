#ifndef __TONES_H
#define __TONES_H

/**
  * @brief   Frequency list of musical notes
  */

/*octave 0*/
#define C0  16
#define CS0 17
#define D0  18
#define DS0 19
#define E0  21
#define F0  22
#define FS0 23
#define G0  24
#define GS0 26
#define A0  27
#define AS0 29
#define B0  31

#define C1  33
#define CS1 35
#define D1  37
#define DS1 39
#define E1  41
#define F1  44
#define FS1 46
#define G1  49
#define GS1 52
#define A1  55
#define AS1 58
#define B1  62

#define C2  65
#define CS2 69
#define D2  73
#define DS2 78
#define E2  82
#define F2  87
#define FS2 93
#define G2  98
#define GS2 104
#define A2  110
#define AS2 117
#define B2  123

#define C3  131
#define CS3 139
#define D3  147
#define DS3 156
#define E3  165
#define F3  175
#define FS3 185
#define G3  196
#define GS3 208
#define A3  220
#define AS3 233
#define B3  247

#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494

#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988

#define C6  1047
#define CS6 1109
#define D6  1175
#define DS6 1245
#define E6  1319
#define F6  1397
#define FS6 1480
#define G6  1568
#define GS6 1661
#define A6  1760
#define AS6 1865
#define B6  1976

#define C7  2093
#define CS7 2217
#define D7  2349
#define DS7 2489
#define E7  2637
#define F7  2794
#define FS7 2960
#define G7  3136
#define GS7 3322
#define A7  3520
#define AS7 3729
#define B7  3951

#define C8  4186
#define CS8 4435
#define D8  4699
#define DS8 4978
#define E8  5274
#define F8  5588
#define FS8 5920
#define G8  6272
#define GS8 6645
#define A8  7040
#define AS8 7459
#define B8  7902

#define C9  8372
#define CS9 8870
#define D9  9397
#define DS9 9956
#define E9  10548
#define F9  11175
#define FS9 11840
#define G9  12544
#define GS9 13290
#define A9  14080
#define AS9 14917
#define B9  15804


// DURATION OF THE NOTES 
#define BPM 120 //  you can change this value changing all the others
// I think tecnically BPM supposed to be 104, but original 120
// from instructable sounds good.  180 sounds speedy, 240 sounds mario, 300 sounds
// middle eastern, 400 middle eastern techno pop?
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4 
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

typedef struct{
    uint16_t note;
     /*	8	4	2	1	1/2		1/4		1/8		1/16	1/32	1/64	1/128	1/256	*/
    uint16_t duration;  
}tone_t;


#endif /* __TONES_H */
