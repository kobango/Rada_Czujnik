// Copyright (c) Acconeer AB, 2018-2019
// All rights reserved

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acc_hal_integration.h"
#include "acc_rss.h"
#include "acc_service_power_bins.h"
#include "acc_sweep_configuration.h"
#include "acc_version.h"

#include "example_power_bin_once.h"



/**
 * @brief Example that shows how to use power bin once
 *
 * This is an example on running power bin once on the radar with HAL.
 * The example executes as follows:
 *   - Activate Radar System Software (RSS) with HAL.
 *   - Creates a power bins configuration
 *   - Creates a power bins service using the previously created configuration
 *   - Gets the meta data from the power_bins_handle.
 *   - Runs power bins once and prints the radar data.
 *   - Deactivates and destroys the service
 *   - Deactivates Radar System Software
 */

#define SENSOR_ID             (1)
#define RANGE_START_M         (0.05f)
#define RANGE_LENGTH_M        (0.4f)
#define UPDATE_RATE_HZ        (0.5f)


static void configure_detector(acc_service_configuration_t distance_configuration)
{
	acc_sweep_configuration_t sweep_configuration;

	acc_service_power_bins_requested_bin_count_set(distance_configuration, 10);

	sweep_configuration = acc_service_get_sweep_configuration(distance_configuration);

	if (sweep_configuration == NULL)
	{
		printf("Sweep configuration not available\n");
	}
	else
	{
		acc_sweep_configuration_sensor_set(sweep_configuration, SENSOR_ID);
		acc_sweep_configuration_requested_start_set(sweep_configuration, RANGE_START_M);
		acc_sweep_configuration_requested_length_set(sweep_configuration, RANGE_LENGTH_M);
		acc_sweep_configuration_repetition_mode_streaming_set(sweep_configuration, UPDATE_RATE_HZ);
	}
}

int acc_example_power_bin_once(void)
{
	printf("Acconeer software version %s\n", ACC_VERSION);
	printf("Acconeer RSS version %s\n", acc_rss_version());

	//Initialize Radar Service System
	acc_hal_t hal = acc_hal_integration_get_implementation();

	if (!acc_rss_activate_with_hal(&hal))
	{
		printf("acc_rss_activate_with_hal() failed\n");
		return 0;
	}

	acc_service_configuration_t power_bins_configuration = acc_service_power_bins_configuration_create();

	if (power_bins_configuration == NULL)
	{
		printf("acc_service_power_bins_configuration_create() failed\n");
		acc_rss_deactivate();
		return 0;
	}

	configure_detector(power_bins_configuration);

	acc_service_handle_t power_bins_handle = acc_service_create(power_bins_configuration);

	if (power_bins_handle == NULL)
	{
		printf("acc_service_create failed\n");
		acc_service_power_bins_configuration_destroy(&power_bins_configuration);
		acc_rss_deactivate();
		return 0;
	}

	acc_service_status_t              status;
	acc_service_power_bins_metadata_t power_bins_metadata;

	acc_service_power_bins_get_metadata(power_bins_handle, &power_bins_metadata);
	int n_bins = power_bins_metadata.actual_bin_count;
	float actual_start_m = power_bins_metadata.actual_start_m;
	float actual_length_m = power_bins_metadata.actual_length_m;
	float actual_end_m    = power_bins_metadata.actual_start_m + power_bins_metadata.actual_length_m;

	float radar_data[5];

	status = acc_service_power_bins_execute_once(power_bins_handle, radar_data, n_bins, NULL);

	if (status == ACC_SERVICE_STATUS_OK)
	{
		printf("Number of bins: %u\n", n_bins);
		printf("Actual start: %u mm\n", (unsigned int)(actual_start_m * 1000.0f + 0.5f));
		printf("Actual length: %u mm\n", (unsigned int)(actual_length_m * 1000.0f + 0.5f));
		printf("Actual end: %u mm\n", (unsigned int)(actual_end_m * 1000.0f + 0.5f));
		for (int i = 0; i < n_bins; i++)
		{
			printf("radar_data[%d] = %u \n", i, (unsigned)radar_data[i]);
		}
		printf("\n");
		for (int i = 0; i < n_bins; i++)
		{
			for(int j = 0; j < radar_data[i]; j++)
			{
				printf("|");
			}
			printf("\n");
		}
	}
	else
	{
		printf("Execute was not successful!\n");
	}

	acc_service_power_bins_configuration_destroy(&power_bins_configuration);
	acc_service_deactivate(power_bins_handle);
	acc_service_destroy(&power_bins_handle);

	acc_rss_deactivate();

	return status == ACC_SERVICE_STATUS_OK;
}
