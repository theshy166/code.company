/*
 * Copyright (c)  quaming
 *
 */
#ifndef QM_SYSFS_H_
#define QM_SYSFS_H_

/**
 * qm_write_sysfs_int() - write an integer value to a sysfs file
 * @filename: name of the file to write to
 * @basedir: the sysfs directory in which the file is to be found
 * @val: integer value to write to file
 *
 * Returns a value >= 0 on success, otherwise a negative error code.
 **/
int qm_write_sysfs_int(const char *filename, const char *basedir, int val);

/**
 * qm_write_sysfs_int_and_verify() - write an integer value to a sysfs file
 *				  and verify
 * @filename: name of the file to write to
 * @basedir: the sysfs directory in which the file is to be found
 * @val: integer value to write to file
 *
 * Returns a value >= 0 on success, otherwise a negative error code.
 **/
int qm_write_sysfs_int_and_verify(const char *filename, const char *basedir,
                                  int val);

/**
 * qm_write_sysfs_string_and_verify() - string write, readback and verify
 * @filename: name of file to write to
 * @basedir: the sysfs directory in which the file is to be found
 * @val: the string to write
 *
 * Returns a value >= 0 on success, otherwise a negative error code.
 **/
int qm_write_sysfs_string_and_verify(const char *filename, const char *basedir,
                                     const char *val);

/**
 * qm_write_sysfs_string() - write string to a sysfs file
 * @filename: name of file to write to
 * @basedir: the sysfs directory in which the file is to be found
 * @val: the string to write
 *
 * Returns a value >= 0 on success, otherwise a negative error code.
 **/
int qm_write_sysfs_string(const char *filename, const char *basedir,
                          const char *val);

/**
 * qm_read_sysfs_posint() - read an integer value from file
 * @filename: name of file to read from
 * @basedir: the sysfs directory in which the file is to be found
 *
 * Returns the read integer value >= 0 on success, otherwise a negative error
 * code.
 **/
int qm_read_sysfs_posint(const char *filename, const char *basedir);

/**
 * qm_read_sysfs_float() - read a float value from file
 * @filename: name of file to read from
 * @basedir: the sysfs directory in which the file is to be found
 * @val: output the read float value
 *
 * Returns a value >= 0 on success, otherwise a negative error code.
 **/
int qm_read_sysfs_float(const char *filename, const char *basedir, float *val);

/**
 * qm_read_sysfs_string() - read a string from file
 * @filename: name of file to read from
 * @basedir: the sysfs directory in which the file is to be found
 * @str: output the read string
 *
 * Returns a value >= 0 on success, otherwise a negative error code.
 **/
int qm_read_sysfs_string(const char *filename, const char *basedir, char *str);

#endif
