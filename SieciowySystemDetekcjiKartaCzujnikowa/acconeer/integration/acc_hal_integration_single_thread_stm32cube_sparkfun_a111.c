// Copyright (c) Acconeer AB, 2018-2019
// All rights reserved

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../../main.h"

#include "../rss/include/acc_definitions.h"
#include "../integration/acc_hal_integration.h"


/* spi handle */
extern SPI_HandleTypeDef hspi3;
#define SPI_HANDLE hspi3

/**
 * @brief The number of sensors available on the board
 */
#define SENSOR_COUNT 1

/**
 * @brief Size of SPI transfer buffer
 */
#define SPI_MAX_TRANSFER_SIZE 4095

/**
 * @brief The reference frequency used by this board
 *
 * This assumes 26 MHz on the Sparkfun A111 Board
 */
#define ACC_BOARD_REF_FREQ 26000000


/**
 * @brief Format for the RSS logging
 *
 */

#define LOG_FORMAT          "%02u:%02u:%02u.%03u [%5u] (%c) (%s): %s\n"


/**
 * @brief Sensor states
 */
typedef enum
{
	SENSOR_STATE_UNKNOWN,
	SENSOR_STATE_READY,
	SENSOR_STATE_BUSY
} acc_hal_integration_sensor_state_t;

/**
 * @brief Sensor state collection that keeps track of current state of the sensor
 */
static acc_hal_integration_sensor_state_t sensor_state = SENSOR_STATE_UNKNOWN;


//----------------------------------------
// Implementation of RSS HAL handlers
//----------------------------------------


static void acc_hal_integration_sleep_us(uint32_t time_usec)
{
	uint32_t time_ms = time_usec / 1000;

	HAL_Delay(time_ms ? time_ms : 1);
}


static void acc_hal_integration_sensor_transfer(acc_sensor_id_t sensor_id, uint8_t *buffer, size_t buffer_size)
{
	(void)sensor_id;  // Ignore parameter sensor_id

	const uint32_t SPI_TRANSMIT_RECEIVE_TIMEOUT = 5000;

	HAL_GPIO_WritePin(A111_CS_N_GPIO_Port, A111_CS_N_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&SPI_HANDLE, buffer, buffer, buffer_size, SPI_TRANSMIT_RECEIVE_TIMEOUT);
	HAL_GPIO_WritePin(A111_CS_N_GPIO_Port, A111_CS_N_Pin, GPIO_PIN_SET);
}


static void acc_hal_integration_sensor_power_on(acc_sensor_id_t sensor_id)
{
	(void)sensor_id;  // Ignore parameter sensor_id

	if (sensor_state == SENSOR_STATE_BUSY)
	{
		return;
	}

	HAL_GPIO_WritePin(A111_ENABLE_GPIO_Port, A111_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A111_CS_N_GPIO_Port, A111_CS_N_Pin, GPIO_PIN_SET);

	// Wait 2 ms to make sure that the sensor crystal have time to stabilize
	acc_hal_integration_sleep_us(2000);

	sensor_state = SENSOR_STATE_BUSY;
}


static void acc_hal_integration_sensor_power_off(acc_sensor_id_t sensor_id)
{
	(void)sensor_id;  // Ignore parameter sensor_id

	if (sensor_state != SENSOR_STATE_BUSY)
	{
		return;
	}

	HAL_GPIO_WritePin(A111_CS_N_GPIO_Port, A111_CS_N_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A111_ENABLE_GPIO_Port, A111_ENABLE_Pin, GPIO_PIN_RESET);

	sensor_state = SENSOR_STATE_UNKNOWN;
}


static bool acc_hal_integration_is_sensor_interrupt_pin_active(void)
{
	uint_fast8_t value;

	value = HAL_GPIO_ReadPin(A111_SENSOR_INTERRUPT_GPIO_Port, A111_SENSOR_INTERRUPT_Pin);

	return value != 0;
}


static bool acc_hal_integration_wait_for_sensor_interrupt(acc_sensor_id_t sensor_id, uint32_t timeout_ms)
{
	(void)sensor_id; // Ignore parameter sensor_id

	const uint32_t poll_us      = 10;
	uint64_t       curr_wait_us = 0;
	uint64_t       timeout_us = (uint64_t)timeout_ms * 1000;

	while (!acc_hal_integration_is_sensor_interrupt_pin_active() && (curr_wait_us <= timeout_us))
	{
		acc_hal_integration_sleep_us(poll_us);
		curr_wait_us += poll_us;
	}

	return curr_wait_us <= timeout_us;
}


static float acc_hal_integration_get_reference_frequency(void)
{
	return ACC_BOARD_REF_FREQ;
}


static void acc_hal_integration_get_current_time(uint32_t *time_usec)
{
	uint32_t ms = HAL_GetTick();

	*time_usec = ms * 1000;
}


static void acc_hal_integration_log(acc_log_level_t level, const char *module, const char *buffer)
{
	acc_os_thread_id_t thread_id = 0;
	uint32_t           time_usec = 0;
	char               level_ch;

	acc_hal_integration_get_current_time(&time_usec);

	unsigned int timestamp    = time_usec;
	unsigned int hours        = timestamp / 1000 / 1000 / 60 / 60;
	unsigned int minutes      = timestamp / 1000 / 1000 / 60 % 60;
	unsigned int seconds      = timestamp / 1000 / 1000 % 60;
	unsigned int milliseconds = timestamp / 1000 % 1000;

	level_ch = (level < ACC_LOG_LEVEL_MAX) ? "EWIVDD"[level] : '?';

	printf(LOG_FORMAT, hours, minutes, seconds, milliseconds, (unsigned int)thread_id, level_ch, module, buffer);

	fflush(stdout);
}



acc_hal_t acc_hal_integration_get_implementation(void)
{
	acc_hal_t hal =
	{
		.properties.sensor_count          = SENSOR_COUNT,
		.properties.max_spi_transfer_size = SPI_MAX_TRANSFER_SIZE,

		.sensor_device.power_on                = acc_hal_integration_sensor_power_on,
		.sensor_device.power_off               = acc_hal_integration_sensor_power_off,
		.sensor_device.wait_for_interrupt      = acc_hal_integration_wait_for_sensor_interrupt,
		.sensor_device.transfer                = acc_hal_integration_sensor_transfer,
		.sensor_device.get_reference_frequency = acc_hal_integration_get_reference_frequency,

		.os.sleep_us  = acc_hal_integration_sleep_us,
		.os.mem_alloc = malloc,
		.os.mem_free  = free,
		.os.gettime   = acc_hal_integration_get_current_time,

		// Configure single threaded mode by setting the function pointers below to NULL
		.os.get_thread_id                   = NULL,
		.os.mutex_create                    = NULL,
		.os.mutex_destroy                   = NULL,
		.os.mutex_lock                      = NULL,
		.os.mutex_unlock                    = NULL,
		.os.thread_create                   = NULL,
		.os.thread_exit                     = NULL,
		.os.thread_cleanup                  = NULL,
		.os.semaphore_create                = NULL,
		.os.semaphore_destroy               = NULL,
		.os.semaphore_wait                  = NULL,
		.os.semaphore_signal                = NULL,
		.os.semaphore_signal_from_interrupt = NULL,

		.log.log_level = ACC_LOG_LEVEL_INFO,
		.log.log       = acc_hal_integration_log
	};

	return hal;
}
