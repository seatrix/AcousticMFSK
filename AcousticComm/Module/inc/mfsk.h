/*
 * common.h
 *
 *  Created on: 2019��1��19��
 *      Author: JingX
 *
 *  ��ͷ�ļ���Ҫ���FSK�źŽ����صĶ���
 *  �뺯������
 */
#ifndef mfsk_H_
#define mfsk_H_

#include <stdio.h>
#include "common.h"

#define SAMPLE_PER_SYMBLE 255
#define Quad              4
#define SYMBOL_NUM        8
#define SAMPLE_RATE       100000
#define BASE_BAND_FREQ0   20000
#define BASE_BAND_FREQ1   22000
#define BASE_BAND_FREQ2   24000
#define BASE_BAND_FREQ3   26000

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

void Normalize(float *ptrData, int len);
void genNonCoherentDemodWave(int freq, int timelen, NCDW *ptrData);
float SquareLawDetection(float * data,const NCDW * decodesig);
void bin2hex(const int * ptrData, char *ptrHex, int ArraySize);
void showData(const char* ptrData, int ArraySize);

#endif
