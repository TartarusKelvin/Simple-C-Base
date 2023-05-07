/*
  I hate typing too much, used for .c files only
*/
#ifndef basemacro_scbase
#define scbase
#include<stddef.h>
typedef size_t SZt;
typedef void Vt;
typedef char Ct;
typedef int It;
#define C const 
#define SIZE sizeof
#define MEMCL(x,l) memset(x,0,l);
#define R return
#define REPEAT(x) for(int i=0;i<x;i++)
#define O printf
#endif
