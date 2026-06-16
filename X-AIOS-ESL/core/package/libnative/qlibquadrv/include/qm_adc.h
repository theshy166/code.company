/*
 * Copyright (c) Quaming
 *
 * This header file defines the interface for accessing ADC (Analog-to-Digital
 * Converter) devices, including functions to retrieve the device number and
 * read values from specified channels.
 */
#ifndef QM_ADC_H
#define QM_ADC_H
/**
 * @brief Retrieve the device number for the specified ADC device by name.
 *
 * This function takes the name of an ADC device as input and returns its
 * corresponding device number. It is useful for applications that need to
 * access ADC devices by name.
 *
 * @param name The name of the ADC device.
 * @return The device number on success, or a negative error code if the device
 * is not found.
 */
int qm_adc_get_devnum(const char *name);

/**
 * @brief Retrieve the ADC value for a specified device and channel.
 *
 * This function reads the ADC value from a specified device and channel number.
 * It can be used to acquire analog input values for further processing.
 *
 * @param dev_num The device number obtained from qm_adc_get_devnum().
 * @param chn_num The channel number on the ADC device from which to read the
 * value.
 * @return The ADC value on success, or a negative error code if an error
 * occurs.
 */
int qm_adc_get_value(uint32_t dev_num, uint32_t chn_num);

#endif // QM_ADC_H