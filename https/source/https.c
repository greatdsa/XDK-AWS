/*
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for Licensee�s application development. 
* Fitness and suitability of the example code for any use within Licensee�s applications need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee. 
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
* 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are 
* met:
* 
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer. 
* 
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.  
*     
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*----------------------------------------------------------------------------*/
/**
* @ingroup APPS_LIST
*
* @defgroup XDK_APPLICATION_TEMPLATE XDK Application Template
* @{
*
* @brief XDK Application Template
*
* @details Empty XDK Application Template without any functionality. Should be used as a template to start new projects.
*
* @file
**/
/* module includes ********************************************************** */

/* system header files */
#include <stdio.h>
/* additional interface header files */
#include "FreeRTOS.h"
#include "timers.h"

/* own header files */
#include "https.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_Assert.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include "PAL_initialize_ih.h"
#include "PAL_socketMonitor_ih.h"
#include <simplelink.h>

/* constant definitions ***************************************************** */
#define HOST_NAME "ac7j26n72nlys.iot.us-west-2.amazonaws.com"
#define CA_FILE_NAME "ca.der"
#define PRIVATE_FILE_NAME "9877620cec-private.pem.key"
#define CERTIFICATE_FILE_NAME "9877620cec-certificate.der"


/* local variables ********************************************************** */

/* global variables ********************************************************* */
unsigned char digicert_root_crt[] = {
 // Place your hex values here.
		
};
int digicert_root_crt_len = 1758;

unsigned char digicert_pvt_crt[] = {
		// Place your hex values here.
};

int digicert_pvt_crt_len = 1679;


unsigned char digicert_pub_crt[] = {
		// Place your hex values here.
};
int digicert_pub_crt_len = 451;
/* inline functions ********************************************************* */

/* local functions ********************************************************** */
// Flash the Certificate BEFORE attempting to connect.
// I.e. call this function in appInitSystem() before connectServerSecure from
// Code 8
void flashCertificate() {
 // For the purpose of readability this code has no error handling.
 // The simplelink API provides return codes of the type _i32 that can be
 // checked for the value SL_RET_CODE_OK
 // The datetime is required for certificate validation:
 SlDateTime_t dateTime;
 dateTime.sl_tm_day = (_u32)01;
 dateTime.sl_tm_mon = (_u32)01;
 dateTime.sl_tm_year = (_u32)2050;
 dateTime.sl_tm_hour = (_u32)0;
 dateTime.sl_tm_min = (_u32)0;
 dateTime.sl_tm_sec = (_u32)0;
 sl_DevSet(
 SL_DEVICE_GENERAL_CONFIGURATION,
 SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME,
 sizeof(SlDateTime_t),
 (_u8 *)(&dateTime)
 );
 // If a file with the same name already exists, call this first:
 // sl_FsDel((_u8*) CA_FILE_NAME, 0)
 // The file handle should not be 0 after a file was successfully created:
 _i32 fileHandle = 0;
 sl_FsOpen(
 (_u8*) CA_FILE_NAME,
 FS_MODE_OPEN_CREATE( 1024, _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ ),
 NULL,
 &fileHandle );
// If the file is longer than 1024 bytes, you need to loop the writing.
// "length" contains the length of the certificate
// "writtenLength" contains the amount of actually written bytes.


 // _i32 writtenLen = sl_FsWrite(fileHandle, 0, data, length);

 _u32 remainingLength = 1758;
 while(remainingLength > 0)
 {
      remainingLength -= sl_FsWrite(
        fileHandle,
        1758 - remainingLength,
		digicert_root_crt+(1758 - remainingLength),
        remainingLength > 1024 ? 1024 : remainingLength // equal to max(remainingLength, 1024)
  );
 }

 sl_FsClose(fileHandle, NULL, NULL, 0);





  sl_FsOpen(
  (_u8*) PRIVATE_FILE_NAME,
  FS_MODE_OPEN_CREATE( 1024, _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ ),
  NULL,
  &fileHandle );
 // If the file is longer than 1024 bytes, you need to loop the writing.
 // "length" contains the length of the certificate
 // "writtenLength" contains the amount of actually written bytes.


  // _i32 writtenLen = sl_FsWrite(fileHandle, 0, data, length);

remainingLength = 1679;
  while(remainingLength > 0)
  {
       remainingLength -= sl_FsWrite(
         fileHandle,
         1679 - remainingLength,
		 digicert_pvt_crt+(1679 - remainingLength),
         remainingLength > 1024 ? 1024 : remainingLength // equal to max(remainingLength, 1024)
   );
  }

  sl_FsClose(fileHandle, NULL, NULL, 0);



   sl_FsOpen(
   (_u8*) CERTIFICATE_FILE_NAME,
   FS_MODE_OPEN_CREATE( 1024, _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ ),
   NULL,
   &fileHandle );

   sl_FsWrite(fileHandle,0, digicert_pub_crt,1024);

}

// Call this function after flashCertificate() in appInitSystem()
void sendGetRequest(_i16 socketHandle, char* host, char* path)
{
 char outBuf[512];
 char inBuf[1024];
 _i16 bytesSent = 0;
 _i16 bytesReceived = 0;
 sprintf(
 outBuf,
 "GET https://%s%s HTTP/1.1\r\nHost: %s\r\n\r\n", host, path, host
 );
 printf("HTTP request:\r\n");
 printf("\r\n======\r\n");
 printf("%s", outBuf);
 printf("\r\n======\r\n");
 bytesSent = sl_Send(
 socketHandle, (const void *) outBuf, strlen(outBuf), 0
 );
 if( bytesSent <= 0 ) {
 printf("sl_Send failed: %i\r\n", bytesSent);
 return;
 }
 printf("HTTP response:\r\n");
 printf("\r\n======\r\n");
 int linesPrinted = 0;
do
{
 bytesReceived = sl_Recv(socketHandle, inBuf, 1024, 0);
 int lastStart = 0;
 int pos = 0;

 while (linesPrinted < 10 && pos < bytesReceived)
  { if (inBuf[pos] == '\n') {
	 printf("%.*s", pos - lastStart - 1, inBuf + lastStart);
	 printf("\r\n");
	 lastStart = pos;
	 linesPrinted++;}
   pos++;
   }

 } while(bytesReceived > 0);

if (linesPrinted == 10) {
 printf("...\r\n");
 }
 printf("\r\n======\r\n");
}


void connectServerSecure(void){
 // For the purpose of readability this code has no error handling.
 // The simplelink API provides return codes of the type _i32 that can be
 // checked for the value SL_RET_CODE_OK
 // Getting the IP address of HOST_NAME:
 Ip_Address_T destAddr = 0;
 PAL_getIpaddress((uint8_t*) HOST_NAME, &destAddr);
 // Creating a Socket (socketHandle ≦ 0 indicates an error):
 _i16 socketHandle = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_SEC_SOCKET);

 //tell the simplelink chip to use TLS for that socket
 SlSockSecureMethod secMethod;
 secMethod.secureMethod = SL_SO_SEC_METHOD_SSLv3_TLSV1_2;
 sl_SetSockOpt(
     socketHandle,
     SL_SOL_SOCKET,
     SL_SO_SECMETHOD,
     (_u8 *)&secMethod,
     sizeof(secMethod)
 );

 // Adding the certificate to the socket:
 sl_SetSockOpt(
 socketHandle,
 SL_SOL_SOCKET,
 SL_SO_SECURE_FILES_CA_FILE_NAME,
 CA_FILE_NAME,
 strlen(CA_FILE_NAME)
 );


 sl_SetSockOpt(
  socketHandle,
  SL_SOL_SOCKET,
  SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME,
  PRIVATE_FILE_NAME,
  strlen(PRIVATE_FILE_NAME)
  );


 sl_SetSockOpt(
  socketHandle,
  SL_SOL_SOCKET,
  SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME,
  CERTIFICATE_FILE_NAME,
  strlen(CERTIFICATE_FILE_NAME)
  );
 // Configuration the connection settings (IP, Port, etc.):
 SlSockAddrIn_t addr;
 addr.sin_family = SL_AF_INET;
 addr.sin_port = sl_Htons(443);
 addr.sin_addr.s_addr = destAddr;
 // Connecting:
 sl_Connect(socketHandle, ( SlSockAddr_t *)&addr, sizeof(SlSockAddrIn_t));
 // Implementation Placeholder
 // Do your request, etc. here
   sendGetRequest(socketHandle, HOST_NAME, "/");
 // Closing:
 sl_Close(socketHandle);
}


/*
void sendGetRequest(_i16 socketHandle, char* host, char* path)
{
 char outBuf[1024];
 char inBuf[1024];
 sprintf(outBuf,"GET https://%s%s HTTP/1.1\r\nHost: %s\r\n\r\n", host, path, host);
 sl_Send(socketHandle, (const void *) outBuf, strlen(outBuf), 0);
 sl_Recv(socketHandle, inBuf, 1024, 0);
 printf("%s\r\n", inBuf);
}

*/




/* global functions ********************************************************* */

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */


void networkSetup(void){
 WlanConnect_SSID_T connectSSID = (WlanConnect_SSID_T) "OnePlus 5T";
 WlanConnect_PassPhrase_T connectPassPhrase = (WlanConnect_PassPhrase_T)
"password123";
 WlanConnect_Init();
 NetworkConfig_SetIpDhcp(0);
 WlanConnect_WPA(connectSSID, connectPassPhrase, NULL);
 PAL_initialize();
 PAL_socketMonitorInit();
}

void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    BCDS_UNUSED(param2);
    networkSetup();
    flashCertificate();
    // Use this before connecting:
    SlSockSecureMethod secMethod;
    secMethod.secureMethod = SL_SO_SEC_METHOD_SSLv3_TLSV1_2;
    connectServerSecure();
}




/**@} */
/** ************************************************************************* */
