/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.7-dev */

#ifndef PB_APIPROTOS_PB_H_INCLUDED
#define PB_APIPROTOS_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _Trs80Model { 
    Trs80Model_UNKNOWN_MODEL = 0, 
    Trs80Model_MODEL_I = 1, 
    Trs80Model_MODEL_III = 2, 
    Trs80Model_MODEL_4 = 3, 
    Trs80Model_MODEL_4P = 4 
} Trs80Model;

typedef enum _MediaType { 
    MediaType_UNKNOWN = 0, 
    MediaType_DISK = 1, 
    MediaType_CASSETTE = 2, 
    MediaType_COMMAND = 3, 
    MediaType_BASIC = 4 
} MediaType;

/* Struct definitions */
typedef struct _ListAppsParams_Trs80Params { 
    pb_callback_t media_types;
} ListAppsParams_Trs80Params;

typedef struct _ApiResponseUploadSystemState { 
    bool success;
    char message[80];
    int64_t token;
} ApiResponseUploadSystemState;

typedef struct _DownloadSystemStateParams { 
    int64_t token;
} DownloadSystemStateParams;

typedef struct _FetchMediaImagesParams { 
    char app_id[40];
} FetchMediaImagesParams;

typedef struct _GetAppParams { 
    char app_id[40];
} GetAppParams;

typedef struct _ListAppsParams { 
    int32_t start;
    int32_t num;
    char query[128];
    bool has_trs80;
    ListAppsParams_Trs80Params trs80;
} ListAppsParams;

typedef struct _MediaImage { 
    MediaType type;
    char filename[30];
    pb_callback_t data;
    int64_t uploadTime;
    char description[128];
} MediaImage;

typedef struct _SystemState_MemoryRegion { 
    int32_t start;
    pb_callback_t data;
    int32_t length;
} SystemState_MemoryRegion;

typedef struct _SystemState_Registers { 
    int32_t ix;
    int32_t iy;
    int32_t pc;
    int32_t sp;
    int32_t af;
    int32_t bc;
    int32_t de;
    int32_t hl;
    int32_t af_prime;
    int32_t bc_prime;
    int32_t de_prime;
    int32_t hl_prime;
    int32_t i;
    int32_t r_1;
    int32_t r_2;
} SystemState_Registers;

typedef struct _Trs80Extension { 
    Trs80Model model;
} Trs80Extension;

typedef struct _ApiResponseMediaImages { 
    bool success;
    char message[80];
    pb_size_t mediaImage_count;
    MediaImage mediaImage[5];
} ApiResponseMediaImages;

typedef struct _App { 
    char id[40];
    char name[64];
    char version[24];
    char description[1000];
    int32_t release_year;
    pb_size_t screenshot_url_count;
    char screenshot_url[5][150];
    char author[64];
    bool has_ext_trs80;
    Trs80Extension ext_trs80;
} App;

typedef struct _SystemState { 
    Trs80Model model;
    bool has_registers;
    SystemState_Registers registers;
    pb_size_t memoryRegions_count;
    SystemState_MemoryRegion memoryRegions[10];
} SystemState;

typedef struct _ApiResponseApps { 
    bool success;
    char message[80];
    pb_size_t app_count;
    App app[5];
} ApiResponseApps;

typedef struct _ApiResponseDownloadSystemState { 
    bool success;
    char message[80];
    bool has_systemState;
    SystemState systemState;
} ApiResponseDownloadSystemState;

typedef struct _UploadSystemStateParams { 
    bool has_state;
    SystemState state;
} UploadSystemStateParams;


/* Helper constants for enums */
#define _Trs80Model_MIN Trs80Model_UNKNOWN_MODEL
#define _Trs80Model_MAX Trs80Model_MODEL_4P
#define _Trs80Model_ARRAYSIZE ((Trs80Model)(Trs80Model_MODEL_4P+1))

#define _MediaType_MIN MediaType_UNKNOWN
#define _MediaType_MAX MediaType_BASIC
#define _MediaType_ARRAYSIZE ((MediaType)(MediaType_BASIC+1))


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define ApiResponseApps_init_default             {0, "", 0, {App_init_default, App_init_default, App_init_default, App_init_default, App_init_default}}
#define ApiResponseMediaImages_init_default      {0, "", 0, {MediaImage_init_default, MediaImage_init_default, MediaImage_init_default, MediaImage_init_default, MediaImage_init_default}}
#define ApiResponseDownloadSystemState_init_default {0, "", false, SystemState_init_default}
#define ApiResponseUploadSystemState_init_default {0, "", 0}
#define App_init_default                         {"", "", "", "", 0, 0, {"", "", "", "", ""}, "", false, Trs80Extension_init_default}
#define Trs80Extension_init_default              {_Trs80Model_MIN}
#define MediaImage_init_default                  {_MediaType_MIN, "", {{NULL}, NULL}, 0, ""}
#define SystemState_init_default                 {_Trs80Model_MIN, false, SystemState_Registers_init_default, 0, {SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default, SystemState_MemoryRegion_init_default}}
#define SystemState_Registers_init_default       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define SystemState_MemoryRegion_init_default    {0, {{NULL}, NULL}, 0}
#define FetchMediaImagesParams_init_default      {""}
#define GetAppParams_init_default                {""}
#define ListAppsParams_init_default              {0, 0, "", false, ListAppsParams_Trs80Params_init_default}
#define ListAppsParams_Trs80Params_init_default  {{{NULL}, NULL}}
#define UploadSystemStateParams_init_default     {false, SystemState_init_default}
#define DownloadSystemStateParams_init_default   {0}
#define ApiResponseApps_init_zero                {0, "", 0, {App_init_zero, App_init_zero, App_init_zero, App_init_zero, App_init_zero}}
#define ApiResponseMediaImages_init_zero         {0, "", 0, {MediaImage_init_zero, MediaImage_init_zero, MediaImage_init_zero, MediaImage_init_zero, MediaImage_init_zero}}
#define ApiResponseDownloadSystemState_init_zero {0, "", false, SystemState_init_zero}
#define ApiResponseUploadSystemState_init_zero   {0, "", 0}
#define App_init_zero                            {"", "", "", "", 0, 0, {"", "", "", "", ""}, "", false, Trs80Extension_init_zero}
#define Trs80Extension_init_zero                 {_Trs80Model_MIN}
#define MediaImage_init_zero                     {_MediaType_MIN, "", {{NULL}, NULL}, 0, ""}
#define SystemState_init_zero                    {_Trs80Model_MIN, false, SystemState_Registers_init_zero, 0, {SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero, SystemState_MemoryRegion_init_zero}}
#define SystemState_Registers_init_zero          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define SystemState_MemoryRegion_init_zero       {0, {{NULL}, NULL}, 0}
#define FetchMediaImagesParams_init_zero         {""}
#define GetAppParams_init_zero                   {""}
#define ListAppsParams_init_zero                 {0, 0, "", false, ListAppsParams_Trs80Params_init_zero}
#define ListAppsParams_Trs80Params_init_zero     {{{NULL}, NULL}}
#define UploadSystemStateParams_init_zero        {false, SystemState_init_zero}
#define DownloadSystemStateParams_init_zero      {0}

/* Field tags (for use in manual encoding/decoding) */
#define ListAppsParams_Trs80Params_media_types_tag 1
#define ApiResponseUploadSystemState_success_tag 1
#define ApiResponseUploadSystemState_message_tag 2
#define ApiResponseUploadSystemState_token_tag   3
#define DownloadSystemStateParams_token_tag      1
#define FetchMediaImagesParams_app_id_tag        1
#define GetAppParams_app_id_tag                  1
#define ListAppsParams_start_tag                 1
#define ListAppsParams_num_tag                   2
#define ListAppsParams_query_tag                 3
#define ListAppsParams_trs80_tag                 4
#define MediaImage_type_tag                      1
#define MediaImage_filename_tag                  2
#define MediaImage_data_tag                      3
#define MediaImage_uploadTime_tag                4
#define MediaImage_description_tag               5
#define SystemState_MemoryRegion_start_tag       1
#define SystemState_MemoryRegion_data_tag        2
#define SystemState_MemoryRegion_length_tag      3
#define SystemState_Registers_ix_tag             1
#define SystemState_Registers_iy_tag             2
#define SystemState_Registers_pc_tag             3
#define SystemState_Registers_sp_tag             4
#define SystemState_Registers_af_tag             5
#define SystemState_Registers_bc_tag             6
#define SystemState_Registers_de_tag             7
#define SystemState_Registers_hl_tag             8
#define SystemState_Registers_af_prime_tag       9
#define SystemState_Registers_bc_prime_tag       10
#define SystemState_Registers_de_prime_tag       11
#define SystemState_Registers_hl_prime_tag       12
#define SystemState_Registers_i_tag              13
#define SystemState_Registers_r_1_tag            14
#define SystemState_Registers_r_2_tag            15
#define Trs80Extension_model_tag                 1
#define ApiResponseMediaImages_success_tag       1
#define ApiResponseMediaImages_message_tag       2
#define ApiResponseMediaImages_mediaImage_tag    3
#define App_id_tag                               1
#define App_name_tag                             2
#define App_version_tag                          3
#define App_description_tag                      4
#define App_release_year_tag                     5
#define App_screenshot_url_tag                   6
#define App_author_tag                           7
#define App_ext_trs80_tag                        8
#define SystemState_model_tag                    1
#define SystemState_registers_tag                2
#define SystemState_memoryRegions_tag            3
#define ApiResponseApps_success_tag              1
#define ApiResponseApps_message_tag              2
#define ApiResponseApps_app_tag                  3
#define ApiResponseDownloadSystemState_success_tag 1
#define ApiResponseDownloadSystemState_message_tag 2
#define ApiResponseDownloadSystemState_systemState_tag 3
#define UploadSystemStateParams_state_tag        1

/* Struct field encoding specification for nanopb */
#define ApiResponseApps_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     success,           1) \
X(a, STATIC,   SINGULAR, STRING,   message,           2) \
X(a, STATIC,   REPEATED, MESSAGE,  app,               3)
#define ApiResponseApps_CALLBACK NULL
#define ApiResponseApps_DEFAULT NULL
#define ApiResponseApps_app_MSGTYPE App

#define ApiResponseMediaImages_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     success,           1) \
X(a, STATIC,   SINGULAR, STRING,   message,           2) \
X(a, STATIC,   REPEATED, MESSAGE,  mediaImage,        3)
#define ApiResponseMediaImages_CALLBACK NULL
#define ApiResponseMediaImages_DEFAULT NULL
#define ApiResponseMediaImages_mediaImage_MSGTYPE MediaImage

#define ApiResponseDownloadSystemState_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     success,           1) \
X(a, STATIC,   SINGULAR, STRING,   message,           2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  systemState,       3)
#define ApiResponseDownloadSystemState_CALLBACK NULL
#define ApiResponseDownloadSystemState_DEFAULT NULL
#define ApiResponseDownloadSystemState_systemState_MSGTYPE SystemState

#define ApiResponseUploadSystemState_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     success,           1) \
X(a, STATIC,   SINGULAR, STRING,   message,           2) \
X(a, STATIC,   SINGULAR, INT64,    token,             3)
#define ApiResponseUploadSystemState_CALLBACK NULL
#define ApiResponseUploadSystemState_DEFAULT NULL

#define App_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   id,                1) \
X(a, STATIC,   SINGULAR, STRING,   name,              2) \
X(a, STATIC,   SINGULAR, STRING,   version,           3) \
X(a, STATIC,   SINGULAR, STRING,   description,       4) \
X(a, STATIC,   SINGULAR, INT32,    release_year,      5) \
X(a, STATIC,   REPEATED, STRING,   screenshot_url,    6) \
X(a, STATIC,   SINGULAR, STRING,   author,            7) \
X(a, STATIC,   OPTIONAL, MESSAGE,  ext_trs80,         8)
#define App_CALLBACK NULL
#define App_DEFAULT NULL
#define App_ext_trs80_MSGTYPE Trs80Extension

#define Trs80Extension_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    model,             1)
#define Trs80Extension_CALLBACK NULL
#define Trs80Extension_DEFAULT NULL

#define MediaImage_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    type,              1) \
X(a, STATIC,   SINGULAR, STRING,   filename,          2) \
X(a, CALLBACK, SINGULAR, BYTES,    data,              3) \
X(a, STATIC,   SINGULAR, INT64,    uploadTime,        4) \
X(a, STATIC,   SINGULAR, STRING,   description,       5)
#define MediaImage_CALLBACK pb_default_field_callback
#define MediaImage_DEFAULT NULL

#define SystemState_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    model,             1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  registers,         2) \
X(a, STATIC,   REPEATED, MESSAGE,  memoryRegions,     3)
#define SystemState_CALLBACK NULL
#define SystemState_DEFAULT NULL
#define SystemState_registers_MSGTYPE SystemState_Registers
#define SystemState_memoryRegions_MSGTYPE SystemState_MemoryRegion

#define SystemState_Registers_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    ix,                1) \
X(a, STATIC,   SINGULAR, INT32,    iy,                2) \
X(a, STATIC,   SINGULAR, INT32,    pc,                3) \
X(a, STATIC,   SINGULAR, INT32,    sp,                4) \
X(a, STATIC,   SINGULAR, INT32,    af,                5) \
X(a, STATIC,   SINGULAR, INT32,    bc,                6) \
X(a, STATIC,   SINGULAR, INT32,    de,                7) \
X(a, STATIC,   SINGULAR, INT32,    hl,                8) \
X(a, STATIC,   SINGULAR, INT32,    af_prime,          9) \
X(a, STATIC,   SINGULAR, INT32,    bc_prime,         10) \
X(a, STATIC,   SINGULAR, INT32,    de_prime,         11) \
X(a, STATIC,   SINGULAR, INT32,    hl_prime,         12) \
X(a, STATIC,   SINGULAR, INT32,    i,                13) \
X(a, STATIC,   SINGULAR, INT32,    r_1,              14) \
X(a, STATIC,   SINGULAR, INT32,    r_2,              15)
#define SystemState_Registers_CALLBACK NULL
#define SystemState_Registers_DEFAULT NULL

#define SystemState_MemoryRegion_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    start,             1) \
X(a, CALLBACK, SINGULAR, BYTES,    data,              2) \
X(a, STATIC,   SINGULAR, INT32,    length,            3)
#define SystemState_MemoryRegion_CALLBACK pb_default_field_callback
#define SystemState_MemoryRegion_DEFAULT NULL

#define FetchMediaImagesParams_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   app_id,            1)
#define FetchMediaImagesParams_CALLBACK NULL
#define FetchMediaImagesParams_DEFAULT NULL

#define GetAppParams_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   app_id,            1)
#define GetAppParams_CALLBACK NULL
#define GetAppParams_DEFAULT NULL

#define ListAppsParams_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    start,             1) \
X(a, STATIC,   SINGULAR, INT32,    num,               2) \
X(a, STATIC,   SINGULAR, STRING,   query,             3) \
X(a, STATIC,   OPTIONAL, MESSAGE,  trs80,             4)
#define ListAppsParams_CALLBACK NULL
#define ListAppsParams_DEFAULT NULL
#define ListAppsParams_trs80_MSGTYPE ListAppsParams_Trs80Params

#define ListAppsParams_Trs80Params_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, UENUM,    media_types,       1)
#define ListAppsParams_Trs80Params_CALLBACK pb_default_field_callback
#define ListAppsParams_Trs80Params_DEFAULT NULL

#define UploadSystemStateParams_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  state,             1)
#define UploadSystemStateParams_CALLBACK NULL
#define UploadSystemStateParams_DEFAULT NULL
#define UploadSystemStateParams_state_MSGTYPE SystemState

#define DownloadSystemStateParams_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT64,    token,             1)
#define DownloadSystemStateParams_CALLBACK NULL
#define DownloadSystemStateParams_DEFAULT NULL

extern const pb_msgdesc_t ApiResponseApps_msg;
extern const pb_msgdesc_t ApiResponseMediaImages_msg;
extern const pb_msgdesc_t ApiResponseDownloadSystemState_msg;
extern const pb_msgdesc_t ApiResponseUploadSystemState_msg;
extern const pb_msgdesc_t App_msg;
extern const pb_msgdesc_t Trs80Extension_msg;
extern const pb_msgdesc_t MediaImage_msg;
extern const pb_msgdesc_t SystemState_msg;
extern const pb_msgdesc_t SystemState_Registers_msg;
extern const pb_msgdesc_t SystemState_MemoryRegion_msg;
extern const pb_msgdesc_t FetchMediaImagesParams_msg;
extern const pb_msgdesc_t GetAppParams_msg;
extern const pb_msgdesc_t ListAppsParams_msg;
extern const pb_msgdesc_t ListAppsParams_Trs80Params_msg;
extern const pb_msgdesc_t UploadSystemStateParams_msg;
extern const pb_msgdesc_t DownloadSystemStateParams_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define ApiResponseApps_fields &ApiResponseApps_msg
#define ApiResponseMediaImages_fields &ApiResponseMediaImages_msg
#define ApiResponseDownloadSystemState_fields &ApiResponseDownloadSystemState_msg
#define ApiResponseUploadSystemState_fields &ApiResponseUploadSystemState_msg
#define App_fields &App_msg
#define Trs80Extension_fields &Trs80Extension_msg
#define MediaImage_fields &MediaImage_msg
#define SystemState_fields &SystemState_msg
#define SystemState_Registers_fields &SystemState_Registers_msg
#define SystemState_MemoryRegion_fields &SystemState_MemoryRegion_msg
#define FetchMediaImagesParams_fields &FetchMediaImagesParams_msg
#define GetAppParams_fields &GetAppParams_msg
#define ListAppsParams_fields &ListAppsParams_msg
#define ListAppsParams_Trs80Params_fields &ListAppsParams_Trs80Params_msg
#define UploadSystemStateParams_fields &UploadSystemStateParams_msg
#define DownloadSystemStateParams_fields &DownloadSystemStateParams_msg

/* Maximum encoded size of messages (where known) */
/* ApiResponseMediaImages_size depends on runtime parameters */
/* ApiResponseDownloadSystemState_size depends on runtime parameters */
/* MediaImage_size depends on runtime parameters */
/* SystemState_size depends on runtime parameters */
/* SystemState_MemoryRegion_size depends on runtime parameters */
/* ListAppsParams_size depends on runtime parameters */
/* ListAppsParams_Trs80Params_size depends on runtime parameters */
/* UploadSystemStateParams_size depends on runtime parameters */
#define ApiResponseApps_size                     9963
#define ApiResponseUploadSystemState_size        94
#define App_size                                 1973
#define DownloadSystemStateParams_size           11
#define FetchMediaImagesParams_size              41
#define GetAppParams_size                        41
#define SystemState_Registers_size               165
#define Trs80Extension_size                      2

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
