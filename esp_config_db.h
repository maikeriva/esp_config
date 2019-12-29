/* @file esp_config_db.h
 * @author michele.riva@protonmail.com
 * @date 29 Dec 2019
 * @brief Default configuration database.
 *
 * This file defines a database containing default values for the system's
 * configuration, as well as the necessary types and structs.
 * It is designed to be imported alongside esp_config.h.
 * 
 * To define a database, first define your namespaces and then refer them
 * from the database[] array as shown in the example below.
 *
 * TODO:
 * - Complete writing functions for missing types (such as UINT8 and UINT32) in config.h
 */

#ifndef COMPONENTS_ESP_CONFIG_DB_H_
#define COMPONENTS_ESP_CONFIG_DB_H_

#include "nvs.h" // We will use types defined in the nvs library

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Possible encodings for values stored in the NVS.
 *
 * Provides an easy way to keep track of different encodings
 * used in the NVS.
 */
typedef enum {
	UINT8,
	INT8,
	UINT16,
	INT16,
	UINT32,
	INT32,
	UINT64,
	INT64,
	STRING,
	BLOB
} esp_config_encoding_t;

/**
 * @brief Configuration entry structure.
 *
 * It includes a key identifier and the associated value's encoding,
 * specified as an esp_config_encoding_t.
 */
typedef struct {
	const char* key;						/**< Key */
	const esp_config_encoding_t encoding;	/**< Value encoding */
	const union {
		const uint8_t uint8;
		const int8_t int8;
		const uint16_t uint16;
		const int16_t int16;
		const uint32_t uint32;
		const int32_t int32;
		const uint64_t uint64;
		const int64_t int64;
		const char* string;
		const void* blob;
	} value;						/**< Default value */
	const size_t value_size;
} esp_config_entry_t;

/**
 * @brief Configuration namespace structure.
 *
 * It includes the namespace name, the number of configuration
 * entries, and a pointer to an array of configuration entries.
 */
typedef struct {
	const char *name;						/**< Namespace name */
	const unsigned int nentries;			/**< Number of entries in the pointed array */
	const esp_config_entry_t *entries;		/**< Pointer to the entries array */
} esp_config_namespace_t;

/**
 * @brief EXAMPLE DEFINITION OF A NAMESPACE
 *
 * Here an example namespace and its key-value entries are
 * defined.
 */
#define ESP_CONFIG_DB_ENTRIES_EXAMPLE 3
static const esp_config_entry_t example[] = {
		{
				.key = "i32",
				.encoding = INT32,
				.value = {.int32 = 12345}
		},
		{
				.key = "str",
				.encoding = STRING,
				.value = {.string = "abcdef"}
		},
		{
				.key = "blob",
				.encoding = BLOB,
				.value = {.blob = "abcdef"},
				.value_size = 6
		}
};

/**
 * @brief DATABASE DEFINITION
 *
 * Here the defaults database is defined as an array of
 * esp_config_entry_t namespaces.
 */
#define ESP_CONFIG_DB_ENTRIES 1
static const esp_config_namespace_t database[] = {
		{
				.name = "example",
				.nentries = ESP_CONFIG_DB_ENTRIES_EXAMPLE,
				.entries = example
		}
};


#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_ESP_CONFIG_DB_H_ */
