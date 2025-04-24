# idf_component_register(SRCS "main.c" "../BSP/ADC.c" "../BSP/SPI_LCD.c" "../BSP/XL9555.c" "../BSP/SPI_LCD_FONT.c" "../BSP/Convert_DemicalPlace.c"
#                     INCLUDE_DIRS "."
#                     )
idf_component_register(SRCS "main.c"
REQUIRES BSP)