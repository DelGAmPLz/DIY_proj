file(GLOB_RECURSE srcs "*.c")
set(include_dirs "." "./spilcd/include" "./DS18B20/include")
idf_component_register(
	SRCS ${srcs}
	INCLUDE_DIRS ${include_dirs}
	REQUIRES driver esp_lcd
)
