#include "reg.h"
#include "reg_pool.h"
#include "dsi_host_v01_reg_def.h"
#include "dsi_host_v01_reg_pool.h"
#include "dsi_host_v01_reg_struct.h"
#include "dsi_host_v01_drv_param.h"

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

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
/********************************************************************************
FUNCTION NAME
    dsi_host_get_version

DESCRIPTION
    get dsi host hardware version

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr


OUTPUT PARAMETER
    uint32_t *version: dsi host version

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_version(uint32_t base_addr, uint32_t *version)
{
    int32_t rtn = SUCCESS;
    union dsi_host_version_tag *reg = getRegValuePtr(DSI_HOST_VERSION, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VERSION);
    *version = reg->mBits.version;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_power_up

DESCRIPTION
    set dsi host power up

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t power_up: 0: reset 1: power up

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_power_up(uint32_t base_addr, uint32_t power_up)
{
    int32_t rtn = SUCCESS;
    union dsi_host_power_up_tag *reg = getRegValuePtr(DSI_HOST_PWR_UP, dsi_host_get_reg_default_tab());

    reg->mBits.shutdownz = power_up;
    regWrite32(base_addr + DSI_HOST_PWR_UP, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_power_up

DESCRIPTION
    his field indicates the division factor for the Time Out clock used as
    the timing unit in the configuration of HS to LP and LP to HS transition
    error

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t tx_esc_clk_division: the division factor

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_tx_esc_clk_division(uint32_t base_addr, uint32_t tx_esc_clk_division)
{
    int32_t rtn = SUCCESS;
    union dsi_host_clkmgr_cfg_tag *reg = getRegValuePtr(DSI_HOST_CLKMGR_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.tx_esc_clk_division = tx_esc_clk_division;
    regWrite32(base_addr + DSI_HOST_CLKMGR_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_to_clk_division

DESCRIPTION
    This field indicates the division factor for the TX Escape clock source
    (lanebyteclk). The values 0 and 1 stop the TX_ESC clock generation.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t to_clk_division: the division factor

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_to_clk_division(uint32_t base_addr, uint32_t to_clk_division)
{
    int32_t rtn = SUCCESS;
    union dsi_host_clkmgr_cfg_tag *reg = getRegValuePtr(DSI_HOST_CLKMGR_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.to_clk_division = to_clk_division;
    regWrite32(base_addr + DSI_HOST_CLKMGR_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_vcid

DESCRIPTION
    this field configures the DPI virtual channel id that is indexed to the
    Video mode packets.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dpi_vcid:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dpi_vcid(uint32_t base_addr, uint32_t dpi_vcid)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_vcid_tag *reg = getRegValuePtr(DSI_HOST_DPI_VCID, dsi_host_get_reg_default_tab());

    reg->mBits.dpi_vcid = dpi_vcid;
    regWrite32(base_addr + DSI_HOST_DPI_VCID, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_vcid

DESCRIPTION
    set host dpi color format.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_dpi_format_e dpi_color_fromat

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dpi_color_format(uint32_t base_addr, dsi_host_dpi_color_format_e dpi_color_fromat)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_color_coding_tag *reg = getRegValuePtr(DSI_HOST_DPI_COLOR_CODING, dsi_host_get_reg_default_tab());

    reg->mBits.dpi_color_coding = dpi_color_fromat;
    regWrite32(base_addr + DSI_HOST_DPI_COLOR_CODING, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_vcid

DESCRIPTION
    loosely packed variant to 18-bit configurations.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t loosely18_en: ebable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_loosely18(uint32_t base_addr, uint32_t loosely18_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_color_coding_tag *reg = getRegValuePtr(DSI_HOST_DPI_COLOR_CODING, dsi_host_get_reg_default_tab());

    reg->mBits.loosely18_en = loosely18_en;
    regWrite32(base_addr + DSI_HOST_DPI_COLOR_CODING, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_dataen_pol

DESCRIPTION
    When set to 1, this bit configures the data enable pin (dpidataen) as active low.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dataen_pol: 0: positive 1: negative

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dpi_dataen_pol(uint32_t base_addr, uint32_t dataen_pol)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_cfg_pol_tag *reg = getRegValuePtr(DSI_HOST_DPI_CFG_POL, dsi_host_get_reg_default_tab());

    reg->mBits.dataen_active_low = dataen_pol;
    regWrite32(base_addr + DSI_HOST_DPI_CFG_POL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_vsync_pol

DESCRIPTION
    When set to 1, this bit configures the vertical synchronism pin
    (dpivsync) as active low

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vsync_pol: 0: positive 1: negative

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dpi_vsync_pol(uint32_t base_addr, uint32_t vsync_pol)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_cfg_pol_tag *reg = getRegValuePtr(DSI_HOST_DPI_CFG_POL, dsi_host_get_reg_default_tab());

    reg->mBits.vsync_active_low = vsync_pol;
    regWrite32(base_addr + DSI_HOST_DPI_CFG_POL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_hsync_pol

DESCRIPTION
    When set to 1, this bit configures the horizontal synchronism pin
    (dpihsync) as active low.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t hsync_pol: 0: positive 1: negative

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dpi_hsync_pol(uint32_t base_addr, uint32_t hsync_pol)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_cfg_pol_tag *reg = getRegValuePtr(DSI_HOST_DPI_CFG_POL, dsi_host_get_reg_default_tab());

    reg->mBits.hsync_active_low = hsync_pol;
    regWrite32(base_addr + DSI_HOST_DPI_CFG_POL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_shutd_pol

DESCRIPTION
    When set to 1, this bit configures the shutdown pin (dpishutdn) as
    active low.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t shutd_pol: 0: positive 1: negative

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dpi_shutd_pol(uint32_t base_addr, uint32_t shutd_pol)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_cfg_pol_tag *reg = getRegValuePtr(DSI_HOST_DPI_CFG_POL, dsi_host_get_reg_default_tab());

    reg->mBits.shutd_active_low = shutd_pol;
    regWrite32(base_addr + DSI_HOST_DPI_CFG_POL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_colorm_pol

DESCRIPTION
    When set to 1, this bit configures the color mode pin (dpicolorm) as
    active low.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t shutd_pol: 0: positive 1: negative

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dpi_colorm_pol(uint32_t base_addr, uint32_t colorm_pol)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_cfg_pol_tag *reg = getRegValuePtr(DSI_HOST_DPI_CFG_POL, dsi_host_get_reg_default_tab());

    reg->mBits.colorm_active_low = colorm_pol;
    regWrite32(base_addr + DSI_HOST_DPI_CFG_POL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_invact_lpcmd_time

DESCRIPTION
    This field is used for the transmission of commands in low-power
    mode. It defines the size, in bytes, of the largest packet that can fit in a
    line during the VACT region

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t invact_lpcmd_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_invact_lpcmd_time(uint32_t base_addr, uint32_t invact_lpcmd_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_lp_cmd_tim_tag *reg = getRegValuePtr(DSI_HOST_DPI_LP_CMD_TIM, dsi_host_get_reg_default_tab());

    reg->mBits.invact_lpcmd_time = invact_lpcmd_time;
    regWrite32(base_addr + DSI_HOST_DPI_LP_CMD_TIM, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_outvact_lpcmd_time

DESCRIPTION
    This field is used for the transmission of commands in low-power
    mode. It defines the size, in bytes, of the largest packet that can fit in a
    line during the VSA, VBP, and VFP regions.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t outvact_lpcmd_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_outvact_lpcmd_time(uint32_t base_addr, uint32_t outvact_lpcmd_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_lp_cmd_tim_tag *reg = getRegValuePtr(DSI_HOST_DPI_LP_CMD_TIM, dsi_host_get_reg_default_tab());

    reg->mBits.outvact_lpcmd_time = outvact_lpcmd_time;
    regWrite32(base_addr + DSI_HOST_DPI_LP_CMD_TIM, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_vcid

DESCRIPTION
    This field configures the virtual channel id that is indexed to the DCS
    packets from DBI.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t outvact_lpcmd_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dbi_vcid(uint32_t base_addr, uint32_t dbi_vcid)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dbi_vcid_tag *reg = getRegValuePtr(DSI_HOST_DBI_VCID, dsi_host_get_reg_default_tab());

    reg->mBits.dbi_vcid = dbi_vcid;
    regWrite32(base_addr + DSI_HOST_DBI_VCID, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_in_dbi_conf

DESCRIPTION
    This field configures the DBI input pixel data.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_dbi_data_e in_dbi_conf:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_in_dbi_conf(uint32_t base_addr, dsi_host_dbi_data_e in_dbi_conf)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dbi_cfg_tag *reg = getRegValuePtr(DSI_HOST_DBI_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.in_dbi_conf = in_dbi_conf;
    regWrite32(base_addr + DSI_HOST_DBI_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_out_dbi_conf

DESCRIPTION
    This field configures the DBI input pixel data.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_dbi_data_e out_dbi_conf:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_out_dbi_conf(uint32_t base_addr, dsi_host_dbi_data_e out_dbi_conf)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dbi_cfg_tag *reg = getRegValuePtr(DSI_HOST_DBI_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.out_dbi_conf = out_dbi_conf;
    regWrite32(base_addr + DSI_HOST_DBI_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_lut_size_conf

DESCRIPTION
    This field configures the size used to transport the write Lut
    commands as follows.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_write_lut_size_e lut_size_conf:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_lut_size_conf(uint32_t base_addr, dsi_host_write_lut_size_e lut_size_conf)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dbi_cfg_tag *reg = getRegValuePtr(DSI_HOST_DBI_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lut_size_conf = lut_size_conf;
    regWrite32(base_addr + DSI_HOST_DBI_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_partitioning

DESCRIPTION
    the DWC_mipi_dsi_host is to partition the DBI traffic
    automatically.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t partitioning_en: When set to 1, this bit enables the use of write_memory_continue
                              input commands (system needs to ensure correct partitioning of Long
                              Write commands). When not set, partitioning is automatically
                              performed in the DWC_mipi_dsi_host.

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_partitioning(uint32_t base_addr, uint32_t partitioning_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dbi_partitioning_en_tag *reg = getRegValuePtr(DSI_HOST_DBI_PARTITIONING_EN, dsi_host_get_reg_default_tab());

    reg->mBits.partitioning_en = partitioning_en;
    regWrite32(base_addr + DSI_HOST_DBI_PARTITIONING_EN, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_wr_cmd_size

DESCRIPTION
    this field configures the size of the DCS write memory commands.
    The size of DSI packet payload is the actual payload size minus 1,
    because the DCS command is in the DSI packet payload.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t wr_cmd_size:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dbi_wr_cmd_size(uint32_t base_addr, uint32_t wr_cmd_size)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dbi_cmdsize_tag *reg = getRegValuePtr(DSI_HOST_DBI_CMDSIZE, dsi_host_get_reg_default_tab());

    reg->mBits.wr_cmd_size = wr_cmd_size;
    regWrite32(base_addr + DSI_HOST_DBI_CMDSIZE, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_allowed_cmd_size

DESCRIPTION
    This field configures the maximum allowed size for a DCS write
    memory command. This field is used to partition a write memory
    command into one write_memory_start and a variable number of
    write_memory_continue commands. It is only used if the
    partitioning_en bit of the DBI_CFG register is disabled.
    The size of the DSI packet payload is the actual payload size minus 1,
    because the DCS command is in the DSI packet payload.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t allowed_cmd_size:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dbi_allowed_cmd_size(uint32_t base_addr, uint32_t allowed_cmd_size)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dbi_cmdsize_tag *reg = getRegValuePtr(DSI_HOST_DBI_CMDSIZE, dsi_host_get_reg_default_tab());

    reg->mBits.allowed_cmd_size = allowed_cmd_size;
    regWrite32(base_addr + DSI_HOST_DBI_CMDSIZE, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_pckhdl_eotp_tx_en

DESCRIPTION
   enables the EoTp transmission.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t eotp_tx_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_pckhdl_eotp_tx_en(uint32_t base_addr, uint32_t eotp_tx_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_pckhdl_cfg_tag *reg = getRegValuePtr(DSI_HOST_PCKHDL_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.eotp_tx_en = eotp_tx_en;
    regWrite32(base_addr + DSI_HOST_PCKHDL_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_pckhdl_eotp_rx_en

DESCRIPTION
   enables the EoTp reception.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t eotp_tx_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_pckhdl_eotp_rx_en(uint32_t base_addr, uint32_t eotp_rx_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_pckhdl_cfg_tag *reg = getRegValuePtr(DSI_HOST_PCKHDL_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.eotp_rx_en = eotp_rx_en;
    regWrite32(base_addr + DSI_HOST_PCKHDL_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_pckhdl_bta_en

DESCRIPTION
   enables the Bus Turn-Around (BTA) request.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t bta_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_pckhdl_bta_en(uint32_t base_addr, uint32_t bta_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_pckhdl_cfg_tag *reg = getRegValuePtr(DSI_HOST_PCKHDL_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.bta_en = bta_en;
    regWrite32(base_addr + DSI_HOST_PCKHDL_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_pckhdl_ecc_rx_en

DESCRIPTION
    When set to 1, this bit enables the CRC reception and error reporting.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3 or
    DSI_GENERIC = 1. Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ecc_rx_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_pckhdl_ecc_rx_en(uint32_t base_addr, uint32_t ecc_rx_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_pckhdl_cfg_tag *reg = getRegValuePtr(DSI_HOST_PCKHDL_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.ecc_rx_en = ecc_rx_en;
    regWrite32(base_addr + DSI_HOST_PCKHDL_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_pckhdl_crc_rx_en

DESCRIPTION
   enables the ECC reception, error correction, and reporting.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t crc_rx_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_pckhdl_crc_rx_en(uint32_t base_addr, uint32_t crc_rx_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_pckhdl_cfg_tag *reg = getRegValuePtr(DSI_HOST_PCKHDL_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.crc_rx_en = crc_rx_en;
    regWrite32(base_addr + DSI_HOST_PCKHDL_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_vcid_rx

DESCRIPTION
    indicates the Generic interface read-back virtual channel identification.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_vcid_rx: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_vcid_rx(uint32_t base_addr, uint32_t gen_vcid_rx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_vcid_tag *reg = getRegValuePtr(DSI_HOST_GEN_VCID, dsi_host_get_reg_default_tab());

    reg->mBits.gen_vcid_rx = gen_vcid_rx;
    regWrite32(base_addr + DSI_HOST_GEN_VCID, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_cmd_video_mode

DESCRIPTION
    configures the operation mode

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t cmd_video_mode: 0:Video mode 1:Command mode

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_cmd_video_mode(uint32_t base_addr, uint32_t cmd_video_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_host_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.cmd_video_mode = cmd_video_mode;
    regWrite32(base_addr + DSI_HOST_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_mode_type

DESCRIPTION
    indicates the video mode transmission type as follows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_vid_mode_type_e vid_mode_type:
        00: Non-burst with sync pulses
        01: Non-burst with sync events
        10 and 11: Burst mode

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_mode_type(uint32_t base_addr, dsi_host_vid_mode_type_e vid_mode_type)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.vid_mode_type = vid_mode_type;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_lp_vsa_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the Vertical Sync Time (VSA) period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vsa_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_lp_vsa_en(uint32_t base_addr, uint32_t lp_vsa_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lp_vsa_en = lp_vsa_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_lp_vbp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the Vertical Back Porch (VBP) period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vbp_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_lp_vbp_en(uint32_t base_addr, uint32_t lp_vbp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lp_vbp_en = lp_vbp_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_lp_vfp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the Vertical Front Porch (VFP) period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vfp_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_lp_vfp_en(uint32_t base_addr, uint32_t lp_vfp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lp_vfp_en = lp_vfp_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_lp_vact_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the Vertical Active (VACT) period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vact_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_lp_vact_en(uint32_t base_addr, uint32_t lp_vact_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lp_vact_en = lp_vact_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_lp_hbp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the
    Horizontal Back Porch (HBP) period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_hbp_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_lp_hbp_en(uint32_t base_addr, uint32_t lp_hbp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lp_hbp_en = lp_hbp_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_lp_hfp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the
    Horizontal Front Porch (HFP) period when timing allows.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_hfp_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_lp_hfp_en(uint32_t base_addr, uint32_t lp_hfp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lp_hfp_en = lp_hfp_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_frame_bta_ack_en

DESCRIPTION
    When set to 1, this bit enables the request for an acknowledge
    response at the end of a frame.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t frame_bta_ack_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_frame_bta_ack_en(uint32_t base_addr, uint32_t frame_bta_ack_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.frame_bta_ack_en = frame_bta_ack_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_lp_cmd_en

DESCRIPTION
    When set to 1, this bit enables the command transmission only in lowpower mode.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_cmd_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_lp_cmd_en(uint32_t base_addr, uint32_t lp_cmd_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lp_cmd_en = lp_cmd_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_VID_vpg_en

DESCRIPTION
    When set to 1, this bit enables the video mode pattern generator.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vpg_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_vpg_en(uint32_t base_addr, uint32_t vpg_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.vpg_en = vpg_en;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_vpg_mode

DESCRIPTION
    When set to 1, this bit enables the video mode pattern generator.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vpg_en: 0:disable 1:enable

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_vpg_mode(uint32_t base_addr, uint32_t vpg_mod)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.vpg_mode = vpg_mod;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_vpg_mode

DESCRIPTION
    This field indicates the color bar orientation as follows:


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vpg_orientation:
        0: Vertical mode
        1: Horizontal mode


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_vpg_orientation(uint32_t base_addr, uint32_t vpg_orientation)
{
    int32_t rtn = SUCCESS;
    union dsi_host_VID_MODE_CFG_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.vpg_orientation = vpg_orientation;
    regWrite32(base_addr + DSI_HOST_VID_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_pkt_size

DESCRIPTION
    This field configures the number of pixels in a single video packet. For
    18-bit not loosely packed data types, this number must be a multiple of
    4. For YCbCr data types, it must be a multiple of 2, as described in the
    DSI specification.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_pkt_size:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_pkt_size(uint32_t base_addr, uint32_t vid_pkt_size)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_pkt_size_tag *reg = getRegValuePtr(DSI_HOST_VID_PKT_SIZE, dsi_host_get_reg_default_tab());

    reg->mBits.vid_pkt_size = vid_pkt_size;
    regWrite32(base_addr + DSI_HOST_VID_PKT_SIZE, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_num_chunks

DESCRIPTION
    This register configures the number of chunks to be transmitted during
    a Line period (a chunk consists of a video packet and a null packet).
    If set to 0 or 1, the video line is transmitted in a single packet.
    If set to 1, the packet is part of a chunk, so a null packet follows it if
    vid_null_size > 0. Otherwise, multiple chunks are used to transmit
    each video line.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_num_chunks:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_num_chunks(uint32_t base_addr, uint32_t vid_num_chunks)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_num_chunks_tag *reg = getRegValuePtr(DSI_HOST_VID_NUM_CHUNKS, dsi_host_get_reg_default_tab());

    reg->mBits.vid_num_chunks = vid_num_chunks;
    regWrite32(base_addr + DSI_HOST_VID_NUM_CHUNKS, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_null_size

DESCRIPTION
    This register configures the number of bytes inside a null packet.
    Setting it to 0 disables the null packets.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_null_size:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_null_size(uint32_t base_addr, uint32_t vid_null_size)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_null_size_tag *reg = getRegValuePtr(DSI_HOST_VID_NULL_SIZE, dsi_host_get_reg_default_tab());

    reg->mBits.vid_null_size = vid_null_size;
    regWrite32(base_addr + DSI_HOST_VID_NULL_SIZE, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_hsa_time

DESCRIPTION
    This field configures the Horizontal Synchronism Active period in lane
    byte clock cycles.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_hsa_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_hsa_time(uint32_t base_addr, uint32_t vid_hsa_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_hsa_time_tag *reg = getRegValuePtr(DSI_HOST_VID_HSA_TIME, dsi_host_get_reg_default_tab());

    reg->mBits.vid_hsa_time = vid_hsa_time;
    regWrite32(base_addr + DSI_HOST_VID_HSA_TIME, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_vid_hbp_time_tag

DESCRIPTION
    This field configures the Horizontal Back Porch period in lane byte
    clock cycles.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_hbp_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_hbp_time(uint32_t base_addr, uint32_t vid_hbp_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_hbp_time_tag *reg = getRegValuePtr(DSI_HOST_VID_HBP_TIME, dsi_host_get_reg_default_tab());

    reg->mBits.vid_hbp_time = vid_hbp_time;
    regWrite32(base_addr + DSI_HOST_VID_HBP_TIME, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_hline_time

DESCRIPTION
    This field configures the size of the total line time
    (HSA+HBP+HACT+HFP) counted in lane byte clock cycles.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_hline_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_hline_time(uint32_t base_addr, uint32_t vid_hline_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_hline_time_tag *reg = getRegValuePtr(DSI_HOST_VID_HLINE_TIME, dsi_host_get_reg_default_tab());

    reg->mBits.vid_hline_time = vid_hline_time;
    regWrite32(base_addr + DSI_HOST_VID_HLINE_TIME, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_vsa_lines

DESCRIPTION
    This field configures the Vertical Synchronism Active period measured
    in number of horizontal lines.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vsa_lines:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_vsa_lines(uint32_t base_addr, uint32_t vsa_lines)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vsa_line_tag *reg = getRegValuePtr(DSI_HOST_VID_VSA_LINES, dsi_host_get_reg_default_tab());

    reg->mBits.vsa_lines = vsa_lines;
    regWrite32(base_addr + DSI_HOST_VID_VSA_LINES, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_vbp_lines

DESCRIPTION
    This field configures the Vertical Back Porch period measured in
    number of horizontal lines.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vbp_lines:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_vbp_lines(uint32_t base_addr, uint32_t vbp_lines)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vbp_line_tag *reg = getRegValuePtr(DSI_HOST_VID_VBP_LINES, dsi_host_get_reg_default_tab());

    reg->mBits.vbp_lines = vbp_lines;
    regWrite32(base_addr + DSI_HOST_VID_VBP_LINES, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_vfp_lines

DESCRIPTION
    This field configures the Vertical Front Porch period measured in
    number of horizontal lines.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vfp_lines:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_vfp_lines(uint32_t base_addr, uint32_t vfp_lines)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vfp_line_tag *reg = getRegValuePtr(DSI_HOST_VID_VFP_LINES, dsi_host_get_reg_default_tab());

    reg->mBits.vfp_lines = vfp_lines;
    regWrite32(base_addr + DSI_HOST_VID_VFP_LINES, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_vactive_line

DESCRIPTION
    This field configures the Vertical Active period measured in number of
    horizontal lines.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t v_active_lines:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_vactive_line(uint32_t base_addr, uint32_t v_active_lines)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vactive_line_tag *reg = getRegValuePtr(DSI_HOST_VID_VACTIVE_LINES, dsi_host_get_reg_default_tab());

    reg->mBits.v_active_lines = v_active_lines;
    regWrite32(base_addr + DSI_HOST_VID_VACTIVE_LINES, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_edpi_allowed_cmd_size

DESCRIPTION
    This field configures the maximum allowed size for an eDPI write
    memory command, measured in pixels. Automatic partitioning of
    data obtained from eDPI is permanently enabled.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t edpi_allowed_cmd_size:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_edpi_allowed_cmd_size(uint32_t base_addr, uint32_t edpi_allowed_cmd_size)
{
    int32_t rtn = SUCCESS;
    union dsi_host_edpi_cmd_size_tag *reg = getRegValuePtr(DSI_HOST_EDPI_CMD_SIZE, dsi_host_get_reg_default_tab());

    reg->mBits.edpi_allowed_cmd_size = edpi_allowed_cmd_size;
    regWrite32(base_addr + DSI_HOST_EDPI_CMD_SIZE, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_tear_fx_en

DESCRIPTION
    When set to 1, this bit enables the tearing effect acknowledge request.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t tear_fx_en: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_tear_fx_en(uint32_t base_addr, uint32_t tear_fx_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.tear_fx_en = tear_fx_en;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_rqst_en

DESCRIPTION
    When set to 1, this bit enables the acknowledge request after each
    packet transmission.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_rqst_en: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_ack_rqst_en(uint32_t base_addr, uint32_t ack_rqst_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.ack_rqst_en = ack_rqst_en;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_sw_0p_tx

DESCRIPTION
    This bit configures the Generic short write packet with zero parameter
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_sw_0p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_sw_0p_tx(uint32_t base_addr, uint32_t gen_sw_0p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.gen_sw_0p_tx = gen_sw_0p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_sw_1p_tx

DESCRIPTION
    This bit configures the Generic short write packet with one parameter
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_sw_1p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_sw_1p_tx(uint32_t base_addr, uint32_t gen_sw_1p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.gen_sw_1p_tx = gen_sw_1p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_sw_2p_tx

DESCRIPTION
    This bit configures the Generic short write packet with two parameters
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_sw_2p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_sw_2p_tx(uint32_t base_addr, uint32_t gen_sw_2p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.gen_sw_2p_tx = gen_sw_2p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_sr_0p_tx

DESCRIPTION
    This bit configures the Generic short read packet with zero parameter
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_sr_0p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_sr_0p_tx(uint32_t base_addr, uint32_t gen_sr_0p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.gen_sr_0p_tx = gen_sr_0p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_sr_1p_tx

DESCRIPTION
    This bit configures the Generic short read packet with one parameter
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_sr_1p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_sr_1p_tx(uint32_t base_addr, uint32_t gen_sr_1p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.gen_sr_1p_tx = gen_sr_1p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_sr_2p_tx

DESCRIPTION
    This bit configures the Generic short read packet with two parameters
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_sr_2p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_sr_2p_tx(uint32_t base_addr, uint32_t gen_sr_2p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.gen_sr_2p_tx = gen_sr_2p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_lw_tx

DESCRIPTION
    This bit configures the Generic long write packet command
    transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_lw_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_lw_tx(uint32_t base_addr, uint32_t gen_lw_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.gen_lw_tx = gen_lw_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dcs_sw_0p_tx

DESCRIPTION
    This bit configures the DCS short write packet with zero parameter
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dcs_sw_0p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dcs_sw_0p_tx(uint32_t base_addr, uint32_t dcs_sw_0p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.dcs_sw_0p_tx = dcs_sw_0p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dcs_sw_1p_tx

DESCRIPTION
    This bit configures the DCS short write packet with one parameter
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dcs_sw_1p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dcs_sw_1p_tx(uint32_t base_addr, uint32_t dcs_sw_1p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.dcs_sw_1p_tx = dcs_sw_1p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dcs_sr_0p_tx

DESCRIPTION
    This bit configures the DCS short read packet with zero parameter
    command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dcs_sr_0p_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dcs_sr_0p_tx(uint32_t base_addr, uint32_t dcs_sr_0p_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.dcs_sr_0p_tx = dcs_sr_0p_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dcs_lw_tx

DESCRIPTION
    This bit configures the DCS long write packet command transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dcs_lw_tx:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_dcs_lw_tx(uint32_t base_addr, uint32_t dcs_lw_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.dcs_lw_tx = dcs_lw_tx;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_max_rd_pkt_size

DESCRIPTION
    This bit configures the maximum read packet size command
    transmission type


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t max_rd_pkt_size:
        0: High-speed
        1: Low-power


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_max_rd_pkt_size(uint32_t base_addr, uint32_t max_rd_pkt_size)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_mode_cfg_tag *reg = getRegValuePtr(DSI_HOST_CMD_MODE_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.max_rd_pkt_size = max_rd_pkt_size;
    regWrite32(base_addr + DSI_HOST_CMD_MODE_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_dt

DESCRIPTION
    This field configures the packet data type of the header packet


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_dt: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_dt(uint32_t base_addr, uint32_t gen_dt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_hdr_tag *reg = getRegValuePtr(DSI_HOST_GEN_HDR, dsi_host_get_reg_default_tab());

    reg->mBits.gen_dt = gen_dt;
    //regWrite32(base_addr + DSI_HOST_GEN_HDR, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_vc

DESCRIPTION
    This field configures the virtual channel id of the header packet


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_dt: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_vc(uint32_t base_addr, uint32_t gen_vc)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_hdr_tag *reg = getRegValuePtr(DSI_HOST_GEN_HDR, dsi_host_get_reg_default_tab());

    reg->mBits.gen_vc = gen_vc;
    //regWrite32(base_addr + DSI_HOST_GEN_HDR, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_wc_lsbyte

DESCRIPTION
    This field configures the least significant byte of the header packet's
    Word count for long packets or data 0 for short packets


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_wc_lsbyte: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_wc_lsbyte(uint32_t base_addr, uint32_t gen_wc_lsbyte)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_hdr_tag *reg = getRegValuePtr(DSI_HOST_GEN_HDR, dsi_host_get_reg_default_tab());

    reg->mBits.gen_wc_lsbyte = gen_wc_lsbyte;
    //regWrite32(base_addr + DSI_HOST_GEN_HDR, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_wc_msbyte

DESCRIPTION
    This field configures the most significant byte of the header packet's
    word count for long packets or data 1 for short packets


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_wc_msbyte: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_wc_msbyte(uint32_t base_addr, uint32_t gen_wc_msbyte)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_hdr_tag *reg = getRegValuePtr(DSI_HOST_GEN_HDR, dsi_host_get_reg_default_tab());

    reg->mBits.gen_wc_msbyte = gen_wc_msbyte;
    //regWrite32(base_addr + DSI_HOST_GEN_HDR, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_packet_header_cfg

DESCRIPTION
    This field configures  the header packet

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_packet_header: generic packet header


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_packet_header_cfg(uint32_t base_addr, uint32_t gen_packet_header)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_hdr_tag *reg = getRegValuePtr(DSI_HOST_GEN_HDR, dsi_host_get_reg_default_tab());

    regWrite32(base_addr + DSI_HOST_GEN_HDR, reg->dwValue);

    return rtn;
}


/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_b1

DESCRIPTION
    This field indicates byte 1 of the packet payload


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_b1: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_pld_b1(uint32_t base_addr, uint32_t gen_pld_b1)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_pld_data_tag *reg = getRegValuePtr(DSI_HOST_GEN_PLD_DATA, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_b1 = gen_pld_b1;
    regWrite32(base_addr + DSI_HOST_GEN_PLD_DATA, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_b2

DESCRIPTION
    This field indicates byte 2 of the packet payload


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_b2: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_pld_b2(uint32_t base_addr, uint32_t gen_pld_b2)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_pld_data_tag *reg = getRegValuePtr(DSI_HOST_GEN_PLD_DATA, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_b2 = gen_pld_b2;
    regWrite32(base_addr + DSI_HOST_GEN_PLD_DATA, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_b3

DESCRIPTION
    This field indicates byte 3 of the packet payload


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_b3: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_pld_b3(uint32_t base_addr, uint32_t gen_pld_b3)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_pld_data_tag *reg = getRegValuePtr(DSI_HOST_GEN_PLD_DATA, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_b3 = gen_pld_b3;
    regWrite32(base_addr + DSI_HOST_GEN_PLD_DATA, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_b4

DESCRIPTION
    This field indicates byte 4 of the packet payload


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_b4: 0:disable 1:enable


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_pld_b4(uint32_t base_addr, uint32_t gen_pld_b4)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_pld_data_tag *reg = getRegValuePtr(DSI_HOST_GEN_PLD_DATA, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_b4 = gen_pld_b4;
    regWrite32(base_addr + DSI_HOST_GEN_PLD_DATA, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_data

DESCRIPTION
    This field indicates word of the packet payload


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_data: data


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_gen_pld_data(uint32_t base_addr, uint32_t gen_pld_data)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_pld_data_tag *reg = getRegValuePtr(DSI_HOST_GEN_PLD_DATA, dsi_host_get_reg_default_tab());

    reg->dwValue = gen_pld_data;
    regWrite32(base_addr + DSI_HOST_GEN_PLD_DATA, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_data

DESCRIPTION
    This field indicates word of the packet payload


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_data: data


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_pld_data(uint32_t base_addr, uint32_t *gen_pld_data)
{
    int32_t rtn = SUCCESS;
    union dsi_host_gen_pld_data_tag *reg = getRegValuePtr(DSI_HOST_GEN_PLD_DATA, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_GEN_PLD_DATA);
    *gen_pld_data = reg->dwValue;

    return rtn;
}


/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_cmd_empty

DESCRIPTION
    This bit indicates the empty status of the generic command FIFO.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved.
    Value after reset: 0x1


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_cmd_empty:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_cmd_empty(uint32_t base_addr, uint32_t *gen_cmd_empty)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *gen_cmd_empty = reg->mBits.gen_cmd_empty;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_cmd_full

DESCRIPTION
    This bit indicates the full status of the generic command FIFO.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_cmd_full:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_cmd_full(uint32_t base_addr, uint32_t *gen_cmd_full)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *gen_cmd_full = reg->mBits.gen_cmd_full;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_w_empty

DESCRIPTION
    This bit indicates the empty status of the generic write payload FIFO.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved.
    Value after reset: 0x1


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_w_empty:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_pld_w_empty(uint32_t base_addr, uint32_t *gen_pld_w_empty)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *gen_pld_w_empty = reg->mBits.gen_pld_w_empty;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_w_full

DESCRIPTION
    This bit indicates the full status of the generic write payload FIFO.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_w_full:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_pld_w_full(uint32_t base_addr, uint32_t *gen_pld_w_full)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *gen_pld_w_full = reg->mBits.gen_pld_w_full;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_r_empty

DESCRIPTION
    This bit indicates the empty status of the generic read payload FIFO.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved.
    Value after reset: 0x1


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_r_empty:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_pld_r_empty(uint32_t base_addr, uint32_t *gen_pld_r_empty)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *gen_pld_r_empty = reg->mBits.gen_pld_r_empty;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_r_full

DESCRIPTION
    This bit indicates the full status of the generic read payload FIFO.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_r_full:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_pld_r_full(uint32_t base_addr, uint32_t *gen_pld_r_full)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *gen_pld_r_full = reg->mBits.gen_pld_r_full;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_rd_cmd_busy

DESCRIPTION
    This bit is set when a read command is issued and cleared when the
    entire response is stored in the FIFO.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_rd_cmd_busy:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_gen_rd_cmd_busy(uint32_t base_addr, uint32_t *gen_rd_cmd_busy)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *gen_rd_cmd_busy = reg->mBits.gen_rd_cmd_busy;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_cmd_empy

DESCRIPTION
    This bit indicates the empty status of the DBI command FIFO.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.
    Value after reset: 0x1


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_cmd_empy:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dbi_cmd_empy(uint32_t base_addr, uint32_t *dbi_cmd_empy)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *dbi_cmd_empy = reg->mBits.dbi_cmd_empy;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_cmd_full

DESCRIPTION
    This bit indicates the full status of the DBI command FIFO.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_cmd_full:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dbi_cmd_full(uint32_t base_addr, uint32_t *dbi_cmd_full)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *dbi_cmd_full = reg->mBits.dbi_cmd_full;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_pld_w_empty

DESCRIPTION
    This bit indicates the empty status of the DBI write payload FIFO.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.
    Value after reset: 0x1


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_pld_w_empty:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dbi_pld_w_empty(uint32_t base_addr, uint32_t *dbi_pld_w_empty)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *dbi_pld_w_empty = reg->mBits.dbi_pld_w_empty;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_pld_w_full

DESCRIPTION
    This bit indicates the full status of the DBI write payload FIFO.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_pld_w_full:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dbi_pld_w_full(uint32_t base_addr, uint32_t *dbi_pld_w_full)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *dbi_pld_w_full = reg->mBits.dbi_pld_w_full;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_pld_r_empty

DESCRIPTION
    This bit indicates the empty status of the DBI read payload FIFO.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.
    Value after reset: 0x1


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_pld_r_empty:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dbi_pld_r_empty(uint32_t base_addr, uint32_t *dbi_pld_r_empty)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *dbi_pld_r_empty = reg->mBits.dbi_pld_r_empty;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_pld_r_full

DESCRIPTION
    This bit indicates the full status of the DBI read payload FIFO.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_pld_r_full:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dbi_pld_r_full(uint32_t base_addr, uint32_t *dbi_pld_r_full)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *dbi_pld_r_full = reg->mBits.dbi_pld_r_full;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_rd_cmd_busy

DESCRIPTION
    This bit is set when a read command is issued and cleared when the
    entire response is stored in the FIFO.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.
    Value after reset: 0x0


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_rd_cmd_busy:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dbi_rd_cmd_busy(uint32_t base_addr, uint32_t *dbi_rd_cmd_busy)
{
    int32_t rtn = SUCCESS;
    union dsi_host_cmd_pkt_status_tag *reg = getRegValuePtr(DSI_HOST_CMD_PKT_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_CMD_PKT_STATUS);
    *dbi_rd_cmd_busy = reg->mBits.dbi_rd_cmd_busy;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_lprx_to_cnt

DESCRIPTION
    This field configures the timeout counter that triggers a low-power
    reception timeout contention detection (measured in
    TO_CLK_DIVISION cycles).


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lprx_to_cnt:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_lprx_to_cnt(uint32_t base_addr, uint32_t lprx_to_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_to_cnt_cfg_tag *reg = getRegValuePtr(DSI_HOST_TO_CNT_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.lprx_to_cnt = lprx_to_cnt;
    regWrite32(base_addr + DSI_HOST_TO_CNT_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_hstx_to_cnt

DESCRIPTION
    This field configures the timeout counter that triggers a high-speed
    transmission timeout contention detection (measured in
    TO_CLK_DIVISION cycles).
    If using the non-burst mode and there is no sufficient time to switch
    from HS to LP and back in the period which is from one line data
    finishing to the next line sync start, the DSI link returns the LP state
    once per frame, then you should configure the TO_CLK_DIVISION
    and hstx_to_cnt to be in accordance with:
    hstx_to_cnt * lanebyteclkperiod * TO_CLK_DIVISION >= the time of
    one FRAME data transmission * (1 + 10%)
    In burst mode, RGB pixel packets are time-compressed, leaving more
    time during a scan line. Therefore, if in burst mode and there is
    sufficient time to switch from HS to LP and back in the period of time
    from one line data finishing to the next line sync start, the DSI link can
    return LP mode and back in this time interval to save power. For this,
    configure the TO_CLK_DIVISION and hstx_to_cnt to be in accordance
    with:
    hstx_to_cnt * lanebyteclkperiod * TO_CLK_DIVISION >= the time of
    one LINE data transmission * (1 + 10%)


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t hstx_to_cnt:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_hstx_to_cnt(uint32_t base_addr, uint32_t hstx_to_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_to_cnt_cfg_tag *reg = getRegValuePtr(DSI_HOST_TO_CNT_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.hstx_to_cnt = hstx_to_cnt;
    regWrite32(base_addr + DSI_HOST_TO_CNT_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_hs_rd_to_cnt

DESCRIPTION
    This field sets a period for which the DWC_mipi_dsi_host keeps the
    link still, after sending a high-speed read operation. This period is
    measured in cycles of lanebyteclk. The counting starts when the
    D-PHY enters the Stop state and causes no interrupts.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t hs_rd_to_cnt:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_hs_rd_to_cnt(uint32_t base_addr, uint32_t hs_rd_to_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_hs_rd_to_cnt_tag *reg = getRegValuePtr(DSI_HOST_HS_RD_TO_CNT, dsi_host_get_reg_default_tab());

    reg->mBits.hs_rd_to_cnt = hs_rd_to_cnt;
    regWrite32(base_addr + DSI_HOST_HS_RD_TO_CNT, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_lp_rd_to_cnt

DESCRIPTION
    This field sets a period for which the DWC_mipi_dsi_host keeps the
    link still, after sending a low-power read operation. This period is
    measured in cycles of lanebyteclk. The counting starts when the
    D-PHY enters the Stop state and causes no interrupts.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_rd_to_cnt:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_lp_rd_to_cnt(uint32_t base_addr, uint32_t lp_rd_to_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_lp_rd_to_cnt_tag *reg = getRegValuePtr(DSI_HOST_LP_RD_TO_CNT, dsi_host_get_reg_default_tab());

    reg->mBits.lp_rd_to_cnt = lp_rd_to_cnt;
    regWrite32(base_addr + DSI_HOST_LP_RD_TO_CNT, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_hs_wr_to_cnt

DESCRIPTION
    This field sets a period for which the DWC_mipi_dsi_host keeps the
    link inactive after sending a high-speed write operation. This period is
    measured in cycles of lanebyteclk. The counting starts when the
    D-PHY enters the Stop state and causes no interrupts.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t hs_wr_to_cnt:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_hs_wr_to_cnt(uint32_t base_addr, uint32_t hs_wr_to_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_hs_wr_to_cnt_tag *reg = getRegValuePtr(DSI_HOST_HS_WR_TO_CNT, dsi_host_get_reg_default_tab());

    reg->mBits.hs_wr_to_cnt = hs_wr_to_cnt;
    regWrite32(base_addr + DSI_HOST_HS_WR_TO_CNT, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_presp_to_mode

DESCRIPTION
    When set to 1, this bit ensures that the peripheral response timeout
    caused by hs_wr_to_cnt is used only once per eDPI frame, when both
    the following conditions are met:
        dpivsync_edpiwms has risen and fallen.
        Packets originated from eDPI have been transmitted and its FIFO
        is empty again.
    In this scenario no non-eDPI requests are sent to the D-PHY, even if
    there is traffic from generic or DBI ready to be sent, making it return to
    stop state. When it does so, PRESP_TO counter is activated and only
    when it finishes does the controller send any other traffic that is ready.
    Dependency: DSI_DATAINTERFACE = 4. Otherwise, this bit is
    reserved.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t presp_to_mode:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_presp_to_mode(uint32_t base_addr, uint32_t presp_to_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_host_hs_wr_to_cnt_tag *reg = getRegValuePtr(DSI_HOST_HS_WR_TO_CNT, dsi_host_get_reg_default_tab());

    reg->mBits.presp_to_mode = presp_to_mode;
    regWrite32(base_addr + DSI_HOST_HS_WR_TO_CNT, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_lp_wr_to_cnt

DESCRIPTION
    This field sets a period for which the DWC_mipi_dsi_host keeps the
    link still, after sending a low-power write operation. This period is
    measured in cycles of lanebyteclk. The counting starts when the
    D-PHY enters the Stop state and causes no interrupts.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_wr_to_cnt:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_lp_wr_to_cnt(uint32_t base_addr, uint32_t lp_wr_to_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_lp_wr_to_cnt_tag *reg = getRegValuePtr(DSI_HOST_LP_WR_TO_CNT, dsi_host_get_reg_default_tab());

    reg->mBits.lp_wr_to_cnt = lp_wr_to_cnt;
    regWrite32(base_addr + DSI_HOST_LP_WR_TO_CNT, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_bta_to_cnt

DESCRIPTION
    This field sets a period for which the DWC_mipi_dsi_host keeps the
    link still, after completing a Bus Turn-Around. This period is measured
    in cycles of lanebyteclk. The counting starts when the D-PHY enters
    the Stop state and causes no interrupts.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t bta_to_cnt:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_bta_to_cnt(uint32_t base_addr, uint32_t bta_to_cnt)
{
    int32_t rtn = SUCCESS;
    union dsi_host_bta_to_cnt_tag *reg = getRegValuePtr(DSI_HOST_BTA_TO_CNT, dsi_host_get_reg_default_tab());

    reg->mBits.bta_to_cnt = bta_to_cnt;
    regWrite32(base_addr + DSI_HOST_BTA_TO_CNT, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_mode_3d

DESCRIPTION
    This field defines the 3D mode on/off and display orientation


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_mode_3d_e mode_3d:
        00: 3D mode off (2D mode on)
        01: 3D mode on, portrait orientation
        10: 3D mode on, landscape orientation
        11: Reserved


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_mode_3d(uint32_t base_addr, dsi_host_mode_3d_e mode_3d)
{
    int32_t rtn = SUCCESS;
    union dsi_host_SDF_3D_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D, dsi_host_get_reg_default_tab());

    reg->mBits.mode_3d = mode_3d;
    regWrite32(base_addr + DSI_HOST_SDF_3D, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_format_3d

DESCRIPTION
    This field defines the 3D image format


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_format_3d_e format_3d:
        00: Line (alternating lines of left and right data)
        01: Frame (alternating frames of left and right data)
        10: Pixel (alternating pixels of left and right data)
        11: Reserved


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_format_3d(uint32_t base_addr, dsi_host_format_3d_e format_3d)
{
    int32_t rtn = SUCCESS;
    union dsi_host_SDF_3D_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D, dsi_host_get_reg_default_tab());

    reg->mBits.format_3d = format_3d;
    regWrite32(base_addr + DSI_HOST_SDF_3D, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_second_vsync

DESCRIPTION
    This field defines whether there is a second VSYNC pulse between
    Left and Right Images, when 3D Image Format is Frame-based


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t second_vsync:
        0: No sync pulses between left and right data
        1: Sync pulse (HSYNC, VSYNC, blanking) between left and right data


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_second_vsync(uint32_t base_addr, uint32_t second_vsync)
{
    int32_t rtn = SUCCESS;
    union dsi_host_SDF_3D_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D, dsi_host_get_reg_default_tab());

    reg->mBits.second_vsync = second_vsync;
    regWrite32(base_addr + DSI_HOST_SDF_3D, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_right_first

DESCRIPTION
    This bit defines the left or right order


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t right_first:
        0: Left eye data is sent first, and then the right eye data is sent.
        1: Right eye data is sent first, and then the left eye data is sent.


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_right_first(uint32_t base_addr, uint32_t right_first)
{
    int32_t rtn = SUCCESS;
    union dsi_host_SDF_3D_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D, dsi_host_get_reg_default_tab());

    reg->mBits.right_first = right_first;
    regWrite32(base_addr + DSI_HOST_SDF_3D, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_send_3d_cfg

DESCRIPTION
    When set, causes the next VSS packet to include 3D control payload
    in every VSS packet.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t send_3d_cfg:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_send_3d_cfg(uint32_t base_addr, uint32_t send_3d_cfg)
{
    int32_t rtn = SUCCESS;
    union dsi_host_SDF_3D_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D, dsi_host_get_reg_default_tab());

    reg->mBits.send_3d_cfg = send_3d_cfg;
    regWrite32(base_addr + DSI_HOST_SDF_3D, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_txrequestclkhs

DESCRIPTION
    This bit controls the D-PHY PPI txrequestclkhs signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_txrequestclkhs:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_txrequestclkhs(uint32_t base_addr, uint32_t phy_txrequestclkhs)
{
    int32_t rtn = SUCCESS;
    union dsi_host_lpclk_ctrl_tag *reg = getRegValuePtr(DSI_HOST_LPCLK_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.phy_txrequestclkhs = phy_txrequestclkhs;
    regWrite32(base_addr + DSI_HOST_LPCLK_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_auto_clklane_ctrl

DESCRIPTION
    This bit enables the automatic mechanism to stop providing clock in
    the clock lane when time allows.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t auto_clklane_ctrl:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_auto_clklane_ctrl(uint32_t base_addr, uint32_t auto_clklane_ctrl)
{
    int32_t rtn = SUCCESS;
    union dsi_host_lpclk_ctrl_tag *reg = getRegValuePtr(DSI_HOST_LPCLK_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.auto_clklane_ctrl = auto_clklane_ctrl;
    regWrite32(base_addr + DSI_HOST_LPCLK_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_clklp2hs_time

DESCRIPTION
    This field configures the maximum time that the D-PHY clock lane
    takes to go from low-power to high-speed transmission measured in
    lane byte clock cycles.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_clklp2hs_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_clklp2hs_time(uint32_t base_addr, uint32_t phy_clklp2hs_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tmr_lpclk_cfg_tag *reg = getRegValuePtr(DSI_HOST_PHY_TMR_LPCLK_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.phy_clklp2hs_time = phy_clklp2hs_time;
    regWrite32(base_addr + DSI_HOST_PHY_TMR_LPCLK_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_clkhs2lp_time

DESCRIPTION
    This field configures the maximum time that the D-PHY clock lane
    takes to go from high-speed to low-power transmission measured in
    lane byte clock cycles.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_clkhs2lp_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_clkhs2lp_time(uint32_t base_addr, uint32_t phy_clkhs2lp_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tmr_lpclk_cfg_tag *reg = getRegValuePtr(DSI_HOST_PHY_TMR_LPCLK_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.phy_clkhs2lp_time = phy_clkhs2lp_time;
    regWrite32(base_addr + DSI_HOST_PHY_TMR_LPCLK_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_lp2hs_time

DESCRIPTION
    This field configures the maximum time that the D-PHY data lanes
    take to go from low-power to high-speed transmission measured in
    lane byte clock cycles.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_lp2hs_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_lp2hs_time(uint32_t base_addr, uint32_t phy_lp2hs_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tmr_cfg_tag *reg = getRegValuePtr(DSI_HOST_PHY_TMR_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.phy_lp2hs_time = phy_lp2hs_time;
    regWrite32(base_addr + DSI_HOST_PHY_TMR_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_hs2lp_time

DESCRIPTION
    This field configures the maximum time that the D-PHY data lanes
    take to go from high-speed to low-power transmission measured in
    lane byte clock cycles


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_hs2lp_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_hs2lp_time(uint32_t base_addr, uint32_t phy_hs2lp_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tmr_cfg_tag *reg = getRegValuePtr(DSI_HOST_PHY_TMR_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.phy_hs2lp_time = phy_hs2lp_time;
    regWrite32(base_addr + DSI_HOST_PHY_TMR_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_shutdownz

DESCRIPTION
    When set to 0, this bit places the D-PHY macro in power-down state


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_shutdownz:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_shutdownz(uint32_t base_addr, uint32_t phy_shutdownz)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_rstz_tag *reg = getRegValuePtr(DSI_HOST_PHY_RSTZ, dsi_host_get_reg_default_tab());

    reg->mBits.phy_shutdownz = phy_shutdownz;
    regWrite32(base_addr + DSI_HOST_PHY_RSTZ, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_rstz

DESCRIPTION
    When set to 0, this bit places the digital section of the D-PHY in the
    reset state.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_rstz:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_rstz(uint32_t base_addr, uint32_t phy_rstz)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_rstz_tag *reg = getRegValuePtr(DSI_HOST_PHY_RSTZ, dsi_host_get_reg_default_tab());

    reg->mBits.phy_rstz = phy_rstz;
    regWrite32(base_addr + DSI_HOST_PHY_RSTZ, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_enableclk

DESCRIPTION
    When set to1, this bit enables the D-PHY Clock Lane module


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_enableclk:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_enableclk(uint32_t base_addr, uint32_t phy_enableclk)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_rstz_tag *reg = getRegValuePtr(DSI_HOST_PHY_RSTZ, dsi_host_get_reg_default_tab());

    reg->mBits.phy_enableclk = phy_enableclk;
    regWrite32(base_addr + DSI_HOST_PHY_RSTZ, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_forcepll

DESCRIPTION
    When the D-PHY is in ULPS, this bit enables the D-PHY PLL.
    Dependency: DSI_HOST_FPGA = 0. Otherwise, this bit is reserved


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_forcepll:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_forcepll(uint32_t base_addr, uint32_t phy_forcepll)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_rstz_tag *reg = getRegValuePtr(DSI_HOST_PHY_RSTZ, dsi_host_get_reg_default_tab());

    reg->mBits.phy_forcepll = phy_forcepll;
    regWrite32(base_addr + DSI_HOST_PHY_RSTZ, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_n_lanes

DESCRIPTION
    This field configures the number of active data lanes


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_n_lanes_e n_lanes:
        00: One data lane (lane 0)
        01: Two data lanes (lanes 0 and 1)
        10: Three data lanes (lanes 0, 1, and 2)
        11: Four data lanes (lanes 0, 1, 2, and 3)


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_n_lanes(uint32_t base_addr, dsi_host_n_lanes_e n_lanes)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_if_cfg_tag *reg = getRegValuePtr(DSI_HOST_PHY_IF_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.n_lanes = n_lanes;
    regWrite32(base_addr + DSI_HOST_PHY_IF_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_stop_wait_time

DESCRIPTION
    This field configures the minimum wait period to request a high-speed
    transmission after the Stop state


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_stop_wait_time:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_stop_wait_time(uint32_t base_addr, uint32_t phy_stop_wait_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_if_cfg_tag *reg = getRegValuePtr(DSI_HOST_PHY_IF_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.phy_stop_wait_time = phy_stop_wait_time;
    regWrite32(base_addr + DSI_HOST_PHY_IF_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_txrequlpsclk

DESCRIPTION
    ULPS mode Request on clock lane.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_txrequlpsclk:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_txrequlpsclk(uint32_t base_addr, uint32_t phy_txrequlpsclk)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_ulps_ctrl_tag *reg = getRegValuePtr(DSI_HOST_PHY_ULPS_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.phy_txrequlpsclk = phy_txrequlpsclk;
    regWrite32(base_addr + DSI_HOST_PHY_ULPS_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_txexitulpsclk

DESCRIPTION
    ULPS mode Exit on clock lane.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_txexitulpsclk:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_txexitulpsclk(uint32_t base_addr, uint32_t phy_txexitulpsclk)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_ulps_ctrl_tag *reg = getRegValuePtr(DSI_HOST_PHY_ULPS_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.phy_txexitulpsclk = phy_txexitulpsclk;
    regWrite32(base_addr + DSI_HOST_PHY_ULPS_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_txrequlpslan

DESCRIPTION
    ULPS mode Request on all active data lanes.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_txrequlpslan:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_txrequlpslan(uint32_t base_addr, uint32_t phy_txrequlpslan)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_ulps_ctrl_tag *reg = getRegValuePtr(DSI_HOST_PHY_ULPS_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.phy_txrequlpslan = phy_txrequlpslan;
    regWrite32(base_addr + DSI_HOST_PHY_ULPS_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_txexitulpslan

DESCRIPTION
    ULPS mode Exit on all active data lanes.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_txexitulpslan:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_txexitulpslan(uint32_t base_addr, uint32_t phy_txexitulpslan)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_ulps_ctrl_tag *reg = getRegValuePtr(DSI_HOST_PHY_ULPS_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.phy_txexitulpslan = phy_txexitulpslan;
    regWrite32(base_addr + DSI_HOST_PHY_ULPS_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_tx_triggers

DESCRIPTION
    This field controls the trigger transmissions.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_tx_triggers:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_tx_triggers(uint32_t base_addr, uint32_t phy_tx_triggers)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tx_triggers_tag *reg = getRegValuePtr(DSI_HOST_PHY_TX_TRIGGERS, dsi_host_get_reg_default_tab());

    reg->mBits.phy_tx_triggers = phy_tx_triggers;
    regWrite32(base_addr + DSI_HOST_PHY_TX_TRIGGERS, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_lock

DESCRIPTION
    This bit indicates the status of phylock D-PHY signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_lock:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_lock(uint32_t base_addr, uint32_t *phy_lock)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_lock = reg->mBits.phy_lock;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_direction

DESCRIPTION
    This bit indicates the status of phydirection D-PHY signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_direction:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_direction(uint32_t base_addr, uint32_t *phy_direction)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_direction = reg->mBits.phy_direction;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_stopstateclklane

DESCRIPTION
    This bit indicates the status of phystopstateclklane D-PHY signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_stopstateclklane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_stopstateclklane(uint32_t base_addr, uint32_t *phy_stopstateclklane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_stopstateclklane = reg->mBits.phy_stopstateclklane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_ulpsactivenotclk

DESCRIPTION
    This bit indicates the status of phyulpsactivenotclk D-PHY signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_ulpsactivenotclk:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_ulpsactivenotclk(uint32_t base_addr, uint32_t *phy_ulpsactivenotclk)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_ulpsactivenotclk = reg->mBits.phy_ulpsactivenotclk;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_stopstate0lane

DESCRIPTION
    This bit indicates the status of phystopstate0lane D-PHY signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_stopstate0lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_stopstate0lane(uint32_t base_addr, uint32_t *phy_stopstate0lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_stopstate0lane = reg->mBits.phy_stopstate0lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_ulpsactivenot0lane

DESCRIPTION
    This bit indicates the status of ulpsactivenot0lane D-PHY signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_ulpsactivenot0lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_ulpsactivenot0lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot0lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_ulpsactivenot0lane = reg->mBits.phy_ulpsactivenot0lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_rxulpsesc0lane

DESCRIPTION
    This bit indicates the status of rxulpsesc0lane D-PHY signal.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_rxulpsesc0lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_rxulpsesc0lane(uint32_t base_addr, uint32_t *phy_rxulpsesc0lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_rxulpsesc0lane = reg->mBits.phy_rxulpsesc0lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_stopstate1lane

DESCRIPTION
    This bit indicates the status of phystopstate1lane D-PHY signal.
    Dependency: DSI_HOST_NUMBER_OF_LANES > 1
    If DSI_HOST_NUMBER_OF_LANES <= 1, this bit is reserved.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_stopstate1lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_stopstate1lane(uint32_t base_addr, uint32_t *phy_stopstate1lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_stopstate1lane = reg->mBits.phy_stopstate1lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_ulpsactivenot1lane

DESCRIPTION
    This bit indicates the status of ulpsactivenot1lane D-PHY signal.
    Dependency: DSI_HOST_NUMBER_OF_LANES > 1
    If DSI_HOST_NUMBER_OF_LANES <= 1, this bit is reserved.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_ulpsactivenot1lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_ulpsactivenot1lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot1lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_ulpsactivenot1lane = reg->mBits.phy_ulpsactivenot1lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_stopstate2lane

DESCRIPTION
    This bit indicates the status of phystopstate2lane D-PHY signal.
    Dependency: DSI_HOST_NUMBER_OF_LANES > 2
    If DSI_HOST_NUMBER_OF_LANES <= 2, this bit is reserved.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_stopstate2lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_stopstate2lane(uint32_t base_addr, uint32_t *phy_stopstate2lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_stopstate2lane = reg->mBits.phy_stopstate2lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_ulpsactivenot2lane

DESCRIPTION
    This bit indicates the status of ulpsactivenot2lane D-PHY signal.
    Dependency: DSI_HOST_NUMBER_OF_LANES > 2
    If DSI_HOST_NUMBER_OF_LANES <= 2, this bit is reserved.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_ulpsactivenot2lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_ulpsactivenot2lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot2lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_ulpsactivenot2lane = reg->mBits.phy_ulpsactivenot2lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_stopstate3lane

DESCRIPTION
    This bit indicates the status of phystopstate3lane D-PHY signal.
    Dependency: DSI_HOST_NUMBER_OF_LANES > 3
    If DSI_HOST_NUMBER_OF_LANES <= 3, this bit is reserved.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_stopstate3lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_stopstate3lane(uint32_t base_addr, uint32_t *phy_stopstate3lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_stopstate3lane = reg->mBits.phy_stopstate3lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_phy_ulpsactivenot3lane

DESCRIPTION
    This bit indicates the status of ulpsactivenot3lane D-PHY signal.
    Dependency: DSI_HOST_NUMBER_OF_LANES > 3
    If DSI_HOST_NUMBER_OF_LANES <= 3, this bit is reserved.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_ulpsactivenot3lane:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_phy_ulpsactivenot3lane(uint32_t base_addr, uint32_t *phy_ulpsactivenot3lane)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_status_tag *reg = getRegValuePtr(DSI_HOST_PHY_STATUS, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_PHY_STATUS);
    *phy_ulpsactivenot3lane = reg->mBits.phy_ulpsactivenot3lane;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_testclr

DESCRIPTION
    PHY test interface clear (active high).


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_testclr:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_testclr(uint32_t base_addr, uint32_t phy_testclr)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tst_ctrl0_tag *reg = getRegValuePtr(DSI_HOST_PHY_TST_CTRL0, dsi_host_get_reg_default_tab());

    reg->mBits.phy_testclr = phy_testclr;
    regWrite32(base_addr + DSI_HOST_PHY_TST_CTRL0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_testclk

DESCRIPTION
    This bit is used to clock the TESTDIN bus into the D-PHY.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_testclk:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_testclk(uint32_t base_addr, uint32_t phy_testclk)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tst_ctrl0_tag *reg = getRegValuePtr(DSI_HOST_PHY_TST_CTRL0, dsi_host_get_reg_default_tab());

    reg->mBits.phy_testclk = phy_testclk;
    regWrite32(base_addr + DSI_HOST_PHY_TST_CTRL0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_testdin

DESCRIPTION
    PHY test interface input 8-bit data bus for internal register
    programming and test functionalities access.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_testdin:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_testdin(uint32_t base_addr, uint32_t phy_testdin)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tst_ctrl1_tag *reg = getRegValuePtr(DSI_HOST_PHY_TST_CTRL1, dsi_host_get_reg_default_tab());

    reg->mBits.phy_testdin = phy_testdin;
    regWrite32(base_addr + DSI_HOST_PHY_TST_CTRL1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_testdout

DESCRIPTION
    PHY output 8-bit data bus for read-back and internal probing
    functionalities.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_testdout:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_testdout(uint32_t base_addr, uint32_t phy_testdout)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tst_ctrl1_tag *reg = getRegValuePtr(DSI_HOST_PHY_TST_CTRL1, dsi_host_get_reg_default_tab());

    reg->mBits.phy_testdout = phy_testdout;
    regWrite32(base_addr + DSI_HOST_PHY_TST_CTRL1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_phy_testen

DESCRIPTION
    PHY test interface operation selector


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t phy_testen:
        1: The address write operation is set on the falling edge of the testclk signal.
        0: The data write operation is set on the rising edge of the testclk signal.


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_phy_testen(uint32_t base_addr, uint32_t phy_testen)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tst_ctrl1_tag *reg = getRegValuePtr(DSI_HOST_PHY_TST_CTRL1, dsi_host_get_reg_default_tab());

    reg->mBits.phy_testen = phy_testen;
    regWrite32(base_addr + DSI_HOST_PHY_TST_CTRL1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_0

DESCRIPTION
    This bit retrieves the SoT error from the Acknowledge error report


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_0:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_0(uint32_t base_addr, uint32_t *ack_with_err_0)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_0 = reg->mBits.ack_with_err_0;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_1

DESCRIPTION
    This bit retrieves the SoT Sync error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_1:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_1(uint32_t base_addr, uint32_t *ack_with_err_1)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_1 = reg->mBits.ack_with_err_1;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_2

DESCRIPTION
    This bit retrieves the EoT Sync error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_2:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_2(uint32_t base_addr, uint32_t *ack_with_err_2)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_2 = reg->mBits.ack_with_err_2;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_3

DESCRIPTION
    This bit retrieves the Escape Mode Entry Command error from the
    Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_3:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_3(uint32_t base_addr, uint32_t *ack_with_err_3)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_3 = reg->mBits.ack_with_err_3;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_4

DESCRIPTION
    This bit retrieves the LP Transmit Sync error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_4:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_4(uint32_t base_addr, uint32_t *ack_with_err_4)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_4 = reg->mBits.ack_with_err_4;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_5

DESCRIPTION
    This bit retrieves the Peripheral Timeout error from the Acknowledge Error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_5:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_5(uint32_t base_addr, uint32_t *ack_with_err_5)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_5 = reg->mBits.ack_with_err_5;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_6

DESCRIPTION
    This bit retrieves the False Control error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_6:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_6(uint32_t base_addr, uint32_t *ack_with_err_6)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_6 = reg->mBits.ack_with_err_6;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_7

DESCRIPTION
    This bit retrieves the reserved (specific to device) from the
    Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_7:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_7(uint32_t base_addr, uint32_t *ack_with_err_7)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_7 = reg->mBits.ack_with_err_7;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_8

DESCRIPTION
    This bit retrieves the ECC error, single-bit (detected and corrected)
    from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_8:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_8(uint32_t base_addr, uint32_t *ack_with_err_8)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_8 = reg->mBits.ack_with_err_8;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_9

DESCRIPTION
    This bit retrieves the ECC error, multi-bit (detected, not corrected) from
    the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_9:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_9(uint32_t base_addr, uint32_t *ack_with_err_9)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_9 = reg->mBits.ack_with_err_9;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_10

DESCRIPTION
    This bit retrieves the checksum error (long packet only) from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_10:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_10(uint32_t base_addr, uint32_t *ack_with_err_10)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_10 = reg->mBits.ack_with_err_10;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_11

DESCRIPTION
    This bit retrieves the not recognized DSI data type from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_11:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_11(uint32_t base_addr, uint32_t *ack_with_err_11)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_11 = reg->mBits.ack_with_err_11;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_12

DESCRIPTION
    This bit retrieves the DSI VC ID Invalid from the Acknowledge error report.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_12:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_12(uint32_t base_addr, uint32_t *ack_with_err_12)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_12 = reg->mBits.ack_with_err_12;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_13

DESCRIPTION
    This bit retrieves the invalid transmission length from the Acknowledge error report.


INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_13:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_13(uint32_t base_addr, uint32_t *ack_with_err_13)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_13 = reg->mBits.ack_with_err_13;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_14

DESCRIPTION
    This bit retrieves the reserved (specific to device) from the Acknowledge error report.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_14:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_14(uint32_t base_addr, uint32_t *ack_with_err_14)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_14 = reg->mBits.ack_with_err_14;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ack_with_err_15

DESCRIPTION
    This bit retrieves the DSI protocol violation from the Acknowledge error report.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ack_with_err_15:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_ack_with_err_15(uint32_t base_addr, uint32_t *ack_with_err_15)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *ack_with_err_15 = reg->mBits.ack_with_err_15;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dphy_errors_0

DESCRIPTION
    This bit indicates ErrEsc escape entry error from Lane 0.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dphy_errors_0:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_dphy_errors_0(uint32_t base_addr, uint32_t *dphy_errors_0)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *dphy_errors_0 = reg->mBits.dphy_errors_0;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dphy_errors_1

DESCRIPTION
    This bit indicates ErrSyncEsc low-power data transmission
    synchronization error from Lane 0.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dphy_errors_1:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_dphy_errors_1(uint32_t base_addr, uint32_t *dphy_errors_1)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *dphy_errors_1 = reg->mBits.dphy_errors_1;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dphy_errors_2

DESCRIPTION
    This bit indicates the ErrControl error from Lane 0.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dphy_errors_2:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_dphy_errors_2(uint32_t base_addr, uint32_t *dphy_errors_2)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *dphy_errors_2 = reg->mBits.dphy_errors_2;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dphy_errors_3

DESCRIPTION
    This bit indicates the LP0 contention error ErrContentionLP0 from Lane 0.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dphy_errors_3:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_dphy_errors_3(uint32_t base_addr, uint32_t *dphy_errors_3)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *dphy_errors_3 = reg->mBits.dphy_errors_3;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dphy_errors_4

DESCRIPTION
    This bit indicates the LP1 contention error ErrContentionLP1 from Lane 0.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dphy_errors_4:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status_dphy_errors_4(uint32_t base_addr, uint32_t *dphy_errors_4)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    *dphy_errors_4 = reg->mBits.dphy_errors_4;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_int0_status

DESCRIPTION
    get dsi host int0 status (read clean).

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dphy_errors_4:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int0_status(uint32_t base_addr, uint32_t *int0_status)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST0, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_INT_ST0);
    *int0_status = reg->dwValue;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_to_hs_tx

DESCRIPTION
    This bit indicates that the high-speed transmission timeout counter
    reached the end and contention is detected

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *to_hs_tx:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_to_hs_tx(uint32_t base_addr, uint32_t *to_hs_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *to_hs_tx = reg->mBits.to_hs_tx;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_to_lp_rx

DESCRIPTION
    This bit indicates that the low-power reception timeout counter reached
    the end and contention is detected

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *to_lp_rx:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_to_lp_rx(uint32_t base_addr, uint32_t *to_lp_rx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *to_lp_rx = reg->mBits.to_lp_rx;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ecc_single_err

DESCRIPTION
    This bit indicates that the ECC single error is detected and corrected in a
    received packet

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ecc_single_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_ecc_single_err(uint32_t base_addr, uint32_t *ecc_single_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *ecc_single_err = reg->mBits.ecc_single_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_ecc_multi_err

DESCRIPTION
    This bit indicates that the ECC multiple error is detected in a received packet

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *ecc_multi_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_ecc_multi_err(uint32_t base_addr, uint32_t *ecc_multi_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *ecc_multi_err = reg->mBits.ecc_multi_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_crc_err

DESCRIPTION
    This bit indicates that the CRC error is detected in the received packet payload.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3 or
    DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *crc_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_crc_err(uint32_t base_addr, uint32_t *crc_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *crc_err = reg->mBits.crc_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_pkt_size_err

DESCRIPTION
    This bit indicates that the packet size error is detected during the packet reception

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *pkt_size_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_pkt_size_err(uint32_t base_addr, uint32_t *pkt_size_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *pkt_size_err = reg->mBits.pkt_size_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_eopt_err

DESCRIPTION
    This bit indicates that the EoTp packet is not received at the end of the
    incoming peripheral transmission

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *eopt_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_eopt_err(uint32_t base_addr, uint32_t *eopt_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *eopt_err = reg->mBits.eopt_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dpi_pld_wr_err

DESCRIPTION
    This bit indicates that during a DPI pixel line storage, the payload FIFO
    becomes full and the data stored is corrupted.
    Dependency: DSI_DATAINTERFACE = 2 or DSI_DATAINTERFACE = 3 or
    DSI_DATAINTERFACE = 4. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dpi_pld_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_dpi_pld_wr_err(uint32_t base_addr, uint32_t *dpi_pld_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *dpi_pld_wr_err = reg->mBits.dpi_pld_wr_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_cmd_wr_err

DESCRIPTION
    This bit indicates that the system tried to write a command through the
    Generic interface and the FIFO is full. Therefore, the command is not written.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *gen_cmd_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_gen_cmd_wr_err(uint32_t base_addr, uint32_t *gen_cmd_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *gen_cmd_wr_err = reg->mBits.gen_cmd_wr_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_wr_err

DESCRIPTION
    This bit indicates that the system tried to write a payload data through the
    Generic interface and the FIFO is full. Therefore, the payload is not written.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *gen_pld_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_gen_pld_wr_err(uint32_t base_addr, uint32_t *gen_pld_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *gen_pld_wr_err = reg->mBits.gen_pld_wr_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_send_err

DESCRIPTION
    This bit indicates that during a Generic interface packet build, the payload
    FIFO becomes empty and corrupt data is sent.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *gen_pld_send_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_gen_pld_send_err(uint32_t base_addr, uint32_t *gen_pld_send_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *gen_pld_send_err = reg->mBits.gen_pld_send_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_rd_err

DESCRIPTION
    This bit indicates that during a DCS read data, the payload FIFO becomes
    empty and the data sent to the interface is corrupted.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *gen_pld_rd_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_gen_pld_rd_err(uint32_t base_addr, uint32_t *gen_pld_rd_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *gen_pld_rd_err = reg->mBits.gen_pld_rd_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_gen_pld_recev_err

DESCRIPTION
    This bit indicates that during a generic interface packet read back, the
    payload FIFO becomes full and the received data is corrupted.
    Dependency: DSI_GENERIC = 1
    If DSI_GENERIC = 0, this bit is reserved.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *gen_pld_recev_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_gen_pld_recev_err(uint32_t base_addr, uint32_t *gen_pld_recev_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *gen_pld_recev_err = reg->mBits.gen_pld_recev_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_cmd_wr_err

DESCRIPTION
    This bit indicates that the system tried to write a command through the
    DBI but the command FIFO is full. Therefore, the command is not written.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dbi_cmd_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_dbi_cmd_wr_err(uint32_t base_addr, uint32_t *dbi_cmd_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *dbi_cmd_wr_err = reg->mBits.dbi_cmd_wr_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_pld_wr_err

DESCRIPTION
    This bit indicates that the system tried to write the payload data through
    the DBI interface and the FIFO is full. Therefore, the command is not written.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dbi_pld_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_dbi_pld_wr_err(uint32_t base_addr, uint32_t *dbi_pld_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *dbi_pld_wr_err = reg->mBits.dbi_pld_wr_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_pld_rd_err

DESCRIPTION
    This bit indicates that during a DCS read data, the payload FIFO goes
    empty and the data sent to the interface is corrupted.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dbi_pld_rd_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_dbi_pld_rd_err(uint32_t base_addr, uint32_t *dbi_pld_rd_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *dbi_pld_rd_err = reg->mBits.dbi_pld_rd_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_pld_recv_err

DESCRIPTION
    This bit indicates that during a DBI read back packet, the payload FIFO
    becomes full and the received data is corrupted.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dbi_pld_recv_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_dbi_pld_recv_err(uint32_t base_addr, uint32_t *dbi_pld_recv_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *dbi_pld_recv_err = reg->mBits.dbi_pld_recv_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dbi_ilegal_comm_err

DESCRIPTION
    This bit indicates that an attempt to write an illegal command on the DBI
    interface is made and the core is blocked by transmission.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dbi_ilegal_comm_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status_dbi_ilegal_comm_err(uint32_t base_addr, uint32_t *dbi_ilegal_comm_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    *dbi_ilegal_comm_err = reg->mBits.dbi_ilegal_comm_err;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_int1_status

DESCRIPTION
    get dsi host int1 status (read clean).

INPUT PARAMETER
    uint32_t *base_addr: dsi host base addr
    uint32_t *dphy_errors_4:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_int1_status(uint32_t base_addr, uint32_t *int1_status)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_ST1, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_INT_ST1);
    *int1_status = reg->dwValue;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_0

DESCRIPTION
    This bit retrieves the SoT error from the Acknowledge error report


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_0:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_0(uint32_t base_addr, uint32_t ack_with_err_0)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_0 = ack_with_err_0;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_1

DESCRIPTION
    This bit retrieves the SoT Sync error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_1:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_1(uint32_t base_addr, uint32_t ack_with_err_1)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_1 = ack_with_err_1;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_2

DESCRIPTION
    This bit retrieves the EoT Sync error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_2:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_2(uint32_t base_addr, uint32_t ack_with_err_2)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_2 = ack_with_err_2;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_3

DESCRIPTION
    This bit retrieves the Escape Mode Entry Command error from the
    Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_3:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_3(uint32_t base_addr, uint32_t ack_with_err_3)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_3 = ack_with_err_3;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_4

DESCRIPTION
    This bit retrieves the LP Transmit Sync error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_4:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_4(uint32_t base_addr, uint32_t ack_with_err_4)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_4 = ack_with_err_4;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_5

DESCRIPTION
    This bit retrieves the Peripheral Timeout error from the Acknowledge Error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_5:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_5(uint32_t base_addr, uint32_t ack_with_err_5)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_5 = ack_with_err_5;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_6

DESCRIPTION
    This bit retrieves the False Control error from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_6:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_6(uint32_t base_addr, uint32_t ack_with_err_6)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_6 = ack_with_err_6;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_7

DESCRIPTION
    This bit retrieves the reserved (specific to device) from the
    Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_7:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_7(uint32_t base_addr, uint32_t ack_with_err_7)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_7 = ack_with_err_7;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_8

DESCRIPTION
    This bit retrieves the ECC error, single-bit (detected and corrected)
    from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_8:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_8(uint32_t base_addr, uint32_t ack_with_err_8)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_8 = ack_with_err_8;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_9

DESCRIPTION
    This bit retrieves the ECC error, multi-bit (detected, not corrected) from
    the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_9:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_9(uint32_t base_addr, uint32_t ack_with_err_9)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_9 = ack_with_err_9;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_10

DESCRIPTION
    This bit retrieves the checksum error (long packet only) from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_10:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_10(uint32_t base_addr, uint32_t ack_with_err_10)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_10 = ack_with_err_10;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_11

DESCRIPTION
    This bit retrieves the not recognized DSI data type from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_11:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_11(uint32_t base_addr, uint32_t ack_with_err_11)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_11 = ack_with_err_11;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_12

DESCRIPTION
    This bit retrieves the DSI VC ID Invalid from the Acknowledge error report.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_12:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_12(uint32_t base_addr, uint32_t ack_with_err_12)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_12 = ack_with_err_12;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_13

DESCRIPTION
    This bit retrieves the invalid transmission length from the Acknowledge error report.


INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_13:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_13(uint32_t base_addr, uint32_t ack_with_err_13)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_13 = ack_with_err_13;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_14

DESCRIPTION
    This bit retrieves the reserved (specific to device) from the Acknowledge error report.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_14:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_14(uint32_t base_addr, uint32_t ack_with_err_14)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_14 = ack_with_err_14;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ack_with_err_15

DESCRIPTION
    This bit retrieves the DSI protocol violation from the Acknowledge error report.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ack_with_err_15:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_ack_with_err_15(uint32_t base_addr, uint32_t ack_with_err_15)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.ack_with_err_15 = ack_with_err_15;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dphy_errors_0

DESCRIPTION
    This bit indicates ErrEsc escape entry error from Lane 0.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dphy_errors_0:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_dphy_errors_0(uint32_t base_addr, uint32_t dphy_errors_0)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.dphy_errors_0 = dphy_errors_0;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dphy_errors_1

DESCRIPTION
    This bit indicates ErrSyncEsc low-power data transmission
    synchronization error from Lane 0.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dphy_errors_1:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_dphy_errors_1(uint32_t base_addr, uint32_t dphy_errors_1)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.dphy_errors_1 = dphy_errors_1;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dphy_errors_2

DESCRIPTION
    This bit indicates the ErrControl error from Lane 0.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dphy_errors_2:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_dphy_errors_2(uint32_t base_addr, uint32_t dphy_errors_2)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.dphy_errors_2 = dphy_errors_2;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dphy_errors_3

DESCRIPTION
    This bit indicates the LP0 contention error ErrContentionLP0 from Lane 0.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dphy_errors_3:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_dphy_errors_3(uint32_t base_addr, uint32_t dphy_errors_3)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.dphy_errors_3 = dphy_errors_3;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dphy_errors_4

DESCRIPTION
    This bit indicates the LP1 contention error ErrContentionLP1 from Lane 0.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dphy_errors_4:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0_dphy_errors_4(uint32_t base_addr, uint32_t dphy_errors_4)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->mBits.dphy_errors_4 = dphy_errors_4;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_int0

DESCRIPTION
    This bit indicates the LP1 contention error ErrContentionLP1 from Lane 0.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t int0_mask:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int0(uint32_t base_addr, uint32_t int0_mask)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int0_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK0, dsi_host_get_reg_default_tab());

    reg->dwValue = int0_mask;
    regWrite32(base_addr + DSI_HOST_INT_MSK0, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_to_hs_tx

DESCRIPTION
    This bit indicates that the high-speed transmission timeout counter
    reached the end and contention is detected

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t to_hs_tx:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_to_hs_tx(uint32_t base_addr, uint32_t to_hs_tx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.to_hs_tx = to_hs_tx;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_to_lp_rx

DESCRIPTION
    This bit indicates that the low-power reception timeout counter reached
    the end and contention is detected

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t to_lp_rx:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_to_lp_rx(uint32_t base_addr, uint32_t to_lp_rx)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.to_lp_rx = to_lp_rx;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ecc_single_err

DESCRIPTION
    This bit indicates that the ECC single error is detected and corrected in a
    received packet

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ecc_single_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_ecc_single_err(uint32_t base_addr, uint32_t ecc_single_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.ecc_single_err = ecc_single_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_ecc_multi_err

DESCRIPTION
    This bit indicates that the ECC multiple error is detected in a received packet

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t ecc_multi_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_ecc_multi_err(uint32_t base_addr, uint32_t ecc_multi_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.ecc_multi_err = ecc_multi_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_crc_err

DESCRIPTION
    This bit indicates that the CRC error is detected in the received packet payload.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3 or
    DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t crc_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_crc_err(uint32_t base_addr, uint32_t crc_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.crc_err = crc_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_pkt_size_err

DESCRIPTION
    This bit indicates that the packet size error is detected during the packet reception

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t pkt_size_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_pkt_size_err(uint32_t base_addr, uint32_t pkt_size_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.pkt_size_err = pkt_size_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_eopt_err

DESCRIPTION
    This bit indicates that the EoTp packet is not received at the end of the
    incoming peripheral transmission

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t eopt_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_eopt_err(uint32_t base_addr, uint32_t eopt_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.eopt_err = eopt_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dpi_pld_wr_err

DESCRIPTION
    This bit indicates that during a DPI pixel line storage, the payload FIFO
    becomes full and the data stored is corrupted.
    Dependency: DSI_DATAINTERFACE = 2 or DSI_DATAINTERFACE = 3 or
    DSI_DATAINTERFACE = 4. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dpi_pld_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_dpi_pld_wr_err(uint32_t base_addr, uint32_t dpi_pld_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.dpi_pld_wr_err = dpi_pld_wr_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_cmd_wr_err

DESCRIPTION
    This bit indicates that the system tried to write a command through the
    Generic interface and the FIFO is full. Therefore, the command is not written.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_cmd_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_gen_cmd_wr_err(uint32_t base_addr, uint32_t gen_cmd_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.gen_cmd_wr_err = gen_cmd_wr_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_wr_err

DESCRIPTION
    This bit indicates that the system tried to write a payload data through the
    Generic interface and the FIFO is full. Therefore, the payload is not written.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_gen_pld_wr_err(uint32_t base_addr, uint32_t gen_pld_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_wr_err = gen_pld_wr_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_send_err

DESCRIPTION
    This bit indicates that during a Generic interface packet build, the payload
    FIFO becomes empty and corrupt data is sent.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_send_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_gen_pld_send_err(uint32_t base_addr, uint32_t gen_pld_send_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_send_err = gen_pld_send_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_rd_err

DESCRIPTION
    This bit indicates that during a DCS read data, the payload FIFO becomes
    empty and the data sent to the interface is corrupted.
    Dependency: DSI_GENERIC = 1. Otherwise, this bit is reserved

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_rd_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_gen_pld_rd_err(uint32_t base_addr, uint32_t gen_pld_rd_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_rd_err = gen_pld_rd_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_gen_pld_recev_err

DESCRIPTION
    This bit indicates that during a generic interface packet read back, the
    payload FIFO becomes full and the received data is corrupted.
    Dependency: DSI_GENERIC = 1
    If DSI_GENERIC = 0, this bit is reserved.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t gen_pld_recev_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_gen_pld_recev_err(uint32_t base_addr, uint32_t gen_pld_recev_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.gen_pld_recev_err = gen_pld_recev_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_cmd_wr_err

DESCRIPTION
    This bit indicates that the system tried to write a command through the
    DBI but the command FIFO is full. Therefore, the command is not written.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_cmd_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_dbi_cmd_wr_err(uint32_t base_addr, uint32_t dbi_cmd_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.dbi_cmd_wr_err = dbi_cmd_wr_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_pld_wr_err

DESCRIPTION
    This bit indicates that the system tried to write the payload data through
    the DBI interface and the FIFO is full. Therefore, the command is not written.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_pld_wr_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_dbi_pld_wr_err(uint32_t base_addr, uint32_t dbi_pld_wr_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.dbi_pld_wr_err = dbi_pld_wr_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_pld_rd_err

DESCRIPTION
    This bit indicates that during a DCS read data, the payload FIFO goes
    empty and the data sent to the interface is corrupted.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_pld_rd_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_dbi_pld_rd_err(uint32_t base_addr, uint32_t dbi_pld_rd_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.dbi_pld_rd_err = dbi_pld_rd_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_pld_recv_err

DESCRIPTION
    This bit indicates that during a DBI read back packet, the payload FIFO
    becomes full and the received data is corrupted.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_pld_recv_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_dbi_pld_recv_err(uint32_t base_addr, uint32_t dbi_pld_recv_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.dbi_pld_recv_err = dbi_pld_recv_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_dbi_ilegal_comm_err

DESCRIPTION
    This bit indicates that an attempt to write an illegal command on the DBI
    interface is made and the core is blocked by transmission.
    Dependency: DSI_DATAINTERFACE = 1 or DSI_DATAINTERFACE = 3.
    Otherwise, this bit is reserved.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dbi_ilegal_comm_err:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1_dbi_ilegal_comm_err(uint32_t base_addr, uint32_t dbi_ilegal_comm_err)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->mBits.dbi_ilegal_comm_err = dbi_ilegal_comm_err;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_int1

DESCRIPTION
    This bit indicates the LP1 contention error ErrContentionLP1 from Lane 0.

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t int1_mask:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_int1(uint32_t base_addr, uint32_t int1_mask)
{
    int32_t rtn = SUCCESS;
    union dsi_host_int1_tag *reg = getRegValuePtr(DSI_HOST_INT_MSK1, dsi_host_get_reg_default_tab());

    reg->dwValue = int1_mask;
    regWrite32(base_addr + DSI_HOST_INT_MSK1, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_txskewcalhs

DESCRIPTION
    High-speed skew calibration is started when txskewcalsh is set high(assuming that PHY is in Stop state)

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t txskewcalhs:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_txskewcalhs(uint32_t base_addr, uint32_t txskewcalhs)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_cal_tag *reg = getRegValuePtr(DSI_HOST_PHY_CAL, dsi_host_get_reg_default_tab());

    reg->mBits.txskewcalhs = txskewcalhs;
    regWrite32(base_addr + DSI_HOST_PHY_CAL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_compression_mode

DESCRIPTION
    When set to 1, this bit enables the compression mode

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t compression_mode:


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_compression_mode(uint32_t base_addr, uint32_t compression_mode)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dsc_parameter_tag *reg = getRegValuePtr(DSI_HOST_DSC_PARAMETER, dsi_host_get_reg_default_tab());

    reg->mBits.compression_mode = compression_mode;
    regWrite32(base_addr + DSI_HOST_DSC_PARAMETER, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_compress_algo

DESCRIPTION
    This field indicates the algorithm identifier

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t compress_algo:
        00 = VESA DSC Standard 1.1
        11 = vendor-specific algorithm
        01 and 10 = reserved, not used


OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_compress_algo(uint32_t base_addr, uint32_t compress_algo)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dsc_parameter_tag *reg = getRegValuePtr(DSI_HOST_DSC_PARAMETER, dsi_host_get_reg_default_tab());

    reg->mBits.compress_algo = compress_algo;
    regWrite32(base_addr + DSI_HOST_DSC_PARAMETER, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_pps_sel

DESCRIPTION
    This field indicates the PPS selector

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_pps_sel_e pps_sel:
        00 = PPS Table 1
        01 = PPS Table 2
        10 = PPS Table 3
        11 = PPS Table 4

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_pps_sel(uint32_t base_addr, dsi_host_pps_sel_e pps_sel)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dsc_parameter_tag *reg = getRegValuePtr(DSI_HOST_DSC_PARAMETER, dsi_host_get_reg_default_tab());

    reg->mBits.pps_sel = pps_sel;
    regWrite32(base_addr + DSI_HOST_DSC_PARAMETER, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_max_rd_time

DESCRIPTION
    This field configures the maximum time required to perform a read command in lane byte
    clock cycles. This register can onlybe modified when no read command is in progress

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t max_rd_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_max_rd_time(uint32_t base_addr, uint32_t max_rd_time)
{
    int32_t rtn = SUCCESS;
    union dsi_host_phy_tmp_rd_cfg_tag *reg = getRegValuePtr(DSI_HOST_TMR_RD_CFG, dsi_host_get_reg_default_tab());

    reg->mBits.max_rd_time = max_rd_time;
    regWrite32(base_addr + DSI_HOST_TMR_RD_CFG, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_shadow_en

DESCRIPTION
    When set to 1, DPI receives the active configuration from the auxiliary
    registers. When this bit is set along with the vid_shadow_req bit, the
    auxiliary registers are automatically updated

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_shadow_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_shadow_en(uint32_t base_addr, uint32_t vid_shadow_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_shadow_ctrl_tag *reg = getRegValuePtr(DSI_HOST_VID_SHADOW_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.vid_shadow_en = vid_shadow_en;
    regWrite32(base_addr + DSI_HOST_VID_SHADOW_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_shadow_req

DESCRIPTION
    When set to 1, the DPI registers are copied to the auxiliary registers.
    After copying, this bit is auto cleared

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_shadow_req:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_shadow_req(uint32_t base_addr, uint32_t vid_shadow_req)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_shadow_ctrl_tag *reg = getRegValuePtr(DSI_HOST_VID_SHADOW_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.vid_shadow_req = vid_shadow_req;
    regWrite32(base_addr + DSI_HOST_VID_SHADOW_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_set_vid_shadow_pin_req

DESCRIPTION
    When set to 1, the video request is done by external pin. In this mode,
    vid_shadow_req is ignored

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_shadow_pin_req:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_set_vid_shadow_pin_req(uint32_t base_addr, uint32_t vid_shadow_pin_req)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_shadow_ctrl_tag *reg = getRegValuePtr(DSI_HOST_VID_SHADOW_CTRL, dsi_host_get_reg_default_tab());

    reg->mBits.vid_shadow_pin_req = vid_shadow_pin_req;
    regWrite32(base_addr + DSI_HOST_VID_SHADOW_CTRL, reg->dwValue);

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dpi_vcid_act

DESCRIPTION
    This field configures the DPI virtual channel id that is indexed to the
    Video mode packets

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t dpi_vcid:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dpi_vcid_act(uint32_t base_addr, uint32_t *dpi_vcid)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_vcid_act_tag *reg = getRegValuePtr(DSI_HOST_DPI_VCID_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_DPI_VCID_ACT);
    *dpi_vcid = reg->mBits.dpi_vcid;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_dpi_color_coding

DESCRIPTION
    This field configures the DPI color coding as follows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_dpi_color_format_e dpi_color_coding:
        0000: 16-bit configuration 1
        0001: 16-bit configuration 2
        0010: 16-bit configuration 3
        0011: 18-bit configuration 4
        0100: 18-bit configuration
        0101: 24-bit
        0110: 20-bit YCbCr 4:2:2 loosely packed
        0111: 24-bit YCbCr 4:2:2
        1000: 16-bit YCbCr 4:2:2
        1001: 30-bit
        1010: 36-bit
        1011-1111: 12-bit YCbCr 4:2:0

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_dpi_color_coding(uint32_t base_addr, dsi_host_dpi_color_format_e *dpi_color_coding)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_color_coding_act_tag *reg = getRegValuePtr(DSI_HOST_DPI_COLOR_CODING_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_DPI_COLOR_CODING_ACT);
    *dpi_color_coding = reg->mBits.dpi_color_coding;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_loosely18_en

DESCRIPTION
    When set to 1, this bit activates loosely packed variant to 18-bit
    configurations

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t loosely18_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_loosely18_en(uint32_t base_addr, uint32_t *loosely18_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_color_coding_act_tag *reg = getRegValuePtr(DSI_HOST_DPI_COLOR_CODING_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_DPI_COLOR_CODING_ACT);
    *loosely18_en = reg->mBits.loosely18_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_invact_lpcmd_tim_act

DESCRIPTION
    This field is used for the transmission of commands in low-power
    mode. It defines the size, in bytes, of the largest packet that can fit in a
    line during the VACT region

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t invact_lpcmd_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_invact_lpcmd_tim_act(uint32_t base_addr, uint32_t *invact_lpcmd_tim_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_lp_cmd_tim_act_tag *reg = getRegValuePtr(DSI_HOST_DPI_LP_CMD_TIM_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_DPI_LP_CMD_TIM_ACT);
    *invact_lpcmd_tim_act = reg->mBits.invact_lpcmd_time;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_outvact_lpcmd_tim_act

DESCRIPTION
    This field is used for the transmission of commands in low-power
    mode. It defines the size, in bytes, of the largest packet that can fit in a
    line during the VSA, VBP, and VFP regions

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t outvact_lpcmd_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_outvact_lpcmd_tim_act(uint32_t base_addr, uint32_t *outvact_lpcmd_tim_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_dpi_lp_cmd_tim_act_tag *reg = getRegValuePtr(DSI_HOST_DPI_LP_CMD_TIM_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_DPI_LP_CMD_TIM_ACT);
    *outvact_lpcmd_tim_act = reg->mBits.outvact_lpcmd_time;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vid_mode_type_act

DESCRIPTION
    This field indicates the video mode transmission type as follows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_vid_mode_type_e vid_mode_type:
        00: Non-burst with sync pulses
        01: Non-burst with sync events
        10 and 11: Burst mode

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vid_mode_type_act(uint32_t base_addr, dsi_host_vid_mode_type_e *vid_mode_type_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *vid_mode_type_act = reg->mBits.vid_mode_type;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_lp_vsa_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the VSA
    period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vsa_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_lp_vsa_en(uint32_t base_addr, uint32_t *lp_vsa_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *lp_vsa_en = reg->mBits.lp_vsa_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_lp_vbp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the VBP
    period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vbp_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_lp_vbp_en(uint32_t base_addr, uint32_t *lp_vbp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *lp_vbp_en = reg->mBits.lp_vbp_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_lp_vfp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the VFP
    period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vfp_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_lp_vfp_en(uint32_t base_addr, uint32_t *lp_vfp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *lp_vfp_en = reg->mBits.lp_vfp_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_lp_vact_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the VACT
    period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_vact_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_lp_vact_en(uint32_t base_addr, uint32_t *lp_vact_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *lp_vact_en = reg->mBits.lp_vact_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_lp_hbp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the HBP
    period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_hbp_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_lp_hbp_en(uint32_t base_addr, uint32_t *lp_hbp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *lp_hbp_en = reg->mBits.lp_hbp_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_lp_hfp_en

DESCRIPTION
    When set to 1, this bit enables the return to low-power inside the HFP
    period when timing allows

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_hfp_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_lp_hfp_en(uint32_t base_addr, uint32_t *lp_hfp_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *lp_hfp_en = reg->mBits.lp_hfp_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_frame_bta_ack_en

DESCRIPTION
    When set to 1, this bit enables the request for an acknowledge
    response at the end of a frame

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t frame_bta_ack_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_frame_bta_ack_en(uint32_t base_addr, uint32_t *frame_bta_ack_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *frame_bta_ack_en = reg->mBits.frame_bta_ack_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_lp_cmd_en

DESCRIPTION
    When set to 1, this bit enables the command transmission only in low-power mode

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t lp_cmd_en:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_lp_cmd_en(uint32_t base_addr, uint32_t *lp_cmd_en)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_mode_cfg_act_tag *reg = getRegValuePtr(DSI_HOST_VID_MODE_CFG_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_MODE_CFG_ACT);
    *lp_cmd_en = reg->mBits.lp_cmd_en;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vid_pkt_size_act

DESCRIPTION
    This field configures the number of pixels in a single video packet. For
    18-bit not loosely packed, this number must be a multiple of four and
    for YCbCr data types, it must be a multiple of 2, as described in DSI
    specification

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_pkt_size:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vid_pkt_size_act(uint32_t base_addr, uint32_t *vid_pkt_size_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_pkt_size_act_tag *reg = getRegValuePtr(DSI_HOST_VID_PKT_SIZE_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_PKT_SIZE_ACT);
    *vid_pkt_size_act = reg->mBits.vid_pkt_size;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vid_num_chunks_act

DESCRIPTION
    This register configures the number of chunks to be transmitted during
    a Line period (a chunk is pair made of a video packet and a null
    packet).
    If set to 0 or 1, video line is still transmitted in a single packet.
    If set to 1 that packet is part of a chunk, meaning that a null packet
    follows it (if vid_null_size>0). Otherwise, multiple chunks are used to
    transmit each video line

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_num_chunks:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vid_num_chunks_act(uint32_t base_addr, uint32_t *vid_num_chunks_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_num_chunks_act_tag *reg = getRegValuePtr(DSI_HOST_VID_NUM_CHUNKS_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_NUM_CHUNKS_ACT);
    *vid_num_chunks_act = reg->mBits.vid_num_chunks;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vid_null_size_act

DESCRIPTION
    This register configures the number of bytes inside a null packet.
    Setting it to 0 disables null packets

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_null_size:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vid_null_size_act(uint32_t base_addr, uint32_t *vid_null_size_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_null_size_act_tag *reg = getRegValuePtr(DSI_HOST_VID_NULL_SIZE_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_NULL_SIZE_ACT);
    *vid_null_size_act = reg->mBits.vid_null_size;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vid_has_time_act

DESCRIPTION
    This field configures the Horizontal Synchronism Active period in lane
    byte clock cycles

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_has_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vid_has_time_act(uint32_t base_addr, uint32_t *vid_has_time_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_hsa_time_act_tag *reg = getRegValuePtr(DSI_HOST_VID_HSA_TIME_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_HSA_TIME_ACT);
    *vid_has_time_act = reg->mBits.vid_has_time;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vid_hbp_time_act

DESCRIPTION
    This field configures the Horizontal Back Porch period in lane byte
    clock cycles

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_hbp_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vid_hbp_time_act(uint32_t base_addr, uint32_t *vid_hbp_time_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_hbp_time_act_tag *reg = getRegValuePtr(DSI_HOST_VID_HBP_TIME_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_HBP_TIME_ACT);
    *vid_hbp_time_act = reg->mBits.vid_hbp_time;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vid_hline_time_act

DESCRIPTION
    This field configures the size of the total line time
    (HSA+HBP+HACT+HFP) counted in lane byte clock cycles

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vid_hline_time:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vid_hline_time_act(uint32_t base_addr, uint32_t *vid_hline_time_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_hline_time_act_tag *reg = getRegValuePtr(DSI_HOST_VID_HLINE_TIME_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_HBP_TIME_ACT);
    *vid_hline_time_act = reg->mBits.vid_hline_time;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vsa_lines_act

DESCRIPTION
    This field configures the Vertical Synchronism Active period measured
    in number of horizontal lines

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vsa_lines:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vsa_lines_act(uint32_t base_addr, uint32_t *vsa_lines_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vsa_lines_act_tag *reg = getRegValuePtr(DSI_HOST_VID_VSA_LINES_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_VSA_LINES_ACT);
    *vsa_lines_act = reg->mBits.vsa_lines;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vbp_lines_act

DESCRIPTION
    This field configures the Vertical Back Porch period measured in
    number of horizontal lines

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vbp_lines:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vbp_lines_act(uint32_t base_addr, uint32_t *vbp_lines_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vbp_lines_act_tag *reg = getRegValuePtr(DSI_HOST_VID_VBP_LINES_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_VBP_LINES_ACT);
    *vbp_lines_act = reg->mBits.vbp_lines;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_vfp_lines

DESCRIPTION
    This field configures the Vertical Front Porch period measured in
    number of horizontal lines

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t vfp_lines:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_vfp_lines_act(uint32_t base_addr, uint32_t *vfp_lines_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vfp_lines_act_tag *reg = getRegValuePtr(DSI_HOST_VID_VFP_LINES_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_VFP_LINES_ACT);
    *vfp_lines_act = reg->mBits.vfp_lines;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_v_active_lines

DESCRIPTION
    This field configures the Vertical Active period measured in number of
    horizontal lines

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t v_active_lines:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_v_active_lines_act(uint32_t base_addr, uint32_t *v_active_lines_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_vid_vactive_lines_act_tag *reg = getRegValuePtr(DSI_HOST_VID_VACTIVE_LINES_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_VID_VACTIVE_LINES_ACT);
    *v_active_lines_act = reg->mBits.v_active_lines;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_mode_3d_act

DESCRIPTION
    This field defines the 3D mode on/off and display orientation

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_mode_3d_e mode_3d:
        00: 3D mode off (2D Mode On)
        01: 3D mode on, portrait orientation
        10: 3D mode on, landscape orientation
        11: Reserved

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_mode_3d_act(uint32_t base_addr, dsi_host_mode_3d_e *mode_3d_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_sdf_3d_act_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_SDF_3D_ACT);
    *mode_3d_act = reg->mBits.mode_3d;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_format_3d_act

DESCRIPTION
    This field defines the 3D image format

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    dsi_host_format_3d_e format_3d:
        00: Line (alternating lines of left and right data)
        01: Frame (alternating frames of left and right data)
        10: Pixel (alternating pixels of left and right data)
        11: Reserved

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_format_3d_act(uint32_t base_addr, dsi_host_format_3d_e *format_3d_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_sdf_3d_act_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_SDF_3D_ACT);
    *format_3d_act = reg->mBits.format_3d;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_second_vsync

DESCRIPTION
    This field defines whether there is a second VSYNC pulse between left
    and right Images, when 3D image format is frame-based

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t second_vsync:
        0: No sync pulses between left and right data
        1: Sync pulse (HSYNC, VSYNC, blanking) between left and right data

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_second_vsync_act(uint32_t base_addr, uint32_t *second_vsync_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_sdf_3d_act_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_SDF_3D_ACT);
    *second_vsync_act = reg->mBits.second_vsync;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_right_first_act

DESCRIPTION
    This bit defines the left/right order

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t right_first:
        0: Left eye is sent first and then right eye
        1: Right eye data is sent first and then left eye

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_right_first_act(uint32_t base_addr, uint32_t *right_first_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_sdf_3d_act_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_SDF_3D_ACT);
    *right_first_act = reg->mBits.right_first;

    return rtn;
}

/********************************************************************************
FUNCTION NAME
    dsi_host_get_send_3d_cfg_act

DESCRIPTION
    When set, this bit causes the next VSS packet to include 3D control
    payload in every VSS packet

INPUT PARAMETER
    uint32_t base_addr: dsi host base addr
    uint32_t send_3d_cfg:

OUTPUT PARAMETER
    NONE

RETURN VALUE
    int32_t : 0:success other:error

********************************************************************************/
int32_t dsi_host_get_send_3d_cfg_act(uint32_t base_addr, uint32_t *send_3d_cfg_act)
{
    int32_t rtn = SUCCESS;
    union dsi_host_sdf_3d_act_tag *reg = getRegValuePtr(DSI_HOST_SDF_3D_ACT, dsi_host_get_reg_default_tab());

    reg->dwValue = regRead32(base_addr + DSI_HOST_SDF_3D_ACT);
    *send_3d_cfg_act = reg->mBits.send_3d_cfg;

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
