#include "xml_parser.h"

/*---------------------------------------------------------------------------*/
/*                    Constants/Types Definitions                            */
/*---------------------------------------------------------------------------*/
#ifndef PRIVATE
#define PRIVATE                 static
#endif

//#define XML_LOG_LEVEL_DBG
#if defined(XML_LOG_LEVEL_DBG)
#define XML_LOG_DBG(_fmt, ...)        printf("[%04d][XML.DBG] "_fmt"\n", __LINE__, ##__VA_ARGS__)
#define XML_LOG_INF(_fmt, ...)        printf("[%04d][XML.INF] "_fmt"\n", __LINE__, ##__VA_ARGS__)
#define XML_LOG_WRN(_fmt, ...)        printf("[%04d][XML.WRN] "_fmt"\n", __LINE__, ##__VA_ARGS__)
#elif defined(XML_LOG_LEVEL_INFO)
#define XML_LOG_DBG(_fmt, ...)        
#define XML_LOG_INF(_fmt, ...)        printf("[%04d][XML.INF] "_fmt"\n", __LINE__, ##__VA_ARGS__)
#define XML_LOG_WRN(_fmt, ...)        printf("[%04d][XML.WRN] "_fmt"\n", __LINE__, ##__VA_ARGS__)
#elif defined(XML_LOG_LEVEL_WARNING)
#define XML_LOG_DBG(_fmt, ...)        
#define XML_LOG_INF(_fmt, ...)        
#define XML_LOG_WRN(_fmt, ...)        printf("[%04d][XML.WRN] "_fmt"\n", __LINE__, ##__VA_ARGS__)
#else
#define XML_LOG_DBG(_fmt, ...)        
#define XML_LOG_INF(_fmt, ...)        
#define XML_LOG_WRN(_fmt, ...)        
#endif

#define XML_LOG_ERR(_fmt, ...)        printf("[%04d][XML.ERR] "_fmt"\n", __LINE__, ##__VA_ARGS__)

#define XML_CHAR_SPACE          ' '
#define XML_CHAR_TAB            '\t'
#define XML_STR_TRUE            "TRUE"
#define XML_STR_FALSE           "FALSE"
#define XML_STR_IS_HEX(s)       ((('0' == s[0]) && (('x' == s[1]) || ('X' == s[1]))) ? 1 : 0)
#define XML_HEX_TO_INT(h)       strtol(h, NULL, 0)
#define XML_STR_TO_INT(s)       (XML_STR_IS_HEX(s) ? XML_HEX_TO_INT(s) : atoi(s))
#define XML_STRCMP(s1, s2)      (strlen(s1) == strlen(s2) ? strncmp(s1, s2, strlen(s1)) : 1)

/*---------------------------------------------------------------------------*/
/*                    Global Variables Definitions                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                    Local Function Prototypes                              */
/*---------------------------------------------------------------------------*/
//PRIVATE char* xmlToUpperCase(char *apStr);
PRIVATE char* xmlTrimString(char *apStr);

PRIVATE xmlErrCode_e xmlStrToVal(char *apStr, void *apVal, xmlDataType_e aValType);

/*---------------------------------------------------------------------------*/
/*                    Global Function Definitions                            */
/*---------------------------------------------------------------------------*/
void xmlFreeDocNode(xmlDoc *apDoc)
{
    if(NULL == apDoc)
    {
        XML_LOG_ERR("apDoc is NULL, cannot free document");
    }
    else
    {
        xmlFreeDoc(apDoc);
    }
}

xmlDoc*  xmlGetDocNode(const char *apFileName)
{
    xmlDoc *lpDoc = NULL;

    if(NULL == apFileName)
    {
        XML_LOG_ERR("filename is NULL");
    }
    else
    {
        XML_LOG_DBG("reading xmlFile=[%s]", apFileName);

        lpDoc = xmlParseFile(apFileName);
        if(NULL == lpDoc)
        {
            XML_LOG_ERR("xmlFile=%s reading failed, check syntax", apFileName);
        }
    }

    return lpDoc;
}

xmlNode* xmlGetRootElement(xmlDoc *apDoc)
{
    xmlNode *lpRoot = NULL;

    if(NULL == apDoc)
    {
        XML_LOG_ERR("apDoc is NULL, cannot read root element");
    }
    else
    {
        lpRoot = xmlDocGetRootElement(apDoc);
        if(NULL == lpRoot)
        {
            XML_LOG_ERR("xml file is empty, no root element found");
        }
    }

    return lpRoot;
}

xmlNode* xmlGetChildElements(xmlNode *apNode, uint32 *apChildCnt)
{
    xmlNode *lpHead = NULL;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read child elements");
    }
    else
    {
        lpHead = xmlFirstElementChild(apNode);
        *apChildCnt = (uint32)xmlChildElementCount(apNode);
    }

    return lpHead;
}

xmlNode* xmlGetChildElementByName(xmlNode *apNode, const char *apName)
{
    xmlNode *lpNode = NULL;
    uint32   lChCnt = 0;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read child element");
    }
    else if(NULL == apName)
    {
        XML_LOG_ERR("name is NULL, cannot read child element");
    }
    else
    {
        lChCnt = (uint32)xmlChildElementCount(apNode);
        xmlNode *lpTmp = xmlFirstElementChild(apNode);

        XML_LOG_DBG("node=%s, childCnt=%u", apNode->name, lChCnt);

        for(uint32 idx=0; idx < lChCnt; idx++)
        {
            if(0 == XML_STRCMP((const char *)lpTmp->name, apName))
            {
                lpNode = lpTmp;
                XML_LOG_DBG("child=%s found at index=%u", apName, idx);
                break;
            }
            lpTmp = xmlNextElementSibling(lpTmp);
        }
    }

    return lpNode;
}

#if 0
xmlNode* xmlGetChildElementsByName(xmlNode *apNode, const char *apName, uint32 *apChildCnt)
{
    XML_LOG_ERR("function not supported");
    return NULL;
}
#endif
xmlNode* xmlGetNextChildNode(xmlNode *apNode)
{
    xmlNode *lpNext = NULL;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read next child");
    }
    else
    {
        lpNext = xmlNextElementSibling(apNode);
    }

    return lpNext;
}

xmlNode* xmlGetPrevChildNode(xmlNode *apNode)
{
    xmlNode *lpNext = NULL;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read prev child");
    }
    else
    {
        lpNext = xmlPreviousElementSibling(apNode);
    }

    return lpNext;
}

xmlNode* xmlGetFirstChildNode(xmlNode *apNode)
{
    xmlNode *lpFirst = NULL;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read first child");
    }
    else
    {
        lpFirst = xmlFirstElementChild(apNode);
    }

    return lpFirst;
}

xmlNode* xmlGetLastChildNode(xmlNode *apNode)
{
    xmlNode *lpLast = NULL;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read last child");
    }
    else
    {
        lpLast = xmlLastElementChild(apNode);
    }

    return lpLast;
}

xmlNode* xmlGetChildNodeByIdx(xmlNode *apNode, uint32 aChildIdx)
{
    xmlNode *lpNode = NULL;
    uint32   lChCnt = 0;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read child element");
    }
    else
    {
        lChCnt = (uint32)xmlChildElementCount(apNode);
        
        if(aChildIdx < lChCnt)
        {
            xmlNode *lpTmp = xmlFirstElementChild(apNode);

            for(uint32 lIdx=0; lIdx < aChildIdx; lIdx++)
            {
                lpTmp = xmlNextElementSibling(lpTmp);
            }

            lpNode = lpTmp;
        }
        else
        {
            XML_LOG_ERR("node=%s, childCnt=%u, invalid childIdx=%u, cannot read child node", 
                            apNode->name, lChCnt, aChildIdx);
        }
    }

    return lpNode;
}

xmlNode* xmlGetChildNodeByNameAndIdx(xmlNode *apNode, const char *apName, uint32 aChildIdx)
{
    xmlNode *lpNode = NULL;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read child element");
    }
    else if(NULL == apName)
    {
        XML_LOG_ERR("name is NULL, cannot read child element");
    }
    else
    {
        xmlNode *lpTmp = xmlFirstElementChild(apNode);
        uint32 lChIdx = 0;

        while(lpTmp)
        {
            if(0 == XML_STRCMP((const char *)lpTmp->name, apName))
            {
                if(lChIdx == aChildIdx)
                {
                    lpNode = lpTmp;
                    XML_LOG_DBG("node=%s, child=%s, idx=%u, found", apNode->name, apName, aChildIdx);
                    break;
                }
                lChIdx++; /* name matched, so increment index */
            }
            lpTmp = xmlNextElementSibling(lpTmp);
        }
        
        if(NULL == lpNode)
        {
            XML_LOG_ERR("node=%s, child=%s, idx=%u, not found", apNode->name, apName, aChildIdx);
        }
    }

    return lpNode;
}

xmlNode* xmlGetNodeByPath(xmlNode *apRoot, const char *apPath)
{
    XML_LOG_ERR("function not supported");
    return NULL;
}


/* function prototypes for reading node values */
bool xmlGetBool(xmlNode *apNode, const char *apChildName)
{
    XML_LOG_ERR("function not supported");
    return true;
}

int32 xmlGetInt(xmlNode *apNode, const char *apChildName)
{
    XML_LOG_ERR("function not supported");
    return 0;
}

uint32 xmlGetUint(xmlNode *apNode, const char *apChildName)
{
    XML_LOG_ERR("function not supported");
    return 0;
}

xmlErrCode_e xmlGetNodeVal(xmlNode *apNode, void *apVal, xmlDataType_e aValType)
{
    xmlErrCode_e lErrCode = XML_E_FAILURE;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read node value");
    }
    else if(NULL == apVal)
    {
        XML_LOG_ERR("val ptr in NULL, cannot read attr value");
    }
    else if (aValType >= XML_DATA_TYPE_MAX)
    {
        XML_LOG_ERR("invalid data type, cannot read attr value");
    }
    else
    {
        char *lpText = (char *)xmlNodeGetContent(apNode);
        if(NULL == lpText)
        {
            XML_LOG_ERR("error in reading node=%s value", apNode->name);
        }
        else
        {
            XML_LOG_DBG("node=%s, text=%s", apNode->name, lpText);
            lErrCode = xmlStrToVal(lpText, apVal, aValType);
        }
    }

    return lErrCode;
}

xmlErrCode_e xmlGetAttrVal(xmlNode *apNode, const char *apName, void *apVal, xmlDataType_e aValType)
{
    xmlErrCode_e lErrCode = XML_E_FAILURE;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read attr value");
    }
    else if(NULL == apName)
    {
        XML_LOG_ERR("attr name is NULL, cannot read attr value");
    }
    else if(NULL == apVal)
    {
        XML_LOG_ERR("val ptr in NULL, cannot read attr value");
    }
    else if (aValType >= XML_DATA_TYPE_MAX)
    {
        XML_LOG_ERR("invalid data type, cannot read attr value");
    }
    else
    {
        char *lpText = (char *)xmlGetProp(apNode, (const xmlChar *)apName);
        if(NULL == lpText)
        {
            XML_LOG_ERR("error in reading attr=%s for node=%s", apName, apNode->name);
        }
        else
        {
            XML_LOG_DBG("node=%s, attr=%s, text=%s", apNode->name, apName, lpText);
            lErrCode = xmlStrToVal(lpText, apVal, aValType);
        }
    }

    return lErrCode;
}

xmlErrCode_e xmlGetChildVal(xmlNode *apNode, const char *apName, void *apVal, xmlDataType_e aValType)
{
    xmlErrCode_e lErrCode = XML_E_FAILURE;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read child value");
    }
    else if(NULL == apName)
    {
        XML_LOG_ERR("attr name is NULL, cannot read child value");
    }
    else if(NULL == apVal)
    {
        XML_LOG_ERR("val ptr in NULL, cannot read child value");
    }
    else if (aValType >= XML_DATA_TYPE_MAX)
    {
        XML_LOG_ERR("invalid data type, cannot read child value");
    }
    else
    {
        xmlNode *lpChild = xmlGetChildElementByName(apNode, apName);

        if(NULL == lpChild)
        {
            XML_LOG_ERR("child name=%s, not found in node=%s", apName, apNode->name);
        }
        else
        {
            char *lpText = (char *)xmlNodeGetContent(lpChild);
            XML_LOG_DBG("node=%s, child=%s, text=%s", apNode->name, apName, lpText);
            lErrCode = xmlStrToVal(lpText, apVal, aValType);
        }
    }

    return lErrCode;
}

xmlErrCode_e xmlGetChildValByIdx(xmlNode *apNode, const char *apName, uint32 aChildIdx, void *apVal, xmlDataType_e aValType)
{
    xmlErrCode_e lErrCode = XML_E_FAILURE;

    if(NULL == apNode)
    {
        XML_LOG_ERR("node is NULL, cannot read child value");
    }
    else if(NULL == apName)
    {
        XML_LOG_ERR("attr name is NULL, cannot read child value");
    }
    else if(NULL == apVal)
    {
        XML_LOG_ERR("val ptr in NULL, cannot read child value");
    }
    else if (aValType >= XML_DATA_TYPE_MAX)
    {
        XML_LOG_ERR("invalid data type, cannot read child value");
    }
    else
    {
        xmlNode *lpChild = xmlGetChildNodeByNameAndIdx(apNode, apName, aChildIdx);
        
        if(lpChild)
        {
            lErrCode = xmlGetNodeVal(lpChild, apVal, aValType);
        }
    }

    return lErrCode;
}

xmlErrCode_e xmlGetNodeValByPath(xmlNode *apRoot, const char *apPath, void *apVal, xmlDataType_e aValType)
{
    XML_LOG_ERR("function not supported");
    return XML_E_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*                    Local Function Definitions                             */
/*---------------------------------------------------------------------------*/
/*PRIVATE char* xmlToUpperCase(char *apStr)
{
    for(uint32 idx = 0; idx < strlen(apStr); idx++) 
    {
        if(apStr[idx] >= 'a' && apStr[idx] <= 'z') 
        {
            apStr[idx] = apStr[idx] - 32;  
        }
   }

   return apStr;
}*/

PRIVATE char* xmlTrimString(char *apStr)
{
	uint32 i, j;

    /* skip all cpaces/tabs from begining of string */
	for(i=0; (XML_CHAR_SPACE == apStr[i] || XML_CHAR_TAB == apStr[i]); i++);
 
	for(j=0; apStr[i]; i++)
	{
		apStr[j++] = apStr[i]; /* copy all characters from string to its begining */
	}
	apStr[j]='\0';

    return apStr;
}

PRIVATE xmlErrCode_e xmlStrToVal(char *apStr, void *apVal, xmlDataType_e aValType)
{
    xmlErrCode_e lErrCode = XML_E_SUCCESS;
    char *lpValStr = xmlTrimString(apStr);
    XML_LOG_DBG("apStr=%s, valType=%u, lpValStr=%s", apStr, aValType, lpValStr);

    switch(aValType)
    {
        case XML_DATA_TYPE_BOOL:
        {
            if(0 == XML_STRCMP(lpValStr, XML_STR_TRUE))
            {
                *((bool *)apVal) = true;
            }
            else
            {
                *((bool *)apVal) = (bool)XML_STR_TO_INT(lpValStr);
            }
            XML_LOG_DBG("val=%u", *((bool *)apVal));
            break;
        }
        case XML_DATA_TYPE_INT32:
        {
            *((int32 *)apVal) = (int32)XML_STR_TO_INT(lpValStr);
            XML_LOG_DBG("val=%d", *((int32 *)apVal));
            break;
        }
        case XML_DATA_TYPE_UINT32:
        {
            *((uint32 *)apVal) = (uint32)XML_STR_TO_INT(lpValStr);
            XML_LOG_DBG("val=%u", *((uint32 *)apVal));
            break;
        }
        case XML_DATA_TYPE_STRING:
        default:
            snprintf(apVal, strlen(lpValStr) + 1, lpValStr); /* +1 for NULL character */
            XML_LOG_DBG("val=%s", (char *)apVal);
            break;
    }  

    return lErrCode; 
}