file(GLOB_RECURSE srcs "*.c")
set(include_dirs "./qma6100p/include" "./spilcd/include")
idf_component_register(
	SRCS ${srcs}
	INCLUDE_DIRS ${include_dirs}
	REQUIRES driver esp_lcd
)