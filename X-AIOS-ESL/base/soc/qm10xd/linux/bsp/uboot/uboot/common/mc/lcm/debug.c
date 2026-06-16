/*

 */
#include "../lcm_module/include/data_type.h"

int32_t debug_fun(uint32_t eb, const char *fun, uint32_t line, char *status)
{
    int32_t rtn = SUCCESS;

    CHECK_PTR(fun, NULL, "handle is null ~_~!\n");
    EXIT_IF_ZERO(line, NULL, "handle is null ~_~!\n");
    CHECK_PTR(status, NULL, "handle is null ~_~!\n");

    if (0 != eb) {
        printf("[LCM]--%s:line:%d--%s\n", fun, line, status);
    }

    EXIT:

    return rtn;
}

/*
0x08c01000 0x00000002 //ctrl bit1:test_en bit9-8: 00-color blk 01-color bar 10-single color 11-chess
0x08c01008 0xffffffff //shadow
0x08c010b4 0x00000002 // bit1-0: dp bit depth 00-24bits 888 01-18bits 666 10-16bit 565
                      // bit5-3: dp rgb order 1:RGB 2:GBR 3:GRB 4:BRG 5:BGR
                      // bit7: mcu mode 0: rgb 1:mcu
                      // bit9: de pol 0:high 1:low
                      // bit10: v_sync blank pol 0:high 1:low
                      // bit11: h_sync blank pol 0:high 1:low
                      // bit14: TE 0:close 1:enable
                      // bit16: dither 0:close 1:enable
                      // bit17: rgb bit swap
                      // bit18: clk negedge 0:rise output 1:down output
                      // bit18: TE negedge 0:rise ouput frame 1:down outpu frame
0x08c010bc 0x00000002 // bit5-4: sys if 00-18bit 01-16bit 10-9bit 11-8bit
                      // bit7-6: rgb if 00-18bit 01-16bit 10-6bit 11-24bit
                      // bit8: mcu timing 0: data active at rise edge wr/rd 0: data active at rise edge cs
                      // bit9: Te mode0: hw te 1: sw te
                      // bit10: mcu H/L byte first 0: low 8/9bits first 1: high 8/9bits first
                      // bit11: mcu 3 trans r/g/b each time
                      // bit14: mcu tx data checker fifo ready
0x08c010c0 0xffffffff //lcm pin map pin0-pin5
0x08c010c4 0xffffffff //lcm pin map pin6-pin11
0x08c010c8 0xffffffff //lcm pin map pin12-pin17
0x08c010cc 0xffffffff //lcm pin map pin18-pin23



*/
