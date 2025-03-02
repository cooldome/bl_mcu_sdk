/**
  ******************************************************************************
  * @file    blsp_boot_parser.h
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
#ifndef __BLSP_BOOT_PARSER_H__
#define __BLSP_BOOT_PARSER_H__

#include "stdint.h"

/** @addtogroup  BL606_BLSP_Boot2
 *  @{
 */

/** @addtogroup  BLSP_BOOT_PARSER
 *  @{
 */

/** @defgroup  BLSP_BOOT_PARSER_Public_Types
 *  @{
 */

/*@} end of group BLSP_BOOT_PARSER_Public_Types */

/** @defgroup  BLSP_BOOT_PARSER_Public_Constants
 *  @{
 */

/*@} end of group BLSP_BOOT_PARSER_Public_Constants */

/** @defgroup  BLSP_BOOT_PARSER_Public_Macros
 *  @{
 */

/*@} end of group BLSP_BOOT_PARSER_Public_Macros */

/** @defgroup  BLSP_BOOT_PARSER_Public_Functions
 *  @{
 */
int32_t blsp_boot_parse_bootheader(boot_image_config *g_boot_img_cfg, uint8_t *data);
int32_t blsp_boot_parse_pkey(boot_image_config *g_boot_img_cfg, uint8_t *data, uint8_t own);
int32_t blsp_boot_parse_signature(boot_image_config *g_boot_img_cfg, uint8_t *data, uint8_t own);
int32_t blsp_boot_parse_aesiv(boot_image_config *g_boot_img_cfg, uint8_t *data);
int32_t blsp_boot_parser_check_signature(boot_image_config *g_boot_img_cfg);
int32_t blsp_boot_parser_check_hash(boot_image_config *g_boot_img_cfg);

/*@} end of group BLSP_BOOT_PARSER_Public_Functions */

/*@} end of group BLSP_BOOT_PARSER */

/*@} end of group BL606_BLSP_Boot2 */

#endif /* __BLSP_BOOT_PARSER_H__ */
