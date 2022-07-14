
#include <stdint.h>

#define REG_DLY 0xffff
#define REGLIST_TAIL 0x0000 /* Array end token */

static const uint16_t gc2145_default_init_regs[][2] = {
    {0xfe, 0xf0},
    {0xfe, 0xf0},
    {0xfe, 0xf0},

    {0xfc, 0x06},
    {0xf6, 0x00},

    {0xf7, 0x1d}, //37 //17 //37 //1d//05
    {0xf8, 0x83}, //87 //83 //82
    {0xfa, 0x00},
    {0xf9, 0xfe}, //ff
    {0xfd, 0x00},
    {0xc2, 0x00},
    {0xf2, 0x0f},
//////////////////////////////////////////////////////
////////////////////  Analog & Cisctl ////////////////
//////////////////////////////////////////////////////
    {0xfe, 0x00},

    {0x03, 0x04}, //exp time
    {0x04, 0x62}, //exp time

    {0x05, 0x01}, //00 //hb[11:8]
    {0x06, 0x3b}, //0b //hb

    {0x09, 0x00}, //row start
    {0x0a, 0x00}, //
    {0x0b, 0x00}, //col start
    {0x0c, 0x00},
    {0x0d, 0x04}, //height
    {0x0e, 0xc0},
    {0x0f, 0x06}, //width
    {0x10, 0x52},

    {0x12, 0x2e}, //sh_delay 太短 YUV出图异常
    {0x17, 0x14}, //CISCTL Mode1 [1:0]mirror flip
    {0x18, 0x22}, //sdark mode
    {0x19, 0x0f}, // AD pipe number
    {0x1a, 0x01}, //AD manual switch mode

    {0x1b, 0x4b}, //48 restg Width,SH width
    {0x1c, 0x07}, //06  帧率快后，横条纹 //12 //TX Width,Space Width
    {0x1d, 0x10}, //double reset
    {0x1e, 0x88}, //90//98 //fix  竖线//Analog Mode1,TX high,Coln_r
    {0x1f, 0x78}, //78 //38 //18 //Analog Mode2,txlow
    {0x20, 0x03}, //07 //Analog Mode3,comv,ad_clk mode
    {0x21, 0x40}, //10//20//40 //fix 灯管横条纹
    {0x22, 0xa0}, //d0//f0 //a2 //Vref vpix  FPN严重
    {0x24, 0x1e},
    {0x25, 0x01}, //col sel
    {0x26, 0x10}, //Analog PGA gain1
    {0x2d, 0x60}, //40//40 //txl drv mode
    {0x30, 0x01}, //Analog Mode4
    {0x31, 0x90}, //b0//70 // Analog Mode7 [7:5]rsgh_r灯管横条纹[4:3]isp_g
    {0x33, 0x06}, //03//02//01 //EQ_hstart_width
    {0x34, 0x01},
//
///////////////////////////////////////////////////
////////////////////  ISP reg  //////////////////////
//////////////////////////////////////////////////////
    {0x80, 0xff}, //outdoor gamma_en, GAMMA_en, CC_en, EE_en, INTP_en, DN_en, DD_en,LSC_en
    {0x81, 0x24}, //26//24 //BLK dither mode, ll_y_en ,skin_en, edge SA, new_skin_mode, autogray_en,ll_gamma_en,BFF test image
    {0x82, 0xfa}, //FA //auto_SA, auto_EE, auto_DN, auto_DD, auto_LSC, ABS_en, AWB_en, NA
    {0x83, 0x00}, //special_effect
    {0x84, 0x02}, //output format
    {0x86, 0x03}, //c2 //46 //c2 //sync mode
    {0x88, 0x03}, //[1]ctl_auto_gating [0]out_auto_gating
    {0x89, 0x03}, //bypass disable
    {0x85, 0x30}, //60//frame start cut
    {0x8a, 0x00}, //ISP_quiet_mode,close aaa pclk,BLK gate mode,exception,close first pipe clock,close dndd clock,close intp clock,DIV_gatedclk_en
    {0x8b, 0x00}, //[7:6]BFF_gate_mode,[5]BLK switch gain,[4]protect exp,[3:2]pipe gate mode,[1]not split sram,[0]dark current update

    {0xb0, 0x55}, //60 //global gain
    {0xc3, 0x00}, //[7:4]auto_exp_gamma_th1[11:8],[3:0]auto_exp_gamma_th2[11:8]
    {0xc4, 0x80}, //auto_exp_gamma_th1[7:0] into
    {0xc5, 0x90}, //auto_exp_gamma_th2[7:0] out //outdoor gamma
    {0xc6, 0x38}, //auto_gamma_th1
    {0xc7, 0x40}, //auto_gamma_th2

    {0xec, 0x06}, //measure window
    {0xed, 0x04},
    {0xee, 0x60}, //16  col
    {0xef, 0x90}, //8  row

    {0xb6, 0x01}, //[0]aec en

    {0x90, 0x01}, //crop
    {0x91, 0x00},
    {0x92, 0x00},
    {0x93, 0x00},
    {0x94, 0x00}, //08
    {0x95, 0x04},
    {0x96, 0xb0},
    {0x97, 0x06},
    {0x98, 0x40},

///////////////////////////////////////////////
///////////  BLK ////////////////////////
///////////////////////////////////////////////
    {0x18, 0x02},
    {0x40, 0x42}, //2b //27
    {0x41, 0x00}, //80 //dark row sel
    {0x43, 0x54}, //[7:4]BLK start not smooth  [3:0]output start frame

    {0x5e, 0x00}, //00//10 //18
    {0x5f, 0x00}, //00//10 //18
    {0x60, 0x00}, //00//10 //18
    {0x61, 0x00}, //00///10 //18
    {0x62, 0x00}, //00//10 //18
    {0x63, 0x00}, //00//10 //18
    {0x64, 0x00}, //00/10 //18
    {0x65, 0x00}, //00//10 //18
    {0x66, 0x20}, //1e
    {0x67, 0x20}, //1e
    {0x68, 0x20}, //1e
    {0x69, 0x20}, //1e


    {0x76, 0x00}, //0f

    {0x6a, 0x00}, //06
    {0x6b, 0x00}, //06
    {0x6c, 0x3e}, //06
    {0x6d, 0x3e}, //06
    {0x6e, 0x3f}, //06
    {0x6f, 0x3f}, //06
    {0x70, 0x00}, //06
    {0x71, 0x00}, //06 //manual offset

    {0x76, 0x00}, //1f//add offset
    {0x72, 0xf0}, //[7:4]BLK DD th [3:0]BLK various th
    {0x7e, 0x3c}, //ndark
    {0x7f, 0x00},

    {0xfe, 0x02},
    {0x48, 0x15},
    {0x49, 0x00}, //04//04 //ASDE OFFSET SLOPE
    {0x4b, 0x0b}, //ASDE y OFFSET SLOPE
    {0xfe, 0x00},

///////////////////////////////////////////////
/////////// AEC ////////////////////////
///////////////////////////////////////////////
    {0xfe, 0x01},

    {0x01, 0x04}, //AEC X1
    {0x02, 0xc0}, //AEC X2
    {0x03, 0x04}, //AEC Y1
    {0x04, 0x90}, //AEC Y2
    {0x05, 0x30}, //20 //AEC center X1
    {0x06, 0x90}, //40 //AEC center X2
    {0x07, 0x20}, //30 //AEC center Y1
    {0x08, 0x70}, //60 //AEC center Y2

    {0x09, 0x00}, //AEC show mode
    {0x0a, 0xc2}, //[7]col gain enable
    {0x0b, 0x11}, //AEC every N
    {0x0c, 0x10}, //AEC_mode3 center weight
    {0x13, 0x40}, //2a //AEC Y target
    {0x17, 0x00}, //AEC ignore mode
    {0x1c, 0x11}, //
    {0x1e, 0x61}, //
    {0x1f, 0x30}, //40//50 //max pre gain
    {0x20, 0x40}, //60//40 //max post gain
    {0x22, 0x80}, //AEC outdoor THD
    {0x23, 0x20}, //target_Y_low_limit
    {0xfe, 0x02},
    {0x0f, 0x04}, //05
    {0xfe, 0x01},

    {0x12, 0x35}, //35 //[5:4]group_size [3]slope_disable [2]outdoor_enable [0]histogram_enable
    {0x15, 0x50}, //target_Y_high_limit
    {0x10, 0x31}, //num_thd_high
    {0x3e, 0x28}, //num_thd_low
    {0x3f, 0xe0}, //luma_thd
    {0x40, 0x20}, //luma_slope
    {0x41, 0x0f}, //color_diff

    {0xfe, 0x02},
    {0x0f, 0x05}, //max_col_level
///////////////////////////
////// INTPEE /////////////
///////////////////////////
    {0xfe, 0x02}, //page2
    {0x90, 0x6c}, //ac //eeintp mode1
    {0x91, 0x03}, //02 ////eeintp mode2
    {0x92, 0xc8}, //44 //low criteria for direction
    {0x94, 0x66},
    {0x95, 0xb5},
    {0x97, 0x64}, //78 ////edge effect
    {0xa2, 0x11}, //fix direction
    {0xfe, 0x00},

/////////////////////////////
//////// DNDD///////////////
/////////////////////////////
    {0xfe, 0x02},
    {0x80, 0xc1}, //c1 //[7]share mode [6]skin mode  [5]is 5x5 mode [1:0]noise value select 0:2  1:2.5  2:3  3:4
    {0x81, 0x08}, //
    {0x82, 0x08}, //signal a 0.6
    {0x83, 0x08}, //04 //signal b 2.5

    {0x84, 0x0a}, //10 //05 dark_DD_TH
    {0x86, 0xf0}, //a0 Y_value_dd_th2
    {0x87, 0x50}, //90 Y_value_dd_th3
    {0x88, 0x15}, //60 Y_value_dd_th4

    {0x89, 0x50}, //80  // asde th2
    {0x8a, 0x30}, //60  // asde th3
    {0x8b, 0x10}, //30  // asde th4

/////////////////////////////////////////////////
///////////// ASDE ////////////////////////
/////////////////////////////////////////////////
    {0xfe, 0x01}, //page 1
    {0x21, 0x14}, //luma_value_div_sel(分频，与0xef呈2倍关系，增大1，0xef的值减小1倍)
//ff  ef  luma_value read_only

    {0xfe, 0x02}, //page2
    {0xa3, 0x40}, //ASDE_low_luma_value_LSC_th_H
    {0xa4, 0x20}, //ASDE_low_luma_value_LSC_th_L

    {0xa5, 0x40}, //80 //ASDE_LSC_gain_dec_slope_H
    {0xa6, 0x80}, // 80 //ASDE_LSC_gain_dec_slope_L
//ff  a7  ASDE_LSC_gain_dec  //read only

    {0xab, 0x40}, //50 //ASDE_low_luma_value_OT_th

    {0xae, 0x0c}, //[3]EE1_effect_inc_or_dec_high,[2]EE2_effect_inc_or_dec_high,
    //[1]EE1_effect_inc_or_dec_low,[0]EE2_effect_inc_or_dec_low,  1:inc  0:dec

    {0xb3, 0x34}, //44 //ASDE_EE1_effect_slope_low,ASDE_EE2_effect_slope_low
    {0xb4, 0x44}, //12 //ASDE_EE1_effect_slope_high,ASDE_EE2_effect_slope_high

    {0xb6, 0x38}, //40//40 //ASDE_auto_saturation_dec_slope
    {0xb7, 0x02}, //04 //ASDE_sub_saturation_slope
    {0xb9, 0x30}, //[7:0]ASDE_auto_saturation_low_limit
    {0x3c, 0x08}, //[3:0]auto gray_dec_slope
    {0x3d, 0x30}, //[7:0]auto gray_dec_th


    {0x4b, 0x0d}, //y offset slope
    {0x4c, 0x20}, //y offset limit

    {0xfe, 0x00},
//
///////////////////gamma1////////////////////
////Gamma
    {0xfe, 0x02},
    {0x10, 0x10},
    {0x11, 0x15},
    {0x12, 0x1a},
    {0x13, 0x1f},
    {0x14, 0x2c},
    {0x15, 0x39},
    {0x16, 0x45},
    {0x17, 0x54},
    {0x18, 0x69},
    {0x19, 0x7d},
    {0x1a, 0x8f},
    {0x1b, 0x9d},
    {0x1c, 0xa9},
    {0x1d, 0xbd},
    {0x1e, 0xcd},
    {0x1f, 0xd9},
    {0x20, 0xe3},
    {0x21, 0xea},
    {0x22, 0xef},
    {0x23, 0xf5},
    {0x24, 0xf9},
    {0x25, 0xff},

/////auto gamma/////
    {0xfe, 0x02},
    {0x26, 0x0f},
    {0x27, 0x14},
    {0x28, 0x19},
    {0x29, 0x1e},
    {0x2a, 0x27},
    {0x2b, 0x33},
    {0x2c, 0x3b},
    {0x2d, 0x45},
    {0x2e, 0x59},
    {0x2f, 0x69},
    {0x30, 0x7c},
    {0x31, 0x89},
    {0x32, 0x98},
    {0x33, 0xae},
    {0x34, 0xc0},
    {0x35, 0xcf},
    {0x36, 0xda},
    {0x37, 0xe2},
    {0x38, 0xe9},
    {0x39, 0xf3},
    {0x3a, 0xf9},
    {0x3b, 0xff},

///////////////////////////////////////////////
///////////   YCP       ///////////////////////
///////////////////////////////////////////////
    {0xfe, 0x02},
    {0xd1, 0x30}, //32 //
    {0xd2, 0x30}, //32 //
    {0xd3, 0x45},
    {0xdd, 0x14}, //edge sa
    {0xde, 0x86}, //asde auto gray
    {0xed, 0x01}, //
    {0xee, 0x28},
    {0xef, 0x30},
    {0xd8, 0xd8}, //autogray protecy

////////////////////////////
//////// LSC  0.8///////////////
////////////////////////////
    {0xfe, 0x01},
    {0xa1, 0x80}, // center_row
    {0xa2, 0x80}, // center_col
    {0xa4, 0x00}, // sign of b1
    {0xa5, 0x00}, // sign of b1
    {0xa6, 0x70}, // sign of b4
    {0xa7, 0x00}, // sign of b4
    {0xa8, 0x77}, // sign of b22
    {0xa9, 0x77}, // sign of b22
    {0xaa, 0x1f}, // Q1_b1 of R
    {0xab, 0x0d}, // Q1_b1 of G
    {0xac, 0x19}, // Q1_b1 of B
    {0xad, 0x24}, // Q2_b1 of R
    {0xae, 0x0e}, // Q2_b1 of G
    {0xaf, 0x1d}, // Q2_b1 of B
    {0xb0, 0x12}, // Q3_b1 of R
    {0xb1, 0x0c}, // Q3_b1 of G
    {0xb2, 0x06}, // Q3_b1 of B
    {0xb3, 0x13}, // Q4_b1 of R
    {0xb4, 0x10}, // Q4_b1 of G
    {0xb5, 0x0c}, // Q4_b1 of B
    {0xb6, 0x6a}, // right_b2 of R
    {0xb7, 0x46}, // right_b2 of G
    {0xb8, 0x40}, // right_b2 of B
    {0xb9, 0x0b}, // right_b4 of R
    {0xba, 0x04}, // right_b4 of G
    {0xbb, 0x00}, // right_b4 of B
    {0xbc, 0x53}, // left_b2 of R
    {0xbd, 0x37}, // left_b2 of G
    {0xbe, 0x2d}, // left_b2 of B
    {0xbf, 0x0a}, // left_b4 of R
    {0xc0, 0x0a}, // left_b4 of G
    {0xc1, 0x14}, // left_b4 of B
    {0xc2, 0x34}, // up_b2 of R
    {0xc3, 0x22}, // up_b2 of G
    {0xc4, 0x18}, // up_b2 of B
    {0xc5, 0x23}, // up_b4 of R
    {0xc6, 0x0f}, // up_b4 of G
    {0xc7, 0x3c}, // up_b4 of B
    {0xc8, 0x20}, // down_b2 of R
    {0xc9, 0x1f}, // down_b2 of G
    {0xca, 0x17}, // down_b2 of B
    {0xcb, 0x2d}, // down_b4 of R
    {0xcc, 0x12}, // down_b4 of G
    {0xcd, 0x20}, // down_b4 of B
    {0xd0, 0x61}, // right_up_b22 of R
    {0xd1, 0x2f}, // right_up_b22 of G
    {0xd2, 0x39}, // right_up_b22 of B
    {0xd3, 0x45}, // right_down_b22 of R
    {0xd4, 0x2c}, // right_down_b22 of G
    {0xd5, 0x21}, // right_down_b22 of B
    {0xd6, 0x64}, // left_up_b22 of R
    {0xd7, 0x2d}, // left_up_b22 of G
    {0xd8, 0x30}, // left_up_b22 of B
    {0xd9, 0x42}, // left_down_b22 of R
    {0xda, 0x27}, // left_down_b22 of G
    {0xdb, 0x13}, // left_down_b22 of B
    {0xfe, 0x00},

/////////////////////////////////////////////////
/////////////    AWB     ////////////////////////
/////////////////////////////////////////////////
    {0xfe, 0x01},

    {0x4f, 0x00},
    {0x4f, 0x00},
    {0x4b, 0x01},
    {0x4f, 0x00},


    {0x4c, 0x01},
    {0x4d, 0x6f},
    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0x70},

    {0x4e, 0x02},
    {0x4c, 0x01},
    {0x4d, 0x8f},
    {0x4e, 0x02},

    {0x4c, 0x01},
    {0x4d, 0x90},
    {0x4e, 0x02}, //light


    {0x4c, 0x01},
    {0x4d, 0xed},
    {0x4e, 0x33}, //light
    {0x4c, 0x01},
    {0x4d, 0xcd},
    {0x4e, 0x33}, //light
    {0x4c, 0x01},
    {0x4d, 0xec},
    {0x4e, 0x03}, //light

    {0x4c, 0x01},
    {0x4d, 0x6c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x6e},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8c},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0x8e},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xab},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xac},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xad},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xae},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xcb},
    {0x4e, 0x03},

    {0x4c, 0x01},
    {0x4d, 0xcc},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xce},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xeb},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xec},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xee},
    {0x4e, 0x03},
    {0x4c, 0x02},
    {0x4d, 0x0c},
    {0x4e, 0x03},
    {0x4c, 0x02},
    {0x4d, 0x0d},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xea},
    {0x4e, 0x03},
    {0x4c, 0x01},
    {0x4d, 0xaf},
    {0x4e, 0x03}, //dark
    {0x4c, 0x01},
    {0x4d, 0xcf},
    {0x4e, 0x03}, //dark

    {0x4c, 0x01},
    {0x4d, 0xca},
    {0x4e, 0x04}, //light
    {0x4c, 0x02},
    {0x4d, 0x0b},
    {0x4e, 0x05}, //light
    {0x4c, 0x02},
    {0x4d, 0xc8},
    {0x4e, 0x06}, //light 100lux
    {0x4c, 0x02},
    {0x4d, 0xa8},

    {0x4e, 0x06}, //light
    {0x4c, 0x02},
    {0x4d, 0xa9},
    {0x4e, 0x06}, //light


    {0x4c, 0x02},
    {0x4d, 0x89},
    {0x4e, 0x06}, //400lux
    {0x4c, 0x02},
    {0x4d, 0x69},
    {0x4e, 0x06}, //f12
    {0x4c, 0x02},
    {0x4d, 0x6a},
    {0x4e, 0x06}, //f12
    {0x4c, 0x02},
    {0x4d, 0xc7},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe7},
    {0x4e, 0x07}, //100lux
    {0x4c, 0x03},
    {0x4d, 0x07},
    {0x4e, 0x07}, //light

    {0x4c, 0x02},
    {0x4d, 0xe8},
    {0x4e, 0x07},
    {0x4c, 0x02},
    {0x4d, 0xe9},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x08},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x09},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x27},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x28},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x29},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x47},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x48},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x49},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x67},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x68},
    {0x4e, 0x07},
    {0x4c, 0x03},
    {0x4d, 0x69},
    {0x4e, 0x07},

    {0x4f, 0x01},
    {0xfe, 0x01},
    {0x50, 0x80}, //AWB_PRE_mode
    {0x51, 0xa8}, //AWB_pre_THD_min[7:0]
    {0x52, 0x57}, //AWB_pre_THD_min[15:8] Dominiate luma 0.25=639c 0.22=57a8
    {0x53, 0x38}, //AWB_pre_THD_min_MIX[7:0]
    {0x54, 0xc7}, //AWB_pre_THD_min_MIX[15:8] Mix luma 0.5

    {0x56, 0x0e}, //AWB_tone mode
    {0x58, 0x08}, //AWB_C_num_sel,AWB_D_num_sel
    {0x5b, 0x00}, //AWB_mix_mode

    {0x5c, 0x74}, //green_num0[7:0]
    {0x5d, 0x8b}, //green_num0[15:8] 0.35

    {0x61, 0xd3}, //R2G_stand0
    {0x62, 0xb5}, //B2G_stand0
    {0x63, 0x00}, //88//a4 //AWB gray mode [7]enable
    {0x65, 0x04}, //AWB margin

    {0x67, 0xb2}, //R2G_stand3[7:0]  FF/CWF
    {0x68, 0xac}, //B2G_stand3[7:0]
    {0x69, 0x00}, //R2G_stand4[9:8] B2G_stand4[9:8] R2G_stand3[9:8] B2G_stand3[9:8]
    {0x6a, 0xb2}, //R2G_stand4[7:0]  TL84/TL84&CWF
    {0x6b, 0xac}, //B2G_stand4[7:0]
    {0x6c, 0xb2}, //R2G_stand5[7:0]  A
    {0x6d, 0xac}, //B2G_stand5[7:0]
    {0x6e, 0x40}, //AWB_skin_weight R2G_stand5[9:8] B2G_stand5[9:8]
    {0x6f, 0x18}, //AWB_indoor_THD (0x21=17 caculate)
    {0x73, 0x00}, //AWB_indoor_mode

    {0x70, 0x10}, //AWB low luma TH
    {0x71, 0xe8}, //AWB outdoor TH
    {0x72, 0xc0}, //outdoor mode
    {0x74, 0x01}, //[2:0]AWB skip mode 2x2,4x4,4x8,8x8
    {0x75, 0x01}, //[1:0]AWB_every_N
    {0x7f, 0x08}, //[3]gray world frame start

    {0x76, 0x70}, //R limit
    {0x77, 0x58}, //G limit
    {0x78, 0xa0}, //d8 //B limit

    {0xfe, 0x00},
//
//////////////////////////////////////////
///////////  CC   ////////////////////////
//////////////////////////////////////////
    {0xfe, 0x02},

    {0xc0, 0x01}, //[5:4] CC mode [0]CCT enable

    {0xC1, 0x50}, //D50/D65
    {0xc2, 0xF9},
    {0xc3, 0x00}, //0
    {0xc4, 0xe8}, //e0
    {0xc5, 0x48},
    {0xc6, 0xf0},


    {0xC7, 0x50},
    {0xc8, 0xf2},
    {0xc9, 0x00},
    {0xcA, 0xE0},
    {0xcB, 0x45},
    {0xcC, 0xec},

    {0xCd, 0x45},
    {0xce, 0xf0},
    {0xcf, 0x00},
    {0xe3, 0xf0},
    {0xe4, 0x45},
    {0xe5, 0xe8},


    {0xfe, 0x00},

    {0xf2, 0x0f},


//////////////frame rate   50Hz
    {0xfe, 0x00},

    {0xf7, 0x1d},
    {0xf8, 0x84},
    {0xfa, 0x00},

    {0x05, 0x01}, //hb
    {0x06, 0x3b},
    {0x07, 0x01}, //Vb
    {0x08, 0x0b},

    {0xfe, 0x01},
    {0x25, 0x01},
    {0x26, 0x32}, //step
    {0x27, 0x03}, //8.15fps
    {0x28, 0x96},
    {0x29, 0x03}, //8.15fps
    {0x2a, 0x96},
    {0x2b, 0x03}, //8.15fps
    {0x2c, 0x96},
    {0x2d, 0x04}, //8.15fps
    {0x2e, 0x62},
    {0x3c, 0x00},
    {0xfe, 0x00},

/////////dark  sun//////
    {0xfe, 0x00},
    {0x18, 0x22},
    {0xfe, 0x02},
    {0x40, 0xbf},
    {0x46, 0xcf},
    {0xfe, 0x00},

    {0xfe, 0x00},

    {0xf7, 0x1d},
    {0xf8, 0x84},
    {0xfa, 0x10},

    {0x05, 0x01}, //hb
    {0x06, 0x18},
    {0x07, 0x00}, //Vb
    {0x08, 0x2e},

    {0xfe, 0x01},
    {0x25, 0x00},
    {0x26, 0xa2}, //step
    {0x27, 0x01},
    {0x28, 0xe6},
    {0x29, 0x01},
    {0x2a, 0xe6},
    {0x2b, 0x01},
    {0x2c, 0xe6},
    {0x2d, 0x04}, // AEC_exp_level4[12:8]
    {0x2e, 0x62}, // AEC_exp_level4[7:0]
    {0x3c, 0x00},
    {0xfe, 0x00},

    {0x09, 0x01}, //row start
    {0x0a, 0xd0}, //
    {0x0b, 0x02}, //col start
    {0x0c, 0x70},
    {0x0d, 0x01}, //height
    {0x0e, 0x00},
    {0x0f, 0x01}, //width
    {0x10, 0x50},

    {0x90, 0x01}, //crop
    {0x91, 0x00},
    {0x92, 0x00},
    {0x93, 0x00},
    {0x94, 0x00},
    {0x95, 0x00},
    {0x96, 0xf0},
    {0x97, 0x01},
    {0x98, 0x40},


    {REGLIST_TAIL, 0x00},
};
