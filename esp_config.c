#include "esp_config_db.h"
#include "esp_config.h"

static const char* tag = "config";

int esp_config_get_i32(const char *ns, const char *key, int32_t *value) {

    int status = -1;
    int interr = -1;
    esp_err_t esperr = ESP_FAIL;
    nvs_handle handle; // Not initialized as we do not know what would fit as an invalid handle

    // Try to fetch the value from the NVS first
    esperr = nvs_open(ns, NVS_READONLY, &handle);
    if (esperr == ESP_OK) {
        esperr = nvs_get_i32(handle, key, value);
        if (esperr == ESP_OK) {
            status = 0;
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }
        nvs_close(handle);
    } else {
        ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
    }

    // If fetching from the NVS failed, retrieve the value from the internal defaults database
    if (status != 0) {
        interr = esp_config_get_i32_default(ns, key, value);
        if (interr != -1) {
            status = 1;
        } else {
            ESP_LOGE(tag,"Could not get default i32 value.");
        }
    }

    assert(status >= 0);
    return status;
}

int esp_config_get_str(const char *ns, const char *key, char *value, size_t *valuesize) {

    int status = -1;
    int interr = -1;
    esp_err_t esperr = ESP_FAIL;
    nvs_handle handle; // Not initialized as we do not know what would fit as an invalid handle

    // If the passed value parameter is NULL, retrieve the string length
    if (value == NULL) {

        // Try with NVS first
        esperr = nvs_open(ns, NVS_READONLY, &handle);
        if (esperr == ESP_OK) {
            esperr = nvs_get_str(handle, key, NULL, valuesize);
            if (esperr == ESP_OK) {
                status = 0;
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
            nvs_close(handle);
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }

        // If NVS retrieval failed, use the internal database
        if (status != 0) {
            interr = esp_config_get_str_default(ns, key, NULL, valuesize);
            if (interr != -1) {
                status = 1;
            } else {
                ESP_LOGE(tag,"Could not get default str value size.");
            }
        }

    } else { // If the passed value parameter is NOT NULL, retrieve the string

        // Try with NVS first
        esperr = nvs_open(ns, NVS_READONLY, &handle);
        if (esperr == ESP_OK) {
            esperr = nvs_get_str(handle, key, value, valuesize);
            if (esperr == ESP_OK) {
                status = 2;
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
            nvs_close(handle);
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }

        // If NVS retrieval failed, use the internal database
        if (status != 2) {
            interr = esp_config_get_str_default(ns, key, value, valuesize);
            if (interr != -1) {
                status = 3;
            } else {
                ESP_LOGE(tag,"Could not get default str value.");
            }
        }

    }

    assert(status >= 0);
    return status;
}

int esp_config_get_blob(const char *ns, const char *key, void *value, size_t *valuesize) {

    int status = -1;
    int interr = -1;
    esp_err_t esperr = ESP_FAIL;
    nvs_handle handle; // Not initialized as we do not know what would fit as an invalid handle

    // If the passed value parameter is NULL, retrieve the string length
    if (value == NULL) {

        // Try with NVS first
        esperr = nvs_open(ns, NVS_READONLY, &handle);
        if (esperr == ESP_OK) {
            esperr = nvs_get_blob(handle, key, NULL, valuesize);
            if (esperr == ESP_OK) {
                status = 0;
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
            nvs_close(handle);
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }

        // If NVS retrieval failed, use the internal database
        if (status != 0) {
            interr = esp_config_get_blob_default(ns, key, NULL, valuesize);
            if (interr != -1) {
                status = 1;
            } else {
                ESP_LOGE(tag,"Could not get default str value size.");
            }
        }

    } else { // If the passed value parameter is NOT NULL, retrieve the string

        // Try with NVS first
        esperr = nvs_open(ns, NVS_READONLY, &handle);
        if (esperr == ESP_OK) {
            esperr = nvs_get_blob(handle, key, value, valuesize);
            if (esperr == ESP_OK) {
                status = 2;
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
            nvs_close(handle);
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }

        // If NVS retrieval failed, use the internal database
        if (status != 2) {
            interr = esp_config_get_blob_default(ns, key, value, valuesize);
            if (interr != -1) {
                status = 3;
            } else {
                ESP_LOGE(tag,"Could not get default str value.");
            }
        }

    }

    assert(status >= 0);
    return status;
}

int esp_config_get_i32_default(const char *ns, const char *key, int32_t *value) {

    for (int i=0; i<ESP_CONFIG_DB_ENTRIES; i++) {
        if (strcmp(ns,database[i].name) == 0) {
            for (int j=0; j<database[i].nentries; j++) {
                if (strcmp(key,database[i].entries[j].key) == 0 && database[i].entries[j].encoding == INT32) {
                    *value = database[i].entries[j].value.int32;
                    return 0;
                }
            }
        }
    }

    return -1;
}

int esp_config_get_str_default(const char *ns, const char *key, char *value, size_t *valuesize) {

    for (int i=0; i<ESP_CONFIG_DB_ENTRIES; i++) {
        if (strcmp(ns,database[i].name) == 0) {
            for (int j=0; j<database[i].nentries; j++) {
                if (strcmp(key,database[i].entries[j].key) == 0 && database[i].entries[j].encoding == STRING) {
                    // printf("namespace %s key %s value %s\n",database[i].name,database[i].entries[j].key,database[i].entries[j].value.string);
                    if (value == NULL) {
                        *valuesize = strlen(database[i].entries[j].value.string);
                        return 0;
                    }
                    strncpy(value, database[i].entries[j].value.string, *valuesize);
                    return 1;
                }
            }
        }
    }

    return -1;
}

int esp_config_get_blob_default(const char *ns, const char *key, void *value, size_t *valuesize) {

    for (int i=0; i<ESP_CONFIG_DB_ENTRIES; i++) {
        if (strcmp(ns,database[i].name) == 0) {
            for (int j=0; j<database[i].nentries; j++) {
                if (strcmp(key,database[i].entries[j].key) == 0 && database[i].entries[j].encoding == BLOB) {
                    // printf("namespace %s key %s value %s\n",database[i].name,database[i].entries[j].key,database[i].entries[j].value.string);
                    if (value == NULL) {
                        *valuesize = database[i].entries[j].value_size;
                        return 0;
                    }
                    memcpy(value, database[i].entries[j].value.blob, *valuesize);
                    return 1;
                }
            }
        }
    }

    return -1;
}

esp_err_t esp_config_set_i32(const char* ns, const char* key, int32_t value) {
    
    esp_err_t esperr = ESP_FAIL;
    nvs_handle handle; // Not initialized as we do not know what would fit as an invalid handle

    esperr = nvs_open(ns, NVS_READWRITE, &handle);
    if (esperr == ESP_OK) {
        esperr = nvs_set_i32(handle, key, value);
        if (esperr == ESP_OK) {
            esperr = nvs_commit(handle);
            if (esperr == ESP_OK) {
                // Do nothing! Success.
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }
        nvs_close(handle);
    } else {
        ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
    }

    return esperr;
}

esp_err_t esp_config_set_str(const char* ns, const char* key, const char* value) {
    
    esp_err_t esperr = ESP_FAIL;
    nvs_handle handle; // Not initialized as we do not know what would fit as an invalid handle

    esperr = nvs_open(ns, NVS_READWRITE, &handle);
    if (esperr == ESP_OK) {
        esperr = nvs_set_str(handle, key, value);
        if (esperr == ESP_OK) {
            esperr = nvs_commit(handle);
            if (esperr == ESP_OK) {
                // Do nothing! Success.
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }
        nvs_close(handle);
    } else {
        ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
    }

    return esperr;
}

esp_err_t esp_config_set_blob(const char* ns, const char* key, const void* value, size_t valuesize) {
    
    esp_err_t esperr = ESP_FAIL;
    nvs_handle handle; // Not initialized as we do not know what would fit as an invalid handle

    esperr = nvs_open(ns, NVS_READWRITE, &handle);
    if (esperr == ESP_OK) {
        esperr = nvs_set_blob(handle, key, value, valuesize);
        if (esperr == ESP_OK) {
            esperr = nvs_commit(handle);
            if (esperr == ESP_OK) {
                // Do nothing! Success.
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }
        nvs_close(handle);
    } else {
        ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
    }

    return esperr;
}

esp_err_t esp_config_reset() {

    esp_err_t esperr = ESP_FAIL;

    // Not sure if deinit() and init() are necessary here, but in a previous code iterations they were added after an initial implementation without.
    esperr = nvs_flash_deinit();
    if (esperr == ESP_OK) {
        esperr = nvs_flash_erase();
        if (esperr == ESP_OK) {
            esperr = nvs_flash_init();
            if (esperr == ESP_OK) {
                // Success!
            } else {
                ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
            }
        } else {
            ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
        }
    } else {
        ESP_LOGE(tag,"%s",esp_err_to_name(esperr));
    }

    return esperr;
}

void esp_config_print_summary() {

    int status = -1;
    const char* ns = NULL;
    const char* key = NULL;

    // int8_t int8value = 0;
    // uint8_t uint8value = 0;
    // int16_t int16value = 0;
    // uint16_t uint16value = 0;
    int32_t int32value = 0;
    // uint32_t uint32value = 0;
    // int64_t int64value = 0;
    // uint64_t uint64value = 0;
    char* strvalue = NULL;
    size_t strlength = 0;
    void* blobvalue = NULL;
    size_t bloblength = 0;

    for (int i = 0; i < ESP_CONFIG_DB_ENTRIES; i++) {
        ns = database[i].name;
        printf("%s\n", ns);
        for (int j = 0; j < database[i].nentries; j++) {
            key = database[i].entries[j].key;
            printf("%s : ", key);
            switch (database[i].entries[j].encoding) {
                // case INT8:
                // 	esp_config_get_i8(ns, key, &int8value);
                // 	printf("%d\n", int8value);
                // 	break;
                // case UINT8:
                // 	esp_config_get_u8(ns, key, &uint8value);
                // 	printf("%u\n", uint8value);
                // 	break;
                // case INT16:
                // 	esp_config_get_i16(ns, key, &int16value);
                // 	printf("%d\n", int16value);
                // 	break;
                // case UINT16:
                // 	esp_config_get_u16(ns, key, &uint16value);
                // 	printf("%u\n", uint16value);
                // 	break;
                case INT32:
                    status = esp_config_get_i32(ns, key, &int32value);
                    assert(status != -1);
                    switch(status) {
                        case 0:
                            printf("%d (NVS)\n", int32value);
                            break;
                        case 1:
                            printf("%d (default)\n", int32value);
                            break;
                        default:
                            printf("Unknown return code.\n");
                    }
                    break;
                // case UINT32:
                // 	esp_config_get_u32(ns, key, &uint32value);
                // 	printf("%u\n", uint32value);
                // 	break;
                // case INT64:
                // 	esp_config_get_i64(ns, key, &int64value);
                // 	printf("%lld\n", int64value);
                // 	break;
                // case UINT64:
                // 	esp_config_get_u64(ns, key, &uint64value);
                // 	printf("%llu\n", uint64value);
                // 	break;
                case STRING:
                    esp_config_get_str(ns, key, NULL, &strlength);
                    strvalue = calloc(1, strlength + 1);
                    assert(strvalue != NULL);
                    status = esp_config_get_str(ns, key, strvalue, &strlength);
                    switch(status) {
                        case 2:
                            printf("%s (NVS)\n", strvalue);
                            break;
                        case 3:
                            printf("%s (default)\n", strvalue);
                            break;
                        default:
                            printf("Unknown return code.\n");
                    }
                    free(strvalue);
                    break;
                case BLOB:
                    esp_config_get_blob(ns, key, NULL, &bloblength);
                    blobvalue = calloc(1, bloblength + 1);
                    assert(blobvalue != NULL);
                    status = esp_config_get_blob(ns, key, blobvalue, &bloblength);
                    switch(status) {
                        case 2:
                            printf("%s (NVS)\n", (char*)blobvalue);
                            break;
                        case 3:
                            printf("%s (default)\n", (char*)blobvalue);
                            break;
                        default:
                            printf("Unknown return code.\n");
                    }
                    free(blobvalue);
                    break;
                default:
                    printf("Unsupported encoding\n");
            }
        }
    }
}
