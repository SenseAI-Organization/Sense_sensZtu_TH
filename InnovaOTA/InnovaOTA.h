
#ifndef INNOVAOTA_H
#define INNOVAOTA_H

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "Arduino.h"
#include <Esp.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>

/** ****************************************************************************
 ** ************ PROTOTYPES ****************************************************
 ** ****************************************************************************/

/**
 * @brief Checks for the latest firmware version available on the server.
 *
 * This function connects to a specified URL to retrieve the latest firmware version
 * and compares it with the current firmware version of the device.
 *
 * @return int
 * @retval 0 If the device is already on the latest firmware version or an error occurred.
 * @retval 1 If a new firmware version is available.
 */
int firmwareVersionCheck();
bool isNewerVersion(const char* serverVersion, const char* deviceVersion);

/**
 * @brief Updates the firmware of the device.
 *
 * This function connects to a specified URL to download and update the firmware
 * of the device. The device will reboot automatically if the update is successful.
 *
 * @note Make sure the device is connected to Wi-Fi and the server URL and root certificate are correctly set.
 */
bool firmwareUpdate();

#endif /* INNOVAOTA_H */

/****** END OF FILE ****/