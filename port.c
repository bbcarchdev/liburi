/* Author: Mo McRoberts <mo.mcroberts@bbc.co.uk>
 *
 * Copyright 2015-2017 BBC
 */

/*
 * Copyright 2012 Mo McRoberts.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_liburi.h"

/* 'port' property accessors */

size_t
uri_port(const URI *restrict uri, char *restrict buf, size_t buflen)
{
	if(!uri->portstr)
	{
		if(buf && buflen)
		{
			*buf = 0;
		}
		return 0;
	}
	if(buf && buflen)
	{
		strncpy(buf, uri->portstr, buflen - 1);
		buf[buflen - 1] = 0;
	}
	return strlen(uri->portstr) + 1;
}

/* Return the port as const string pointer */
const char *
uri_port_str(const URI *uri)
{
	return uri->portstr;
}

/* Return the port as a newly-allocated string (which must be freed by
 * the caller)
 */
char *
uri_port_stralloc(const URI *uri)
{
	if(!uri->portstr)
	{
		errno = 0;
		return NULL;
	}
	return strdup(uri->portstr);
}

/* Return the port as a parsed integer */
int
uri_portnum(const URI *uri)
{
	return (int) uri->port;
}

/* Set a new port (or remove it if newport is zero) */
int
uri_set_port(URI *restrict uri, int newport)
{
	char portbuf[16];
	char *sbuf;

	if(newport < 0 || newport > 65535)
	{
		errno = EINVAL;
		return -1;
	}
	if(newport)
	{
		snprintf(portbuf, sizeof(portbuf), "%d", newport);
		sbuf = strdup(portbuf);
		if(!sbuf)
		{
			return -1;
		}
	}
	else
	{
		sbuf = NULL;
	}
	free(uri->portstr);
	uri->portstr = sbuf;
	uri->uri.portText.first = sbuf;
	if(sbuf)
	{
		uri->uri.portText.afterLast = strchr(sbuf, 0);
	}
	else
	{
		uri->uri.portText.afterLast = NULL;
	}
	uri->port = (unsigned int) newport;
	return 0;
}
