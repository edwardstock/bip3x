//
// Created by edwar on 22-Oct-19.
//

#ifdef PACKED
#define TMP_PACKED PACKED
#endif

#ifdef _MSC_VER // for WIN
#define PACKED
#pragma pack(push,1)
#else // GNU
#define PACKED __attribute__ ((__packed__))
#endif // END
