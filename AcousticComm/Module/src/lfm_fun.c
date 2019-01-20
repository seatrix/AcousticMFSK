/*
 * hello_lfm_func.c
 *
 *  Created on: 2019-1-7
 *      Author: JingX
 */
#include "lfm.h"
#include "common.h"

extern float TwiddleCoff[cFFT_NUM];
extern unsigned char Brev[64];

void genLFM(float *ptrDesArray)
{
	/*
	 * ������ʼƵ��ΪLFM_FRES��LFM�ź�
	 * ��ΪCCS�е�FFT��Ϊ����FFT
	 * �ʲ������źŸ�ʽΪż����Ϊ���ݣ�������Ϊ0
	 */
	float theta, coff;
	coff = (LFM_FREE - LFM_FRES) / LFM_LENGTH * SAMPLE_RATE;
	int counter;
	for(counter = 0; counter < LFM_LENGTH; counter++)
	{
		//			theta = 2 * PI * LFM_FRES * counter / 100000;
		theta =  2 * PI * (LFM_FRES + 0.5 * coff * counter / SAMPLE_RATE) * counter / SAMPLE_RATE;
		ptrDesArray[2 * counter] = cos(theta);
	}
}


void calcuTwiddle(float *w, int n)
{
    int i,j,k;
    double theta1,theta2,theta3;

    for(j = 1, k = 0; j<=n>>2; j = j<<2)
    {
            for(i = 0; i <n>>2;i += j)
            {
                    theta1 = 2 * PI * i / n;
                    w[k]     = (float)cos(theta1);
                    w[k + 1] = (float)sin(theta1);

                    theta2 = 4 * PI * i / n;
                    w[k + 2] = (float)cos(theta2);
                    w[k + 3] = (float)sin(theta2);

                    theta3 = 6 * PI * i / n;
                    w[k + 4] = (float)cos(theta3);
                    w[k + 5] = (float)sin(theta3);
                    k += 6;
            }
    }

}

void xCorr(const float *ptrSigy, const float *ptrFFTx, float *ptrCorr)
{
	/*
	 * ���������
	 * �������ź�ptrSigy�뱾���ź�ptrSigx������أ���ȡ��ط�
	 * ʹ��FFT���㷽����ע��FFT������оƬ�仯���仯��ע������
	 * �Ա����ź���FFT��ȡ�����������Ľ��ΪptrSigy���ptrSigx��ʱ��
	 */
	float sigy_temp[cFFT_NUM];
	float fft_sigx[cFFT_NUM], fft_sigy[cFFT_NUM], xcorr_temp[cFFT_NUM];
	int counter;

	for(counter = 0; counter < cFFT_NUM; counter++)
	{
		fft_sigx[counter] = ptrFFTx[counter];
		sigy_temp[counter] = ptrSigy[counter];
	}
	DSPF_sp_fftSPxSP(FFT_NUM, sigy_temp, TwiddleCoff, fft_sigy, Brev, 4, 0, FFT_NUM);
//	float real_fft_sigy[FFT_NUM];
//	cmplx2real(real_fft_sigy, fft_sigy, FFT_NUM);
	for(counter = 0; counter < FFT_NUM; counter++)
	{
		/* FFT֮����FFT_x�Ĺ�����FFT_y�ĳ˻�
		 * x = a + jb; y = c + jd
		 * conj(x).*y = (ac + bd) + j(ad - bc)
		 */
		float a, b, c, d;
		a = fft_sigx[2 * counter];
		b = fft_sigx[2 * counter +  1];
		c = fft_sigy[2 * counter];
		d = fft_sigy[2 * counter + 1];

		xcorr_temp[2 * counter] = a*c + b*d;
		xcorr_temp[2 * counter + 1] = a*d - b*c;
	}
//	float real_fft_sigy[FFT_NUM];
//	cmplx2real(real_fft_sigy, xcorr_temp, FFT_NUM);
	DSPF_sp_ifftSPxSP(FFT_NUM, xcorr_temp, TwiddleCoff, ptrCorr,  Brev, 4, 0, FFT_NUM);

	/// --- xcorrǰ�������ν��� ---//
	RightShift(ptrCorr, cFFT_NUM, FFT_NUM);
}

void cmplx2real(float *real, float *cmplx, int ArraySize)
{
	/* ����תʵ������
	 * ���罫����fftת��Ϊʵ��fft�����㻭ͼȷ���Ƿ�������ȷ��
	 * ��������3������
	 *      ����ָ��real��Ŀ���ַ
	 *      ����ָ��cmplx�� ���������ַ
	 *      ����ArraySize��realָ������鳤�ȣ�ͨ��Ϊcmplxָ������鳤�ȵ�һ��
	 */
	int i;
	for(i = 0; i < ArraySize; i++)
	{
		real[i] = cmplx[2 * i] * cmplx[2 * i] + cmplx[2 * i + 1] * cmplx[2 * i + 1];
	}

}

void maxValue(maxStruct *ptrMax, const float *ptrData, int ArraySize)
{
    /* ���ֵ��Ѱ
     * ��������3��������
     *      ptrMax Ϊ maxStruct �ṹ��ָ��
     *      ptrData ΪĿ������ָ��
     *      ArraySize ΪĿ�������С
     * ������ѰptrData�е����ֵ���������ֵValue��Location����ptrMax��
     */
    int counter = 0;
    ptrMax->Val = -1;
    ptrMax->Loc = -1;

    for (counter = 0; counter < ArraySize; counter ++)
    {
        if(ptrMax->Val < ptrData[counter])
        {
            ptrMax->Val = ptrData[counter];
            ptrMax->Loc = counter;
        }
    }
}

int isPeak(float ptrMaxVal, float *ptrData, int ArraySize)
{
    /*
     * ��ֵ�ж�
     * ��ֵ�ж��������ǣ�
     * �������ݶ��г������ֵ1/3�����ݵ���һ��������ʱ��
     * �ж������ֵΪ��ֵ
     *
     * ��ֵ�ж����㷨����Ҫ����ĸĽ���Ҫô��Ӹ�����б𷽷�������֤��
     * Ҫô���ø�����ѧ�İ취
     */
    int bigger_counter = 0;
    int counter = 0;
    ptrMaxVal = ptrMaxVal / 3;
    for(counter; counter < ArraySize; counter++)
    {
        if(ptrMaxVal < ptrData[counter])
        {
            bigger_counter++;
        }
    }

    if(bigger_counter < 20 && bigger_counter > 3) //����3���趨��Ϊ�˷�ֹ�����о�Ϊ0��ĳһ�����������ֵ����
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int LFMsp(int *ptrLoc, const int ptrFlag, const int ArraySize)
{
/*
 * �������ڷ���LFM�źŵ���ʼλ��
 * ���������ź�һ���ǻ���������źŶε�
 * ���ڴ��������ʼ���ְ����˲����źŵ������δ���뿼��
 * ����������
 * 1. �źŵ���ʼλ��һ��Ϊż������Ϊ����λ���źŵ��鲿
 */
    int counter = ptrFlag;
    for(counter; counter < ArraySize; counter++)
    {
        ;
    }

    return 1;
}

void RightShift(float *arr, int N, int K)
{
    /*
     * �������ƺ���
     * ����������������
     *      ����ָ��arr�� �����Ƶ�����
     *      ����N�����鳤��
     *      ����K�����Ƶĳ���
     */
    K = K % N ;
    Reverse(arr, 0, N-K-1);     //ǰ��N-K��������
    Reverse(arr, N-K, N-1);     //����K��������
    Reverse(arr, 0, N-1);       //ȫ������
}

void Reverse(float *arr, int b, int e)      //��������
{
    for( ; b < e; b++, e--)    //�������ǰ����һ�����
    {
        float temp = arr[e];
        arr[e] = arr[b];
        arr[b] = temp;
    }
}
