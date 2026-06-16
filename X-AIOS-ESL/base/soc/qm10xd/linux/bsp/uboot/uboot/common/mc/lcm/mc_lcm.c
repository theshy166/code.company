#include "../lcm_module/include/mol_lcm.h"
//#include "fy_common.h"
#include "clk/vou_clk_v01_reg_def.h"
#include "dsi/dsi_host_v01_reg_def.h"
#include "dsi/dsi_phy_v01_reg_def.h"
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
#define MC_LCM_DEBUG 0

#define REG_TOP_RST_REG_BASE                     DMC_SYS_RST_BASE_ADDR
#define REG_TOP_CLK_CFG_BASE                     DMC_SYS_CLK_BASE_ADDR
#define REG_TOP_VOU_SEL_BASE                     DMC_VOU_SEL_BASE_ADDR
#define REG_VOU_BASE                             VOU_BASE_ADDR
#define REG_DIS_HOST_2LANE_BASE                  DIS_HOST_2LANE_BASE_ADDR
#define REG_DIS_HOST_4LANE_BASE                  DIS_HOST_4LANE_BASE_ADDR
#define REG_DSI_DPHY_BASE                        DSI_DPHYTX_BASE_ADDR
#define REG_PIN_MUX_BASE                         0x10200000

#ifndef ZERO_FILL_POINTER
#define ZERO_FILL_POINTER(v)                                    \
    do {                                                        \
        (void)memset(v, 0, sizeof(*v));                         \
    } while (0)
#endif /* ZERO_FILL_POINTER */

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

lcm_handle_t *gHandle[2] = {NULL, NULL};
/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
static int lcm0_id_cfg = 1;
static int lcm1_id_cfg = 1;
static int lcm0_id = 0;
static int lcm1_id = 0;

lcm_handle_t *get_lcm_handle(int lcm_id)
{
    lcm_handle_t * lcm_handle = NULL;

    if (1 < lcm_id) {
        printf("%s:%d, lcm_id:%d\n", __FUNCTION__, __LINE__, lcm_id);
    } else {
        lcm_handle = gHandle[lcm_id];
        #if MC_LCM_DEBUG
        printf("get gHandle[%d]:%px, handle:%px\n", lcm_id, gHandle[lcm_id], lcm_handle);
        #endif
    }

    return lcm_handle;
}

static int _set_lcm_handle(int lcm_id, lcm_handle_t *handle)
{
    gHandle[lcm_id] = handle;

    #if MC_LCM_DEBUG
    printf("set gHandle[%d]:%px, handle:%px\n", lcm_id, gHandle[lcm_id], handle);
    #endif

    return 0;
}

int mc_lcm_probe(void)
{
    int rtn = 0;
    int i = 0;
    lcm_handle_t *lcmHandle[2] = {NULL, NULL};

    #if MC_LCM_DEBUG
    printf("--mc_lcm_probe--\n");
    printf("|----------------------------------------------------|\n");
    printf("| lcm0_id_cfg=%d, lcm1_id_cfg=%d, lcm0_id=%d, lcm1_id=%d |\n", lcm0_id_cfg, lcm1_id_cfg, lcm0_id, lcm1_id);
    printf("| lcm0_id_cfg: 0-normal 1-assign 2-disable           |\n");
    printf("|----------------------------------------------------|\n\n");
    #endif

    if (1 == lcm0_id_cfg) { //for max loading test
        sfc_write(NULL, LCM_ID_MAIN, lcm0_id);
    }
    if (1 == lcm1_id_cfg) { //for max loading test
        sfc_write(NULL, LCM_ID_SUB,  lcm1_id);
    }

    for (i = 0; i < LCM_ID_MAX; i++) {
        lcmHandle[i] = os_k_malloc(sizeof(lcm_handle_t), 0);
        if (!lcmHandle[i]) {
            printf("lcm gHandle mem alloc failed\n");
            return -ENOMEM;
        }
        ZERO_FILL_POINTER(lcmHandle[i]);

        lcmHandle[i]->reg = os_k_malloc(sizeof(lcm_reg_base_t), 0);
        if (!lcmHandle[i]->reg) {
            printf("lcm gHandle regbase mem alloc failed\n");
            return -ENOMEM;
        }
        ZERO_FILL_POINTER(lcmHandle[i]->reg);
        lcmHandle[i]->lcm_id_cfg = (0 == i)? lcm0_id_cfg : lcm1_id_cfg;
        rtn = _set_lcm_handle(i, lcmHandle[i]);
        lcmHandle[i] = NULL;
    }

    lcmHandle[LCM_ID_MAIN] = get_lcm_handle(LCM_ID_MAIN);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPRST]         = (void *)REG_TOP_RST_REG_BASE;
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPCLK]         = (void *)REG_TOP_CLK_CFG_BASE;
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU_SEL]        = (void *)REG_TOP_VOU_SEL_BASE;

    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU]            = (void *)REG_VOU_BASE;
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_DPHY]       = (void *)REG_DSI_DPHY_BASE;
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_4LANE] = (void *)REG_DIS_HOST_4LANE_BASE;
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_2LANE] = (void *)REG_DIS_HOST_2LANE_BASE;
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_PIN_MUX]        = (void *)REG_PIN_MUX_BASE;

    #if MC_LCM_DEBUG
    printf("LCM...reg top rst    0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPRST], REG_TOP_RST_REG_BASE);
    printf("LCM...reg top clk    0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPCLK], REG_TOP_CLK_CFG_BASE);
    printf("LCM...reg vou sel    0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU_SEL], REG_TOP_VOU_SEL_BASE);
    printf("LCM...reg vou base   0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU], REG_VOU_BASE);
    printf("LCM...reg phy base   0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_DPHY], REG_DSI_DPHY_BASE);
    printf("LCM...reg host0 base 0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_4LANE], REG_DIS_HOST_4LANE_BASE);
    printf("LCM...reg host1 base 0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_2LANE], REG_DIS_HOST_2LANE_BASE);
    printf("LCM...reg pin        0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_PIN_MUX], REG_PIN_MUX_BASE);
    #endif

    for (i = (LCM_ID_MAIN + 1); i < LCM_ID_MAX; i++) {
        lcmHandle[i] = get_lcm_handle(i);
        if ((NULL != lcmHandle[i])
            && (NULL != lcmHandle[i]->reg)) {
            memcpy(lcmHandle[i]->reg, lcmHandle[LCM_ID_MAIN]->reg, sizeof(lcm_reg_base_t));
        }
    }

    if (rtn) {

    }
    return 0;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
