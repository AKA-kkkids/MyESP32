set(src_dirs
            LED
            KEY
            utils
            EXIT
            LEDC
            IIC
            XL9555
            USART
            24CXX
            )

set(include_dirs
            LED
            KEY
            utils
            EXIT
            LEDC
            IIC
            XL9555
            USART
            24CXX
            )

set(requires
            driver log)

idf_component_register(
    SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})
component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)