#include "detect.h"
#include "grideye_api_common.h"
// static const char *TAG = "CV functions";

struct Filter
{
    int *kernel;
    int side;
    int *weight;
};

struct Filter *gkern_1d(double sigma)
{
    struct Filter *f = malloc(sizeof(struct Filter));
    if (f == NULL)
    {
        return NULL;
    }
    int l = round(2 * 3 * sigma);
    if (l % 2 == 0)
    {
        l = l - 1;
    }
    int radius = (l - 1) / 2;
    int *kernel = malloc(sizeof(int) * l);
    if (kernel == NULL)
    {
        free(f);
        return NULL;
    }
    int *weight = malloc(sizeof(int) * l);
    if (weight == NULL)
    {
        free(kernel);
        free(f);
        return NULL;
    }
    double sigma2 = sigma * sigma;
    int double_to_int_factor = 4 * radius;
    for (int x = -radius; x <= radius; x++)
    {
        int ref = x + radius;
        double value = exp(-0.5 * (x * x) / sigma2);
        value *= double_to_int_factor;
        kernel[ref] = round(value);
    }
    for (int i = 0; i <= radius; i++)
    {
        int weight_sum = 0;
        for (int x = 0; x <= radius + i; x++)
        {
            weight_sum += kernel[x];
        }
        weight[i] = weight_sum;
        weight[2 * radius - i] = weight_sum;
    }
    f->side = l;
    f->kernel = kernel;
    f->weight = weight;
    return f;
}

struct Filter *avg_kern1d(int side)
{
    if (side % 2 == 0)
    {
        side = side - 1;
        printf("update avg kernel side to %d\n", side);
    }
    struct Filter *f = malloc(sizeof(struct Filter));
    if (f == NULL)
    {
        return NULL;
    }
    int *vec = malloc(sizeof(int) * side);
    if (vec == NULL)
    {
        free(f);
        return NULL;
    }
    int *weight = malloc(sizeof(int) * side);
    if (weight == NULL)
    {
        free(f);
        free(vec);
        return NULL;
    }
    int radius = (side - 1) / 2;
    for (int i = 0; i <= radius; i++)
    {
        vec[i] = 1;
        vec[2 * radius - i] = 1;
        weight[i] = radius + i + 1;
        weight[2 * radius - i] = radius + i + 1;
    }
    f->side = side;
    f->kernel = vec;
    f->weight = weight;
    return f;
}

int max_of_array(short *array, int size)
{
    int loc = 0;
    for (int index = 1; index < size; index++)
    {
        if (array[index] > array[loc])
        {
            loc = index;
        }
    }
    return array[loc];
}

/*return the smallest non-zero value*/
int min_of_array(short *array, int size)
{
    int loc = 0;
    for (int index = loc; index < size; index++)
    {
        if (array[index] != 0)
        {
            loc = index;
            break;
        }
    }

    for (int index = loc; index < size; index++)
    {
        if (array[index] == 0)
            continue;
        if (array[index] < array[loc])
        {
            loc = index;
        }
    }
    return array[loc];
}

/*average of all non-zero values in an array*/
int avg_of_array(short *array, int size)
{
    int sum = 0;
    int count = 0;
    for (int index = 0; index < size; index++)
    {
        if (array[index] == 0)
            continue;
        sum += array[index];
        count++;
    }
    return sum / count;
}

/*std of all non-zero values in an array*/
int std_of_array(short *array, int size)
{
    int avg = avg_of_array(array, size);
    int var = 0;
    int count = 0;
    for (int index = 0; index < size; index++)
    {
        if (array[index] == 0)
            continue;
        // do not use pow in loop, too slow
        var += (array[index] - avg) * (array[index] - avg);
        count++;
    }
    var /= count;
    return (int)sqrt(var);
}

/*******************************************************************************
 Copyright (C) <2015>, <Panasonic Corporation>
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1.	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2.	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3.	The name of copyright holders may not be used to endorse or promote products derived from this software without specific prior written permission.
4.	This software code may only be redistributed and used in connection with a grid-eye product.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS �AS IS� AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR POFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND OR ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY; OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the authors and should not be interpreted as representing official policies, either expressed or implied, of the FreeBSD Project. 

 ******************************************************************************/
UCHAR ucAMG_PUB_ODT_CalcDataLabeling8(UCHAR ucWidth, UCHAR ucHeight, UCHAR ucMark, USHORT usArea, UCHAR *pucImg, USHORT *pusSearchList)
{
	USHORT usImg = 0;
	USHORT usSize = ucWidth * ucHeight;
	UCHAR ucDetectNum = 0;

	for (usImg = 0; usImg < usSize; usImg++)
	{
		UCHAR ucLabelNum = 0;
		USHORT usIndex = 0;
		USHORT usIndexAdd = 0;

		if (ucMark == pucImg[usImg])
		{
			pucImg[usImg] = 0;
			pusSearchList[usIndex] = usImg;
			usIndexAdd = 1;
		}

		while (usIndex < usIndexAdd)
		{
			UCHAR ucX = pusSearchList[usIndex] % ucWidth;
			UCHAR ucY = pusSearchList[usIndex] / ucWidth;
			{
				if (0 <= (ucY - 1)) // 不是第一行
				{
					USHORT usCheckIndex = (ucX) + (ucY - 1) * ucWidth;
					if (ucMark == pucImg[usCheckIndex]) // 看上面一个像素是不是物体
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
				if (ucHeight > (ucY + 1)) //不是最后一行
				{
					USHORT usCheckIndex = (ucX) + (ucY + 1) * ucWidth;
					if (ucMark == pucImg[usCheckIndex]) // 看下面一个像素是不是物体
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
			}
			if (0 <= (ucX - 1)) //不是最左
			{
				{
					USHORT usCheckIndex = (ucX - 1) + (ucY)*ucWidth; //正左边
					if (ucMark == pucImg[usCheckIndex])
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
				if (0 <= (ucY - 1))
				{
					USHORT usCheckIndex = (ucX - 1) + (ucY - 1) * ucWidth; //左上
					if (ucMark == pucImg[usCheckIndex])
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
				if (ucHeight > (ucY + 1)) //左下
				{
					USHORT usCheckIndex = (ucX - 1) + (ucY + 1) * ucWidth;
					if (ucMark == pucImg[usCheckIndex])
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
			}
			if (ucWidth > (ucX + 1)) //不是最右
			{
				{
					USHORT usCheckIndex = (ucX + 1) + (ucY)*ucWidth; //正右边
					if (ucMark == pucImg[usCheckIndex])
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
				if (0 <= (ucY - 1))
				{
					USHORT usCheckIndex = (ucX + 1) + (ucY - 1) * ucWidth; //右上
					if (ucMark == pucImg[usCheckIndex])
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
				if (ucHeight > (ucY + 1))
				{
					USHORT usCheckIndex = (ucX + 1) + (ucY + 1) * ucWidth; //右下
					if (ucMark == pucImg[usCheckIndex])
					{
						pucImg[usCheckIndex] = 0;
						pusSearchList[usIndexAdd++] = usCheckIndex;
					}
				}
			}
			usIndex++;
		}
		
		if (usIndex <= usArea) //小于三个扩容后像素则认为是没有检测到物体
		{
			ucLabelNum = 0;
		}
		else
		{
			ucDetectNum++;
			ucLabelNum = ucDetectNum;
		}

		{
			USHORT usImg2 = 0;
			for (usImg2 = 0; usImg2 < usIndex; usImg2++)
			{
				pucImg[pusSearchList[usImg2]] = ucLabelNum;
			}
		}
	}

	return (ucDetectNum);
}