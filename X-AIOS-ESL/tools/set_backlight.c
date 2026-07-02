#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern int qm_backlight_getbri(void);
extern int qm_backlight_setbri(uint32_t brightness);

int main(int argc, char **argv)
{
    uint32_t value = 80;
    if(argc > 1) {
        value = (uint32_t)strtoul(argv[1], NULL, 0);
    }

    int before = qm_backlight_getbri();
    int ret = qm_backlight_setbri(value);
    int after = qm_backlight_getbri();

    printf("backlight before=%d set=%u ret=%d after=%d\n", before, value, ret, after);
    return ret == 0 ? 0 : 1;
}
