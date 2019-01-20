#include <math.h>
#include "lfm.h"
#include "mfsk.h"
#include "common.h"

void Normalize(float *ptrData, int len)
{
    /*
     * ��һ������
     * �������źŹ�һ��
     * ������������������
     *      ptrDataΪ���ݶ�ָ�룬
     *      lenΪ���ݶγ���
     * ������������maxValue
     */
	int counter = 0;
	maxStruct maxval;
	maxValue(&maxval, ptrData, len);
	while(counter < len)
	{
		ptrData[counter] = ptrData[counter] / maxval.Val * 2;
		counter++;
	}
}

void genNonCoherentDemodWave(NCDW *ptrData, int freq, int timelen, int fs)
{
    /*
     * ��������ź�
     * �÷��������FSK����Ƶ�ʲ�������źţ�ͨ���Ƚ���ط�ֵ��С
     * ��ȷ����Ϣλ
     *
     * ��������4������
     *      �ṹ��ָ��ptrDataΪ�����������źţ���Ϊ���ź�
     *      freq��        �ź�Ƶ��
     *      timelen�� �źŵĳ���ʱ�䣬�����������źŵĲ�������
     *      fs��            ����Ƶ��
     */
	int counter = 0;
	float theta;
	while(counter < timelen)
	{
		theta = 2 * PI * freq * counter / fs;
		ptrData->deSin[2 * counter] = sin(theta);
		ptrData->deCos[2 * counter] = cos(theta);
		counter++;
	}
	//	SW_BREAKPOINT ;
}

float SquareLawDetection(float * data, const NCDW * decodesig, int ArraySize)
{
    /*
     * ����첨
     * ����������������
     *      ����ָ��data�� ��Ҫ�����fsk�źŶΣ�����Ϊһ����Ԫ����
     *      �ṹ��ָ��decodesig������ɽ���źţ���dataͬƵͬ���ȵ�sin��cos�ź�
     *      ����ArraySize�� ���鳤��
     */
	int counter = 0;
	float desintemp = 0, decostemp = 0, powtemp = 0;
	while(counter < ArraySize)
	{
		decostemp = data[counter] * decodesig->deCos[counter] + decostemp;
		desintemp = data[counter] * decodesig->deSin[counter] + desintemp;
		counter++;
	}

	powtemp = decostemp * decostemp + desintemp * desintemp;
	return powtemp;
}

void bin2hex(char* ptrHex, const int* ptrData, int ArraySize)
{
    /*
     * 2����ת16����
     * �����������������2��������ָ�� ptrData��
     * �������±�Խ�ͣ������Խ�ߣ�ÿ4λת��Ϊ
     * 16�����ַ�����ptrHex��
     *
     * ArraySize ΪptrDataָ�������Ĵ�С
     */
	int counter, decTemp, i = 0;
	for(counter = 0; counter < ArraySize; counter = counter + 4)
	{
		decTemp = ptrData[counter + 3] + 2 * ptrData[counter + 2] + 4 * ptrData[counter + 1] + 8 * ptrData[counter];
		if(decTemp < 10)
		{
			ptrHex[i] = '0' + decTemp;
		}
		else
		{
			ptrHex[i] = 'A' + decTemp - 10;
		}
		i++;
	}
}

void showData(const char* ptrData, int ArraySize)
{
	int counter;
	printf("Code = ");
	for(counter = 0; counter < ArraySize; counter++)
		{
			printf("%c", ptrData[counter]);
		}

	printf("\n");
}
