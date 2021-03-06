/*
 * bww.c - generator and parser of BWW message
 *
 * Copyright (C) 2012 I. S. Gorbunov <igor.genius at gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libnavigate/bww.h>
#include <libnavigate/generator.h>
#include <libnavigate/parser.h>

#include <string.h>
#include <assert.h>
#include <stdio.h>

#ifdef _MSC_VER
	#include "win32/win32navi.h"
#endif // MSVC_VER

//
// Initializes BWW sentence structure with default values
navierr_status_t navi_init_bww(struct bww_t *msg)
{
	assert(msg != NULL);

	navi_init_offset(&msg->bearingT);
	navi_init_offset(&msg->bearingM);

	memset(msg->to_waypoint, 0, sizeof(msg->to_waypoint));
	memset(msg->from_waypoint, 0, sizeof(msg->from_waypoint));

	return navi_Ok;
}

//
// Creates BWW message
navierr_status_t navi_create_bww(const struct bww_t *msg, char *buffer,
	size_t maxsize, size_t *nmwritten)
{
	size_t msglength = 0;

	char local_str[NAVI_SENTENCE_MAXSIZE];

	assert(msg != NULL);
	assert(buffer != NULL);
	assert(maxsize > 0);

	msglength = navi_print_offset(&msg->bearingT, local_str + msglength,
		sizeof(local_str) - msglength);
	if (msglength + 2 > sizeof(local_str))
	{
		navierr_set_last(navi_MsgExceedsMaxSize);
		return navi_Error;
	}
	(void)strcat(local_str, ",");
	msglength += 1;

	msglength += navi_print_offset(&msg->bearingM, local_str + msglength,
		sizeof(local_str) - msglength);
	if (msglength + 2 > sizeof(local_str))
	{
		navierr_set_last(navi_MsgExceedsMaxSize);
		return navi_Error;
	}
	(void)strcat(local_str, ",");
	msglength += 1;

	if (navi_print_character_field(msg->to_waypoint, local_str + msglength,
		sizeof(local_str) - msglength) != navi_Ok)
	{
		navierr_set_last(navi_MsgExceedsMaxSize);
		return navi_Error;
	}
	msglength = strlen(local_str);
	(void)strcat(local_str, ",");
	msglength += 1;

	if (navi_print_character_field(msg->from_waypoint, local_str + msglength,
		sizeof(local_str) - msglength) != navi_Ok)
	{
		navierr_set_last(navi_MsgExceedsMaxSize);
		return navi_Error;
	}
	msglength = strlen(local_str);

	msglength = snprintf(buffer, maxsize, "%s", local_str);
	if (nmwritten != NULL)
		*nmwritten = msglength;

	return navi_Ok;
}

//
// Parses BWW message
navierr_status_t navi_parse_bww(struct bww_t *msg, char *buffer)
{
	size_t i = 0, nmread;

	if (navi_parse_offset(buffer + i, &msg->bearingT, &nmread) != navi_Ok)
	{
		if (navierr_get_last()->errclass != navi_NullField)
			return navi_Error;
	}
	i += nmread;

	if (navi_parse_offset(buffer + i, &msg->bearingM, &nmread) != navi_Ok)
	{
		if (navierr_get_last()->errclass != navi_NullField)
			return navi_Error;
	}
	i += nmread;

	if (navi_parse_character_field(buffer + i, msg->to_waypoint,
		sizeof(msg->to_waypoint), &nmread) != navi_Ok)
	{
		if (navierr_get_last()->errclass != navi_NullField)
			return navi_Error;
	}
	i += nmread;

	if (navi_parse_character_field(buffer + i, msg->from_waypoint,
		sizeof(msg->from_waypoint), &nmread) != navi_Ok)
	{
		if (navierr_get_last()->errclass != navi_NullField)
			return navi_Error;
	}

	return navi_Ok;
}
