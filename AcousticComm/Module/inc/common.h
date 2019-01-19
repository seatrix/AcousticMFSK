/*
 * common.h
 *
 *  Created on: 2019��1��19��
 *      Author: JingX
 *  ��ͷ�ļ���Ҫ�����Ŀ�й��õ�Ԥ����ָ��
 *  ��ṹ�嶨��
 */

#ifndef MODULE_INC_COMMON_H_
#define MODULE_INC_COMMON_H_

#define PI           3.1415926
#define FFT_NUM      4096
#define cFFT_NUM     (2 * FFT_NUM)
#define SLIDER_LEN   (2 * 512)   // ������
#define DATA_LEN     (2 * 2048)  // ÿ�δ�������ݳ���
#define INTERVAL     1500        // LFM��FSK�ź�֮��ļ��������ʵ����

typedef struct
{
    float Val;
    int Loc;
}maxStruct;

typedef struct
{
   float deSin[2 * SAMPLE_PER_SYMBLE];
   float deCos[2 * SAMPLE_PER_SYMBLE];
}NCDW;

#endif /* MODULE_INC_COMMON_H_ */
