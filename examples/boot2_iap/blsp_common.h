/**
  ******************************************************************************
  * @file    blsp_common.h
  * @version V1.2
  * @date
  * @brief   This file is the peripheral case header file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 Bouffalo Lab</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of Bouffalo Lab nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#ifndef __BLSP_COMMON_H__
#define __BLSP_COMMON_H__

#include "stdint.h"

/** @addtogroup  BL606_BLSP_Boot2
 *  @{
 */

/** @addtogroup  BLSP_COMMON
 *  @{
 */

/** @defgroup  BLSP_COMMON_Public_Types
 *  @{
 */

/*@} end of group BLSP_COMMON_Public_Types */

/** @defgroup  BLSP_COMMON_Public_Constants
 *  @{
 */

/*@} end of group BLSP_COMMON_Public_Constants */

/** @defgroup  BLSP_COMMON_Public_Macros
 *  @{
 */
#define BLSP_BOOT2_CP_FLAG 0x02
#define BLSP_BOOT2_MP_FLAG 0x01
#define BLSP_BOOT2_SP_FLAG 0x00
/*@} end of group BLSP_COMMON_Public_Macros */

/** @defgroup  BLSP_COMMON_Public_Functions
 *  @{
 */
void blsp_dump_data(void *datain, int len);
void blsp_boot2_jump_entry(void);
int32_t blsp_mediaboot_pre_jump(void);
uint8_t blsp_boot2_get_feature_flag(void);
uint8_t blsp_boot2_get_log_disable_flag(void);
uint8_t blsp_boot2_8m_support_flag(void);
uint8_t blsp_boot2_dump_critical_flag(void);
uint32_t blsp_boot2_get_baudrate(void);
uint8_t blsp_boot2_get_tx_gpio(void);

/*@} end of group BLSP_COMMON_Public_Functions */

/*@} end of group BLSP_COMMON */

/*@} end of group BL606_BLSP_Boot2 */

#endif /* __BLSP_COMMON_H__ */
