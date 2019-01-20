/*
 * Simulation.c
 *
 *  Created on: 2019��1��14��
 *      Author: JingX
 */
#include "simulation.h"

void readADC(float *ptrData, FILE *fp, int ArraySize)
{
    /* ��ȡ����
     * ����ʽ��ȡ�Ա�֤���ݲ����ж�
     *      ArraySize = N
     *      ptrData��Ӧ���ڳ���������ݣ�����Ϊ2N
     *      ptrDataBuffer �Ǵ������ж�ȡ�������ݣ�����ΪN
     *
     * ����ԭ��
     * ���ȴ��������ж�ȡ����ΪN����������ptrDataBuffer
     * Ȼ��ptrDataβ������ΪN�����ݷ���ͷ��ΪN�����ݶ���
     * ���ptrDataBuffer���뵽ptrData��β��
     */
    float *ptrDataBuffer = malloc(ArraySize * sizeof(float));
    fread(ptrDataBuffer, sizeof(float), ArraySize, fp);
    memcpy((char *)&ptrData[0], (char *)&ptrData[ArraySize], ArraySize * sizeof(float));
    memcpy((char *)&ptrData[ArraySize], (char *)&ptrDataBuffer[0], ArraySize * sizeof(float));
    free(ptrDataBuffer);
}

void genTestLFM(float *ptrTest, int offset)
{
    genLFM(ptrTest);
    RightShift(ptrTest, cFFT_NUM, offset);
}
