--retain=g_pfnVectors

#define APP_BASE 0x00000000
#define RAM_BASE 0x20000000

MEMORY
{
    FLASH (RX) : origin = APP_BASE, length = 0x00040000
    SRAM (RWX) : origin = 0x20000000, length = 0x00008000
}

SECTIONS
{
    .intvecs:   > APP_BASE
    .text   :   > FLASH
    .const  :   > FLASH
    .cinit  :   > FLASH
    .pinit  :   > FLASH
    .init_array : > FLASH

    .vtable :   > RAM_BASE
    .data   :   > SRAM
    .bss    :   > SRAM
    .sysmem :   > SRAM
    .stack  :   > SRAM
}

__STACK_TOP = __stack + 1024;
