
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
#define URL_FW_VER "https://raw.githubusercontent.com/danielescobars9/SmartBeddingOTA/main/bin_version.txt"   //URL WITH FIRMWARE VERSION
#define URL_FW_BIN "https://raw.githubusercontent.com/danielescobars9/SmartBeddingOTA/main/firmware.bin"   //URL WITH BIN

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/
const char *rootCACertificate =     //Certificado de GitHub; deberá cambiarse por URL elegido
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
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