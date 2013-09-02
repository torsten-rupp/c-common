#include <stdlib.h>
#include <assert.h>

#include "global.h"
#include "stringmaps.h"
#include "strings.h"

typedef enum
{
  A=123,
  B,
  C,
  UNKNOWN
} Enum;

static bool parse(const char *name, Enum *value)
{
  if      (strcmp(name,"A") == 0) { (*value) = A; return TRUE;  }
  else if (strcmp(name,"B") == 0) { (*value) = B; return TRUE;  }
  else if (strcmp(name,"C") == 0) { (*value) = C; return TRUE;  }
  else                            {               return FALSE; }
}

int main(int argc, char *argv[])
{
  StringMap      stringMap;
  uint           z;
  const char     *name;
  StringMapValue value;
  String         s;
  int            i;
  int64          l;
  double         d;
  bool           b;
  Enum           e;
  char           ch;
  char           buffer[256];
  String         string;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  stringMap = StringMap_new();

  StringMap_putInt(stringMap,"a",123);
  StringMap_putDouble(stringMap,"b",456.789);
  StringMap_putCString(stringMap,"c","Hello World!");

  STRINGMAP_ITERATE(stringMap,z,name,value)
  {
    printf("%s: %p\n",name,value.data.p);
  }

  printf("a=%d\n",StringMap_get(stringMap,"a").data.i);
  printf("b=%lf\n",StringMap_get(stringMap,"b").data.d);
  printf("c=%s\n",StringMap_get(stringMap,"c").data.s);

  StringMap_remove(stringMap,"b");

  STRINGMAP_ITERATE(stringMap,z,name,value)
  {
    printf("%s: %p\n",name,value.data.p);
  }

  printf("a=%d\n",StringMap_get(stringMap,"a").data.i);
  printf("b=%lf\n",StringMap_get(stringMap,"b").data.d);
  printf("c=%s\n",StringMap_get(stringMap,"c").data.s);

  StringMap_delete(stringMap);

  // map parser
  s = String_new();
//  String_setCString(s,"a=123 b=123456789 c=456.789 e=B f=A g=Fight h=\"Hello \\\"World!\\\"\" d=yes");
  String_setCString(s,"a=123 h=\"Hello \\\"World!\\\"\" d=yes");
  printf("String: %s\n",String_cString(s));

  string    = String_new();
  stringMap = StringMap_new();
  if (StringMap_parse(stringMap,s,"'\"",0,NULL))
  {
    StringMap_getInt(stringMap,"a",&i,0); printf("a=%d\n",i);
    StringMap_getInt64(stringMap,"b",&l,0); printf("b=%lld\n",l);
    StringMap_getDouble(stringMap,"c",&d,0.0); printf("c=%lf\n",d);
    StringMap_getBool(stringMap,"d",&b,FALSE); printf("d=%d\n",b);
    StringMap_getEnum(stringMap,"e",&e,(StringMapParseFunction)parse,UNKNOWN); printf("e=%d\n",e);
    StringMap_getChar(stringMap,"f",&ch,'\0'); printf("f=%c\n",ch);
    StringMap_getCString(stringMap,"g",buffer,sizeof(buffer),NULL); printf("g=%s\n",buffer);
    StringMap_getString(stringMap,"h",string,NULL); printf("h=%s\n",String_cString(string));
fprintf(stderr,"%s, %d: %d\n",__FILE__,__LINE__,
    StringMap_getString(stringMap,"d",string,NULL)
    );
  }
  StringMap_delete(stringMap);
  String_delete(string);
  String_delete(s);

  return 0;
}
