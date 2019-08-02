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

	float radar_data[n_bins];

	status = acc_service_power_bins_execute_once(power_bins_handle, radar_data, n_bins, NULL);

	if (status == ACC_SERVICE_STATUS_OK)
	{
		printf("Number of bins: %u\n", n_bins);
		for (int i = 0; i < n_bins; i++)
		{
			printf("radar_data[%d] = %u \n", i, (unsigned)radar_data[i]);
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
