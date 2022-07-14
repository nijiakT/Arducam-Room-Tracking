#include "stdbool.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "grideye_api_common.h"
#include <stdint.h>
#include "macro.h"

typedef struct Filter Filter;
typedef int (*pool_function_t)(short[], int);

int blob_detection(short *raw, uint8_t *result,short temp_min_thres);

/*API functions*/
void image_copy(short *src, short *dst, int size);
void mask_copy(uint8_t *src, uint8_t *dst, int size);
int labeling8(uint8_t *mask, int width, int height);
void manhattan(uint8_t *image, int width, int height);
void dilate_blobs(uint8_t *image, int width, int height, uint8_t factor);

/*low level functions*/
struct Filter *gkern_1d(double sigma);
struct Filter *avg_kern1d(int side);

int max_of_array(short *array, int size);
int min_of_array(short *array, int size);
int avg_of_array(short *array, int size);
int std_of_array(short *array, int size);

UCHAR ucAMG_PUB_ODT_CalcDataLabeling8( UCHAR ucWidth, UCHAR ucHeight, UCHAR ucMark, USHORT usArea, UCHAR* pucImg, USHORT* pusSearchList);
