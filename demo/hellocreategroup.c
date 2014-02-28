/*
 * This file is part of buxton.
 *
 * Copyright (C) 2013 Intel Corporation
 *
 * buxton is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 */

#define _GNU_SOURCE
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buxton.h"

void create_cb(BuxtonResponse response, void *data)
{
	if (buxton_response_status(response) != BUXTON_STATUS_OK) {
		printf("Failed to create group\n");
	} else {
		printf("Created group\n");
	}
}

int main(void)
{
	BuxtonClient client;
	BuxtonKey key;
	struct pollfd pfd[1];
	int r;
	int fd;

	if ((fd = buxton_open(&client)) < 0) {
		printf("couldn't connect\n");
		return -1;
	}

	key = buxton_key_create("hello", NULL, "user", STRING);
	if (!key)
		return -1;

	if (!buxton_create_group(client, key, create_cb,
				     NULL, false)) {
		printf("create group call failed to run\n");
		return -1;
	}

	pfd[0].fd = fd;
	pfd[0].events = POLLIN;
	pfd[0].revents = 0;
	r = poll(pfd, 1, 5000);

	if (r <= 0) {
		printf("poll error\n");
		return -1;
	}

	if (!buxton_client_handle_response(client)) {
		printf("bad response from daemon\n");
		return -1;
	}

	buxton_key_free(key);
	buxton_close(client);
	return 0;
}

/*
 * Editor modelines  -	http://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: t
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 noexpandtab:
 * :indentSize=8:tabSize=8:noTabs=false:
 */
