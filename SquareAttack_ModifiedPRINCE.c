#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_Round 3

typedef unsigned char word8;

//S-box
const unsigned char sBox[16] = {
  0xb, 0xf, 0x3, 0x2, 0xa, 0xc, 0x9, 0x1, 0x6, 0x7, 0x8, 0x0, 0xe, 0x5, 0xd, 0x4
};

//Inverse S-box
const unsigned char inv_s[16] = {
  0xb, 0x7, 0x3, 0x2, 0xf, 0xd, 0x8, 0x9, 0xa, 0x6, 0x4, 0x0, 0x5, 0xe, 0xc, 0x1
};

const unsigned char alpha[16] = {
  0xc, 0x0, 0xa, 0xc, 0x2, 0x9, 0xb, 0x7, 0xc, 0x9, 0x7, 0xc, 0x5, 0x0, 0xd, 0xd
};

const unsigned char constants[6][16] = {
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x1, 0x3, 0x1, 0x9, 0x8, 0xa, 0x2, 0xe, 0x0, 0x3, 0x7, 0x0, 0x7, 0x3, 0x4, 0x4,
  0xa, 0x4, 0x0, 0x9, 0x3, 0x8, 0x2, 0x2, 0x2, 0x9, 0x9, 0xf, 0x3, 0x1, 0xd, 0x0,
  0x0, 0x8, 0x2, 0xe, 0xf, 0xa, 0x9, 0x8, 0xe, 0xc, 0x4, 0xe, 0x6, 0xc, 0x8, 0x9,
  0x4, 0x5, 0x2, 0x8, 0x2, 0x1, 0xe, 0x6, 0x3, 0x8, 0xd, 0x0, 0x1, 0x3, 0x7, 0x7,
  0xb, 0xe, 0x5, 0x4, 0x6, 0x6, 0xc, 0xf, 0x3, 0x4, 0xe, 0x9, 0x0, 0xc, 0x6, 0xc
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*GENERATE RANDOM NIBBLE*/

int randomInRange(int min, int max){

  int range = max - min + 1;
  return (min + rand() % range);

}

word8 randomNibble(){
  return (word8) randomInRange(0, 15);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*INIZIALIZE A VECTOR*/

void inizialize(word8 *p, unsigned char initialMessage[]){

  int i;

  for(i=0; i<16; i++){
    *(p+i) = initialMessage[i];
  }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*ADD ROUND KEY OPERATIONT*/

void addRoundKey(word8 *p, word8 key[]){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= key[i];
  }

}

void addRoundKeyMod(word8 *p, word8 key[]){

  int i;

  for(i=0; i<4; i++){
    *(p+i) ^= key[i];
  }

}

/*ADD ROUND CONSTANT*/

void addCostante(word8 *p, int round){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= constants[round][i];
  }

}

/*ADD ROUND CONSTANT and ALPHA*/

void addCostante2(word8 *p, int round){

  int i;

  for(i=0; i<16; i++){
    *(p+i) ^= constants[round][i];
    *(p+i) ^= alpha[i];
  }

}

void addCostante2Mod(word8 *p, int round){

  int i;

  for(i=0; i<4; i++){
    *(p+i) ^= constants[round][i];
    *(p+i) ^= alpha[i];
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*PRINT THE VALUES*/

void printtt(word8 file[]){

    int i;

    for(i=0; i<16; i++)
    {
    printf("0x%x", file[i]);
    if(i<15)
        printf(", ");
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Byte sub transformation with S-box*/

word8 byteTransformation(word8 byte){

  return sBox[byte];

}

/*Inverse byte sub transformation with Inverse S-box*/

word8 inverseByteTransformation(word8 byte){

  return inv_s[byte];

}

/*S-BOX TRANSFORMATION*/

void byteSubTransformation(word8 *puntatore){

  int i;

  for(i=0; i<16; i++){
     *(puntatore+i)=byteTransformation(*(puntatore+i));
  }
}

void byteSubTransformationMod(word8 *puntatore){

  int i;

  for(i=0; i<4; i++){
     *(puntatore+i)=byteTransformation(*(puntatore+i));
  }
}

/*INVERSE S-BOX TRANSFORMATION*/

void inverseByteSubTransformation(word8 *puntatore){

  int i;

  for(i=0; i<16; i++){
     *(puntatore+i)=inverseByteTransformation(*(puntatore+i));
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*GENERATION OF THE FINAL KEY OF THE FX CONSTRUCTION*/

void generazioneKey(word8 k[], word8 *k2)
{
    int i;
    word8 temp, temp2;

    temp = 0;
    temp2 = (k[1] & 0x08) >> 3;
    k2[0] = ((k[0] &0xe)>> 1) ^ ((k[0] & 0x07) << 1) ^ temp ^ temp2;

    for(i=1; i<15; i++)
    {
        temp = (k[i-1] & 0x01) << 3;
        temp2 = (k[i+1] & 0x08) >> 3;
        k2[i] = ((k[i]&0xe) >> 1) ^ ((k[i] & 0x07) << 1) ^ temp ^ temp2;
    }

    temp = (k[14] & 0x01) << 3;
    temp2 = (k[0] & 0x08) >> 3;
    k2[15] = ((k[15]&0xe) >> 1) ^ ((k[15] & 0x07) << 1) ^ temp ^ temp2;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*SHIFT ROWS OPERATION*/

void shiftRows(word8 *p){

    int i;
    word8 vett[16];

    for(i=0; i<16; i++)
        vett[i] = *(p+i);

    *p = vett[0];
    *(p+1) = vett[5];
    *(p+2) = vett[10];
    *(p+3) = vett[15];
    *(p+4) = vett[4];
    *(p+5) = vett[9];
    *(p+6) = vett[14];
    *(p+7) = vett[3];
    *(p+8) = vett[8];
    *(p+9) = vett[13];
    *(p+10) = vett[2];
    *(p+11) = vett[7];
    *(p+12) = vett[12];
    *(p+13) = vett[1];
    *(p+14) = vett[6];
    *(p+15) = vett[11];

}

/*INVERSE SHIFT ROWS OPERATION*/

void inverseShiftRows(word8 *p){

  int i;
    word8 vett[16];

    for(i=0; i<16; i++)
        vett[i] = *(p+i);

    *p = vett[0];
    *(p+1) = vett[13];
    *(p+2) = vett[10];
    *(p+3) = vett[7];
    *(p+4) = vett[4];
    *(p+5) = vett[1];
    *(p+6) = vett[14];
    *(p+7) = vett[11];
    *(p+8) = vett[8];
    *(p+9) = vett[5];
    *(p+10) = vett[2];
    *(p+11) = vett[15];
    *(p+12) = vett[12];
    *(p+13) = vett[9];
    *(p+14) = vett[6];
    *(p+15) = vett[3];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*MIX COLUMN OPERATION*/

void mixColumn0(word8 *p)
{
    word8 x[4];
    int i;

    for(i=0; i<4;i++)
        x[i]=*(p+i);

    *p = ((x[1]^x[2]^x[3])&0x08) ^ ((x[3]^x[2]^x[0])&0x04) ^ ((x[1]^x[0]^x[3])&0x2) ^ ((x[1]^x[2]^x[0])&0x1);
    *(p+1) = ((x[0]^x[2]^x[1])&0x08) ^ ((x[1]^x[2]^x[3])&0x04) ^ ((x[3]^x[0]^x[2])&0x2) ^ ((x[1]^x[0]^x[3])&0x1);
    *(p+2) = ((x[0]^x[1]^x[3])&0x08) ^ ((x[0]^x[2]^x[1])&0x04) ^ ((x[1]^x[3]^x[2])&0x2) ^ ((x[3]^x[0]^x[2])&0x1);
    *(p+3) = ((x[0]^x[2]^x[3])&0x08) ^ ((x[1]^x[0]^x[3])&0x04) ^ ((x[1]^x[0]^x[2])&0x2) ^ ((x[1]^x[2]^x[3])&0x1);
}

void mixColumn1(word8 *p)
{
    word8 x[4];
    int i;

    for(i=0; i<4;i++)
        x[i]=*(p+i);

    *p = ((x[0]^x[2]^x[1])&0x08) ^ ((x[1]^x[2]^x[3])&0x04) ^ ((x[3]^x[0]^x[2])&0x2) ^ ((x[1]^x[0]^x[3])&0x1);
    *(p+1) = ((x[0]^x[1]^x[3])&0x08) ^ ((x[0]^x[2]^x[1])&0x04) ^ ((x[1]^x[3]^x[2])&0x2) ^ ((x[3]^x[0]^x[2])&0x1);
    *(p+2) = ((x[0]^x[2]^x[3])&0x08) ^ ((x[1]^x[0]^x[3])&0x04) ^ ((x[1]^x[0]^x[2])&0x2) ^ ((x[1]^x[2]^x[3])&0x1);
    *(p+3) = ((x[1]^x[2]^x[3])&0x08) ^ ((x[3]^x[2]^x[0])&0x04) ^ ((x[1]^x[0]^x[3])&0x2) ^ ((x[1]^x[2]^x[0])&0x1);
}

void mixColumn(word8 *stato)
{
    mixColumn0(stato);
    mixColumn1(stato+4);
    mixColumn1(stato+8);
    mixColumn0(stato+12);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*COMPLETE ENCRYPTION -> MODIFIED VERSION OF PRINCE (i.e. SR computed before/after the M' in forward/backward rounds)*/

void encryption(word8 initialMessage[], word8 initialKey[], word8 initialKey2[], word8 *ciphertext){

  int i, j, differe;

  unsigned char state[16], keyy22[16];

  inizialize(&(state[0]), initialMessage);
  generazioneKey(initialKey, &(keyy22[0]));

  //Prince
  addRoundKey(&(state[0]), initialKey);

  //PrinceCore
  addRoundKey(&(state[0]), initialKey2);

  for(i=1;i<N_Round;i++)
  {
      byteSubTransformation(&(state[0]));
      shiftRows(&(state[0]));
      mixColumn(&(state[0]));
      addRoundKey(&(state[0]), initialKey2);
      addCostante(&(state[0]), i);
  }

  //Middle Round
  byteSubTransformation(&(state[0]));
  mixColumn(&(state[0]));
  inverseByteSubTransformation(&(state[0]));

  for(i=N_Round;i<2 * N_Round - 1;i++)
  {
      differe = 11 - i;
      addRoundKey(&(state[0]), initialKey2);
      addCostante2(&(state[0]), differe);
      mixColumn(&(state[0]));
      inverseShiftRows(&(state[0]));
      inverseByteSubTransformation(&(state[0]));
  }
  byteSubTransformation(&(state[0]));

  addRoundKey(&(state[0]), initialKey2);
  addCostante2(&(state[0]), 0);
  addRoundKey(&(state[0]), keyy22);

  //salvo messaggio cifrato
  for(i=0; i<16; i++){
     *(ciphertext+i)=state[i];
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**Check of the balanced property on 5.5 rounds of the Modified Version of PRINCE.*/

void squareAttack(word8 initialKey[], word8 initialKey2[])
{
    unsigned char message[4096][16], cifrato[4096][16], t1[16], t2[16], somma[16];

    int i, j, k, l, n;

    for(i = 0; i<16; i++)
    {
        message[0][i] = randomNibble();
        for(j = 1; j<16; j++)
        {
            message[j][i] = message[0][i];

        }

    }

    for(i = 0; i<16; i++)
    {
        message[i][0] = (unsigned char) i;
    }

    for(i = 0; i<16; i++)
    {
        for(j = 0; j<16; j++)
        {
            t1[j] = message[i][j];
        }

        encryption(t1, initialKey, initialKey2, &(t2[0]));

        for(j = 0; j<16; j++)
        {
            cifrato[i][j] = t2[j];
        }

    }

    for(i=0;i<16;i++)
    {
        somma[i] = 0;

    }

    for(i=0;i<16;i++)
    {

        for(j=0;j<4096;j++)
        {
            somma[i] ^= cifrato[j][i];
        }
    }

    for(i=0;i<16;i++)
    {
        printf("Nibble in position: %i - Sum: %x\n", i, somma[i]);

    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Check of the balanced property on 5.5 rounds (the 6 middle rounds) of the Modified Version of PRINCE, in which
ShiftRows operation is computed before (after) the MixLayer in the forward rounds (backwards).
See Sect. 4 for a complete description of the attack. */

int main(){

  //plaintext
  const unsigned char initialMessage[16] = {
    0x9, 0x0, 0xb, 0x7, 0x0, 0xf, 0x4, 0xf, 0xf, 0xe, 0x7, 0x7, 0x9, 0x2, 0x6, 0x8
  };

  //key of FXConstruction k_0
  const unsigned char initialKey[16] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
  };

  //key of PRINCE Core k_1
  const unsigned char initialKey2[16] = {
    0xc, 0x0, 0xa, 0xc, 0x2, 0x9, 0xb, 0x7, 0xc, 0x9, 0x7, 0xc, 0x5, 0x0, 0xd, 0xd
  };

  /*word8 ciphertext[16];

  encryption(initialMessage, initialKey, initialKey2, &(ciphertext[0]));

  printf("Plaintext:\n");
  printtt(initialMessage);
  printf("\n\n");

  printf("Key k_0:\n");
  printtt(initialKey);
  printf("\n\n");

  printf("Key of PRINCE_core k_1:\n");
  printtt(initialKey2);
  printf("\n\n");

  printf("Ciphertext:\n");
  printtt(ciphertext);
  printf("\n");*/

  printf("SQUARE ATTACK on the MODIFIED PRINCE (see Sect. 4 for details).\n");

  printf("We consider a modified version of PRINCE, in which the ShiftRows and the MixLayer are exchanged in positions.");
  printf("We show that the balance property holds for 5.5 rounds.\n\n");

  printf("A single active input nibble. Result:\n");

  squareAttack(initialKey, initialKey2);

  return 0;

}
