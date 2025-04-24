# idf_component_register(SRCS "Convert_DemicalPlace.c" "SPI_LCD.c" "ADC.c" "../BSP/ SPI_LCD_FONT.c" "../BSP/SPI_LCD.c" "../BSP/ADC.c"
#                     INCLUDE_DIRS ".")
file(GLOB_RECURSE srcs "*.c")
set(include_dirs "." "include")
idf_component_register(
	SRCS ${srcs}
	INCLUDE_DIRS ${include_dirs}
	REQUIRES driver esp_lcd esp_adc
)