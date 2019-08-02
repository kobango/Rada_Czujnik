// Copyright (c) Acconeer AB, 2018-2019
// All rights reserved

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acc_hal_integration.h"
#include "acc_rss.h"
#include "acc_service_envelope.h"
#include "acc_sweep_configuration.h"
#include "acc_version.h"

#include "newenv.h"



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
#define RANGE_START_M         (0.2f)
#define RANGE_LENGTH_M        (0.94f)
#define UPDATE_RATE_HZ        (0.5f)

static void configure_detector(acc_service_configuration_t distance_configuration)
{
	acc_sweep_configuration_t sweep_configuration;

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

int my_envelope(void)
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

	acc_service_configuration_t envelope_configuration = acc_service_envelope_configuration_create();

	if (envelope_configuration == NULL)
	{
		printf("acc_service_envelope_configuration_create() failed\n");
		acc_rss_deactivate();
		return 0;
	}

	configure_detector(envelope_configuration);

	acc_service_handle_t envelope_handle = acc_service_create(envelope_configuration);

	if (envelope_handle == NULL)
	{
		printf("acc_service_create failed\n");
		acc_service_envelope_configuration_destroy(&envelope_configuration);
		acc_rss_deactivate();
		return 0;
	}

	acc_service_status_t              service_status;
	acc_service_envelope_metadata_t envelope_metadata;
	acc_service_envelope_result_info_t result_info;

	acc_service_envelope_get_metadata(envelope_handle, &envelope_metadata);
	printf("Actual start: %d mm\n", (int)(envelope_metadata.actual_start_m * 1000.0f));
	printf("Actual length: %u mm\n", (unsigned int)(envelope_metadata.actual_length_m * 1000.0f));
	printf("Actual end: %d mm\n", (int)((envelope_metadata.actual_start_m + envelope_metadata.actual_length_m) * 1000.0f));
	printf("Data length: %u\n", (unsigned int)(envelope_metadata.data_length));

	uint16_t radar_data[2000];

	service_status = acc_service_activate(envelope_handle);

	if (service_status == ACC_SERVICE_STATUS_OK)
	{
		service_status = acc_service_envelope_get_next(envelope_handle, radar_data, envelope_metadata.data_length, &result_info);

		if (service_status == ACC_SERVICE_STATUS_OK)
			{
				printf("Envelope result_info.sequence_number: %u\n", (unsigned int)result_info.sequence_number);
				printf("Envelope data:\n");
				for (uint_fast16_t index = 0; index < envelope_metadata.data_length; index++)
				{
					if (index && !(index % 8))
					{
						printf("\n");
					}

					printf("%6u", (unsigned int)(radar_data[index]));
				}

				printf("\n");
			}
			else
			{
				printf("Execute was not successful!\n");
			}
	}



	acc_service_envelope_configuration_destroy(&envelope_configuration);
	acc_service_deactivate(envelope_handle);
	acc_service_destroy(&envelope_handle);

	acc_rss_deactivate();

	return service_status == ACC_SERVICE_STATUS_OK;
}
