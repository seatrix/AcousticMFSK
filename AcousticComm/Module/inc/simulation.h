/*
 * simulation.h
 *
 *  Created on: 2019��1��14��
 *      Author: JingX
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lfm.h"

void readADC(float *ptrData, FILE *fp, int ArraySize);
void genTestLFM(float *ptrTest, int offset);

#endif /* SIMULATION_H_ */
