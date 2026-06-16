/*
 * Copyright (c) quaming
 *
 * Header file for motor control API.
 * Provides functions to control motor operations like running, stopping, etc.
 */

#ifndef __QM_MOTOR_H__
#define __QM_MOTOR_H__

/**
 * @brief Control the motor's operation.
 *
 * This function allows you to control the motor's direction, step count, and
 * speed.
 *
 * @param idx The motor index (identifier) to control.
 * @param direction The direction of rotation (e.g., 0 for clockwise, 1 for
 * counterclockwise).
 * @param step The number of steps to rotate the motor.
 * @param speed The speed at which to rotate the motor, in appropriate units.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_motor_run_control(int idx, int direction, int step, int speed);

/**
 * @brief Manually stop the motor.
 *
 * This function stops the motor operation immediately.
 *
 * @param idx The motor index (identifier) to stop.
 * @return 0 on success, or a negative error code on failure.
 */
int qm_motor_manualStop(int idx);
/**
 * @brief 控制电机开关并设置模式
 * 
 * 该函数用于初始化电机配置，设置PWM参数，并根据指定模式调整PWM周期。
 *
 * @param index   马达pwm号
 * @param enabled 电机使能状态，true为启用，false为禁用
 * @param intensity 震动强度，0 ~ 100
 * @param time 震动时间，-1为常震动，其他为定时震动,单位us
 * @return int 返回0表示成功，非0表示失败
 */
int qm_motor_control(int index,bool enabled, int intensity, int time);
#endif
