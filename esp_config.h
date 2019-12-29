/* @file esp_config.h
 * @author michele.riva@protonmail.com
 * @date 29 Dec 2019
 * @brief Configuration storage library with defaults.
 *
 * This library provides methods to read and write configuration
 * values. It builds on the ESP-IDF library and works by either 
 * relying on an internal immutable database where default values
 * are defined, and on the NVS memory where configuration
 * overrides are recorded.
 * 
 * It is reccommended to read the documentation related to the NVS
 * and related methods, as this library usage closely resembles them.
 */

#ifndef COMPONENTS_ESP_CONFIG_H_
#define COMPONENTS_ESP_CONFIG_H_

#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convenience function for retrieving an int32_t configuration value
 * 
 * This function tries to find a specific configuration value
 * in the NVS memory. If it cannot be found, it proceeds on
 * fetching it from the defaults database.
 * 
 * @return 0 if value found in NVS, 1 if value found in defaults database.
 */
int esp_config_get_i32(const char *ns, const char *key, int32_t *value);

/**
 * @brief Convenience function for retrieving a string configuration value
 * 
 * This function tries to find a specific configuration value
 * in the NVS memory. If it cannot be found, it proceeds on
 * fetching it from the defaults database.
 * To get the size necessary to allocate the value, call the
 * function with *value = NULL. *value_size will be set to the
 * value length.
 * 
 * NOTE: value_length is calculated via strlen() when fetched
 * from the defaults database.
 * 
 * @return 0 if *valuesize set via NVS, 1 if valuesize set via defaults database, 2 if *value set via NVS, 3 if *value set via defaults database.
 */
int esp_config_get_str(const char *ns, const char *key, char *value, size_t *valuesize);

/**
 * @brief Convenience function for retrieving a blob configuration value
 * 
 * This function tries to find a specific configuration value
 * in the NVS memory. If it cannot be found, it proceeds on
 * fetching it from the defaults database.
 * To get the size necessary to allocate the blob, call the
 * function with *value = NULL. *value_size will be set to the
 * blob length.
 * 
 * @return 0 if *valuesize set via NVS, 1 if valuesize set via defaults database, 2 if *value set via NVS, 3 if *value set via defaults database.
 */
int esp_config_get_blob(const char *ns, const char *key, void *value, size_t *valuesize);

/**
 * @brief Retrieve an int32_t configuration value from the defaults database
 * 
 * This function fetches a configuration value directly from the
 * defaults database.
 * 
 * @return 0 if value is found, -1 if value is not found.
 */
int esp_config_get_i32_default(const char *ns, const char *key, int32_t *value);

/**
 * @brief Retrieve a string configuration value from the defaults database
 * 
 * This function fetches a configuration value directly from the
 * defaults database.
 * To get the size necessary to allocate the value, call the
 * function with *value = NULL. *value_size will be set to the
 * value length.
 * 
 * @return 0 if *value_size is set, 1 if *value is set, -1 if value is not found.
 */
int esp_config_get_str_default(const char *ns, const char *key, char *value, size_t *valuesize);

/**
 * @brief Retrieve a blob configuration value from the defaults database
 * 
 * This function fetches a configuration value directly from the
 * defaults database.
 * To get the size necessary to allocate the value, call the
 * function with *value = NULL. *value_size will be set to the
 * value length.
 * 
 * @return 0 if *value_size is set, 1 if *value is set, -1 if value is not found.
 */
int esp_config_get_blob_default(const char *ns, const char *key, void *value, size_t *valuesize);

// NVS config write functions

/**
 * @brief Convenience function for setting an int32_t configuration value in the NVS.
 * 
 * This function sets a configuration value in the NVS, overriding
 * the one in the defaults database.
 * 
 * @return ESP_OK if success, other error codes if fail.
 */
esp_err_t esp_config_set_i32(const char* ns, const char* key, const int32_t value);

/**
 * @brief Convenience function for setting an int32_t configuration value in the NVS.
 * 
 * This function sets a configuration value in the NVS, overriding
 * the one in the defaults database.
 * 
 * @return ESP_OK if success, other error codes if fail.
 */
esp_err_t esp_config_set_str(const char* ns, const char* key, const char* value);

/**
 * @brief Convenience function for setting an int32_t configuration value in the NVS.
 * 
 * This function sets a configuration value in the NVS, overriding
 * the one in the defaults database.
 * 
 * @return ESP_OK if success, other error codes if fail.
 */
esp_err_t esp_config_set_blob(const char* ns, const char* key, const void* value, size_t valuesize);

/**
 * @brief Convenience function for erasing the NVS memory and all configuration overrides.
 * 
 * This function completely erases the NVS and all the configuration
 * overrides with it.
 * 
 * @return ESP_OK if success, other error codes if fail.
 */
esp_err_t esp_config_reset();

/**
 * @brief Prints a summary of all known configuration values.
 * 
 * This function prints a summary of all known configuration values
 * together with informations such as namespace and override status.
 */
void esp_config_print_summary();

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_ESP_CONFIG_H_ */
