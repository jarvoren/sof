/*
 * Copyright (c) 2018, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the Intel Corporation nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
 *         Keyon Jie <yang.jie@linux.intel.com>
 */

/**
 * \file include/uapi/ipc/dai-intel.h
 * \brief IPC definitions
 * \author Liam Girdwood <liam.r.girdwood@linux.intel.com>
 * \author Keyon Jie <yang.jie@linux.intel.com>
 */

#ifndef __INCLUDE_UAPI_IPC_DAI_INTEL_H__
#define __INCLUDE_UAPI_IPC_DAI_INTEL_H__

#include <uapi/ipc/header.h>

 /* ssc1: TINTE */
#define SOF_DAI_INTEL_SSP_QUIRK_TINTE		(1 << 0)
 /* ssc1: PINTE */
#define SOF_DAI_INTEL_SSP_QUIRK_PINTE		(1 << 1)
 /* ssc2: SMTATF */
#define SOF_DAI_INTEL_SSP_QUIRK_SMTATF		(1 << 2)
 /* ssc2: MMRATF */
#define SOF_DAI_INTEL_SSP_QUIRK_MMRATF		(1 << 3)
 /* ssc2: PSPSTWFDFD */
#define SOF_DAI_INTEL_SSP_QUIRK_PSPSTWFDFD	(1 << 4)
 /* ssc2: PSPSRWFDFD */
#define SOF_DAI_INTEL_SSP_QUIRK_PSPSRWFDFD	(1 << 5)
 /* here is the possibility to define others aux macros */

#define SOF_DAI_INTEL_SSP_FRAME_PULSE_WIDTH_MAX		38
#define SOF_DAI_INTEL_SSP_SLOT_PADDING_MAX		31

/* SSP clocks control settings
 *
 * Macros for clks_control field in sof_ipc_dai_ssp_params struct.
 */

/* mclk 0 disable */
#define SOF_DAI_INTEL_SSP_MCLK_0_DISABLE		BIT(0)
/* mclk 1 disable */
#define SOF_DAI_INTEL_SSP_MCLK_1_DISABLE		BIT(1)
/* mclk keep active */
#define SOF_DAI_INTEL_SSP_CLKCTRL_MCLK_KA		BIT(2)
/* bclk keep active */
#define SOF_DAI_INTEL_SSP_CLKCTRL_BCLK_KA		BIT(3)
/* fs keep active */
#define SOF_DAI_INTEL_SSP_CLKCTRL_FS_KA			BIT(4)
/* bclk idle */
#define SOF_DAI_INTEL_SSP_CLKCTRL_BCLK_IDLE_HIGH	BIT(5)

/* SSP Configuration Request - SOF_IPC_DAI_SSP_CONFIG */
struct sof_ipc_dai_ssp_params {
	uint16_t reserved1;
	uint16_t mclk_id;

	uint32_t mclk_rate;	/* mclk frequency in Hz */
	uint32_t fsync_rate;	/* fsync frequency in Hz */
	uint32_t bclk_rate;	/* bclk frequency in Hz */

	/* TDM */
	uint32_t tdm_slots;
	uint32_t rx_slots;
	uint32_t tx_slots;

	/* data */
	uint32_t sample_valid_bits;
	uint16_t tdm_slot_width;
	uint16_t reserved2;	/* alignment */

	/* MCLK */
	uint32_t mclk_direction;

	uint16_t frame_pulse_width;
	uint16_t tdm_per_slot_padding_flag;
	uint32_t clks_control;
	uint32_t quirks;
} __attribute__((packed));

/* HDA Configuration Request - SOF_IPC_DAI_HDA_CONFIG */
struct sof_ipc_dai_hda_params {
	struct sof_ipc_hdr hdr;
	/* TODO */
} __attribute__((packed));

/* DMIC Configuration Request - SOF_IPC_DAI_DMIC_CONFIG */

/* This struct is defined per 2ch PDM controller available in the platform.
 * Normally it is sufficient to set the used microphone specific enables to 1
 * and keep other parameters as zero. The customizations are:
 *
 * 1. If a device mixes different microphones types with different polarity
 * and/or the absolute polarity matters the PCM signal from a microphone
 * can be inverted with the controls.
 *
 * 2. If the microphones in a stereo pair do not appear in captured stream
 * in desired order due to board schematics choises they can be swapped with
 * the clk_edge parameter.
 *
 * 3. If PDM bit errors are seen in capture (poor quality) the skew parameter
 * that delays the sampling time of data by half cycles of DMIC source clock
 * can be tried for improvement. However there is no guarantee for this to fix
 * data integrity problems.
 */
struct sof_ipc_dai_dmic_pdm_ctrl {
	uint16_t id;		/**< PDM controller ID */

	uint16_t enable_mic_a;	/**< Use A (left) channel mic (0 or 1)*/
	uint16_t enable_mic_b;	/**< Use B (right) channel mic (0 or 1)*/

	uint16_t polarity_mic_a; /**< Optionally invert mic A signal (0 or 1) */
	uint16_t polarity_mic_b; /**< Optionally invert mic B signal (0 or 1) */

	uint16_t clk_edge;	/**< Optionally swap data clock edge (0 or 1) */
	uint16_t skew;		/**< Adjust PDM data sampling vs. clock (0..15) */

	uint16_t reserved[3];	/**< Make sure the total size is 4 bytes aligned */
} __attribute__((packed));

/* This struct contains the global settings for all 2ch PDM controllers. The
 * version number used in configuration data is checked vs. version used by
 * device driver src/drivers/dmic.c need to match. It is incremented from
 * initial value 1 if updates done for the to driver would alter the operation
 * of the microhone.
 *
 * Note: The microphone clock (pdmclk_min, pdmclk_max, duty_min, duty_max)
 * parameters need to be set as defined in microphone data sheet. E.g. clock
 * range 1.0 - 3.2 MHz is usually supported microphones. Some microphones are
 * multi-mode capable and there may be denied mic clock frequencies between
 * the modes. In such case set the clock range limits of the desired mode to
 * avoid the driver to set clock to an illegal rate.
 *
 * The duty cycle could be set to 48-52% if not known. Generally these
 * parameters can be altered within data sheet specified limits to match
 * required audio application performance power.
 *
 * The microphone clock needs to be usually about 50-80 times the used audio
 * sample rate. With highest sample rates above 48 kHz this can relaxed
 * somewhat.
 */
struct sof_ipc_dai_dmic_params {
	uint32_t driver_ipc_version;	/**< Version (1..N) */

	uint32_t pdmclk_min;	/**< Minimum microphone clock in Hz (100000..N) */
	uint32_t pdmclk_max;	/**< Maximum microphone clock in Hz (min...N) */

	uint32_t fifo_fs_a;	/**< FIFO A sample rate in Hz (8000..96000) */
	uint32_t fifo_fs_b;	/**< FIFO B sample rate in Hz (8000..96000) */
	uint16_t fifo_bits_a;	/**< FIFO A word length (16 or 32) */
	uint16_t fifo_bits_b;	/**< FIFO B word length (16 or 32) */

	uint16_t duty_min;	/**< Min. mic clock duty cycle in % (20..80) */
	uint16_t duty_max;	/**< Max. mic clock duty cycle in % (min..80) */

	uint32_t num_pdm_active; /**< Number of active pdm controllers */

	/* reserved for future use */
	uint32_t reserved[8];

	/**< variable number of pdm controller config */
	struct sof_ipc_dai_dmic_pdm_ctrl pdm[0];
} __attribute__((packed));

#endif