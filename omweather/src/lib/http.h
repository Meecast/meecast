/***************************************************************************
 *
 *     Library: http
 *
 * Description: header for libhttp
 *
 ***************************************************************************
 *
 * Copyright (C) 1994 by Sami Tikka <sti@iki.fi>
 * Copyright (C) 2001 by Alan DuBoff <aland@SoftOrchestra.com>
 * Copyrigth (C) 2006 by Moises Martinez <moimart@moimart.org> 
 *
 * The right to use, modify and redistribute this code is allowed
 * provided the above copyright notice and the below disclaimer appear
 * on all copies.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
 *
 ***************************************************************************/

/**
 *
 * NOTE!!! 02-2006 Added NON-BLOCKING SOCKETS support 
 * 
 **/

#ifndef _LIBHTTP_
#define _LIBHTTP_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <memory.h>
#include <errno.h>
#include <ctype.h>

/* Compile time options.
 * Allow you to disable library functionality you don't need. - Jean II
 */
#define HF_FIND_HEADER      /* find_header() function */
#define HF_DO_FILE      /* do_file() function & functionality */

#define BUFLEN 8192
#define GETLEN 8192
#define XFERLEN 65536
#define HCODESIZE 4
#define HMSGSIZE 32

/* Flags is a mask with the xor of the various options - Jean II */
#define HFLAG_NONE      0x0000      /* No flags */
#define HFLAG_RETURN_HEADER 0x0001      /* Return HTTP headers */
#define HFLAG_POST_USER_TYPE    0x0002      /* Do not add post type */
#define HFLAG_FORCE_NO_PROXY    0x0004      /* Do not use proxy settings */
/* Maybe HFLAG_USER_ACCEPT ? */

#define LIBHTTP_VERSION "1.1"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
    char *pData;                            //  pointer to data
    long lSize;                             //  size of data allocated
    char *pHdr;                             //  pointer to header, if requested
    int  iError;                            //  error upon failures
    char *pError;                           //  text description of error
    char szHCode[HCODESIZE];                //  http response code
    char szHMsg[HMSGSIZE];                  //  message/description of http code
} HTTP_Response, *PHTTP_Response;

typedef struct
{
  /* Those fields won't be touched or free'd - Jean II */
  char *Headers;        // Additional HTTP header
  char *PostData;       // Data for POST request
  int   PostLen;        // Length of Post data
  /* Fields set by the library - Jean II */
  int	Socket;		// To be used to implement watchdog timeout
} HTTP_Extra;

typedef enum 
{
    kHMethodOptions = 1,
    kHMethodGet,
    kHMethodHead,
    kHMethodPost,
    kHMethodPut,
    kHMethodDelete,
    kHMethodTrace
}HTTP_Method;

char *find_header_end( char *buf, int bytes );
char *parse_url( char *url, char *scheme, char *host, int *port );
HTTP_Response http_request( char *in_URL, HTTP_Extra *in_Extra, HTTP_Method in_Method, unsigned long in_Flags );
#ifdef HF_DO_FILE
HTTP_Response do_file(char *in_URL);
#endif /* HF_DO_FILE */
#ifdef HF_FIND_HEADER
char *find_header( char *buf, int bytes, char *type, char *value, int maxv );
#endif /*  HF_FIND_HEADER */

#ifdef __cplusplus
}
#endif

#endif
