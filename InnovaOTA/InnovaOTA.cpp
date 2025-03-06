
/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "InnovaOTA.h"

extern const char* firmwareVer;
extern const char* hardwareVer;

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/
// URLs to query new firmware version
#define URL_FW_VER "https://raw.githubusercontent.com/SenseAI-Organization/Sense_sensZtu_TH/refs/heads/main/bin_version.txt"   //URL WITH FIRMWARE VERSION
#define URL_FW_BIN "https://raw.githubusercontent.com/SenseAI-Organization/Sense_sensZtu_TH/refs/heads/main/firmware.bin"   //URL WITH BIN

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/
const char *rootCACertificate =     //Certificado de GitHub; deberá cambiarse por URL elegido
    "-----BEGIN CERTIFICATE-----\n"
"MIIEoTCCBEigAwIBAgIRAKtmhrVie+gFloITMBKGSfUwCgYIKoZIzj0EAwIwgY8x\n"
"CzAJBgNVBAYTAkdCMRswGQYDVQQIExJHcmVhdGVyIE1hbmNoZXN0ZXIxEDAOBgNV\n"
"BAcTB1NhbGZvcmQxGDAWBgNVBAoTD1NlY3RpZ28gTGltaXRlZDE3MDUGA1UEAxMu\n"
"U2VjdGlnbyBFQ0MgRG9tYWluIFZhbGlkYXRpb24gU2VjdXJlIFNlcnZlciBDQTAe\n"
"Fw0yNTAyMDUwMDAwMDBaFw0yNjAyMDUyMzU5NTlaMBUxEzARBgNVBAMTCmdpdGh1\n"
"Yi5jb20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQgNFxG/yzL+CSarvC7L3ep\n"
"H5chNnG6wiYYxR5D/Z1J4MxGnIX8KbT5fCgLoyzHXL9v50bdBIq6y4AtN4gN7gbW\n"
"o4IC/DCCAvgwHwYDVR0jBBgwFoAU9oUKOxGG4QR9DqoLLNLuzGR7e64wHQYDVR0O\n"
"BBYEFFPIf96emE7HTda83quVPjA9PdHIMA4GA1UdDwEB/wQEAwIHgDAMBgNVHRMB\n"
"Af8EAjAAMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjBJBgNVHSAEQjBA\n"
"MDQGCysGAQQBsjEBAgIHMCUwIwYIKwYBBQUHAgEWF2h0dHBzOi8vc2VjdGlnby5j\n"
"b20vQ1BTMAgGBmeBDAECATCBhAYIKwYBBQUHAQEEeDB2ME8GCCsGAQUFBzAChkNo\n"
"dHRwOi8vY3J0LnNlY3RpZ28uY29tL1NlY3RpZ29FQ0NEb21haW5WYWxpZGF0aW9u\n"
"U2VjdXJlU2VydmVyQ0EuY3J0MCMGCCsGAQUFBzABhhdodHRwOi8vb2NzcC5zZWN0\n"
"aWdvLmNvbTCCAX4GCisGAQQB1nkCBAIEggFuBIIBagFoAHUAlpdkv1VYl633Q4do\n"
"NwhCd+nwOtX2pPM2bkakPw/KqcYAAAGU02uUSwAABAMARjBEAiA7i6o+LpQjt6Ae\n"
"EjltHhs/TiECnHd0xTeer/3vD1xgsAIgYlGwRot+SqEBCs//frx/YHTPwox9QLdy\n"
"7GjTLWHfcMAAdwAZhtTHKKpv/roDb3gqTQGRqs4tcjEPrs5dcEEtJUzH1AAAAZTT\n"
"a5PtAAAEAwBIMEYCIQDlrInx7J+3MfqgxB2+Fvq3dMlk1qj4chOw/+HkYVfG0AIh\n"
"AMT+JKAQfUuIdBGxfryrGrwsOD3pRs1tyAyykdPGRgsTAHYAyzj3FYl8hKFEX1vB\n"
"3fvJbvKaWc1HCmkFhbDLFMMUWOcAAAGU02uUJQAABAMARzBFAiEA1GKW92agDFNJ\n"
"IYrMH3gaJdXsdIVpUcZOfxH1FksbuLECIFJCfslINhc53Q0TIMJHdcFOW2tgG4tB\n"
"A1dL881tXbMnMCUGA1UdEQQeMByCCmdpdGh1Yi5jb22CDnd3dy5naXRodWIuY29t\n"
"MAoGCCqGSM49BAMCA0cAMEQCIHGMp27BBBJ1356lCe2WYyzYIp/fAONQM3AkeE/f\n"
"ym0sAiBtVfN3YgIZ+neHEfwcRhhz4uDpc8F+tKmtceWJSicMkA==\n"
"-----END CERTIFICATE-----\n";


/* ***************************************************************************
 * **** FUNCTIONS ************************************************************
 * ***************************************************************************/
// Función auxiliar para comparar versiones
bool isNewerVersion(const char* serverVersion, const char* deviceVersion) {
    int serverNums[3] = {0};
    int deviceNums[3] = {0};
    
    // Parsear versión del servidor
    sscanf(serverVersion, "%d.%d.%d", &serverNums[0], &serverNums[1], &serverNums[2]);
    
    // Parsear versión del dispositivo
    sscanf(deviceVersion, "%d.%d.%d", &deviceNums[0], &deviceNums[1], &deviceNums[2]);
    
    // Comparar versiones
    for(int i = 0; i < 3; i++) {
        if(serverNums[i] > deviceNums[i]) return true;
        if(serverNums[i] < deviceNums[i]) return false;
    }
    
    return false; // Si son iguales, no es una versión más nueva
}

int firmwareVersionCheck() {
    if (WiFi.status() != WL_CONNECTED) return -1;
    
    HTTPClient http;
    http.begin("https://raw.githubusercontent.com/SenseAI-Organization/Sense_sensZtu_TH/refs/heads/main/bin_version.txt");
    
    int httpCode = http.GET();
    if(httpCode != HTTP_CODE_OK) {
        http.end();
        return -1;
    }
    
    String newFWVersion = http.getString();
    http.end();
    
    // Limpiar cualquier caracter no deseado
    newFWVersion.trim();
    
    Serial.print("Server version: ");
    Serial.println(newFWVersion);
    Serial.print("Device version: ");
    Serial.println(firmwareVer);
    
    // Usar la nueva función de comparación
    if(isNewerVersion(newFWVersion.c_str(), firmwareVer)) {
        return 1;
    }
    
    return 0;
}


bool firmwareUpdate()
{
    WiFiClientSecure wificlient;
    wificlient.setInsecure();
    
    t_httpUpdate_return ret = httpUpdate.update(wificlient, URL_FW_BIN);

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        return false;
        
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        return false;
        
    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        return true;
    }
    
    return false; // Por si acaso llegamos aquí
}


/****** END OF FILE ****/