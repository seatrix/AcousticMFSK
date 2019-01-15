/*
 * Simulation.c
 *
 *  Created on: 2019��1��14��
 *      Author: JingX
 */
#include "simulation.h"

void readADC(float *ptrDataBuffer, float *ptrData, FILE *fp, int ArraySize)
{
    /* ��ȡ����
     * ����ʽ��ȡ�Ա�֤���ݲ����ж�
     * ArraySize = N
     * ptrDataBuffer��Ӧ���ڳ���������ݣ�����Ϊ2N
     * ptrData �Ǵ������ж�ȡ�������ݣ�����ΪN
     *
     * ����ԭ��
     * ���ȴ��������ж�ȡ����ΪN����������ptrDataBuffer
     * Ȼ��ptrDataBufferβ������ΪN�����ݷ���ͷ��ΪN�����ݶ���
     * ���ptrData���뵽ptrDataBuffer��β��
     */
    fread(ptrDataBuffer, sizeof(float), ArraySize, fp);
    memcpy((char *)&ptrData[0], (char *)&ptrData[ArraySize], ArraySize * sizeof(float));
    memcpy((char *)&ptrData[ArraySize], (char *)&ptrDataBuffer[0], ArraySize * sizeof(float));
}
