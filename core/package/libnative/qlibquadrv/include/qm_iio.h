/*
 * Copyright (c) quaming
 *
 * Header file for the IIO (Industrial I/O) functionality.
 * Provides functionality to find an IIO type by device name.
 *
 */
#ifndef QM_IIO_H_
#define QM_IIO_H_

/**
 * @brief Finds the IIO type associated with the given device name.
 *
 * This function searches for an IIO type based on the device name
 * and returns the type if found.
 *
 * @param device_name The name of the device to search for.
 * @param type The IIO type associated with the device name.
 * @return 0 if found successfully, otherwise a negative error code.
 */
int qm_iio_find_type_by_name(const char *device_name, const char *type);

#endif /* QM_IIO_H_ */
