/***********************************************************************\
*
* Contents: semaphores demo
* Systems: all
*
\***********************************************************************/

/****************************** Includes *******************************/
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "common/global.h"
#include "common/stringmaps.h"
#include "common/strings.h"

/****************** Conditional compilation switches *******************/

/***************************** Constants *******************************/

/***************************** Datatypes *******************************/
typedef enum
{
  A=123,
  B,
  C,
  UNKNOWN
} Enum;

typedef struct
{
  int a;
  int b;
  int c;
} Data;

/***************************** Variables *******************************/

/****************************** Macros *********************************/

/***************************** Forwards ********************************/

/***************************** Functions *******************************/

static bool parseEnum(const char *name, Enum *value, void *userData)
{
  UNUSED_VARIABLE(userData);

  if      (strcmp(name,"A") == 0) { (*value) = A; return TRUE;  }
  else if (strcmp(name,"B") == 0) { (*value) = B; return TRUE;  }
  else if (strcmp(name,"C") == 0) { (*value) = C; return TRUE;  }
  else                            {               return FALSE; }
}

static String formatData(const Data *data, void *userData)
{
  UNUSED_VARIABLE(userData);

  return String_format(String_new(),
                       "%d-%d-%d",
                       data->a,
                       data->b,
                       data->c
                      );
}

static bool parseData(const String string, const Data *data, void *userData)
{
  UNUSED_VARIABLE(userData);

  return String_scan(string,0,"%d-%d-%d",&data->a,&data->b,&data->c);
}

int main(int argc, char *argv[])
{
  StringMap      stringMap;
  const char     *name;
  StringMapTypes type;
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
  Data           data;

  UNUSED_VARIABLE(argc);
  UNUSED_VARIABLE(argv);

  stringMap = StringMap_new();

  data.a = 11;
  data.b = 22;
  data.c = 33;

  StringMap_putInt(stringMap,"a",123);
  StringMap_putDouble(stringMap,"b",456.789);
  StringMap_putCString(stringMap,"c","Hello World!");
  StringMap_putData(stringMap,"d",&data,(StringMapFormatFunction)formatData,NULL);

  STRINGMAP_ITERATE(stringMap,name,type,value)
  {
    printf("%d %s: %s/%p\n",type,name,String_cString(value.text),value.data.p);
  }

  printf("a=%d\n",StringMap_get(stringMap,"a").data.i);
  printf("b=%lf\n",StringMap_get(stringMap,"b").data.d);
  printf("c=%s\n",StringMap_get(stringMap,"c").data.s);
  printf("d=%s\n",String_cString(StringMap_get(stringMap,"d").text));

  StringMap_remove(stringMap,"b");

  STRINGMAP_ITERATE(stringMap,name,type,value)
  {
    printf("%s: %p\n",name,value.data.p);
  }

  printf("a=%d\n",StringMap_get(stringMap,"a").data.i);
  printf("b=%lf\n",StringMap_get(stringMap,"b").data.d);
  printf("c=%s\n",StringMap_get(stringMap,"c").data.s);

  StringMap_delete(stringMap);

  // map parser
  s = String_new();
  String_setCString(s,"a=123 b=123456789 c=456.789 e=B f=A g=Fight h=\"Hello \\\"World!\\\"\" d=yes i=3-2-1");
//  String_setCString(s,"a=123 h=\"Hello \\\"World!\\\"\" d=yes");
  printf("String: %s\n",String_cString(s));

  string    = String_new();
  stringMap = StringMap_new();
  if (StringMap_parse(stringMap,s,STRINGMAP_ASSIGN,"'\"",NULL,0,NULL))
  {
    StringMap_getInt(stringMap,"a",&i,0); printf("a=%d\n",i);
    StringMap_getInt64(stringMap,"b",&l,0); printf("b=%"PRIi64"\n",l);
    StringMap_getDouble(stringMap,"c",&d,0.0); printf("c=%lf\n",d);
    StringMap_getBool(stringMap,"d",&b,FALSE); printf("d=%d\n",b);
    StringMap_getEnum(stringMap,"e",&e,(StringMapParseEnumFunction)parseEnum,NULL,UNKNOWN); printf("e=%d\n",e);
    StringMap_getChar(stringMap,"f",&ch,'\0'); printf("f=%c\n",ch);
    StringMap_getCString(stringMap,"g",buffer,sizeof(buffer),NULL); printf("g=%s\n",buffer);
    StringMap_getString(stringMap,"h",string,NULL); printf("h=%s\n",String_cString(string));
    StringMap_getData(stringMap,"i",&data,(StringMapParseFunction)parseData,NULL); printf("i=%d,%d,%d\n",data.a,data.b,data.c);
  }
  StringMap_delete(stringMap);
  String_delete(string);
  String_delete(s);

  return 0;
}

/* end of file */
