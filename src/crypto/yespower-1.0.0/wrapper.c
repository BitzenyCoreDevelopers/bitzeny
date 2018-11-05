/**
 * These codes are licensed under CC0.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */

#include "yespower.h"

void yespower_hash(const void *input, void *output)
{
    yespower_params_t params = {
		.version = YESPOWER_0_5,
		.N = 2048,
		.r = 8,
		.pers = (const uint8_t *)"Client Key",
		.perslen = 10
	};

    yespower_tls(input, 80, &params, output);
}
