#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SdkApi::static_sdk_api" for configuration "Debug"
set_property(TARGET SdkApi::static_sdk_api APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SdkApi::static_sdk_api PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/sdk_api.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS SdkApi::static_sdk_api )
list(APPEND _IMPORT_CHECK_FILES_FOR_SdkApi::static_sdk_api "${_IMPORT_PREFIX}/lib/sdk_api.lib" )

# Import target "SdkApi::dynamic_sdk_api" for configuration "Debug"
set_property(TARGET SdkApi::dynamic_sdk_api APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SdkApi::dynamic_sdk_api PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sdk_api.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sdk_api.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS SdkApi::dynamic_sdk_api )
list(APPEND _IMPORT_CHECK_FILES_FOR_SdkApi::dynamic_sdk_api "${_IMPORT_PREFIX}/lib/sdk_api.lib" "${_IMPORT_PREFIX}/bin/sdk_api.dll" )

# Import target "SdkApi::sdk_tester" for configuration "Debug"
set_property(TARGET SdkApi::sdk_tester APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SdkApi::sdk_tester PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sdk_tester.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS SdkApi::sdk_tester )
list(APPEND _IMPORT_CHECK_FILES_FOR_SdkApi::sdk_tester "${_IMPORT_PREFIX}/bin/sdk_tester.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
