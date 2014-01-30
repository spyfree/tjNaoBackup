#ifndef COMMONDATA_H
#define COMMONDATA_H
#undef SIZEOF_INT
#define SIZEOF_INT        4
#undef SIZEOF_BYTE
#define SIZEOF_BYTE       1
#undef SIZEOF_FLOAT
#define SIZEOF_FLOAT      SIZEOF_INT
#undef SIZEOF_LLONG
#define SIZEOF_LLONG      8
#undef SIZEOF_DOUBLE
#define SIZEOF_DOUBLE     SIZEOF_LLONG

#define TYPE_INT          0
#define TYPE_BYTE         1
#define TYPE_FLOAT        2
#define TYPE_LONG         3
#define TYPE_DOUBLE       4
#define TYPE_INT_ARRAY    5
#define TYPE_BYTE_ARRAY   6
#define TYPE_FLOAT_ARRAY  7
#define TYPE_LONG_ARRAY   8
#define TYPE_DOUBLE_ARRAY 9
typedef long long llong;
#ifndef byte
typedef unsigned char byte;
#endif


#endif
