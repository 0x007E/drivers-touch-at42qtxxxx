/**
 * @file at42qtxxxx.h
 * @brief AT42QTXXXX touch controller interface for microcontrollers.
 *
 * This header file defines the interface for controlling the AT42QTXXXX touch controller, including configuration macros and data structures for touch detection and response. Functions are provided to initialize the touch controller hardware, read touch status via I2C, and handle touch events. The driver supports multiple touch points with configurable thresholds and debounce settings.
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

#ifndef AT42QTXXXX_H_
#define AT42QTXXXX_H_

    #ifndef AT42QTXXXX_HAL_PLATFORM
        /**
         * @def AT42QTXXXX_HAL_PLATFORM
         * @brief Sets the target platform for the AT42QTXXXX hardware abstraction layer (HAL), e.g., avr or avr0
         * 
         * @details
         * Define this macro with the name of the target platform to select the corresponding platform-specific HAL implementation (such as TWI communication functions) for the AT42QTXXXX touch controller. Common values are avr (classic AVR architecture) or avr0 (AVR0/1 series).
         * 
         * @note Set this macro as a global compiler symbol to ensure that the correct HAL implementation is used across the entire project.
        */
        #define AT42QTXXXX_HAL_PLATFORM avr0
    #endif

	#ifndef AT42QTXXXX_ADDRESS
		/**
		 * @def AT42QTXXXX_ADDRESS
		 * @brief Defines the TWI/I2C address of the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the 7-bit I2C slave address used to communicate with the AT42QTXXXX device on the TWI/I2C bus. The value can be overridden by defining `AT42QTXXXX_ADDRESS` before including this header if the hardware configuration uses a different address.
		 *
		 * @note By default, `AT42QTXXXX_ADDRESS` is set to `0x1B`.
		 */
		#define AT42QTXXXX_ADDRESS 0x1B
	#endif

    #ifndef AT42QTXXXX_REGISTER_CHIPID
		/**
		 * @def AT42QTXXXX_REGISTER_CHIPID
		 * @brief Defines the register address of the chip ID for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the chip ID of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_CHIPID` before including this header if the hardware configuration uses a different chip ID address.
		 *
		 * @note By default, `AT42QTXXXX_REGISTER_CHIPID` is set to `0x00`.
         * * The following bit masks are provided to extract the major and minor parts of the chip ID value:
         * - `AT42QTXXXX_CHIPID_MINORID_MASK`: Mask for the lower 4 bits (minor ID).
         * - `AT42QTXXXX_CHIPID_MAJORID_MASK`: Mask for the upper 4 bits (major ID).
		 */
		#define AT42QTXXXX_REGISTER_CHIPID 0x00

        #define AT42QTXXXX_CHIPID_MINORID_MASK 0x0F
        #define AT42QTXXXX_CHIPID_MAJORID_MASK 0xF0
	#endif

    #ifndef AT42QTXXXX_REGISTER_FIRMWARE_REVISION
		/**
		 * @def AT42QTXXXX_REGISTER_FIRMWARE_REVISION
		 * @brief Defines the register address of the firmware revision for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the firmware revision of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_FIRMWARE_REVISION` before including this header if the hardware configuration uses a different firmware revision address.
		 *
		 * @note By default, `AT42QTXXXX_REGISTER_FIRMWARE_REVISION` is set to `0x01`.
		 */
		#define AT42QTXXXX_REGISTER_FIRMWARE_REVISION 0x01
	#endif

    #ifndef AT42QTXXXX_KEY_NUMBER
        /**
         * @def AT42QTXXXX_KEYX_NUMBER
         * @brief Defines the numbers of touch keys supported by the AT42QTXXXX device.
         *
         * @details
         * This macro specifies the touch key numbers that the AT42QTXXXX device can detect. The value can be overridden by defining `AT42QTXXXX_KEYX_NUMBER` before including this header if the hardware configuration supports a different number of touch keys.
         */
        #define AT42QTXXXX_KEY0_NUMBER 0
        #define AT42QTXXXX_KEY1_NUMBER 1
        #define AT42QTXXXX_KEY2_NUMBER 2
        #define AT42QTXXXX_KEY3_NUMBER 3
        #define AT42QTXXXX_KEY4_NUMBER 4
        #define AT42QTXXXX_KEY5_NUMBER 5
        #define AT42QTXXXX_KEY6_NUMBER 6
	#endif

    #ifndef AT42QTXXXX_REGISTER_DETECT_STATUS
		/**
		 * @def AT42QTXXXX_REGISTER_DETECT_STATUS
		 * @brief Defines the register address of the detection status for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the detection status of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_DETECT_STATUS` before including this header if the hardware configuration uses a different detection status address.
		 *
         * - Bit 0 (0x01): Touch detected (1 = touch detected, 0 = no touch)
         * - Bit 6 (0x40): Overflow flag (1 = overflow occurred, 0 = no overflow)
         * - Bit 7 (0x80): Calibration flag (1 = calibration in progress, 0 = normal operation)
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_DETECT_STATUS` is set to `0x02`.
		 */
		#define AT42QTXXXX_REGISTER_DETECT_STATUS 0x02

        #define AT42QTXXXX_DETECT_STATUS_TOUCH_bm 0x01
        #define AT42QTXXXX_DETECT_STATUS_OVERFLOW_bm 0x40
        #define AT42QTXXXX_DETECT_STATUS_CALIBRATE_bm 0x80
	#endif

    #ifndef AT42QTXXXX_REGISTER_KEY_STATUS
		/**
		 * @def AT42QTXXXX_REGISTER_KEY_STATUS
		 * @brief Defines the register address of the key status for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the key status of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_KEY_STATUS` before including this header if the hardware configuration uses a different key status address.
		 *
         * - Bit X (0x0X): Key pressed (1 = key pressed, 0 = key released)
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_KEY_STATUS` is set to `0x02`.
		 */
		#define AT42QTXXXX_REGISTER_KEY_STATUS 0x03
	#endif

    #ifndef AT42QTXXXX_REGISTER_KEY_SIGNAL_BASE_ADDRESS
		/**
		 * @def AT42QTXXXX_REGISTER_KEY_SIGNAL_BASE_ADDRESS
		 * @brief Defines the base register address for key signals for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the key status of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_KEY_STATUS` before including this header if the hardware configuration uses a different key status address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_KEY_SIGNAL_BASE_ADDRESS` is set to `0x04`.
		 */
		#define AT42QTXXXX_REGISTER_KEY_SIGNAL_BASE_ADDRESS 0x04

        #ifndef AT42QTXXXX_REGISTER_KEY_SIGNAL_ADDRESS_SIZE
            /**
             * @def AT42QTXXXX_REGISTER_KEY_SIGNAL_ADDRESS_SIZE
             * @brief Defines the address size for key signals for the AT42QTXXXX device.
             *
             * @details
             * This macro specifies the address size of the key signal register tatus of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_KEY_SIGNAL_ADDRESS_SIZE` before including this header if the hardware configuration uses a different key status address size.
             * 
             * @note By default, `AT42QTXXXX_REGISTER_KEY_SIGNAL_ADDRESS_SIZE` is set to `2UL` byte.
             */
            #define AT42QTXXXX_REGISTER_KEY_SIGNAL_ADDRESS_SIZE 2UL
        #endif
	#endif

    #ifndef AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS
		/**
		 * @def AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS
		 * @brief Defines the base register address for reference data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the reference data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS` before including this header if the hardware configuration uses a different reference data address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS` is set to `0x12`.
		 */
		#define AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS 0x12

        #ifndef AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS_SIZE
            /**
             * @def AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS_SIZE
             * @brief Defines the address size for reference data for the AT42QTXXXX device.
             *
             * @details
             * This macro specifies the address size of the reference data register of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS_SIZE` before including this header if the hardware configuration uses a different reference data address size.
             * 
             * @note By default, `AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS_SIZE` is set to `2UL` byte.
             */
            #define AT42QTXXXX_REGISTER_REFERENCE_DATA_BASE_ADDRESS_SIZE 2UL
        #endif
	#endif

    #ifndef AT42QTXXXX_REGISTER_NEGATIVE_THRESHOLD_BASE_ADDRESS
		/**
		 * @def AT42QTXXXX_REGISTER_NEGATIVE_THRESHOLD_BASE_ADDRESS
		 * @brief Defines the base register address for negative threshold data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the negative threshold data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_NEGATIVE_THRESHOLD_BASE_ADDRESS` before including this header if the hardware configuration uses a different negative threshold data address.
		 *
         * - Bit X (0x0X): Negative threshold value (1 = threshold exceeded, 0 = threshold not exceeded)
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_NEGATIVE_THRESHOLD_BASE_ADDRESS` is set to `0x20`.
		 */
		#define AT42QTXXXX_REGISTER_NEGATIVE_THRESHOLD_BASE_ADDRESS 0x20
	#endif

    #ifndef AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_BASE_ADDRESS
		/**
		 * @def AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_BASE_ADDRESS
		 * @brief Defines the base register address for adjacent key suppression data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the adjacent key suppression data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_BASE_ADDRESS` before including this header if the hardware configuration uses a different adjacent key suppression data address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_BASE_ADDRESS` is set to `0x27`.
         *
         * The following helper macros are provided for configuring AKS registers:
         * - `AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_FACTOR_bp`: Bit position of the AKS factor field within the register.
         * - `AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_GROUP_MASK`: Bit mask for the AKS group selection field within the register.
		 */
		#define AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_BASE_ADDRESS 0x27

        #ifndef AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_FACTOR_bp
            #define AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_FACTOR_bp 2
        #endif

        #ifndef AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_GROUP_MASK
            #define AT42QTXXXX_REGISTER_ADJACENT_KEY_SUPPRESSION_GROUP_MASK 0x03
        #endif
	#endif

    #ifndef AT42QTXXXX_REGISTER_DETECTION_INTEGRATOR_BASE_ADDRESS
		/**
		 * @def AT42QTXXXX_REGISTER_DETECTION_INTEGRATOR_BASE_ADDRESS
		 * @brief Defines the base register address for detection integrator data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the detection integrator data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_DETECTION_INTEGRATOR_BASE_ADDRESS` before including this header if the hardware configuration uses a different detection integrator data address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_DETECTION_INTEGRATOR_BASE_ADDRESS` is set to `0x2E`.
		 */
		#define AT42QTXXXX_REGISTER_DETECTION_INTEGRATOR_BASE_ADDRESS 0x2E
	#endif

    #ifndef AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL
		/**
		 * @def AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL
		 * @brief Defines the base register address for fast out max cal guard channel data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the fast out max cal guard channel data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL` before including this header if the hardware configuration uses a different fast out max cal guard channel data address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL` is set to `0x35`.
         *
         * The following helper macros are provided for manipulating individual
         * fields of this register:
         * - `AT42QTXXXX_REGISTER_FAST_OUT_bm`: Bit mask for enabling/disabling fast-out mode.
         * - `AT42QTXXXX_REGISTER_MAX_CALIBRATION_bm`: Bit mask for enabling the max calibration feature.
         * - `AT42QTXXXX_REGISTER_GUARD_CHANNEL_MASK`: Bit mask for selecting the guard channel index.
         */
		#define AT42QTXXXX_REGISTER_FAST_OUT_MAX_CAL_GUARD_CHANNEL 0x35
        
        #ifndef AT42QTXXXX_REGISTER_FAST_OUT_bm
            #define AT42QTXXXX_REGISTER_FAST_OUT_bm 0x10
        #endif

        #ifndef AT42QTXXXX_REGISTER_MAX_CALIBRATION_bm
            #define AT42QTXXXX_REGISTER_MAX_CALIBRATION_bm 0x80
        #endif

        #ifndef AT42QTXXXX_REGISTER_GUARD_CHANNEL_MASK
            #define AT42QTXXXX_REGISTER_GUARD_CHANNEL_MASK 0x07
        #endif
	#endif

    #ifndef AT42QTXXXX_REGISTER_LOW_POWER
		/**
		 * @def AT42QTXXXX_REGISTER_LOW_POWER
		 * @brief Defines the base register address for low power data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the low power data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_LOW_POWER` before including this header if the hardware configuration uses a different low power data address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_LOW_POWER` is set to `0x36`.
		 */
		#define AT42QTXXXX_REGISTER_LOW_POWER 0x36
	#endif

    #ifndef AT42QTXXXX_REGISTER_MAX_ON_DURATION
		/**
		 * @def AT42QTXXXX_REGISTER_MAX_ON_DURATION
		 * @brief Defines the base register address for max on duration data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read/write the max on duration data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_MAX_ON_DURATION` before including this header if the hardware configuration uses a different max on duration data address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_MAX_ON_DURATION` is set to `0x37`.
         *
         * The following constants define the possible encoded values for this register:
         * - `AT42QTXXXX_REGISTER_MAX_ON_DURATION_OFF_gc`: Disable max on-duration limiting.
         * - `AT42QTXXXX_REGISTER_MAX_ON_DURATION_160MS_gc`: Limit on-duration to 160 ms.
         * - `AT42QTXXXX_REGISTER_MAX_ON_DURATION_320MS_gc`: Limit on-duration to 320 ms.
         * - `AT42QTXXXX_REGISTER_MAX_ON_DURATION_480MS_gc`: Limit on-duration to 480 ms.
         * - `AT42QTXXXX_REGISTER_MAX_ON_DURATION_640MS_gc`: Limit on-duration to 640 ms.
         * - `AT42QTXXXX_REGISTER_MAX_ON_DURATION_40800MS_gc`: Limit on-duration to 40.8 s.
         */
		 */
		#define AT42QTXXXX_REGISTER_MAX_ON_DURATION 0x37
        
        #ifndef AT42QTXXXX_REGISTER_MAX_ON_DURATION_OFF_gc
            #define AT42QTXXXX_REGISTER_MAX_ON_DURATION_OFF_gc 0x00
        #endif

        #ifndef AT42QTXXXX_REGISTER_MAX_ON_DURATION_160MS_gc
            #define AT42QTXXXX_REGISTER_MAX_ON_DURATION_160MS_gc 0x01
        #endif

        #ifndef AT42QTXXXX_REGISTER_MAX_ON_DURATION_320MS_gc
            #define AT42QTXXXX_REGISTER_MAX_ON_DURATION_320MS_gc 0x02
        #endif

        #ifndef AT42QTXXXX_REGISTER_MAX_ON_DURATION_480MS_gc
            #define AT42QTXXXX_REGISTER_MAX_ON_DURATION_480MS_gc 0x03
        #endif

        #ifndef AT42QTXXXX_REGISTER_MAX_ON_DURATION_640MS_gc
            #define AT42QTXXXX_REGISTER_MAX_ON_DURATION_640MS_gc 0x04
        #endif

        #ifndef AT42QTXXXX_REGISTER_MAX_ON_DURATION_40800MS_gc
            #define AT42QTXXXX_REGISTER_MAX_ON_DURATION_40800MS_gc 0xFF
        #endif
	#endif

    #ifndef AT42QTXXXX_REGISTER_CALIBRATE
		/**
		 * @def AT42QTXXXX_REGISTER_CALIBRATE
		 * @brief Defines the base register address for calibrate data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the calibrate data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_CALIBRATE` before including this header if the hardware configuration uses a different calibrate data address.
         * 
		 * @note By default, `AT42QTXXXX_REGISTER_CALIBRATE` is set to `0x38`.
         *
         * The following bit mask is provided for starting calibration:
         * - `AT42QTXXXX_CALIBRATE_START_bm`: Control bit to initiate calibration.
		 */
		#define AT42QTXXXX_REGISTER_CALIBRATE 0x38

        #define AT42QTXXXX_CALIBRATE_START_bm 0x01
	#endif

    #ifndef AT42QTXXXX_REGISTER_RESET
		/**
		 * @def AT42QTXXXX_REGISTER_RESET
		 * @brief Defines the base register address for reset data for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the register address used to read the reset data of the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_REGISTER_RESET` before including this header if the hardware configuration uses a different reset data address.
         * 
         * - Bit 0 (0x01): Reset (1 = reset, 0 = normal operation).
         *
         * The following bit mask is provided for controlling the reset:
         * - `AT42QTXXXX_RESET_bm`: Control bit to request a device reset.
         */
		#define AT42QTXXXX_REGISTER_RESET 0x39

        #define AT42QTXXXX_RESET_bm 0x01
	#endif

    #ifndef AT42QTXXXX_RESET_TIME
		/**
		 * @def AT42QTXXXX_RESET_TIME
		 * @brief Defines the reset time for the AT42QTXXXX device.
		 *
		 * @details
		 * This macro specifies the time (in milliseconds) to wait after sending a reset signal to the AT42QTXXXX device. The value can be overridden by defining `AT42QTXXXX_RESET_TIME` before including this header if a different reset time is required.
         * 
         * @note By default, `AT42QTXXXX_RESET_TIME` is set to `200` (Watchdog Timer@125ms and 30ms during first initializzation).
		 */
		#define AT42QTXXXX_RESET_TIME 200
	#endif

    #include "../../../utils/macros/stringify.h"
	#include "../../../utils/systick/systick.h"

    #include _STR(../../../hal/AT42QTXXXX_HAL_PLATFORM/twi/twi.h)

    /**
     * @enum AT24QTXXXX_Key_Status_t
     * @brief Represents the touch status of a single AT42QTXXXX key.
     *
     * @details
     * This enumeration defines the possible logical states of an individual touch key as reported by the AT42QTXXXX device. It is typically used as the return type of key status functions to indicate whether a key is currently released or being touched.
     */
    enum AT24QTXXXX_Key_Status_t
    {
        AT24QTXXXX_Key_Status_Released = 0, /**< Key is not being touched (inactive state). */
        AT24QTXXXX_Key_Status_Touch         /**< Key is currently detected as touched (active state). */
    };
    /**
     * @typedef AT24QTXXXX_Key_Status
     * @brief Typedef alias for @ref AT24QTXXXX_Key_Status_t.
     */
    typedef enum AT24QTXXXX_Key_Status_t AT24QTXXXX_Key_Status;

	/**
     * @enum AT42QTXXXX_Key_t
     * @brief Identifies individual touch keys of the AT42QTXXXX device.
     *
     * @details
     * This enumeration provides symbolic names for each touch key (sensor channel) supported by the AT42QTXXXX device. Each value maps to the corresponding key index defined by the `AT42QTXXXX_KEYx_NUMBER` macros and is typically used as a parameter when reading or configuring per-key settings.
     */
    enum AT42QTXXXX_Key_t
    {
        AT24QTXXXX_Key_S0 = AT42QTXXXX_KEY0_NUMBER, /**< Key S0 mapped to key index 0. */
        AT24QTXXXX_Key_S1 = AT42QTXXXX_KEY1_NUMBER, /**< Key S1 mapped to key index 1. */
        AT24QTXXXX_Key_S2 = AT42QTXXXX_KEY2_NUMBER, /**< Key S2 mapped to key index 2. */
        AT24QTXXXX_Key_S3 = AT42QTXXXX_KEY3_NUMBER, /**< Key S3 mapped to key index 3. */
        AT24QTXXXX_Key_S4 = AT42QTXXXX_KEY4_NUMBER, /**< Key S4 mapped to key index 4. */
        AT24QTXXXX_Key_S5 = AT42QTXXXX_KEY5_NUMBER, /**< Key S5 mapped to key index 5. */
        AT24QTXXXX_Key_S6 = AT42QTXXXX_KEY6_NUMBER  /**< Key S6 mapped to key index 6. */
    };
    /**
     * @typedef AT42QTXXXX_Key
     * @brief Typedef alias for @ref AT42QTXXXX_Key_t.
     */
    typedef enum AT42QTXXXX_Key_t AT42QTXXXX_Key;

	/**
     * @enum AT24QTXXXX_Detection_Status_t
     * @brief Represents global detection status flags of the AT42QTXXXX device.
     *
     * @details
     * This enumeration defines symbolic names for the global detection status flags reported by the AT42QTXXXX device. The values correspond to bit masks applied to the detection status register and may be combined to indicate simultaneous conditions such as touch, overflow or ongoing calibration.
     */
    enum AT24QTXXXX_Detection_Status_t
    {
        AT24QTXXXX_Detection_Status_Touch = AT42QTXXXX_DETECT_STATUS_TOUCH_bm,         /**< One or more keys are detected as touched. */
        AT24QTXXXX_Detection_Status_Overflow = AT42QTXXXX_DETECT_STATUS_OVERFLOW_bm,   /**< Measurement overflow condition has occurred. */
        AT24QTXXXX_Detection_Status_Calibrate = AT42QTXXXX_DETECT_STATUS_CALIBRATE_bm  /**< Device is currently performing calibration. */
    };
    /**
     * @typedef AT24QTXXXX_Detection_Status
     * @brief Typedef alias for @ref AT24QTXXXX_Detection_Status_t.
     */
    typedef enum AT24QTXXXX_Detection_Status_t AT24QTXXXX_Detection_Status;

	/**
     * @enum AT42QTXXXX_Averaging_Factor_t
     * @brief Defines possible filter averaging factors for AT42QTXXXX key signals.
     *
     * @details
     * This enumeration specifies the selectable averaging factors applied to key measurement data on the AT42QTXXXX device. Higher averaging factors increase noise immunity at the expense of response time, while lower factors provide faster response with less filtering.
     */
    enum AT42QTXXXX_Averaging_Factor_t
    {
        AT42QTXXXX_Averaging_Factor_x1  = 1,   /**< No additional averaging (fastest response, lowest filtering). */
        AT42QTXXXX_Averaging_Factor_x2  = 2,   /**< Average over 2 samples. */
        AT42QTXXXX_Averaging_Factor_x4  = 4,   /**< Average over 4 samples. */
        AT42QTXXXX_Averaging_Factor_x8  = 8,   /**< Average over 8 samples. */
        AT42QTXXXX_Averaging_Factor_x16 = 16,  /**< Average over 16 samples (high filtering). */
        AT42QTXXXX_Averaging_Factor_x32 = 32   /**< Average over 32 samples (maximum filtering, slowest response). */
    };
    /**
     * @typedef AT42QTXXXX_Averaging_Factor
     * @brief Typedef alias for @ref AT42QTXXXX_Averaging_Factor_t.
     */
    typedef enum AT42QTXXXX_Averaging_Factor_t AT42QTXXXX_Averaging_Factor;
	
	/**
     * @enum AT42QTXXXX_Averaging_Group_t
     * @brief Selects the averaging/filter group for AT42QTXXXX keys.
     *
     * @details
     * This enumeration defines logical averaging groups that can be assigned to individual keys on the AT42QTXXXX device. Grouping keys allows different sets of keys to share common filtering or timing parameters, depending on the device configuration.
     */
    enum AT42QTXXXX_Averaging_Group_t
    {
        AT42QTXXXX_Averaging_Group_0 = 0x00, /**< Averaging group 0. */
        AT42QTXXXX_Averaging_Group_1 = 0x01, /**< Averaging group 1. */
        AT42QTXXXX_Averaging_Group_2 = 0x02, /**< Averaging group 2. */
        AT42QTXXXX_Averaging_Group_3 = 0x03  /**< Averaging group 3. */
    };
    /**
     * @typedef AT42QTXXXX_Averaging_Group
     * @brief Typedef alias for @ref AT42QTXXXX_Averaging_Group_t.
     */
    typedef enum AT42QTXXXX_Averaging_Group_t AT42QTXXXX_Averaging_Group;
	
	/**
     * @enum AT42QTXXXX_Max_On_Duration_t
     * @brief Selectable maximum on-duration limits for the AT42QTXXXX device.
     *
     * @details
     * This enumeration defines the encoded values used to configure the maximum time a key is allowed to remain in the detected (on) state before the device automatically forces it off. The values map directly to the corresponding `AT42QTXXXX_REGISTER_MAX_ON_DURATION_xxx_gc` constants written to the max on-duration register.
     */
    enum AT42QTXXXX_Max_On_Duration_t
    {
        AT42QTXXXX_Max_On_Duration_Off = AT42QTXXXX_REGISTER_MAX_ON_DURATION_OFF_gc,        /**< Disable max on-duration limiting. */
        AT42QTXXXX_Max_On_Duration_160ms = AT42QTXXXX_REGISTER_MAX_ON_DURATION_160MS_gc,    /**< Limit on-duration to 160 ms. */
        AT42QTXXXX_Max_On_Duration_320ms = AT42QTXXXX_REGISTER_MAX_ON_DURATION_320MS_gc,    /**< Limit on-duration to 320 ms. */
        AT42QTXXXX_Max_On_Duration_480ms = AT42QTXXXX_REGISTER_MAX_ON_DURATION_480MS_gc,    /**< Limit on-duration to 480 ms. */
        AT42QTXXXX_Max_On_Duration_640ms = AT42QTXXXX_REGISTER_MAX_ON_DURATION_640MS_gc,    /**< Limit on-duration to 640 ms. */
        AT42QTXXXX_Max_On_Duration_40800ms = AT42QTXXXX_REGISTER_MAX_ON_DURATION_40800MS_gc /**< Limit on-duration to 40.8 s. */
    };
    /**
     * @typedef AT42QTXXXX_Max_On_Duration
     * @brief Typedef alias for @ref AT42QTXXXX_Max_On_Duration_t.
     */
    typedef enum AT42QTXXXX_Max_On_Duration_t AT42QTXXXX_Max_On_Duration;

	/**
     * @enum AT42QTXXXX_Mode_t
     * @brief Generic enable/disable mode used by AT42QTXXXX configuration functions.
     *
     * @details
     * This enumeration represents a simple logical mode flag used by various configuration routines of the AT42QTXXXX driver (for example, enabling or disabling fast-out mode or maximum calibration). It provides readable symbolic names instead of using raw numeric constants.
     */
    enum AT42QTXXXX_Mode_t
    {
        AT42QTXXXX_Mode_Disable = 0, /**< Feature or option is disabled. */
        AT42QTXXXX_Mode_Enable       /**< Feature or option is enabled. */
    };
    /**
     * @typedef AT42QTXXXX_Mode
     * @brief Typedef alias for @ref AT42QTXXXX_Mode_t.
     */
    typedef enum AT42QTXXXX_Mode_t AT42QTXXXX_Mode;
    
    /**
     * @struct AT24QTXXXX_Chip_Identifier_t
     * @brief Holds basic identification and revision information for the AT42QTXXXX device.
     *
     * @details
     * This structure contains fields for the major and minor identifier, as well as the firmware or silicon revision reported by the AT42QTXXXX device. It is typically filled by reading the chip ID and firmware revision registers via I2C and is used to distinguish device variants or firmware versions at runtime.
     */
    struct AT24QTXXXX_Chip_Identifier_t
    {
        unsigned char major_id;  /**< Major identifier (high nibble of the chip ID). */
        unsigned char minor_id;  /**< Minor identifier (low nibble of the chip ID). */
        unsigned char revision;  /**< Firmware or silicon revision code. */
    };
    /**
     * @typedef AT24QTXXXX_Chip_Identifier
     * @brief Alias for struct AT24QTXXXX_Chip_Identifier_t representing AT42QTXXXX device identification data.
     */
    typedef struct AT24QTXXXX_Chip_Identifier_t AT24QTXXXX_Chip_Identifier;
	
                           void at42qtxxxx_init(void);
	                       void at42qtxxxx_firmware_info(AT24QTXXXX_Chip_Identifier *id);
	      AT24QTXXXX_Key_Status at42qtxxxx_key_status(AT42QTXXXX_Key key);
                  unsigned char at42qtxxxx_key_status_all(AT42QTXXXX_Key key);
	AT24QTXXXX_Detection_Status at42qtxxxx_detection_status(void);
	               unsigned int at42qtxxxx_signal(AT42QTXXXX_Key key);
	               unsigned int at42qtxxxx_reference_data(AT42QTXXXX_Key key);
	                       void at42qtxxxx_negative_threshold(AT42QTXXXX_Key key, unsigned char value);
	                       void at42qtxxxx_averaging_factor(AT42QTXXXX_Key key, AT42QTXXXX_Averaging_Factor factor, AT42QTXXXX_Averaging_Group group);
	                       void at42qtxxxx_detection_integrator(AT42QTXXXX_Key key, unsigned char value);
	                       void at42qtxxxx_fast_out_di(AT42QTXXXX_Mode mode);
	                       void at42qtxxxx_max_calibration(AT42QTXXXX_Mode mode);
	                       void at42qtxxxx_guard_channel(AT42QTXXXX_Key key);
	                       void at42qtxxxx_low_power(unsigned char value);
	                       void at42qtxxxx_max_on_duration(AT42QTXXXX_Max_On_Duration duration);
	                       void at42qtxxxx_calibrate(void);
	                       void at42qtxxxx_reset(void);
	
#endif /* AT42QTXXXX_H_ */