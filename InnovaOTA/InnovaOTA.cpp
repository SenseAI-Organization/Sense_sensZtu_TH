
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
int firmwareVersionCheck(void) {
    String payload;
    int httpCode;
    String fwurl = String(URL_FW_VER) + "?" + String(rand());
    Serial.println(fwurl);

    WiFiClientSecure wificlient;
    wificlient.setCACert(rootCACertificate);

    HTTPClient https;
    if (https.begin(wificlient, fwurl)) {
        Serial.print("[HTTPS] GET...\n");
        httpCode = https.GET();

        if (httpCode == HTTP_CODE_OK) {
            payload = https.getString();
        } else {
            Serial.print("Error in downloading version file: ");
            Serial.println(httpCode);
        }
        https.end();
    } else {
        Serial.println("Unable to connect");
        return 0;
    }

    if (httpCode == HTTP_CODE_OK) {
        payload.trim();
        if (payload.equals(firmwareVer)) {
            Serial.printf("\nDevice already on latest firmware version: %s\n", firmwareVer);
            return 0;
        } else {
            Serial.printf("Current Version: %s\n", firmwareVer);
            Serial.printf("Available Version: %s\n", payload.c_str());
            Serial.println("New firmware detected");
            return 1;
        }
    } else {
        Serial.println("HTTP request failed");
    }

    return 0;
}


void firmwareUpdate(void) {
    WiFiClientSecure wificlient;
    wificlient.setCACert(rootCACertificate);

    Serial.println("Proceeding to update firmware. Device will reboot if successful.");
    t_httpUpdate_return ret = httpUpdate.update(wificlient, URL_FW_BIN);

    switch (ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
    }
}

/****** END OF FILE ****/