/**
 * *****************************************************************************
 * @file hal_boot2_custom.h
 * @version 0.1
 * @date 2021-07-17
 * @brief
 * *****************************************************************************
 * @attention
 *
 *  <h2><center>&copy; COPYRIGHT(c) 2020 Bouffalo Lab</center></h2>
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *    3. Neither the name of Bouffalo Lab nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * *****************************************************************************
 */
#ifndef __HAL_BOOT2_H__
#define __HAL_BOOT2_H__

#include "drv_device.h"
#include "bl702_sflash.h"
#include "bl702_glb.h"

#define HAL_EFUSE_CPU_MAX      2
#define HAL_EFUSE_PK_HASH_SIZE 256 / 8

#define BL_TCM_BASE           BL702_TCM_BASE
#define BL_SYS_CLK_PLL        GLB_SYS_CLK_DLL144M
#define BL_SFLASH_CLK         GLB_SFLASH_CLK_72M
#define HAL_PLL_CFG_MAGICCODE "PCFG"

#define BL_FLASH_XIP_BASE BL702_FLASH_XIP_BASE
#define FLASH_NOT_DETECT  0x10

typedef struct
{
    uint8_t encrypted[HAL_EFUSE_CPU_MAX];
    uint8_t sign[HAL_EFUSE_CPU_MAX];
    uint8_t hbn_check_sign;
    uint8_t rsvd[3];
    uint8_t chip_id[8];
    uint8_t pk_hash_cpu0[HAL_EFUSE_PK_HASH_SIZE];
    uint8_t pk_hash_cpu1[HAL_EFUSE_PK_HASH_SIZE];
} boot_efuse_hw_config;

typedef struct
{
    uint8_t xtal_type;
    uint8_t pll_clk;
    uint8_t hclk_div;
    uint8_t bclk_div;

    uint8_t flash_clk_type;
    uint8_t flash_clk_div;
    uint8_t rsvd[2];
} hal_sys_clk_config;

typedef struct
{
    uint32_t magicCode; /*'PCFG'*/
    hal_sys_clk_config cfg;
    uint32_t crc32;
} hal_pll_config;

typedef struct
{
    uint32_t magicCode; /*'FCFG'*/
    SPI_Flash_Cfg_Type cfg;
    uint32_t crc32;
} hal_flash_config;

uint32_t hal_boot2_custom(void);
void hal_reset_sec_eng(void);
void hal_sw_system_reset(void);
int32_t hal_pll_init(hal_pll_config *pll_cfg);
void hal_hbn_set_status_flag(uint32_t flag);
uint32_t hal_hbn_get_status_flag(void);
uint8_t *hal_hbn_get_user_specified_fw(void);
void hal_hbn_clr_user_specified_fw(void);
void efuse_get_boot2_cfg(boot_efuse_hw_config *g_efuse_cfg);
int32_t flash_get_clk_cfg(hal_pll_config *cfg);
#endif