#define IM_W (160)
#define IM_H (120)
#define IM_LEN (IM_W * IM_H)
#define DILATION (6)

#define DEBUG 1

#if DEBUG
#define DBG_PRINT(format, args...)\                             
        do{\
        printf(format, ##args);\
        }while (0)
#else
#define DBG_PRINT(format, args...)
#endif

#define IOT_IP "138.246.236.181"
#define IOT_PORT "1883"
#define IOT_USERNAME "JWT"
#define IOT_PASSWORD "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2NTUyMDU5MjcsImlzcyI6ImlvdHBsYXRmb3JtIiwic3ViIjoiMTNfMjYifQ.viv4x3HKFKY_nrb4rzMWlbIfA5ezA_tVsHHLSLQH3lyD9B8ubLr3hyzM__2_M4QfKVEbN5JBgipPcFyu9bGG73rbvjnPUAdt8Hl3LCXGvP6WDiZ_5lFkg0M3G6wOGW65TkgD5ZuO7zbo40n82BEHKM1zWTwuQDyo-6bVRZjiLkJUeLqs2ytK6lI32Z2p7TrApJN3RW8HvpmSP9GnhociyV4lmM_GYHTE-iWgRa99jek3VH9T2yc4shqk28o0GGzeXg0teXFO6WlQuaGtLDlOXhFu90mjfgQ-GLiExQAN1_iGXg6x8jWLlLhqIbWRGY3bEncn3Yplc01EDwkU0GHR3RLkTVbFPdWMkvBsflRJUeWBjUnUlRoLCjpo5YMFrYK61n0X6VbSFvm59jLbHA8UPhKXtZm3RnMPXFidSHN9jMZC3tvL4iDi4_RIdKtQxCWSPX7phF-Xvl5fjnzCldq6_qSgKG8COMiUuxgrCc5LzWhpx9jsPRV62YaSiVx_vbNxU1oPz7o-2wDMtV6T6CKsw8cI6XLUIIMWYm2NJbSxoowW9s3yCTUUC2xaKnN9XAMI-0DwQ3CbNX1viKXpdTCRsTErQA5jy0PdQtfTCFowFkDkvP2QIVKKFCyxANNry0vQG8LUWaMHlWEidAAgrjlxwcgCjrQ6UUIltXHDgfnInYU"
#define IOT_DEVICE_ID "26"
#define IOT_USER_ID "13"
#define IOT_USERNAME_PUB "kai"
#define IOT_TOPIC IOT_USER_ID "_" IOT_DEVICE_ID
#define IOT_URI "mqtt://" IOT_USERNAME ":" IOT_PASSWORD "@" IOT_IP ":" IOT_PORT
#define IOT_MESSAGE_PATTERN "{\"username\":\"%s\",\"%s\":%d,\"device_id\":%s,\"timestamp\":%lld}"
#define IOT_SENSOR "Count"