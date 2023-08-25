#include "debug.h"
#include "basemacro.h"
#include <stdio.h>

Vt hexDump(C Ct*d,C Vt*a,C It l,It p){
 if(p<4||p>64)p=16;Ct b[p+1];
 C Ct*pc=(C Ct*)a;
 if(d!=NULL)O("%s:\n",d);
 if(l<=0)R;It i=0;
 while(i<l){
  if((i%p)==0){if(i!=0)O("  %s\n",b);O("  %04x ",i);}
  O(" %02x",pc[i]);
  if(pc[i]<0x20||pc[i]>0x7e)b[i%p]='.';else b[i%p]=pc[i];
  b[(i%p)+1]='\0';i++;}
 while((i%p)!=0){O("   ");i++;}O("  %s\n",b);
}
