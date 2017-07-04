/****************************************************************************
*
*    Copyright (c) 2005 - 2012 by Vivante Corp.  All rights reserved.
*
*    The material in this file is confidential and contains trade secrets
*    of Vivante Corporation. This is proprietary information owned by
*    Vivante Corporation. No part of this work may be disclosed,
*    reproduced, copied, transmitted, or used in any way for any purpose,
*    without the express written permission of Vivante Corporation.
*
*****************************************************************************/


#ifndef __GAL_UTIL_H__
#define __GAL_UTIL_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gc_hal.h"
#include "gc_hal_raster.h"

#define MAX_BUFFER_SIZE 1024
#define MAX_ARG_COUNT   20

#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN  extern "C" {
#else
#define EXTERN_C_BEGIN
#endif
#endif

#ifndef EXTERN_C_END
#ifdef __cplusplus
#define EXTERN_C_END    }
#else
#define EXTERN_C_END
#endif
#endif

EXTERN_C_BEGIN

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

typedef enum _GalOutputType {
    GalOutputType_Error     = 0x00000001, /* This is the error message. If something is unexpected, then it's an error. */
    GalOutputType_Result    = 0x00000002, /* This is the data generated by the case for manual analysis. */
    GalOutputType_Log       = 0x00000004, /* This means the data to output is general log. */
    GalOutputType_Console   = 0x00000008, /* Output the data to Console. */
} GalOutputType;

#ifdef __DLL_EXPORT
#define DLL_API     __declspec(dllexport)
#else
#define DLL_API     __declspec(dllimport)
#endif

#ifndef CDECL
#define CDECL __cdecl
#endif

#undef CDECL
#undef DLL_API
#define CDECL
#define DLL_API

typedef struct GalRuntime {
    gcoOS           os;
    gcoHAL          hal;
    gco2D           engine2d;

    gceCHIPMODEL    ChipModel;
    gctUINT32       ChipRevision;
    gctUINT32       ChipFeatures;
    gctUINT32       ChipMinorFeatures;

    gcoSURF         target;
    gctUINT         width;
    gctUINT         height;
    gceSURF_FORMAT  format;
    gctINT          argc;
    gctPOINTER      argv;
    gctBOOL         pe20;
    gctBOOL         fullDFB;
} GalRuntime;

typedef struct _GalTest GalTest;

typedef gctBOOL CDECL (* PGalRender)    (GalTest *object, gctUINT frameNo);
typedef void CDECL (* PGalDestroy)   (GalTest *object);

typedef GalTest * CDECL (* PGalCreateTestObject)(const GalRuntime *runtime);

GalTest * GalCreateTestObject(GalRuntime *runtime);

struct _GalTest {
    PGalRender      render;
    PGalDestroy     destroy;
    gctUINT         frameCount;
    gctCONST_STRING description;
};

DLL_API void CDECL GalInitializeOutput(GalOutputType type, const char *name);
DLL_API void CDECL GalFinalizeOutput();
DLL_API void CDECL GalOutput(GalOutputType type, const char *format, ...);
DLL_API void GalPrintf(const char *format, ...);

typedef struct __GalMonoStream
{
    gceSURF_MONOPACK type;
    gctPOINTER data;
    gctUINT32 width;
    gctUINT32 height;
} GalMonoStream, *GalMonoStream_PTR;

/********************************* BMP header ******************************/
#define BF_TYPE 0x4D42             /* "MB" */

#pragma pack(1)
/**** BMP file header structure ****/
typedef struct BMPFILEHEADER {
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} BMPFILEHEADER;

/**** BMP file info structure ****/
typedef struct BMPINFOHEADER {
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} BMPINFOHEADER;

/*
 * Constants for the biCompression field...
 */

#define BIT_RGB       0             /* No compression - straight BGR data */
#define BIT_RLE8      1             /* 8-bit run-length compression */
#define BIT_RLE4      2             /* 4-bit run-length compression */
#define BIT_BITFIELDS 3             /* RGB bitmap with RGB masks */

/**** Colormap entry structure ****/
typedef struct RGB {
    unsigned char   rgbBlue;          /* Blue value */
    unsigned char   rgbGreen;         /* Green value */
    unsigned char   rgbRed;           /* Red value */
    unsigned char   rgbReserved;      /* Reserved */
} RGB;

/**** Bitmap information structure ****/
typedef struct _BMPINFO {
    BMPINFOHEADER   bmiHeader;      /* Image header */
    union {
    RGB             bmiColors[256]; /* Image colormap */
    gctUINT         mask[3];        /* RGB masks */
    };
} BMPINFO;
#pragma pack()

/********************************* YUV header ******************************/
/* YUV_HEADER type list. */

#define YUV_HEADER_TYPE_YUY2 1
#define YUV_HEADER_TYPE_UYVY 2
#define YUV_HEADER_TYPE_I420 3
#define YUV_HEADER_TYPE_YV12 4
#define YUV_HEADER_TYPE_NV12 5
#define YUV_HEADER_TYPE_NV16 6
#define YUV_HEADER_TYPE_NV21 7
#define YUV_HEADER_TYPE_NV61 8

#pragma pack(1)
typedef struct
{
    gctUINT32 Width;
    gctUINT32 Height;
    gctUINT8 Type;
} YUV_HEADER;
#pragma pack()

#define VIMG_MAGIC 0x474D4956             /* "VIMG" */

#pragma pack(1)

typedef struct
{
    unsigned int magic;
    unsigned int format;
    unsigned int tiling;
    unsigned int rectWidth;
    unsigned int rectHeight;
    unsigned int imageWidth;
    unsigned int imageHeight;
    unsigned int bitsOffset;
    unsigned int size;
    unsigned int tileStatusOptionOffset;
    unsigned int reserved2;
} VIMG_HEADER;

typedef struct
{
    unsigned int tileStatusConfig;
    unsigned int compressedFormat;
    unsigned int clearValue;
    unsigned int bitsOffset;
    unsigned int size;
    unsigned int reserved;
} VIMG_HEADER_TILE_STATUS;

#pragma pack()

#define MAX_ADDRESS_NUM 3

#if gcvVERSION_MAJOR >= 4
typedef struct
{
    gceSURF_FORMAT format;
    gceTILING tiling;
    gceSURF_ROTATION rotation;
    gctUINT32 width;
    gctUINT32 height;
    gctUINT32 aWidth;
    gctUINT32 aHeight;
    gctUINT32 validAddressNum;
    gctUINT32 address[MAX_ADDRESS_NUM];
    gctPOINTER logical[MAX_ADDRESS_NUM];
    gctUINT32 stride[MAX_ADDRESS_NUM];
    gctUINT32 validStrideNum;
    struct {
        gcuVIDMEM_NODE_PTR node;
        gcePOOL pool;
        gctUINT size;
        gctUINT32 address;
        gctPOINTER memory;
    } vNode, tileStatusNode;

    gce2D_TILE_STATUS_CONFIG tileStatusConfig;
    gceSURF_FORMAT tileStatusFormat;
    gctUINT32 tileStatusClear;
    gctUINT32 tileStatusAddress;
    gctPOINTER tileStatuslogical;
} T2D_SURF;

typedef T2D_SURF *T2D_SURF_PTR;
#endif

#define COLOR_ARGB8(A, R, G, B) \
    ((((A)&0xFF) << 24) | (((R)&0xFF) << 16) | (((G)&0xFF) << 8) | ((B)&0xFF))

#define COLOR_XRGB8(R, G, B) \
    ((((R)&0xFF) << 16) | (((G)&0xFF) << 8) | ((B)&0xFF))

#define COLOR_RGBA8(R, G, B, A) \
    ((((R)&0xFF) << 24) | (((G)&0xFF) << 16) | (((B)&0xFF) << 8) | ((A)&0xFF))

#define COLOR_R5G6B5(R, G, B) \
    ((((R)&0x1F) << 11) | (((G)&0x3F) << 5) | ((B)&0x1F))

#define COLOR_A1R5G5B5(A, R, G, B) \
    ((((A)&0x1) << 15) | (((R)&0x1F) << 10) | (((G)&0x1F) << 5) | ((B)&0x1F))

#define COLOR_X1R5G5B5(R, G, B) \
    ((((R)&0x1F) << 10) | (((G)&0x1F) << 5) | ((B)&0x1F))

#define COLOR_ARGB4(A, R, G, B) \
    ((((A)&0x0F) << 12) | (((R)&0x0F) << 8) | (((G)&0x0F) << 4) | ((B)&0x0F))

#define COLOR_XRGB4(R, G, B) \
    ((((R)&0x0F) << 8) | (((G)&0x0F) << 4) | ((B)&0x0F))

#define CONVERT_BYTE(n) \
    ((((n) & 0xFF) << 24) | (((n) & 0xFF00) << 8) | (((n) & 0xFF0000) >> 8) | (((n) & 0xFF000000) >> 16))

#define PATTERN_0 0x0018242424241800LL
#define PATTERN_1 0x003C181818381800LL
#define PATTERN_2 0x003C100804241800LL
#define PATTERN_3 0x0018240408241800LL
#define PATTERN_4 0x000404043C242400LL
#define PATTERN_5 0x0018240438203800LL
#define PATTERN_6 0x0018242418080400LL
#define PATTERN_7 0x0010101008043C00LL
#define PATTERN_8 0x0018242418241800LL
#define PATTERN_9 0x0020101824241800LL
#define PATTERN_A 0x0042427E24241800LL

#define GAL_POINTER_OFFSET(p, offset) \
    ((gctPOINTER)((gctUINT32)(p) + (offset)))

/*************************** Exported API ************************************/
DLL_API unsigned char * CDECL GalLoadDIBitmap(
    const char *filename,
    BMPINFO **info
    );

DLL_API int CDECL GalSaveDIBitmap(
    const char *filename,
    BMPINFO *info,
    unsigned char *bits,
    int alignedWidth
    );

DLL_API gcoSURF CDECL GalLoadDIB2Surface(
    gcoHAL hal,
    const char *filename
    );

DLL_API gceSTATUS CDECL Gal2DCleanSurface(
    gcoHAL hal,
    gcoSURF surface,
    gctUINT32 color
    );

DLL_API gctBOOL CDECL GalSaveSurface2DIB(
    gcoSURF surface,
    const char *bmpFileName
    );

DLL_API gctBOOL CDECL Gal2DRectangle(
    gcoHAL hal,
    gcoSURF surface,
    gcoBRUSH brush,
    gcsRECT rect
    );

DLL_API const char* CDECL GalStatusString(
    gceSTATUS status
    );

DLL_API gceSTATUS CDECL GalPackStream(
    gctUINT8_PTR UnpackedStream,
    gctUINT32 StreamWidth,
    gctUINT32 StreamHeight,
    gceSURF_MONOPACK Pack,
    gctUINT8_PTR * PackedStream
    );

DLL_API gcoSURF CDECL GalLoadYUV2Surface(
    gcoHAL hal,
    char *filename
    );

DLL_API gceSTATUS CDECL GalLoadYUV2Surface2(
    IN gcoOS Os,
    IN gcoHAL Hal,
    IN gctCONST_STRING filename,
    OUT gcoSURF *surface
    );

DLL_API gctUINT32 CDECL GalGetStretchFactor(
    gctINT32 SrcSize,
    gctINT32 DestSize
    );

DLL_API gceSTATUS CDECL GalGetStretchFactors(
    IN gcsRECT_PTR SrcRect,
    IN gcsRECT_PTR DestRect,
    OUT gctUINT32 * HorFactor,
    OUT gctUINT32 * VerFactor
    );

DLL_API gctBOOL CDECL GalFlushCache(
    gctPOINTER addr,
    gctSIZE_T size
    );

DLL_API gcoSURF CDECL GalLoadA82Surface(
    gcoHAL hal,
    const char *filename
    );

DLL_API gctBOOL CDECL GalRandRect(
    gcsRECT* rect,
    gctUINT rectNum,
    gctUINT rectWidth,
    gctUINT rectHeight,
    gctUINT surfWidth,
    gctUINT surfHeight
    );

DLL_API gceSTATUS CDECL Gal2DStartPixelEfficiencyCount(
    gcoOS os,
    gctUINT32* startClock
    );

DLL_API gctUINT32 CDECL Gal2DStopPixelEfficiencyCount(
    gcoOS os,
    gctUINT32* endClock,
    gctUINT32* pixelCount
    );

DLL_API gceSTATUS CDECL GalGetCurrentCycles(
    gcoOS os,
    gctUINT32* cycle
    );

DLL_API gceSTATUS CDECL GalQueryUVStride(
    IN gceSURF_FORMAT Format,
    IN gctUINT32 yStride,
    OUT gctUINT32_PTR uStride,
    OUT gctUINT32_PTR vStride
    );

DLL_API gceSTATUS CDECL
GalIsYUVFormat(IN gceSURF_FORMAT Format);

DLL_API gceSTATUS CDECL
GalStrSearch(
    IN gctCONST_STRING String,
    IN gctCONST_STRING SubString,
    OUT gctSTRING * Output
    );

DLL_API gceSTATUS CDECL
GalColorConvertFromARGB8(
    IN gceSURF_FORMAT Format,
    IN gctUINT32 NumColors,
    IN gctUINT32_PTR Color32,
    OUT gctUINT32_PTR Color
    );

DLL_API gceSTATUS CDECL
GalCreateMonoStream(
    IN gcoOS Os,
    IN gctCONST_STRING filename,
    OUT GalMonoStream_PTR *MonoStream
    );

DLL_API gceSTATUS CDECL
GalDestroyMonoStream(
    IN gcoOS Os,
    IN GalMonoStream_PTR MonoStream
    );

DLL_API gceSTATUS CDECL
GalLoadImageToSurface(
    IN gcoOS Os,
    IN gcoHAL Hal,
    IN gco2D egn2D,
    IN gctCONST_STRING sourcefile,
    IN gcoSURF surface);

#if gcvVERSION_MAJOR >= 4
DLL_API gceSTATUS
GalCreateTSurf(
    IN gcoHAL Hal,
    IN gceSURF_FORMAT Format,
    IN gceTILING Tiling,
    IN gce2D_TILE_STATUS_CONFIG TileStatusConfig,
    IN gctUINT32 Width,
    IN gctUINT32 Height,
    OUT T2D_SURF_PTR *Surface
    );

DLL_API gceSTATUS
GalDeleteTSurf(
    IN gcoHAL Hal,
    IN T2D_SURF_PTR Surface
    );

DLL_API gceSTATUS
GalLoadVIMG2Surface(
    IN gcoOS Os,
    IN gcoHAL Hal,
    IN gctCONST_STRING Filename,
    OUT T2D_SURF_PTR *Surface
    );
#endif

DLL_API gceSURF_FORMAT
GalQueryFormat(
               const char *name
               );

DLL_API const char *
GalQueryFormatStr(
                  gceSURF_FORMAT format
                  );

/*****************************************************************************/
EXTERN_C_END

#endif // __GAL_UTIL_H__
// __GAL_UTIL_H__
// __GAL_UTIL_H__
// __GAL_UTIL_H__
// __GAL_UTIL_H__
// __GAL_UTIL_H__
