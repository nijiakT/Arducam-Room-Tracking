#include "detect.h"
#include "libmorpho.h"

int blob_detection(short *raw, uint8_t *result,short temp_min_thres)
{
    erosionByAnchor_2D(result, result, IM_W, IM_H, 3, 3);
    dilate_blobs(result, IM_W, IM_H, DILATION);
    // binary_fill_holes(result, IM_W, IM_H);
    int num = labeling8(result, IM_W, IM_H);
    //DBG_PRINT("detected %d blobs\n", num);
    return num;
}

void image_copy(short *src, short *dst, int size)
{
    for (int i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

void mask_copy(uint8_t *src, uint8_t *dst, int size)
{
    for (int i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

int labeling8(uint8_t *mask, int width, int height)
{
    static uint16_t search_list[IM_LEN];
    if ((width > 256) || (height > 256))
    {
        printf("ERROR: grideye api support image size up to uint8_t\n");
        return -1;
    }
    //change object indicator mark from number 1 to 0xff(255) to avoid
    //meaning overload between "object exist" and "object label 1"
    uint8_t mark = 0xff;
    for (int i = 0; i < (width * height); i++)
    {
        if (mask[i])
        {
            mask[i] = mark;
        }
    }
    uint16_t area_min = 20;
    uint8_t num = ucAMG_PUB_ODT_CalcDataLabeling8((uint8_t)width, (uint8_t)height, mark, area_min, mask, search_list);
    return num;
}

void manhattan(uint8_t *image, int width, int height){
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            //image[i][j] is image[j*width + i]
            if(image[j*width + i] == 1){
                //foreground pixel, set manhattan distance to 0
                image[j*width + i] = 0;
            }
            else {
                //potential undetected foreground pixel
                //max dist is width + height, but set maximum "dist" as 255 for comparison
                uint8_t temp = 255;
                if(i > 0){
                    temp = (temp < (image[j*width + i - 1] + 1)) ? temp : (image[j*width + i - 1] + 1);
                }
                if(j > 0){
                    temp = (temp < (image[(j-1)*width + i] + 1)) ? temp : (image[(j-1)*width + i] + 1);
                }
                image[j*width + i] = temp;
            }
        }
    }
    for(int i = width - 1; i >= 0; i--){
        for(int j = height - 1; j >= 0; j--){
            if(i < width - 1){
                image[j*width + i] = (image[j*width + i] < (image[j*width + i + 1] + 1)) ? image[j*width + i] : (image[j*width + i + 1] + 1);
            }
            if(j < height - 1){
                image[j*width + i] = (image[j*width + i] < (image[(j+1)*width + i] + 1)) ? image[j*width + i] : (image[(j+1)*width + i] + 1);
            }
        }
    }
    return;
}

void dilate_blobs(uint8_t *image, int width, int height, uint8_t factor){
    manhattan(image, width, height);
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            image[j*width + i] = (image[j*width + i] <= factor) ? 1 : 0;
        }
    }
    return;
}

