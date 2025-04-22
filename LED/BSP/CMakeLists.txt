idf_component_register(SRCS "LED.c"
                    INCLUDE_DIRS "include"
					REQUIRES driver)

component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)  
