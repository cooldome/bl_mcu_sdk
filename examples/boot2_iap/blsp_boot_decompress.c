/**
  ******************************************************************************
  * @file    blsp_boot_decompress.c
  * @version V1.2
  * @date
  * @brief   This file is the peripheral case c file
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

#include "xz.h"
#include "blsp_bootinfo.h"
#include "blsp_common.h"
#include "blsp_media_boot.h"
#include "bflb_platform.h"
#include "softcrc.h"
#include "partition.h"
#include "hal_flash.h"

/** @addtogroup  BL606_BLSP_Boot2
 *  @{
 */

/** @addtogroup  BLSP_BOOT_DECOMPRESS
 *  @{
 */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Private_Macros
 *  @{
 */

/*@} end of group BLSP_BOOT_DECOMPRESS_Private_Macros */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Private_Types
 *  @{
 */

/*@} end of group BLSP_BOOT_DECOMPRESS_Private_Types */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Private_Variables
 *  @{
 */
static uint8_t g_xz_output[BFLB_BOOT2_READBUF_SIZE] __attribute__((section(".noinit_data")));
static uint8_t g_malloc_buf[64 * 1024] __attribute__((section(".noinit_data")));

/*@} end of group BLSP_BOOT_DECOMPRESS_Private_Variables */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Global_Variables
 *  @{
 */

/*@} end of group BLSP_BOOT_DECOMPRESS_Global_Variables */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Private_Fun_Declaration
 *  @{
 */

/*@} end of group BLSP_BOOT_DECOMPRESS_Private_Fun_Declaration */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Private_Functions_User_Define
 *  @{
 */

/*@} end of group BLSP_BOOT_DECOMPRESS_Private_Functions_User_Define */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Private_Functions
 *  @{
 */

/****************************************************************************/ /**
 * @brief  Decompress XZ Firmware
 *
 * @param  srcAddress: Source address on flash
 * @param  destAddress: Destination address on flash
 * @param  destMaxSize: Destination flash region size for erase
 * @param  pDestSize: Pointer for output destination firmware size
 *
 * @return Decompress result status
 *
*******************************************************************************/
static int32_t blsp_boot2_fw_decompress(uint32_t src_address, uint32_t dest_address, uint32_t dest_max_size, uint32_t *p_dest_size)
{
    struct xz_buf b;
    struct xz_dec *s;
    enum xz_ret ret;

    *p_dest_size = 0;

    if (dest_max_size > 0) {
        flash_erase(dest_address, dest_max_size);
    }

    xz_crc32_init();
    simple_malloc_init(g_malloc_buf, sizeof(g_malloc_buf));

    /*
    * Support up to 32k dictionary. The actually needed memory
    * is allocated once the headers have been parsed.
    */
    s = xz_dec_init(XZ_PREALLOC, 1 << 15);

    if (s == NULL) {
        MSG_ERR("Memory allocation failed\n");
        return BFLB_BOOT2_MEM_ERROR;
    }

    b.in = g_boot2_read_buf;
    b.in_pos = 0;
    b.in_size = 0;
    b.out = g_xz_output;
    b.out_pos = 0;
    b.out_size = sizeof(g_xz_output);

    while (1) {
        if (b.in_pos == b.in_size) {
            MSG("XZ Feeding\r\n");

            if (BFLB_BOOT2_SUCCESS != blsp_mediaboot_read(src_address, g_boot2_read_buf, sizeof(g_boot2_read_buf))) {
                MSG_ERR("Read XZFW fail\r\n");
                return BFLB_BOOT2_FLASH_READ_ERROR;
            }

            b.in_size = sizeof(g_boot2_read_buf);
            b.in_pos = 0;
            src_address += sizeof(g_boot2_read_buf);
        }

        ret = xz_dec_run(s, &b);

        if (b.out_pos == sizeof(g_xz_output)) {
            //if (fwrite(out, 1, b.out_pos, stdout) != b.out_pos) {
            //  msg = "Write error\n";
            //  goto error;
            //}
            MSG("XZ outputing\r\n");

            if (dest_max_size > 0) {
                flash_write(dest_address, g_xz_output, sizeof(g_xz_output));
            }

            dest_address += sizeof(g_xz_output);
            *p_dest_size += sizeof(g_xz_output);
            b.out_pos = 0;
        }

        if (ret == XZ_OK) {
            continue;
        }

        //if (fwrite(out, 1, b.out_pos, stdout) != b.out_pos
        //  || fclose(stdout)) {
        //  msg = "Write error\n";
        //  goto error;
        //}
        if (b.out_pos > 0) {
            if (dest_max_size > 0) {
                flash_write(dest_address, g_xz_output, b.out_pos);
            }

            dest_address += b.out_pos;
            *p_dest_size += b.out_pos;
        }

        switch (ret) {
            case XZ_STREAM_END:
                xz_dec_end(s);
                return 0;

            case XZ_MEM_ERROR:
                MSG_ERR("Memory allocation failed\n");
                goto error;

            case XZ_MEMLIMIT_ERROR:
                MSG_ERR("Memory usage limit reached\n");
                goto error;

            case XZ_FORMAT_ERROR:
                MSG_ERR("Not a .xz file\n");
                goto error;

            case XZ_OPTIONS_ERROR:
                MSG_ERR("Unsupported options in the .xz headers\n");
                goto error;

            case XZ_DATA_ERROR:
            case XZ_BUF_ERROR:
                MSG_ERR("File is corrupt\n");
                goto error;

            default:
                MSG_ERR("XZ Bug!\n");
                goto error;
        }
    }

error:
    xz_dec_end(s);
    return BFLB_BOOT2_FAIL;
}

/*@} end of group BLSP_BOOT_DECOMPRESS_Private_Functions */

/** @defgroup  BLSP_BOOT_DECOMPRESS_Public_Functions
 *  @{
 */

/****************************************************************************/ /**
 * @brief  Update decompressed firmware to flash according to XZ firmware
 *
 * @param  activeID: Active partition table ID
 * @param  ptStuff: Pointer of partition table stuff
 * @param  ptEntry: Pointer of active entry
 *
 * @return XZ firmware update result status
 *
*******************************************************************************/
int32_t blsp_boot2_update_fw(pt_table_id_type active_id, pt_table_stuff_config *pt_stuff, pt_table_entry_config *pt_entry)
{
    uint8_t active_index = pt_entry->active_index;
    uint32_t new_fw_len;
    int32_t ret;
#ifdef BLSP_BOOT2_ROLLBACK

    /* Try to check Image integrity: try to decompress */
    if (BFLB_BOOT2_SUCCESS != blsp_boot2_fw_decompress(pt_entry->start_address[active_index],
                                                       pt_entry->start_address[!(active_index & 0x01)],
                                                       0, &new_fw_len)) {
        /* Decompress fail, try to rollback to old one */
        pt_entry->active_index = !(active_index & 0x01);
        pt_entry->age++;
        ret = pt_table_update_entry((pt_table_id_type)(!active_id), pt_stuff, pt_entry);

        if (ret != PT_ERROR_SUCCESS) {
            MSG_ERR("Rollback Update Partition table entry fail\r\n");
            return BFLB_BOOT2_FAIL;
        }

        return BFLB_BOOT2_SUCCESS;
    }

#endif

    /* Do decompress */
    if (BFLB_BOOT2_SUCCESS == blsp_boot2_fw_decompress(pt_entry->start_address[active_index],
                                                       pt_entry->start_address[!(active_index & 0x01)],
                                                       pt_entry->max_len[!(active_index & 0x01)], &new_fw_len)) {
        pt_entry->active_index = !(active_index & 0x01);
        pt_entry->len = new_fw_len;
        pt_entry->age++;
        ret = pt_table_update_entry((pt_table_id_type)(!active_id), pt_stuff, pt_entry);

        if (ret != PT_ERROR_SUCCESS) {
            MSG_ERR("Do Decompress Update Partition table entry fail\r\n");
            return BFLB_BOOT2_FAIL;
        }
    } else {
        MSG_ERR("XZ Decompress fail\r\n");
        return BFLB_BOOT2_FAIL;
    }

    return BFLB_BOOT2_SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Check if buffer is XZ header
 *
 * @param  buffer: Buffer of firmware
 *
 * @return 1 for XZ firmware and 0 for not
 *
*******************************************************************************/
int blsp_boot2_verify_xz_header(uint8_t *buffer)
{
    const uint8_t xz_header[6] = { 0xFD, '7', 'z', 'X', 'Z', 0x00 };

    //const uint8_t xz_header[6] = { 0x42, 0x46, 0x4e, 0x50, 0x01, 0x00 };
    if (buffer) {
        if (!memcmp(buffer, xz_header, 6)) {
            return 1;
        }
    }

    return 0;
}

/*@} end of group BLSP_BOOT_DECOMPRESS_Public_Functions */

/*@} end of group BLSP_BOOT_DECOMPRESS */

/*@} end of group BL606_BLSP_Boot2 */
