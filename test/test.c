#include "xml_parser.h"

#define XML_FILE_NAME       "test_file.xml"

void testXmlParser(void)
{
    printf("TEST STARTED\n");
    char     lStr[256] = {0};
    uint32   lU32 = 0;
    bool     lBool = false;
    xmlNode *lpNode = NULL;
    xmlDoc  *lpDoc  = xmlGetDocNode(XML_FILE_NAME);
    xmlNode *lpRoot = xmlGetRootElement(lpDoc);
    printf("root element=%s\n", lpRoot->name);

    printf("reading node <req1>\n");
    lpNode = xmlGetChildElementByName(lpRoot, "req1");
    
    xmlGetChildVal(lpNode, "number", (void *)&lU32, XML_DATA_TYPE_UINT32);
    printf("\nreq1.number=%u\n", lU32);
    
    xmlGetChildVal(lpNode, "ip", (void *)lStr, XML_DATA_TYPE_STRING);
    printf("req1.ip=%s\n", lStr);

    printf("\nreading node <req2>\n");
    lpNode = xmlGetChildElementByName(lpRoot, "req2");

    xmlGetChildVal(lpNode, "name", (void *)lStr, XML_DATA_TYPE_STRING);
    printf("req2.name=%s\n", lStr);
    
    xmlGetChildVal(lpNode, "salary", (void *)&lU32, XML_DATA_TYPE_UINT32);
    printf("req2.salary=%u\n", lU32);

    printf("\nreading node <req3>\n");
    lpNode = xmlGetChildElementByName(lpRoot, "req3");

    if(xmlGetChildValByIdx(lpNode, "id", 2, (void *)&lU32, XML_DATA_TYPE_UINT32))
        printf("req3.id[2]=%u\n", lU32);

    if(xmlGetChildValByIdx(lpNode, "id", 1, (void *)&lU32, XML_DATA_TYPE_UINT32))
        printf("req3.id[1]=%u\n", lU32);

    if(xmlGetChildValByIdx(lpNode, "id", 0, (void *)&lU32, XML_DATA_TYPE_UINT32))
        printf("req3.id[0]=%u\n", lU32);

    if(xmlGetChildValByIdx(lpNode, "id", 4, (void *)&lU32, XML_DATA_TYPE_UINT32))
        printf("req3.id[4]=%u\n", lU32);

    if(xmlGetChildVal(lpNode, "bool_true", (void *)&lBool, XML_DATA_TYPE_BOOL))
        printf("req3.bool_true=%u\n", lBool);

    printf("TEST FINISHED\n");
}

int main(int argc, char** argv)
{
    testXmlParser();
    return 0;
}