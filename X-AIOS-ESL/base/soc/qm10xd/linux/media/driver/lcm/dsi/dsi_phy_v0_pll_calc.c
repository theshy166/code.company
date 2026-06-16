#include "dsi_pixel_info.h"
#include "dsi_phy_v0_pll_calc_param.h"
#include "dsi_phy_v01_drv.h"
#include "debug.h"
#include "os.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/
#define PLL_DEBUG_LOG 0

#define _REF_CLK 1536000000 // 24M (Q6X)
#define _MAX_F_VCO 40000000000 // ->2.5G (Q4X)
#define _MIN_F_VCO 16000000000 // ->1.0G (Q4X)

#define MAX_PLL_K BIT(23)

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef struct {
    uint64_t pixelclk_div;
    uint64_t pixel_clk;
    uint32_t bpp; // bits per pixel
    uint64_t data_rate; // Mbits per second
    uint64_t F_VCO;
    uint64_t F_VCO_calc;
    uint64_t F_VCO_FRACT;
    uint64_t N_Post_div;
    uint64_t N_Post_div_dec;
    uint64_t mif_div;
    uint64_t dhd_div;
    uint32_t mif_div_valid;
    uint64_t ref_clk;
    uint64_t f_pfd;
    uint32_t ref_clk_doubler;

    uint64_t pll_kint; // f_pfd_div fractional [31-9] for fractional mode
    uint64_t pll_nint; // f_pfd_div integer [8-0] for fractional mode

    uint32_t pll_div_s; // feadback div sel 0x78[28] 0:integer 1:fractional
    uint32_t pll_n; // f_pfd_div integer 0x80[24-14] for integer mode
    uint32_t laneNum;
}dsi_pll_context_t;
/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
dsi_pll_context_t s_dsi_pll_context;

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
dsi_pll_context_t * _get_dsi_pll_context(void)
{
    return &s_dsi_pll_context;
}

static int32_t _dsi_lane_data_rate_get(uint32_t pixel_clk, uint32_t bpp, uint32_t lane_num, uint64_t *data_rate)
{
    int32_t rtn = 0;
    uint32_t lane_clk = 0x00;
    uint32_t _data_rate = 0x00;

    if (0 < lane_num) {
        _data_rate = (pixel_clk * bpp ) / lane_num;
        lane_clk = (_data_rate) >> 1;
    }

    *data_rate = (uint64_t)_data_rate;

    if (lane_clk) {
    }

    #if PLL_DEBUG_LOG
    printf("lane_num:%d, data_rate:%d.%dM, lane_clk:%d.%dM\n", lane_num,
                                                        (_data_rate)/1000000, (_data_rate)%1000000,
                                                        lane_clk/1000000, lane_clk%1000000);
    #endif

    return rtn;
}

static int32_t _dsi_lane_data_rate_get_by_pll(mif_clk_t *clk, uint64_t *data_rate)
{
    int32_t rtn = SUCCESS;
    dsi_pll_context_t * context = _get_dsi_pll_context();
    uint64_t REF_CLK = _REF_CLK;
    uint64_t post_div = 0x00;

    REF_CLK = REF_CLK >> 6;

    context->f_pfd = REF_CLK << clk->pll_doubler;

    if (0 == clk->pll_div_s) {
            context->F_VCO = context->f_pfd * clk->pll_n;
    } else if (1 == clk->pll_div_s) {
        context->F_VCO = context->f_pfd * clk->pll_kint + ((context->f_pfd * clk->pll_nint) >> 23);
    }

    post_div = (1 << clk->pll_pdiv);

    *data_rate = do_div64_u64(context->F_VCO, post_div);

    #if PLL_DEBUG_LOG
    printf("lane data rate : %lld\n", *data_rate);
    #endif

    return rtn;
}

static int32_t _dsi_host_sync_div_get(uint32_t bpp, uint32_t lane_num, uint32_t *sync_div)
{
    int32_t rtn = 0;

    CHECK_PTR(sync_div, NULL, "host sync_div ptr is null ~_~!\n");
    EXIT_IF_ZERO(lane_num, NULL, "data lane_num is zero ~_~!\n");

    *sync_div = bpp / lane_num;

    #if PLL_DEBUG_LOG
    printf("dsi_host_sync_div:%d\n", *sync_div);
    #endif

    EXIT:

    return rtn;
}

int32_t dsi_phy_pll_cfg_get(dsi_phy_pll_calc_in_t *inParam, dsi_phy_pll_calc_out_t *outParam)
{
    int32_t rtn = 0x00;
    dsi_pll_context_t * context = _get_dsi_pll_context();
    dsi_phy_pll_calc_in_t *phy_pll_cfg_in_ptr = inParam;
    dsi_phy_pll_calc_out_t *phy_pll_cfg_out_ptr = outParam;
    dsi_get_pixel_clk_in_t getPixelClkIn = {0x00};
    dsi_get_pixel_clk_out_t getPixelClkOut = {0x00};
    uint64_t MIN_F_VCO = _MIN_F_VCO;
    uint64_t MAX_F_VCO = _MAX_F_VCO;
    uint64_t REF_CLK = _REF_CLK;

    /*
    printf("N_loop_div = 8 * (N_I_post_div + N_F_post_div);\n");
    printf("F_vco = ref_clk * N_loop_div / ref_clk_div;\n");
    printf("data_rate = F_vco / rate;\n");

    typedef struct {
        uint32_t pll_doubler; // src clk doubler en
        uint32_t pll_div_s; // feadback div sel 0:integer 1:fractional
        uint32_t pll_n; // f_pfd_div integer [24-14] for integer mode
        uint32_t pll_kint; // f_pfd_div fractional [31-9] for fractional mode
        uint32_t pll_nint; // f_pfd_div integer [8-0] for fractional mode
        uint32_t pixelclk_div; // fvco/N [29-26]
        uint32_t pll_pdiv;     // post div [25-23]
        uint32_t dhd_div;
        uint32_t lane_clk;
    } dsi_phy_pll_cfg_out_t;

    */

    #if PLL_DEBUG_LOG
    printf("\n");
    #endif

    context->laneNum = phy_pll_cfg_in_ptr->laneNum + 1;
    rtn = dsi_bits_per_pixel_get(phy_pll_cfg_in_ptr->format, &context->bpp);

    if ((NULL == phy_pll_cfg_in_ptr->clk)
        || (0 == phy_pll_cfg_in_ptr->clk->mode)) {
        #if PLL_DEBUG_LOG
        printf("-----------------mipi phy pll auto calc--------------------\n");
        #endif

        getPixelClkIn.dp_sync = inParam->dp_sync;
        getPixelClkIn.fps = inParam->fps;

        rtn = dsi_pixel_clk_get(&getPixelClkIn, &getPixelClkOut);
        context->pixel_clk = (uint64_t)getPixelClkOut.pixel_clk;

        rtn = _dsi_lane_data_rate_get(getPixelClkOut.pixel_clk, context->bpp, context->laneNum, &context->data_rate);

        MIN_F_VCO = MIN_F_VCO >> 4;
        MAX_F_VCO = MAX_F_VCO >> 4;
        REF_CLK = REF_CLK >> 6;

        /* calc F_VCO and post_div  start */
        if (context->data_rate < MAX_F_VCO) {
            for (context->N_Post_div = 0; context->N_Post_div < 5; context->N_Post_div++) {

                context->N_Post_div_dec = (1 << context->N_Post_div);
                context->F_VCO = context->data_rate * context->N_Post_div_dec;

                if ((MIN_F_VCO < context->F_VCO)
                    && (MAX_F_VCO >= context->F_VCO)){
                    break ;
                }
            }
        } else {
            printf("data_rate:%lld more than MAX_F_VCO:%lld ~_~!\n", context->data_rate, MAX_F_VCO);
        }
        context->N_Post_div_dec = (1 << context->N_Post_div);
        context->F_VCO = context->data_rate * context->N_Post_div_dec;
        #if PLL_DEBUG_LOG
        printf("F_VCO:%lld N_Post_div_dec:%lld !\n", context->F_VCO, context->N_Post_div_dec);
        #endif

        if (context->F_VCO < MIN_F_VCO) {
            printf("F_VCO:%lld less than MIN_F_VCO:%lld ~_~!\n", context->F_VCO, MIN_F_VCO);
            goto EXIT;
        }
        /* calc F_VCO and post_div end */

        /* calc N of f_vco = f_pfd start * N  start*/
        context->pll_div_s = 0x01;
        for (context->ref_clk_doubler = 0; context->ref_clk_doubler < 2; context->ref_clk_doubler++) {
            context->f_pfd = (REF_CLK << context->ref_clk_doubler);
            for (context->pll_n = 0; context->pll_n < 0x7ff; context->pll_n++) {

                context->F_VCO_calc = context->f_pfd * context->pll_n;

                if (context->F_VCO < context->F_VCO_calc) { // no equal got to next
                    break;
                }

                if (context->F_VCO == context->F_VCO_calc) {
                    context->pll_div_s = 0x00;
                    goto PLL_DIV_S_END;
                }
            }
        }

        PLL_DIV_S_END:

        if (1 == context->pll_div_s) {
            context->pll_n = 0x00;
            for (context->ref_clk_doubler = 0; context->ref_clk_doubler < 2; context->ref_clk_doubler++) {
                context->f_pfd = (uint32_t)(REF_CLK << context->ref_clk_doubler);
                if (context->F_VCO < (context->f_pfd * 1024)) {
                    context->pll_nint = do_div64_u64(context->F_VCO, context->f_pfd);
                    context->F_VCO_FRACT = context->F_VCO - (context->f_pfd * context->pll_nint);
                    context->F_VCO_FRACT = ((context->F_VCO - (context->f_pfd * context->pll_nint)) * MAX_PLL_K);
                    context->pll_kint = do_div64_u64(context->F_VCO_FRACT, context->f_pfd);
                    break;
                }
            }
        }

        #if PLL_DEBUG_LOG
        printf("f_pfd:%lld !\n", context->f_pfd);
        #endif
        /* calc N of f_vco = f_pfd start * N  end*/

        /* calc pixel clk div and dhd_div and check mif_div valid start */
        context->mif_div = do_div64_u64(context->F_VCO, context->pixel_clk);
        for (context->dhd_div = 1; context->dhd_div < 17; context->dhd_div++) {
            for (context->pixelclk_div = 1; context->pixelclk_div < 17; context->pixelclk_div++) {
                if (context->mif_div == (context->pixelclk_div * context->dhd_div)) {
                    context->mif_div_valid = SUCCESS;

                    goto DHD_DIV_END;
                }
            }
        }

        DHD_DIV_END:

        #if PLL_DEBUG_LOG
        printf("mif_div:%lld pixelclk_div=%lld dhd_div=%lld!\n", context->mif_div, context->pixelclk_div, context->dhd_div);
        #endif

        context->pixelclk_div = (16 == context->pixelclk_div) ? 0 : context->pixelclk_div;
        context->dhd_div -= 1;

        if (SUCCESS != context->mif_div_valid) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "mif_div_valid ~_~!\n");
        }

        /* calc pixel clk div and dhd_div and check mif_div valid end */

    } else {
        #if PLL_DEBUG_LOG
        printf("-----------------mipi phy pll manual calc--------------------\n");
        #endif
        _dsi_lane_data_rate_get_by_pll(phy_pll_cfg_in_ptr->clk, &context->data_rate);
        context->ref_clk_doubler = phy_pll_cfg_in_ptr->clk->pll_doubler;
        context->pll_div_s = phy_pll_cfg_in_ptr->clk->pll_div_s;
        context->pll_n = phy_pll_cfg_in_ptr->clk->pll_n;
        context->pll_kint = phy_pll_cfg_in_ptr->clk->pll_kint;
        context->pll_nint = phy_pll_cfg_in_ptr->clk->pll_nint;
        context->N_Post_div = phy_pll_cfg_in_ptr->clk->pll_pdiv;
        context->pixelclk_div = phy_pll_cfg_in_ptr->clk->pixelclk_div;
        context->dhd_div = phy_pll_cfg_in_ptr->clk->dhd_div;
        context->data_rate = phy_pll_cfg_in_ptr->clk->data_rate;
    }

    phy_pll_cfg_out_ptr->pll_doubler = context->ref_clk_doubler;
    phy_pll_cfg_out_ptr->pll_div_s = context->pll_div_s;
    phy_pll_cfg_out_ptr->pll_n = context->pll_n;
    phy_pll_cfg_out_ptr->pll_kint = (uint32_t)context->pll_kint;
    phy_pll_cfg_out_ptr->pll_nint = (uint32_t)context->pll_nint;
    phy_pll_cfg_out_ptr->pll_pdiv = (uint32_t)context->N_Post_div;
    phy_pll_cfg_out_ptr->pixelclk_div = context->pixelclk_div;
    phy_pll_cfg_out_ptr->dhd_div = (uint32_t)context->dhd_div;
    phy_pll_cfg_out_ptr->lane_clk = (uint32_t)(context->data_rate >> 1);
    rtn = _dsi_host_sync_div_get(context->bpp, context->laneNum, phy_pll_cfg_out_ptr->host_sync_div);

    #if PLL_DEBUG_LOG
    printf("pll_doubler  :%d\n", phy_pll_cfg_out_ptr->pll_doubler );
    printf("pll_div_s    :%d\n", phy_pll_cfg_out_ptr->pll_div_s   );
    printf("pll_n        :%d\n", phy_pll_cfg_out_ptr->pll_n       );
    printf("pll_kint     :%d\n", phy_pll_cfg_out_ptr->pll_kint    );
    printf("pll_nint     :%d\n", phy_pll_cfg_out_ptr->pll_nint    );
    printf("pll_pdiv     :%d\n", phy_pll_cfg_out_ptr->pll_pdiv    );
    printf("pixelclk_div :%d\n", phy_pll_cfg_out_ptr->pixelclk_div);
    printf("dhd_div      :%d\n", phy_pll_cfg_out_ptr->dhd_div     );
    printf("lane_clk     :%d\n", phy_pll_cfg_out_ptr->lane_clk    );
    printf("host_sync_div:%d\n", *phy_pll_cfg_out_ptr->host_sync_div    );
    printf("-----------------mipi phy pll calc end-----------------------\n");
    printf("\n");
    #endif

    EXIT:

    return rtn;
}

int32_t rgb_pll_cfg_get(dsi_phy_pll_calc_in_t *inParam, dsi_phy_pll_calc_out_t *outParam)
{
    int32_t rtn = 0x00;
    dsi_pll_context_t * context = _get_dsi_pll_context();
    dsi_phy_pll_calc_in_t *phy_pll_cfg_in_ptr = inParam;
    dsi_phy_pll_calc_out_t *phy_pll_cfg_out_ptr = outParam;
    dsi_get_pixel_clk_in_t getPixelClkIn = {0x00};
    dsi_get_pixel_clk_out_t getPixelClkOut = {0x00};
    uint64_t MIN_F_VCO = _MIN_F_VCO;
    uint64_t MAX_F_VCO = _MAX_F_VCO;
    uint64_t REF_CLK = _REF_CLK;
    uint64_t mif_clk = 0x00;
    uint64_t tmp_mif_clk = 0x00;

    /*
    printf("N_loop_div = 8 * (N_I_post_div + N_F_post_div);\n");
    printf("F_vco = ref_clk * N_loop_div / ref_clk_div;\n");
    printf("data_rate = F_vco / rate;\n");

    typedef struct {
        uint32_t pll_doubler; // src clk doubler en
        uint32_t pll_div_s; // feadback div sel 0:integer 1:fractional
        uint32_t pll_n; // f_pfd_div integer [24-14] for integer mode
        uint32_t pll_kint; // f_pfd_div fractional [31-9] for fractional mode
        uint32_t pll_nint; // f_pfd_div integer [8-0] for fractional mode
        uint32_t pixelclk_div; // fvco/N [29-26]
        uint32_t pll_pdiv;     // post div [25-23]
        uint32_t dhd_div;
        uint32_t lane_clk;
    } dsi_phy_pll_cfg_out_t;

    */

    #if PLL_DEBUG_LOG
    printf("\n");
    #endif

    if ((NULL == phy_pll_cfg_in_ptr->clk)
        || (0 == phy_pll_cfg_in_ptr->clk->mode)) {
        #if PLL_DEBUG_LOG
        printf("-----------------rgb pll auto calc--------------------\n");
        #endif

        getPixelClkIn.dp_sync = inParam->dp_sync;
        getPixelClkIn.fps = inParam->fps;

        rtn = dsi_pixel_clk_get(&getPixelClkIn, &getPixelClkOut);
        context->pixel_clk = (uint64_t)getPixelClkOut.pixel_clk;

        MIN_F_VCO = MIN_F_VCO >> 4; //2500000000
        MAX_F_VCO = MAX_F_VCO >> 4; //1000000000
        REF_CLK = REF_CLK >> 6; //24000000

        context->laneNum = phy_pll_cfg_in_ptr->laneNum;
        mif_clk = context->pixel_clk * context->laneNum;

        if (mif_clk < MAX_F_VCO) {
            for (context->pixelclk_div = 1; context->pixelclk_div < 17; context->pixelclk_div++) {
                context->F_VCO = mif_clk * context->pixelclk_div;

                if ((MIN_F_VCO < context->F_VCO)
                    && (MAX_F_VCO >= context->F_VCO)){
                    break ;
                }
            }
        } else {
            printf("mif_clk:%lld more than MAX_F_VCO:%lld ~_~!\n", mif_clk, MAX_F_VCO);
        }

        if (context->F_VCO < MIN_F_VCO) {
            for (context->dhd_div = 1; context->dhd_div < 17; context->dhd_div++) {
                tmp_mif_clk = (mif_clk * context->dhd_div);

                for (context->pixelclk_div = 1; context->pixelclk_div < 17; context->pixelclk_div++) {
                    context->F_VCO = tmp_mif_clk * context->pixelclk_div;

                    if ((MIN_F_VCO < context->F_VCO)
                        && (MAX_F_VCO >= context->F_VCO)){
                        goto PLL_FVCO_END;
                    }
                }
            }
        } else {
            context->dhd_div = 0;
        }

        PLL_FVCO_END:

        context->pixelclk_div = (16 == context->pixelclk_div) ? 0 : context->pixelclk_div;
        if (context->dhd_div > 1) {
            context->dhd_div -= 1;
        }

#if PLL_DEBUG_LOG
        printf("pixelclk_div=%lld dhd_div=%lld! \n", context->pixelclk_div, context->dhd_div);
        printf("F_VCO=%lld pixel_clk=%lld mif_clk:%lld intf_clk=%lld !\n", context->F_VCO, context->pixel_clk, tmp_mif_clk, do_div64_u64(tmp_mif_clk, (context->dhd_div + 1)));
#endif

        /* calc F_VCO and post_div end */

        /* calc N of f_vco = f_pfd start * N  start*/
        context->pll_div_s = 0x01;
        for (context->ref_clk_doubler = 0; context->ref_clk_doubler < 2; context->ref_clk_doubler++) {
            context->f_pfd = (REF_CLK << context->ref_clk_doubler);
            for (context->pll_n = 0; context->pll_n < 0x7ff; context->pll_n++) {

                context->F_VCO_calc = context->f_pfd * context->pll_n;

                if (context->F_VCO < context->F_VCO_calc) { // no equal got to next
                    break;
                }

                if (context->F_VCO == context->F_VCO_calc) {
                    context->pll_div_s = 0x00;
                    goto PLL_DIV_S_END;
                }
            }
        }

        PLL_DIV_S_END:

        if (1 == context->pll_div_s) {
            context->pll_n = 0x00;
            for (context->ref_clk_doubler = 0; context->ref_clk_doubler < 2; context->ref_clk_doubler++) {
                context->f_pfd = (uint32_t)(REF_CLK << context->ref_clk_doubler);
                if (context->F_VCO < (context->f_pfd * 1024)) {
                    context->pll_nint = do_div64_u64(context->F_VCO, context->f_pfd);
                    context->F_VCO_FRACT = context->F_VCO - (context->f_pfd * context->pll_nint);
                    context->F_VCO_FRACT = ((context->F_VCO - (context->f_pfd * context->pll_nint)) * MAX_PLL_K);
                    context->pll_kint = do_div64_u64(context->F_VCO_FRACT, context->f_pfd);
                    break;
                }
            }
        }

        #if PLL_DEBUG_LOG
        printf("f_pfd:%lld ! \n", context->f_pfd);
        #endif

    } else {
        #if PLL_DEBUG_LOG
        printf("-----------------rgb pll manual calc--------------------\n");
        #endif
        context->ref_clk_doubler = phy_pll_cfg_in_ptr->clk->pll_doubler;
        context->pll_div_s = phy_pll_cfg_in_ptr->clk->pll_div_s;
        context->pll_n = phy_pll_cfg_in_ptr->clk->pll_n;
        context->pll_kint = phy_pll_cfg_in_ptr->clk->pll_kint;
        context->pll_nint = phy_pll_cfg_in_ptr->clk->pll_nint;
        context->N_Post_div = phy_pll_cfg_in_ptr->clk->pll_pdiv;
        context->pixelclk_div = phy_pll_cfg_in_ptr->clk->pixelclk_div;
        context->dhd_div = phy_pll_cfg_in_ptr->clk->dhd_div;
        context->data_rate = phy_pll_cfg_in_ptr->clk->data_rate;
    }

    phy_pll_cfg_out_ptr->pll_doubler = context->ref_clk_doubler;
    phy_pll_cfg_out_ptr->pll_div_s = context->pll_div_s;
    phy_pll_cfg_out_ptr->pll_n = context->pll_n;
    phy_pll_cfg_out_ptr->pll_kint = (uint32_t)context->pll_kint;
    phy_pll_cfg_out_ptr->pll_nint = (uint32_t)context->pll_nint;
    phy_pll_cfg_out_ptr->pll_pdiv = 0;
    phy_pll_cfg_out_ptr->pixelclk_div = context->pixelclk_div;
    phy_pll_cfg_out_ptr->dhd_div = (uint32_t)context->dhd_div;
    phy_pll_cfg_out_ptr->lane_clk = 0;
    phy_pll_cfg_out_ptr->host_sync_div = 0;

    #if PLL_DEBUG_LOG
    printf("pll_doubler  :%d\n", phy_pll_cfg_out_ptr->pll_doubler );
    printf("pll_div_s    :%d\n", phy_pll_cfg_out_ptr->pll_div_s   );
    printf("pll_n        :%d\n", phy_pll_cfg_out_ptr->pll_n       );
    printf("pll_kint     :%d\n", phy_pll_cfg_out_ptr->pll_kint    );
    printf("pll_nint     :%d\n", phy_pll_cfg_out_ptr->pll_nint    );
    printf("pll_pdiv     :%d\n", phy_pll_cfg_out_ptr->pll_pdiv    );
    printf("pixelclk_div :%d\n", phy_pll_cfg_out_ptr->pixelclk_div);
    printf("dhd_div      :%d\n", phy_pll_cfg_out_ptr->dhd_div     );
    printf("-----------------rgb pll calc end-----------------------\n");
    printf("\n");
    #endif

    return rtn;
}

int32_t mcu_pll_cfg_get(dsi_phy_pll_calc_in_t *inParam, dsi_phy_pll_calc_out_t *outParam)
{
#define INTF_TO_MIF_CLK 4   //default 0x260010b4/0x260010d8

    int32_t rtn = 0x00;
    dsi_pll_context_t * context = _get_dsi_pll_context();
    dsi_phy_pll_calc_in_t *phy_pll_cfg_in_ptr = inParam;
    dsi_phy_pll_calc_out_t *phy_pll_cfg_out_ptr = outParam;
    dsi_get_pixel_clk_in_t getPixelClkIn = {0x00};
    dsi_get_pixel_clk_out_t getPixelClkOut = {0x00};
    uint64_t MIN_F_VCO = _MIN_F_VCO;
    uint64_t MAX_F_VCO = _MAX_F_VCO;
    uint64_t REF_CLK = _REF_CLK;
    uint64_t mif_clk = 0x00;
    uint64_t tmp_mif_clk = 0x00;

    /*
    printf("N_loop_div = 8 * (N_I_post_div + N_F_post_div);\n");
    printf("F_vco = ref_clk * N_loop_div / ref_clk_div;\n");
    printf("data_rate = F_vco / rate;\n");

    typedef struct {
        uint32_t pll_doubler; // src clk doubler en
        uint32_t pll_div_s; // feadback div sel 0:integer 1:fractional
        uint32_t pll_n; // f_pfd_div integer [24-14] for integer mode
        uint32_t pll_kint; // f_pfd_div fractional [31-9] for fractional mode
        uint32_t pll_nint; // f_pfd_div integer [8-0] for fractional mode
        uint32_t pixelclk_div; // fvco/N [29-26]
        uint32_t pll_pdiv;     // post div [25-23]
        uint32_t dhd_div;
        uint32_t lane_clk;
    } dsi_phy_pll_cfg_out_t;

    */

    #if PLL_DEBUG_LOG
    printf("\n");
    #endif

    if ((NULL == phy_pll_cfg_in_ptr->clk)
        || (0 == phy_pll_cfg_in_ptr->clk->mode)) {
        #if PLL_DEBUG_LOG
        printf("-----------------mcu pll auto calc--------------------\n");
        #endif

        getPixelClkIn.dp_sync = inParam->dp_sync;
        getPixelClkIn.fps = inParam->fps;

        rtn = mcu_pixel_clk_get(&getPixelClkIn, &getPixelClkOut);
        context->pixel_clk = (uint64_t)getPixelClkOut.pixel_clk;

        MIN_F_VCO = MIN_F_VCO >> 4;
        MAX_F_VCO = MAX_F_VCO >> 4;
        REF_CLK = REF_CLK >> 6;

        context->laneNum = phy_pll_cfg_in_ptr->laneNum;
        mif_clk = context->pixel_clk * context->laneNum * INTF_TO_MIF_CLK;

        if (mif_clk < MAX_F_VCO) {
            for (context->pixelclk_div = 1; context->pixelclk_div < 17; context->pixelclk_div++) {
                context->F_VCO = mif_clk * context->pixelclk_div;

                if ((MIN_F_VCO < context->F_VCO)
                    && (MAX_F_VCO >= context->F_VCO)){
                    break ;
                }
            }
        } else {
            printf("mif_clk:%lld more than MAX_F_VCO:%lld ~_~!\n", mif_clk, MAX_F_VCO);
        }

        if (context->F_VCO < MIN_F_VCO) {
            for (context->dhd_div = 1; context->dhd_div < 17; context->dhd_div++) {
                tmp_mif_clk = (mif_clk * context->dhd_div);

                for (context->pixelclk_div = 1; context->pixelclk_div < 17; context->pixelclk_div++) {
                    context->F_VCO = tmp_mif_clk * context->pixelclk_div;

                    if ((MIN_F_VCO < context->F_VCO)
                        && (MAX_F_VCO >= context->F_VCO)){
                        goto PLL_FVCO_END;
                    }
                }
            }
        } else {
            context->dhd_div = 0;
        }

        PLL_FVCO_END:

        context->pixelclk_div = (16 == context->pixelclk_div) ? 0 : context->pixelclk_div;
        if (context->dhd_div > 1) {
            context->dhd_div -= 1;
        }

#if PLL_DEBUG_LOG
        printf("pixelclk_div=%lld dhd_div=%lld! \n", context->pixelclk_div, context->dhd_div);
        printf("F_VCO=%lld pixel_clk=%lld mif_clk:%lld intf_clk=%lld !\n", context->F_VCO, context->pixel_clk, tmp_mif_clk, do_div64_u64(tmp_mif_clk, (context->dhd_div + 1)));
#endif

        /* calc F_VCO and post_div end */

        /* calc N of f_vco = f_pfd start * N  start*/
        context->pll_div_s = 0x01;
        for (context->ref_clk_doubler = 0; context->ref_clk_doubler < 2; context->ref_clk_doubler++) {
            context->f_pfd = (REF_CLK << context->ref_clk_doubler);
            for (context->pll_n = 0; context->pll_n < 0x7ff; context->pll_n++) {

                context->F_VCO_calc = context->f_pfd * context->pll_n;

                if (context->F_VCO < context->F_VCO_calc) { // no equal got to next
                    break;
                }

                if (context->F_VCO == context->F_VCO_calc) {
                    context->pll_div_s = 0x00;
                    goto PLL_DIV_S_END;
                }
            }
        }

        PLL_DIV_S_END:

        if (1 == context->pll_div_s) {
            context->pll_n = 0x00;
            for (context->ref_clk_doubler = 0; context->ref_clk_doubler < 2; context->ref_clk_doubler++) {
                context->f_pfd = (uint32_t)(REF_CLK << context->ref_clk_doubler);
                if (context->F_VCO < (context->f_pfd * 1024)) {
                    context->pll_nint = do_div64_u64(context->F_VCO, context->f_pfd);
                    context->F_VCO_FRACT = context->F_VCO - (context->f_pfd * context->pll_nint);
                    context->F_VCO_FRACT = ((context->F_VCO - (context->f_pfd * context->pll_nint)) * MAX_PLL_K);
                    context->pll_kint = do_div64_u64(context->F_VCO_FRACT, context->f_pfd);
                    break;
                }
            }
        }

        #if PLL_DEBUG_LOG
        printf("f_pfd:%lld ! \n", context->f_pfd);
        #endif

    } else {
        #if PLL_DEBUG_LOG
        printf("-----------------mcu pll manual calc--------------------\n");
        #endif
        context->ref_clk_doubler = phy_pll_cfg_in_ptr->clk->pll_doubler;
        context->pll_div_s = phy_pll_cfg_in_ptr->clk->pll_div_s;
        context->pll_n = phy_pll_cfg_in_ptr->clk->pll_n;
        context->pll_kint = phy_pll_cfg_in_ptr->clk->pll_kint;
        context->pll_nint = phy_pll_cfg_in_ptr->clk->pll_nint;
        context->N_Post_div = phy_pll_cfg_in_ptr->clk->pll_pdiv;
        context->pixelclk_div = phy_pll_cfg_in_ptr->clk->pixelclk_div;
        context->dhd_div = phy_pll_cfg_in_ptr->clk->dhd_div;
        context->data_rate = phy_pll_cfg_in_ptr->clk->data_rate;
    }

    phy_pll_cfg_out_ptr->pll_doubler = context->ref_clk_doubler;
    phy_pll_cfg_out_ptr->pll_div_s = context->pll_div_s;
    phy_pll_cfg_out_ptr->pll_n = context->pll_n;
    phy_pll_cfg_out_ptr->pll_kint = (uint32_t)context->pll_kint;
    phy_pll_cfg_out_ptr->pll_nint = (uint32_t)context->pll_nint;
    phy_pll_cfg_out_ptr->pll_pdiv = 0;
    phy_pll_cfg_out_ptr->pixelclk_div = context->pixelclk_div;
    phy_pll_cfg_out_ptr->dhd_div = (uint32_t)context->dhd_div;
    phy_pll_cfg_out_ptr->lane_clk = 0;
    phy_pll_cfg_out_ptr->host_sync_div = 0;

    #if PLL_DEBUG_LOG
    printf("pll_doubler  :%d\n", phy_pll_cfg_out_ptr->pll_doubler );
    printf("pll_div_s    :%d\n", phy_pll_cfg_out_ptr->pll_div_s   );
    printf("pll_n        :%d\n", phy_pll_cfg_out_ptr->pll_n       );
    printf("pll_kint     :%d\n", phy_pll_cfg_out_ptr->pll_kint    );
    printf("pll_nint     :%d\n", phy_pll_cfg_out_ptr->pll_nint    );
    printf("pll_pdiv     :%d\n", phy_pll_cfg_out_ptr->pll_pdiv    );
    printf("pixelclk_div :%d\n", phy_pll_cfg_out_ptr->pixelclk_div);
    printf("dhd_div      :%d\n", phy_pll_cfg_out_ptr->dhd_div     );
    printf("-----------------mcu pll calc end-----------------------\n");
    printf("\n");
    #endif

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
