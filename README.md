# esp_config

This ESP-IDF component allows to define configuration defaults in a central hardcoded database, and to use the NVS only as a non-default configuration storage. This is useful in applications in which NVS might not be present, get corrupted, or become filled up, and makes the rest of the firmware completely oblivious of its presence.

The idea is to make firmware components attempt to first retrieve a customized setting from the NVS, while still providing a default value whether the retrieval should fail. Defaults are defined in a developer-defined internal data structure, centralized and standardized accross the whole firmware.

The library currently supports the following types: int32, strings, and blobs. It provides functions to:
- Attempt to get a value from the NVS first, and from the internal database as fallback
- Get directly the default value from the internal database
- Set a new value in the NVS to override the default value
- Print a summary of the current configuration

The internal database of defaults can be defined in `esp_config_db.h` towards the end of the file. It should be quite self-explanatory.

To use the library, include the `esp_config.h` and `esp_config_db.h` headers in your main application.

The library is written in pure C, yet a simple C++ port could be considered to avoid repetion through templating.