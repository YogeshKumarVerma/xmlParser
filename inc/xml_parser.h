#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

#ifndef uint8
typedef uint8_t uint8;
#endif

#ifndef uint16
typedef uint16_t uint16;
#endif

#ifndef uint32
typedef uint32_t uint32;
#endif

#ifndef int8
typedef uint8_t int8;
#endif

#ifndef int16
typedef uint16_t int16;
#endif

#ifndef int32
typedef uint32_t int32;
#endif


typedef enum
{
    XML_E_FAILURE,
    XML_E_SUCCESS,
    XML_E_FILE_NOT_FOUND,
    XML_E_TAG_NOT_FOUND,
    XML_E_INVALID_TYPE,
    XML_E_CODE_MAX
}xmlErrCode_e;

typedef enum
{
    XML_NODE_TYPE_ELEMENT   = XML_ELEMENT_NODE,
    XML_NODE_TYPE_ATTRIBUTE = XML_ATTRIBUTE_NODE,
    XML_NODE_TYPE_TEXT      = XML_TEXT_NODE,
    XML_NODE_TYPE_COMMENT   = XML_COMMENT_NODE,
    XML_NODE_TYPE_MAX
}xmlNodeType_e;

typedef enum
{
    XML_DATA_TYPE_BOOL,
    XML_DATA_TYPE_INT32,
    XML_DATA_TYPE_UINT32,
    XML_DATA_TYPE_STRING,
    XML_DATA_TYPE_MAX
}xmlDataType_e;

#if 0
/* documentation of types used from xml library */
/* xmlDocPtr structure members */
struct xmlDoc {
    void           *_private;	/* application data */
    xmlElementType  type;       /* XML_DOCUMENT_NODE, must be second ! */
    char           *name;	    /* name/filename/URI of the document */
    struct _xmlNode *children;	/* the document tree */
    struct _xmlNode *last;	    /* last child link */
    struct _xmlNode *parent;	/* child->parent link */
    struct _xmlNode *next;	    /* next sibling link  */
    struct _xmlNode *prev;	    /* previous sibling link  */
    struct _xmlDoc  *doc;	    /* autoreference to itself */

    /* End of common part */
    int             compression;/* level of zlib compression */
    int             standalone; /* standalone document (no external refs) */
    struct _xmlDtd  *intSubset;	/* the document internal subset */
    struct _xmlDtd  *extSubset;	/* the document external subset */
    struct _xmlNs   *oldNs;	    /* Global namespace, the old way */
    const xmlChar  *version;	/* the XML version string */
    const xmlChar  *encoding;   /* external initial encoding, if any */
    void           *ids;        /* Hash table for ID attributes if any */
    void           *refs;       /* Hash table for IDREFs attributes if any */
    const xmlChar  *URL;	    /* The URI for that document */
    int             charset;    /* encoding of the in-memory content actually an xmlCharEncoding */
};

struct xmlNode {
    void           *_private;	/* application data */
    xmlElementType   type;	    /* type number, must be second ! */
    const xmlChar   *name;      /* the name of the node, or the entity */
    struct _xmlNode *children;	/* parent->childs link */
    struct _xmlNode *last;	    /* last child link */
    struct _xmlNode *parent;	/* child->parent link */
    struct _xmlNode *next;	    /* next sibling link  */
    struct _xmlNode *prev;	    /* previous sibling link  */
    struct _xmlDoc  *doc;	    /* the containing document */

    /* End of common part */
    xmlNs           *ns;        /* pointer to the associated namespace */
    xmlChar         *content;   /* the content */
    struct _xmlAttr *properties;/* properties list */
    xmlNs           *nsDef;     /* namespace definitions on this node */
};
#endif

/* external function prototypes */
/* function prototypes for reading xmlNode */
xmlDoc*  xmlGetDocNode(const char *apFileName);
xmlNode* xmlGetRootElement(xmlDoc *apDoc);
xmlNode* xmlGetChildElements(xmlNode *apNode, uint32 *apChildCnt);
xmlNode* xmlGetChildElementByName(xmlNode *apNode, const char *apName);
/* xmlNode* xmlGetChildElementsByName(xmlNode *apNode, const char *apName, uint32 *apChildCnt); */
xmlNode* xmlGetNextChildNode(xmlNode *apNode);
xmlNode* xmlGetPrevChildNode(xmlNode *apNode);
xmlNode* xmlGetFirstChildNode(xmlNode *apNode);
xmlNode* xmlGetLastChildNode(xmlNode *apNode);
xmlNode* xmlGetChildNodeByIdx(xmlNode *apNode, uint32 aChildIdx);
xmlNode* xmlGetChildNodeByNameAndIdx(xmlNode *apNode, const char *apName, uint32 aChildIdx);
xmlNode* xmlGetNodeByPath(xmlNode *apRoot, const char *apPath);

/* function prototypes for reading node values */
bool   xmlGetBool(xmlNode *apNode, const char *apChildName);
int32  xmlGetInt(xmlNode *apNode, const char *apChildName);
uint32 xmlGetUint(xmlNode *apNode, const char *apChildName);

xmlErrCode_e xmlGetNodeVal(xmlNode *apNode, void *apVal, xmlDataType_e aValType);
xmlErrCode_e xmlGetAttrVal(xmlNode *apNode, const char *apName, void *apVal, xmlDataType_e aValType);
xmlErrCode_e xmlGetChildVal(xmlNode *apNode, const char *apName, void *apVal, xmlDataType_e aValType);
xmlErrCode_e xmlGetChildValByIdx(xmlNode *apNode, const char *apName, uint32 aChildIdx, void *apVal, xmlDataType_e aValType);

xmlErrCode_e xmlGetNodeValByPath(xmlNode *apRoot, const char *apPath, void *apVal, xmlDataType_e aValType);

#endif /* _XML_PARSER_H_ */