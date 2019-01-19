#include <math.h>
#include "lfm.h"
#include "mfsk.h"

void Normalize(float *ptrData, int len)
{
    /*
     * ��һ������
     * �������źŹ�һ��
     * ������������������
     * ptrDataΪ���ݶ�ָ�룬 lenΪ���ݶγ���
     * ������������maxValue
     */
	int counter = 0;
	maxStruct maxval;
	maxValue(&maxval, ptrData, SAMPLE_PER_SYMBLE);
	while(counter < len)
	{
		ptrData[counter] = ptrData[counter] / maxval.Val * 2;
		counter++;
	}
}

void genNonCoherentDemodWave(int freq, int timelen, NCDW *ptrData)
{
    /*
     * ��������ź�
     * �÷��������FSK����Ƶ�ʲ�������źţ�ͨ���Ƚ���ط�ֵ��С
     * ��ȷ����Ϣλ��ֵ
     *
     * ��Ϊ�����ź��Ǹ��źţ����Բ����ķ�����ź����Ǹ��ź�
     */
	int counter = 0;
	float theta;
	while(counter < timelen)
	{
		theta = 2 * PI * freq * counter / SAMPLE_RATE;
		ptrData->deSin[2 * counter] = sin(theta);
		ptrData->deCos[2 * counter] = cos(theta);
		counter++;
	}
	//	SW_BREAKPOINT ;
}

float SquareLawDetection(float * data, const NCDW * decodesig)
{
    /*
     * ����첨
     * �õ����ֵ�İ���
     */
	int counter = 0;
	float desintemp = 0, decostemp = 0, powtemp = 0;
	while(counter < SAMPLE_PER_SYMBLE)
	{
		decostemp = data[counter] * decodesig->deCos[counter] + decostemp;
		desintemp = data[counter] * decodesig->deSin[counter] + desintemp;
		counter++;
	}

	powtemp = decostemp * decostemp + desintemp * desintemp;
	return powtemp;
}

void bin2hex(const int* ptrData, char* ptrHex, int ArraySize)
{
    /*
     * ������ת16����
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
