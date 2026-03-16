/**
 * @file at42qtxxxx.c
 * @brief Implementation of AT42QTXXXX touch controller functions.
 *
 * This source file provides functions to initialize and control the AT42QTXXXX touch controller, including reading touch status and handling touch events. It utilizes the I2C communication peripheral to communicate with the device.
 *
 * @author g.raf
 * @date 2026-03-05
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2026 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 *
 * @note This file is part of a larger embedded systems project and subject to the license specified in the repository. For updates  and the complete revision history, see the GitHub repository.
 *
 * @see https://github.com/0x007e/drivers-touch-at42qtxxxx "AT42QTXXXX Touch Controller GitHub Repository"
 */

#include "at42qtxxxx.h"

unsigned char at42qtxxxx_buffer[2];

static void at42qtxxxx_write(unsigned char reg, const unsigned char data)
{
    twi_start();
    twi_address(AT42QTXXXX_ADDRESS, TWI_WRITE);
    twi_set(reg);
    twi_set(data);
    twi_stop();
}

static void at42qtxxxx_read(unsigned char reg, unsigned char *data, unsigned char length)
{
    twi_start();
    twi_address(AT42QTXXXX_ADDRESS, TWI_WRITE);
    twi_set(reg);
    twi_stop();

    twi_start();
    twi_address(AT42QTXXXX_ADDRESS, TWI_WRITE);

    for(unsigned char i=0; i < length; i++)
    {
        if(i < (length - 1))
        {
            twi_get(&data[i], TWI_ACK);
            continue;
        }
        twi_get(&data[i], TWI_NACK);
    }
    twi_stop();
}


void at42qtxxxx_init(void)
{
    at42qtxxxx_reset();
    at42qtxxxx_calibrate();
    
    while(at42qtxxxx_detection_status() & AT24QTXXXX_Detection_Status_Calibrate);
}

/**
 * @brief Read chip and firmware identification information from the AT42QTXXXX device.
 *
 * @details
 * This function reads the chip ID and firmware revision registers of the AT42QTXXXX device and fills the provided AT24QTXXXX_Chip_Identifier structure with the decoded information. The major and minor identifiers are extracted from the chip ID using the corresponding bit masks, and the revision field is populated from the firmware revision register. The caller must provide a valid pointer to an AT24QTXXXX_Chip_Identifier instance.
 *
 * @param id Pointer to an AT24QTXXXX_Chip_Identifier structure that will receive the decoded major ID, minor ID, and revision information.
 */
void at42qtxxxx_firmware_info(AT24QTXXXX_Chip_Identifier *id)
{
    at42qtxxxx_read(AT42QTXXXX_REGISTER_CHIPID, at42qtxxxx_buffer, 1);

    id->major_id = ((at42qtxxxx_buffer[0] & AT42QTXXXX_CHIPID_MAJORID_MASK)>>4);
    id->minor_id = (at42qtxxxx_buffer[0] & AT42QTXXXX_CHIPID_MINORID_MASK);
	
	at42qtxxxx_read(AT42QTXXXX_REGISTER_FIRMWARE_REVISION, at42qtxxxx_buffer, 1);

	id->revision = at42qtxxxx_buffer[0];
}

/**
 * @brief Get the touch status of a single AT42QTXXXX key.
 *
 * @details
 * This function checks the global detection status of the AT42QTXXXX device and, if a touch is indicated, reads the key status register to determine whether the specified key is currently touched. If the corresponding bit for the given key index is set, the function returns AT24QTXXXX_Key_Status_Touch; otherwise, it returns AT24QTXXXX_Key_Status_Released. The function implicitly clears and re-reads the key status register before evaluating the bit state.
 *
 * @param key Logical key identifier selecting the touch key whose status should be queried.
 *
 * @return AT24QTXXXX_Key_Status_Touch if the specified key is detected as touched, otherwise AT24QTXXXX_Key_Status_Released.
 */
AT24QTXXXX_Key_Status at42qtxxxx_key_status(AT42QTXXXX_Key key);
{
    at42qtxxxx_buffer[0] = 0x00;

    if(at42qtxxxx_detection_status() & AT24QTXXXX_Detection_Status_Touch)
    {
        at42qtxxxx_write(AT42QTXXXX_REGISTER_KEY_STATUS, 0x00);
        at42qtxxxx_read(AT42QTXXXX_REGISTER_KEY_STATUS, at42qtxxxx_buffer, 1);

        if(at42qtxxxx_buffer[0] & (1<<key))
        {
            return AT24QTXXXX_Key_Status_Touch;
        }
    }
    return AT24QTXXXX_Key_Status_Released;
}

/**
 * @brief Get the raw touch status bit mask for all AT42QTXXXX keys.
 *
 * @details
 * This function reads the key status register of the AT42QTXXXX device and returns its raw 8-bit value. Each bit in the returned value corresponds to one key, where a set bit indicates that the associated key is currently detected as touched. The function writes a zero to the key status register before reading it, as required by the device protocol, and then returns the retrieved bit mask unchanged.
 *
 * @return An 8-bit bit mask representing the touch status of all keys; bits set to 1 indicate touched keys.
 */
unsigned char at42qtxxxx_key_status_all(void)
{
    at42qtxxxx_buffer[0] = 0x00;

    at42qtxxxx_write(AT42QTXXXX_REGISTER_KEY_STATUS, 0x00);
    at42qtxxxx_read(AT42QTXXXX_REGISTER_KEY_STATUS, at42qtxxxx_buffer, 1);

    return at42qtxxxx_buffer[0];
}

/**
 * @brief Read the global detection status flags of the AT42QTXXXX device.
 *
 * @details
 * This function accesses the detection status register of the AT42QTXXXX device and returns its raw value cast to the AT24QTXXXX_Detection_Status type. The returned bit field may contain one or more flags indicating conditions such as active touches, measurement overflow, or an ongoing calibration cycle. The function writes a zero to the detection status register before reading it, according to the device’s access protocol.
 *
 * @return An AT24QTXXXX_Detection_Status value representing the current detection status bit field of the device.
 */
AT24QTXXXX_Detection_Status at42qtxxxx_detection_status(void)
{
    at42qtxxxx_buffer[0] = 0x00;

    at42qtxxxx_write(AT42QTXXXX_REGISTER_DETECT_STATUS, 0x00);
    at42qtxxxx_read(AT42QTXXXX_REGISTER_DETECT_STATUS, at42qtxxxx_buffer, 1);

    return (AT24QTXXXX_Detection_Status)at42qtxxxx_buffer[0];
}

/**
 * @brief Read the raw signal value for a specific AT42QTXXXX key.
 *
 * @details
 * This function reads the 16-bit signal value associated with the specified key from the key signal register block of the AT42QTXXXX device. The register address is computed from the base key signal address plus an offset derived from the key index and the configured address size. The two received bytes are combined into a single unsigned 16-bit value with the first byte as the high byte.
 *
 * @param key Logical key identifier selecting the touch key whose signal value should be read.
 *
 * @return The 16-bit raw signal value for the specified key.
 */
unsigned int at42qtxxxx_signal(AT42QTXXXX_Key key)
{
    at42qtxxxx_read(AT42QTXXXX_REGISTER_KEY_SIGNAL_BASE_ADDRESS + (key * AT42QTXXXX_REGISTER_KEY_SIGNAL_ADDRESS_SIZE), at42qtxxxx_buffer, 2);

    return (unsigned int)((at42qtxxxx_buffer[0]<<8) | at42qtxxxx_buffer[1]);
}

/**
 * @brief Read the reference (baseline) value for a specific AT42QTXXXX key.
 *
 * @details
 * This function reads the 16-bit reference data associated with the specified key from the reference data register block of the AT42QTXXXX device. The register address is computed from the base reference data address plus an offset derived from the key index and the configured address size. The two received bytes are combined into a single unsigned 16-bit value with the first byte as the high byte.
 *
 * @param key Logical key identifier selecting the touch key whose reference value should be read.
 *
 * @return The 16-bit reference (baseline) value for the specified key.
 */
unsigned int at42qtxxxx_reference_data(AT42QTXXXX_Key key)
{
    at42qtxxxx_read(AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS + (key * AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS_SIZE), at42qtxxxx_buffer, 2);

    return (unsigned int)((at42qtxxxx_buffer[0]<<8) | at42qtxxxx_buffer[1]);
}

/**
 * @brief Configure the negative threshold for a specific AT42QTXXXX key.
 *
 * @details
 * This function writes the provided threshold value to the negative threshold register corresponding to the specified key. The register address is computed from the base negative threshold address plus the key index, allowing each key to have its own independent threshold setting.
 *
 * @param key Logical key identifier selecting the touch key whose negative threshold should be configured.
 * @param value 8-bit negative threshold value to be written to the device.
 */
void at42qtxxxx_negative_threshold(AT42QTXXXX_Key key, unsigned char value)
{
    at42qtxxxx_buffer[0] = 0x00;
    at42qtxxxx_write((AT42QTXXXX_REGISTER_NEGATIVE_THRESHOLD_BASE_ADDRESS + key), value);
}

/**
 * @brief Configure the averaging factor and group for a specific AT42QTXXXX key.
 *
 * @details This function programs the adjacent key suppression register for the specified key to set both the averaging factor and the averaging group. The register address is calculated from the adjacent key suppression base address plus the key index, and the provided factor and group are combined into a single byte using the defined bit position and mask.
 *
 * @param key Logical key identifier selecting the touch key whose averaging configuration should be set.
 * @param factor Averaging factor to be applied to the key’s measurement data.
 * @param group Averaging group to which the key should be assigned.
 */
void at42qtxxxx_averaging_factor(AT42QTXXXX_Key key, AT42QTXXXX_Averaging_Factor factor, AT42QTXXXX_Averaging_Group group)
{
    at42qtxxxx_write((AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_BASE_ADDRESS + key), ((factor<<AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_FACTOR_bp) | (AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_GROUP_MASK & group)));
}

/**
 * @brief Configure the detection integrator value for a specific AT42QTXXXX key.
 *
 * @details This function writes the given value to the detection integrator register corresponding to the specified key. The register address is computed from the detection integrator base address plus the key index, allowing each key to have an independent integrator setting that influences touch detection sensitivity and debounce behavior.
 *
 * @param key Logical key identifier selecting the touch key whose detection integrator should be configured.
 * @param value 8-bit detection integrator value to be written to the device.
 */
void at42qtxxxx_detection_integrator(AT42QTXXXX_Key key, unsigned char value)
{
    at42qtxxxx_write((AT42QTXXXX_REGISTER_DETECTION_INTEGRATOR_BASE_ADDRESS + key), value);
}

/**
 * @brief Enable or disable the fast-out mode of the AT42QTXXXX device.
 *
 * @details This function reads the shared configuration register that controls fast-out mode, maximum calibration behavior and the guard channel selection, clears the fast-out bit and then conditionally sets it again depending on the requested mode. The updated value is written back to the device so that fast-out mode is either enabled or disabled without affecting the other bits in the same register.
 *
 * @param mode Operation mode selecting whether fast-out should be enabled or disabled.
 */
void at42qtxxxx_fast_out_di(AT42QTXXXX_Mode mode)
{
    at42qtxxxx_read(AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL, at42qtxxxx_buffer, 1);

    at42qtxxxx_buffer[0] &= ~(AT42QTXXXX_REGISTER_FAST_OUT_bm);

    if(mode == AT42QTXXXX_Mode_Enable)
    {
        at42qtxxxx_buffer[0] |= AT42QTXXXX_REGISTER_FAST_OUT_bm;
    }

    at42qtxxxx_write(AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL, at42qtxxxx_buffer[0]);
}

/**
 * @brief Enable or disable the maximum calibration feature of the AT42QTXXXX device.
 *
 * @details This function reads the shared configuration register that controls fast-out mode, maximum calibration behavior and the guard channel selection, clears the maximum calibration bit and then conditionally sets it again depending on the requested mode. The updated value is written back to the device so that the max calibration feature is either enabled or disabled without modifying the other bits in the same register.
 *
 * @param mode Operation mode selecting whether maximum calibration should be enabled or disabled.
 */
void at42qtxxxx_max_calibration(AT42QTXXXX_Mode mode)
{
    at42qtxxxx_read(AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL, at42qtxxxx_buffer, 1);

    at42qtxxxx_buffer[0] &= ~(AT42QTXXXX_REGISTER_MAX_CALIBRATION_bm);

    if(mode == AT42QTXXXX_Mode_Enable)
    {
        at42qtxxxx_buffer[0] |= AT42QTXXXX_REGISTER_MAX_CALIBRATION_bm;
    }

    at42qtxxxx_write(AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL, at42qtxxxx_buffer[0]);
}

/**
 * @brief Select the guard channel used by the AT42QTXXXX device.
 *
 * @details This function reads the shared configuration register that controls fast-out mode, maximum calibration behavior and the guard channel selection, clears the guard channel field using the guard channel mask and then writes the new key index into that field. The updated value is written back so that the specified key is used as the guard channel without affecting the other bits in the same register.
 *
 * @param key Logical key identifier selecting which key should be configured as the guard channel.
 */
void at42qtxxxx_guard_channel(AT42QTXXXX_Key key)
{
    at42qtxxxx_read(AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL, at42qtxxxx_buffer, 1);

    at42qtxxxx_buffer[0] &= ~(AT42QTXXXX_REGISTER_GUARD_CHANNEL_MASK);
    at42qtxxxx_buffer[0] |= key;

    at42qtxxxx_write(AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL, at42qtxxxx_buffer[0]);
}

/**
 * @brief Configure the low-power mode timing of the AT42QTXXXX device.
 *
 * @details This function writes the given value to the low-power register of the AT42QTXXXX device, adjusting the device’s low-power behavior such as sleep or burst spacing according to the encoding defined in the datasheet. The caller is responsible for providing a value that matches the desired low-power timing configuration.
 *
 * @param value 8-bit value to be written to the low-power configuration register.
 */
void at42qtxxxx_low_power(unsigned char value)
{
    at42qtxxxx_write(AT42QTXXXX_REGISTER_LOW_POWER, value);
}

/**
 * @brief Configure the maximum on-duration limit of the AT42QTXXXX device.
 *
 * @details This function writes the specified encoded duration value to the max on-duration register of the AT42QTXXXX device. The duration parameter selects how long a key is allowed to remain in the detected state before the device automatically forces it off, using one of the predefined AT42QTXXXX_Max_On_Duration enumeration values.
 *
 * @param duration Encoded maximum on-duration value to be written to the device.
 */
void at42qtxxxx_max_on_duration(AT42QTXXXX_Max_On_Duration duration)
{
    at42qtxxxx_write(AT42QTXXXX_REGISTER_MAX_ON_DURATION, duration);
}

/**
 * @brief Start a calibration cycle on the AT42QTXXXX device.
 *
 * @details This function writes the calibration start bit to the calibration register of the AT42QTXXXX device, causing it to begin recalibrating its sensing channels. It does not wait for the calibration to complete; callers that require completion must poll the detection status register separately.
 */
void at42qtxxxx_calibrate(void)
{
    at42qtxxxx_write(AT42QTXXXX_REGISTER_CALIBRATE, AT42QTXXXX_CALIBRATE_START_bm);
}

/**
 * @brief Issue a software reset to the AT42QTXXXX device.
 *
 * @details This function writes the reset control bit to the reset register of the AT42QTXXXX device, causing it to perform an internal reset. After issuing the reset, it waits for a fixed period defined by AT42QTXXXX_RESET_TIME using the system tick timer to ensure the device has completed its startup sequence before further communication.
 */
void at42qtxxxx_reset(void)
{
    at42qtxxxx_write(AT42QTXXXX_REGISTER_RESET, AT42QTXXXX_RESET_bm);
    systick_timer_wait_ms(AT42QTXXXX_RESET_TIME);
}