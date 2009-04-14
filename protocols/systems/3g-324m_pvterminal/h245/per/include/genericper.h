/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
#ifndef GENERICPER
#define GENERICPER

#include "oscl_base.h"
#include "per_common.h"

/*========================================*/
/*========== MISCELLANEOUS DEFS ==========*/
/*========================================*/

#define EPASS (void (*)(uint8*,PS_OutStream))


/*=========================================================*/
/*========== STRUCTURE DEFINITIONS (Generic PER) ==========*/
/*=========================================================*/

typedef struct _OCTETSTRING    /* ASN OCTET STRING */
{
    uint16 size;
    uint8* data;
} S_OCTETSTRING;
typedef S_OCTETSTRING *PS_OCTETSTRING;

typedef struct _BITSTRING    /* ASN BIT STRING */
{
    uint16 size;
    uint8* data;
} S_BITSTRING;

typedef S_BITSTRING *PS_BITSTRING;

typedef struct _int8STRING    /* ASN int8 STRING */
{
    uint16 size;
    uint8* data;
} S_int8STRING;

typedef S_int8STRING *PS_int8STRING;

typedef struct _OBJECTIDENT    /* ASN OBJECT IDENTIFIER */
{
    uint16 size;
    uint8* data;
} S_OBJECTIDENT;

typedef S_OBJECTIDENT *PS_OBJECTIDENT;

typedef struct _UnknownSigMap    /* Unknown SigMap */
{
    uint16 size;
    uint8* optionFlags;
    uint16 extensionsRead;
} S_UnknownSigMap;

typedef S_UnknownSigMap *PS_UnknownSigMap;

typedef struct _InStream    /* Input Stream */
{
    uint8* data;           /* Current byte in stream */
    uint8 bitIndex;        /* Next bit within the byte */
} S_InStream;

typedef S_InStream *PS_InStream;

typedef struct _OutStream    /* Output Stream */
{
    uint8* data;           /* Start of allocated space */
    uint16 size;           /* Size of allocated space */
    uint16 byteIndex;      /* Index: next byte to write */
    uint16 bitIndex;       /* Index: next bit to write */
    uint8 buildByte;       /* Next byte, under construction */
} S_OutStream;

typedef S_OutStream *PS_OutStream;

/*=========================================================*/
/*============ DECODING ROUTINES (Generic PER) ============*/
/*=========================================================*/
/* ------------- LOW LEVEL STREAM -------------- */
uint8 ReadBits(uint32 number, PS_InStream stream);
void  ReadRemainingBits(PS_InStream stream);
void  ReadOctets(uint32 number, uint8* octets, uint8 reorder, PS_InStream stream);

/* ------------- HIGH LEVEL ASN DATA ------------- */
uint8 GetBoolean(PS_InStream stream);
uint32  GetInteger(uint32 lower, uint32 upper, PS_InStream stream);
int32   GetSignedInteger(int32 lower, int32 upper, PS_InStream stream);
uint32  GetUnboundedInteger(PS_InStream stream);
uint32  GetExtendedInteger(uint32 lower, uint32 upper, PS_InStream stream);
void  GetOctetString(uint8 unbounded, uint32 min, uint32 max,
                     PS_OCTETSTRING x, PS_InStream stream);
void  GetBitString(uint8 unbounded, uint32 min, uint32 max,
                   PS_BITSTRING x, PS_InStream stream);
void  GetCharString(const char *stringName,
                    uint8 unbounded, uint32 min, uint32 max, const char *from,
                    PS_int8STRING x, PS_InStream stream);
void  GetObjectID(PS_OBJECTIDENT x, PS_InStream stream);

/* ------------- OTHER CALLS ----------------*/
uint32 GetLengthDet(PS_InStream stream);
/* General length det, e.g. for extension wrapper */
uint32 GetNormSmallLength(PS_InStream stream);
/* e.g. for length of extensions SigMap */
uint32 GetNormSmallValue(PS_InStream stream);
/* e.g. for choice index when extension is ON */
void SkipOneExtension(PS_InStream stream);
/* Reads a General Length Det, skips that many octets. */
void SkipAllExtensions(PS_InStream stream);
/* Reads SigMap including length.  Skips each */
/*   extension which SigMap says is present.  */
/* NOTE: Not generated by MiniParser.  Delete? */
uint16 GetChoiceIndex(uint32 rootnum, uint8 extmarker, PS_InStream stream);
/* Gets (possibly extended) choice index */
PS_UnknownSigMap GetUnknownSigMap(PS_InStream stream);
/* Gets the unknown sig-map for SEQUENCE extensions */
uint8 SigMapValue(uint32 index, PS_UnknownSigMap map);
/* Reads a value from the unknown sig-map */
void ExtensionPrep(PS_UnknownSigMap map, PS_InStream stream);
/* Does ++map->extensionsRead */
/* Also calls GetLengthDet(stream) to skip wrapper */
uint32 SkipUnreadExtensions(PS_UnknownSigMap, PS_InStream stream);
/* Use map->extensionsRead to determine how many have */
/*   been read, and how many are left to be read. */
/* Skips unread exts via calls to SkipOneExtension() */
/* Frees the map */
/* Returns the number of exts skipped. */
void SkipOneOctet(PS_InStream stream);

/*=========================================================*/
/*============ ENCODING ROUTINES (Generic PER) ============*/
/*=========================================================*/

/* ------------- LOW LEVEL STREAM -------------- */
void WriteBits(uint32 number, uint8 bits, PS_OutStream stream);
void WriteRemainingBits(PS_OutStream stream);
void WriteOctets(uint32 number, uint8* octets, uint8 reorder, PS_OutStream stream);
PS_OutStream NewOutStream(void);
void ExpandOutStream(PS_OutStream x);
void FreeOutStream(PS_OutStream x);

/* ------------- HIGH LEVEL ASN DATA ------------- */
void PutBoolean(uint32 value, PS_OutStream stream);
void PutInteger(uint32 lower, uint32 upper, uint32 value, PS_OutStream stream);
void PutSignedInteger(int32 lower, int32 upper, int32 value, PS_OutStream stream);
void PutUnboundedInteger(uint32 value, PS_OutStream stream);
void PutExtendedInteger(uint32 lower, uint32 upper, uint32 value, PS_OutStream stream);
void PutOctetString(uint8 unbounded,
                    uint32 min, uint32 max, PS_OCTETSTRING x, PS_OutStream stream);
void PutBitString(uint8 unbounded,
                  uint32 min, uint32 max, PS_BITSTRING x, PS_OutStream stream);
void PutCharString(const char *stringName,
                   uint8 unbounded, uint32 min, uint32 max, const char *from,
                   PS_int8STRING x, PS_OutStream stream);
void PutObjectID(PS_OBJECTIDENT x, PS_OutStream stream);
void PutExtensionNull(PS_OutStream stream);
void PutExtensionBoolean(uint32 value, PS_OutStream stream);
void PutExtensionInteger(uint32 lower, uint32 upper, uint32 value, PS_OutStream stream);
void PutExtensionOctetString(uint8 unbounded,
                             uint32 min, uint32 max, PS_OCTETSTRING x, PS_OutStream stream);

/* ------------- OTHER CALLS ----------------*/
void PutNormSmallValue(uint32 value, PS_OutStream stream);
/* e.g. for choice index when extension is ON */
void PutChoiceIndex(uint32 rootnum, uint8 extmarker, uint32 index, PS_OutStream stream);
/* Writes extension bit, choice index */
void PutNormSmallLength(uint32 value, PS_OutStream stream);
/* e.g. for length of extensions SigMap */
void PutLengthDet(uint32 value, PS_OutStream stream);
/* General length det, e.g. for extension wrapper */
void PutExtensionItem(
    void (*Func)(uint8* x, PS_OutStream stream),
    uint8* x, PS_OutStream stream);
/* Write extension item, including length det */
void PutTempStream(PS_OutStream tempStream, PS_OutStream stream);
/* Copy contents of tempStream to the real one */

/*======================================================*/
/*============ OTHER ROUTINES (Generic PER) ============*/
/*======================================================*/
void ErrorMessage(const char *msg);
void ErrorMessageAndLeave(const char *msg);
/* Generic error call.  Just print it for now */
PS_InStream ConvertOutstreamToInstream(PS_OutStream outstream);
PS_OCTETSTRING NewOctetString(void);
PS_BITSTRING NewBitString(void);
PS_int8STRING NewCharString(void);
PS_OBJECTIDENT NewObjectID(void);
void InitOctetString(PS_OCTETSTRING x);
void InitBitString(PS_BITSTRING x);
void InitCharString(PS_int8STRING x);
void InitObjectid(PS_OBJECTIDENT x);
void FreeOctetString(PS_OCTETSTRING x);
void FreeBitString(PS_BITSTRING x);
void FreeCharString(PS_int8STRING x);
void FreeObjectID(PS_OBJECTIDENT x);
#endif
