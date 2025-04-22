idf_component_register(SRCS "UART1.c" "LED.c"
                    INCLUDE_DIRS "include")
					component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)  
