/*
 * parser.c - IEC messages parsing utilities
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

#include <navigate.h>

#include <math.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "common.h"

#ifndef NO_PARSER

#include "dtm.h"
#include "gll.h"
#include "gns.h"
#include "rmc.h"
#include "vtg.h"
#include "zda.h"

#endif // NO_PARSER

//
// navi_parse_msg
//

int navi_parse_msg(char *buffer, int maxsize, int msgsize,
	void *msg, int *msgtype, int *nmread)
{

#ifndef NO_PARSER

	int tid;	// talker id

	int som;	// start of message index
	int eom;	// end of message index

	unsigned ucs, cs;

	//
	//	Determine the borders of message in buffer
	//

	// Skip up to beginning of the next message
	for (som = 0; buffer[som] != '$' && som < maxsize; som++);

	if (som >= maxsize)
	{	// No valid message
		navierr_set_last(navi_NoValidMessage);
		return navi_Error;
	}

	// Skip up to end of the message
	for (eom = som + 1; eom < maxsize; eom++)
	{
		if (buffer[eom - 1] == '\r' && buffer[eom] == '\n')
		{
			break;
		}
	}
	if (eom >= maxsize)
	{	// No valid message
		navierr_set_last(navi_NoValidMessage);
		return navi_Error;
	}

	// At least read a message
	*nmread = eom + 1;

	// Check that the message is not broken
	if (navi_checksum(buffer + som, maxsize - (som + eom), NULL, &ucs) < 0)
	{
		navierr_set_last(navi_InvalidMessage);
		return navi_Error;
	}

	cs = strtoul(buffer + eom - 3, NULL, 16);
	if (ucs != cs)
	{
		navierr_set_last(navi_CrcEror);
		return navi_Error;
	}

	// Determine the talker ID and message type
	if (navi_parse_address(buffer + som + 1, &tid, msgtype) < 0)
	{
		return navi_Error;
	}

	// Parse the message fields
	switch (*msgtype)
	{
	case navi_AAM:
	case navi_ACK:
	case navi_ALM:
	case navi_ALR:
	case navi_APB:
	case navi_BEC:
	case navi_BOD:
	case navi_BWC:
	case navi_BWR:
	case navi_BWW:
	case navi_DBT:
	case navi_DCN:
	case navi_DPT:
	case navi_DSC:
	case navi_DSE:
	case navi_DSI:
	case navi_DSR:
		break;
	case navi_DTM:
		if (msgsize < sizeof(struct dtm_t))
		{
			navierr_set_last(navi_NotEnoughBuffer);
			return navi_Error;
		}
		((struct dtm_t *)msg)->tid = tid;
		return navi_parse_dtm((struct dtm_t *)msg, buffer + som + 7);
	case navi_FSI:
	case navi_GBS:
	case navi_GGA:
	case navi_GLC:
		break;
	case navi_GLL:
		if (msgsize < sizeof(struct gll_t))
		{
			navierr_set_last(navi_NotEnoughBuffer);
			return navi_Error;
		}
		((struct gll_t *)msg)->tid = tid;
		return navi_parse_gll((struct gll_t *)msg, buffer + som + 7);
	case navi_GNS:
		if (msgsize < sizeof(struct gns_t))
		{
			navierr_set_last(navi_NotEnoughBuffer);
			return navi_Error;
		}
		((struct gns_t *)msg)->tid = tid;
		return navi_parse_gns((struct gns_t *)msg, buffer + som + 7);
	case navi_GRS:
	case navi_GSA:
	case navi_GST:
	case navi_GSV:
	case navi_HDG:
	case navi_HDT:
	case navi_HMR:
	case navi_HMS:
	case navi_HSC:
	case navi_HTC:
	case navi_HTD:
	case navi_LCD:
	case navi_MLA:
	case navi_MSK:
	case navi_MSS:
	case navi_MTW:
	case navi_MWD:
	case navi_MWV:
	case navi_OSD:
	case navi_RMA:
	case navi_RMB:
		break;
	case navi_RMC:
		if (msgsize < sizeof(struct rmc_t))
		{
			navierr_set_last(navi_NotEnoughBuffer);
			return navi_Error;
		}
		((struct rmc_t *)msg)->tid = tid;
		return navi_parse_rmc((struct rmc_t *)msg, buffer + som + 7);
	case navi_ROT:
	case navi_RPM:
	case navi_RSA:
	case navi_RSD:
	case navi_RTE:
	case navi_SFI:
	case navi_STN:
	case navi_TLB:
	case navi_TLL:
	case navi_TTM:
	case navi_TXT:
	case navi_VBW:
	case navi_VDR:
	case navi_VHW:
	case navi_VLW:
	case navi_VPW:
		break;
	case navi_VTG:
		if (msgsize < sizeof(struct vtg_t))
		{
			navierr_set_last(navi_NotEnoughBuffer);
			return navi_Error;
		}
		((struct vtg_t *)msg)->tid = tid;
		return navi_parse_vtg((struct vtg_t *)msg, buffer + som + 7);
	case navi_WCV:
	case navi_WNC:
	case navi_WPL:
	case navi_XDR:
	case navi_XTE:
	case navi_XTR:
		break;
	case navi_ZDA:
		if (msgsize < sizeof(struct zda_t))
		{
			navierr_set_last(navi_NotEnoughBuffer);
			return navi_Error;
		}
		((struct zda_t *)msg)->tid = tid;
		return navi_parse_zda((struct zda_t *)msg, buffer + som + 7);
	case navi_ZDL:
	case navi_ZFO:
	case navi_ZTG:
		break;
	default:
		break;
	}

	navierr_set_last(navi_MsgNotSupported);

#else

	navierr_set_last(navi_NotImplemented);

#endif // NO_PARSER

	return navi_Error;
}

//
// navi_parse_offset
//

#define PARSE_OFFSET_INIT		0
#define PARSE_OFFSET_INTEGRAL	1
#define PARSE_OFFSET_FRACTION	2
#define PARSE_OFFSET_SIGN		3
#define PARSE_OFFSET_FINI		4

int navi_parse_offset(char *buffer, struct navi_offset_t *offset,
		int *nmread)
{

#ifndef NO_PARSER

	double t;
	int i = 0, j = -1, state, c, s = 0, error = 0;

	assert(buffer != NULL);
	assert(offset != NULL);
	assert(nmread != NULL);

	t = 0.;
	state = PARSE_OFFSET_INIT;

	for ( ; ; )
	{
		c = buffer[i++];

		switch (state)
		{
		case PARSE_OFFSET_INIT:
			if (isdigit(c))
			{
				state = PARSE_OFFSET_INTEGRAL;
				t = t * 10. + (c - '0');
			}
			else if (c == '.')
			{
				state = PARSE_OFFSET_FRACTION;
			}
			else if (c == ',')
			{	// null field
				state = PARSE_OFFSET_FINI;
				error = navi_NullField;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_OFFSET_INTEGRAL:
			if (isdigit(c))
			{
				t = t * 10. + (c - '0');
			}
			else if (c == '.')
			{
				state = PARSE_OFFSET_FRACTION;
			}
			else if (c == ',')
			{
				state = PARSE_OFFSET_SIGN;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_OFFSET_FRACTION:
			if (isdigit(c))
			{
				t = t + pow(10., j--) * (c - '0');
			}
			else if (c == ',')
			{
				state = PARSE_OFFSET_SIGN;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_OFFSET_SIGN:
			if (c == 'N')
			{
				s = navi_North;
			}
			else if (c == 'S')
			{
				s = navi_South;
			}
			else if (c == 'E')
			{
				s = navi_East;
			}
			else if (c == 'W')
			{
				s = navi_West;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			state = PARSE_OFFSET_FINI;
			break;
		case PARSE_OFFSET_FINI:
			if (c != ',' && c != '*')
				error = navi_InvalidMessage;
			goto _Exit;
		}
	}

_Exit:

	*nmread = i;

	if (error)
	{
		navierr_set_last(error);
		return navi_Error;
	}
	else
	{
		offset->offset = t;
		offset->sign = s;
	}

	return navi_Ok;

#else

	navierr_set_last(navi_NotImplemented);
	return -1;

#endif // NO_PARSER

}

#undef PARSE_OFFSET_INIT
#undef PARSE_OFFSET_INTEGRAL
#undef PARSE_OFFSET_FRACTION
#undef PARSE_OFFSET_SIGN
#undef PARSE_OFFSET_FINI

//
// navi_parse_position_fix
//

#define PARSE_POSITION_INIT				0
#define PARSE_POSITION_LAT_INTEGRAL		1
#define PARSE_POSITION_LAT_FRACTION		2
#define PARSE_POSITION_LAT_SIGN			3
#define PARSE_POSITION_LON_INTEGRAL		4
#define PARSE_POSITION_LON_FRACTION		5
#define PARSE_POSITION_LON_SIGN			6
#define PARSE_POSITION_NULLFIELD		7
#define PARSE_POSITION_FINI				8

int navi_parse_position_fix(char *buffer, struct navi_position_t *fix,
	int *nmread)
{

#ifndef NO_PARSER

	int state, i = 0, j = -1, k, c, error = 0;
	double deg, min;

	assert(buffer != NULL);
	assert(fix != NULL);
	assert(nmread != NULL);

	state = PARSE_POSITION_INIT;
	deg = min = 0.;

	for ( ; ; )
	{
		c = buffer[i++];

		switch (state)
		{
		case PARSE_POSITION_INIT:
			if (c == ',')
			{	// check for null field
				state = PARSE_POSITION_NULLFIELD;
				j = 1;
			}
			else if (isdigit(c))
			{	// proceed latitude
				state = PARSE_POSITION_LAT_INTEGRAL;
				deg = c - '0';
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_POSITION_LAT_INTEGRAL:
			// parse second digit of degrees
			if (isdigit(c))
			{
				deg = deg * 10. + (c - '0');
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			// parse two integral digits of minutes
			for (k = 0; k < 2; k++, i++)
			{
				c = buffer[i];
				if (isdigit(c))
				{
					min = min * 10. + (c - '0');
				}
				else
				{
					error = navi_InvalidMessage;
					goto _Exit;
				}
			}
			// check if there is fractional part of latitude minutes
			c = buffer[i++];
			if (c == '.')
			{	// yes, there is
				state = PARSE_POSITION_LAT_FRACTION;
				j = -1;
			}
			else if (c == ',')
			{	// no, proceed to the latitude sign
				state = PARSE_POSITION_LAT_SIGN;
				fix->latitude = deg + min / 60.;
			}
			else
			{	// invalid character
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_POSITION_LAT_FRACTION:
			if (isdigit(c))
			{
				min = min + pow(10., j--) * (c - '0');
			}
			else if (c == ',')
			{
				state = PARSE_POSITION_LAT_SIGN;
				fix->latitude = deg + min / 60.;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_POSITION_LAT_SIGN:
			if (c == 'N')
			{
				fix->latsign = navi_North;
			}
			else if (c == 'S')
			{
				fix->latsign = navi_South;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			// check if this is the only character
			c = buffer[i++];
			if (c == ',')
			{
				state = PARSE_POSITION_LON_INTEGRAL;
				min = 0.;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_POSITION_LON_INTEGRAL:
			// parse three digits of degrees
			deg = c - '0';
			for (k = 0; k < 2; k++, i++)
			{
				c = buffer[i];
				if (isdigit(c))
				{
					deg = deg * 10. + (c - '0');
				}
				else
				{
					error = navi_InvalidMessage;
					goto _Exit;
				}
			}
			// parse two integral digits of minutes
			for (k = 0; k < 2; k++, i++)
			{
				c = buffer[i];
				if (isdigit(c))
				{
					min = min * 10. + (c - '0');
				}
				else
				{
					error = navi_InvalidMessage;
					goto _Exit;
				}
			}
			// check if there is fractional part of longitude minutes
			c = buffer[i++];
			if (c == '.')
			{	// yes, there is
				state = PARSE_POSITION_LON_FRACTION;
				j = -1;
			}
			else if (c == ',')
			{	// no, proceed to the longitude sign
				state = PARSE_POSITION_LON_SIGN;
				fix->longitude = deg + min / 60.;
			}
			else
			{	// invalid character
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_POSITION_LON_FRACTION:
			if (isdigit(c))
			{
				min = min + pow(10., j--) * (c - '0');
			}
			else if (c == ',')
			{
				state = PARSE_POSITION_LON_SIGN;
				fix->longitude = deg + min / 60.;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_POSITION_LON_SIGN:
			if (c == 'E')
			{
				fix->lonsign = navi_East;
			}
			else if (c == 'W')
			{
				fix->lonsign = navi_West;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			state = PARSE_POSITION_FINI;
			break;
		case PARSE_POSITION_NULLFIELD:
			if (j < 2)
			{
				if (c == ',')
				{
					j++;
				}
				else
				{
					error = navi_InvalidMessage;
					goto _Exit;
				}
			}
			else
			{	// indeed null field
				state = PARSE_POSITION_FINI;
				error = navi_NullField;
			}
			break;
		case PARSE_POSITION_FINI:
			if (c != ',' && c != '*')
				error = navi_InvalidMessage;
			goto _Exit;
		}
	}

_Exit:

	*nmread = i;

	if (error)
	{
		navierr_set_last(error);
		return navi_Error;
	}

	return navi_Ok;

#else

	navierr_set_last(navi_NotImplemented);
	return -1;

#endif // NO_PARSER

}

#undef PARSE_POSITION_INIT
#undef PARSE_POSITION_LAT_INTEGRAL
#undef PARSE_POSITION_LAT_FRACTION
#undef PARSE_POSITION_LAT_SIGN
#undef PARSE_POSITION_LON_INTEGRAL
#undef PARSE_POSITION_LON_FRACTION
#undef PARSE_POSITION_LON_SIGN
#undef PARSE_POSITION_NULLFIELD
#undef PARSE_POSITION_FINI

//
// navi_parse_utc
//

#define PARSE_UTC_INIT			0
#define PARSE_UTC_INTEGRAL		1
#define PARSE_UTC_FRACTION		2

int navi_parse_utc(char *buffer, struct navi_utc_t *utc, int *nmread)
{

#ifndef NO_PARSER

	int state, i = 0, j = -1, k, c, error = 0;
	int t = 0;
	double d = 0.;

	assert(buffer != NULL);
	assert(utc != NULL);
	assert(nmread != NULL);

	state = PARSE_UTC_INIT;

	for ( ; ; )
	{
		c = buffer[i++];

		switch (state)
		{
		case PARSE_UTC_INIT:
			if ((c == ',') || (c == '*'))
			{	// null field
				error = navi_NullField;
				goto _Exit;
			}
			else if (isdigit(c))
			{	// proceed utc time
				state = PARSE_UTC_INTEGRAL;
				t = c - '0';
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_UTC_INTEGRAL:
			// parse second digit of hours
			if (isdigit(c))
			{
				t = t * 10 + (c - '0');
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			utc->hour = t;
			t = 0;
			// parse two digits of minutes
			for (k = 0; k < 2; k++, i++)
			{
				c = buffer[i];
				if (isdigit(c))
				{
					t = t * 10 + (c - '0');
				}
				else
				{
					error = navi_InvalidMessage;
					goto _Exit;
				}
			}
			utc->min = t;
			// parse two digits of seconds
			for (k = 0; k < 2; k++, i++)
			{
				c = buffer[i];
				if (isdigit(c))
				{
					d = d * 10. + (c - '0');
				}
				else
				{
					error = navi_InvalidMessage;
					goto _Exit;
				}
			}
			utc->sec = d;
			// check if there is fractional part of seconds
			c = buffer[i++];
			if (c == '.')
			{	// yes, there is
				state = PARSE_UTC_FRACTION;
				j = -1;
			}
			else if ((c == ',') || (c == '*'))
			{	// no, finish the parsing
				goto _Exit;
			}
			else
			{	// invalid character
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_UTC_FRACTION:
			if (isdigit(c))
			{
				d = d + pow(10., j--) * (c - '0');
			}
			else if ((c == ',') || (c == '*'))
			{
				utc->sec = d;
				goto _Exit;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		}
	}

_Exit:

	*nmread = i;

	if (error)
	{
		navierr_set_last(error);
		return navi_Error;
	}

	return navi_Ok;

#else

	navierr_set_last(navi_NotImplemented);
	return -1;

#endif // NO_PARSER

}

#undef PARSE_UTC_INIT
#undef PARSE_UTC_INTEGRAL
#undef PARSE_UTC_FRACTION


//
// navi_parse_number
//

#define PARSE_NUMBER_INIT			0
#define PARSE_NUMBER_INTEGRAL		1
#define PARSE_NUMBER_FRACTION		2

int navi_parse_number(char *buffer, double *parsed, int *nmread)
{

#ifndef NO_PARSER

	int state = PARSE_NUMBER_INIT, i = 0, j = -1,
		c, s = 1, error = 0;
	double d = 0.;

	assert(buffer != NULL);
	assert(parsed != NULL);
	assert(nmread != NULL);

	for ( ; ; )
	{
		c = buffer[i++];

		switch (state)
		{
		case PARSE_NUMBER_INIT:
			if (isdigit(c))
			{
				state = PARSE_NUMBER_INTEGRAL;
				d = c - '0';
			}
			else if (c == '+')
			{	// 'plus' sign
				state = PARSE_NUMBER_INTEGRAL;
			}
			else if (c == '-')
			{	// 'minus' sign
				s = -1;
				state = PARSE_NUMBER_INTEGRAL;
			}
			else if (c == '.')
			{
				state = PARSE_NUMBER_FRACTION;
			}
			else if ((c == ',') || (c == '*'))
			{	// null field
				error = navi_NullField;
				goto _Exit;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_NUMBER_INTEGRAL:
			if (isdigit(c))
			{
				d = d * 10. + (c - '0');
			}
			else if (c == '.')
			{
				state = PARSE_NUMBER_FRACTION;
			}
			else if ((c == ',') || (c == '*'))
			{	// field ends
				goto _Exit;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		case PARSE_NUMBER_FRACTION:
			if (isdigit(c))
			{
				d = d + pow(10., j--) * (c - '0');
			}
			else if ((c == ',') || (c == '*'))
			{	// field ends
				goto _Exit;
			}
			else
			{
				error = navi_InvalidMessage;
				goto _Exit;
			}
			break;
		}
	}

_Exit:

	*nmread = i;

	if (error)
	{
		navierr_set_last(error);
		return navi_Error;
	}

	*parsed = s * d;
	return navi_Ok;

#else

	navierr_set_last(navi_NotImplemented);
	return -1;

#endif // NO_PARSER

}

#undef PARSE_NUMBER_INIT
#undef PARSE_NUMBER_INTEGRAL
#undef PARSE_NUMBER_FRACTION

//
// navi_parse_datum
//
int navi_parse_datum(char *buffer, int *datum, int *nmread)
{
	int error = 0;

	if (strncmp("W84", buffer, 3) == 0)
		*datum = navi_WGS84;
	else if (strncmp("W72", buffer, 3) == 0)
		*datum = navi_WGS72;
	else if (strncmp("S85", buffer, 3) == 0)
		*datum = navi_SGS85;
	else if (strncmp("P90", buffer, 3) == 0)
		*datum = navi_PE90;
	else if (strncmp("999", buffer, 3) == 0)
		*datum = navi_UserDefined;
	else if ((strncmp(",", buffer, 1) == 0) ||
		(strncmp("*", buffer, 1) == 0))
	{
		*nmread = 1;
		navierr_set_last(navi_NullField);
		error = -1;
	}
	else
	{
		*nmread = 1;
		navierr_set_last(navi_MsgNotSupported);
		error = -1;
	}

	if ((error == 0) && ((strncmp(",", buffer + 3, 1) == 0) ||
		(strncmp("*", buffer + 3, 1) == 0)))
	{
		*nmread = 4;
		return 0;
	}
	else if (error == 0)
	{
		*nmread = 4;
		navierr_set_last(navi_MsgNotSupported);
		return -1;
	}
	else
	{
		return -1;
	}
}

//
// navi_parse_datumsub
//
int navi_parse_datumsub(char *buffer, int *datumsub, int *nmread)
{
	if (strncmp(",", buffer, 1) == 0)
	{
		*nmread = 1;
		*datumsub = navi_Null;

		navierr_set_last(navi_NullField);
		return navi_Error;
	}
	else
	{
		*nmread = 1;

		navierr_set_last(navi_MsgNotSupported);
		return navi_Error;
	}
}

//
// Parses mode navi_parse_miarray array
//
int navi_parse_miarray(char *buffer, int mi[], int *misize, int *nmread)
{
	int i = 0, c, error = 0;

	assert(misize != NULL);
	assert(*misize >= 2);

	*misize = 0;

	for (i = 0; ; i++)
	{
		c = buffer[i];

		switch (c)
		{
		case 'A':
			mi[i] = navi_Autonomous;
			(*misize)++;
			break;
		case 'D':
			mi[i] = navi_Differential;
			(*misize)++;
			break;
		case 'E':
			mi[i] = navi_Estimated;
			(*misize)++;
			break;
		case 'M':
			mi[i] = navi_ManualInput;
			(*misize)++;
			break;
		case 'S':
			mi[i] = navi_Simulator;
			(*misize)++;
			break;
		case 'N':
			mi[i] = navi_DataNotValid;
			(*misize)++;
			break;
		case 'P':
			mi[i] = navi_Precise;
			(*misize)++;
			break;
		case 'R':
			mi[i] = navi_RTKinematic;
			(*misize)++;
			break;
		case 'F':
			mi[i] = navi_FloatRTK;
			(*misize)++;
			break;
		case ',': case '*':
			goto _Exit;
		default:
			error = 1;
			goto _Exit;
		}
	}

_Exit:

	*nmread = i + 1;
	if ((i < 2) || (error != 0))
	{
		navierr_set_last(navi_InvalidMessage);
		return navi_Error;
	}

	return navi_Ok;
}

//
// navi_parse_date
//
int navi_parse_date(char *buffer, struct navi_date_t *date, int *nmread)
{
	int i, c;

	assert(date != NULL);
	assert(nmread != NULL);

	for (i = 0; ; i++)
	{
		c = buffer[i];

		if (isdigit(c))
		{
			if (i == 0)
				date->day = c - '0';
			else if (i == 1)
				date->day = date->day * 10 + (c - '0');
			else if (i == 2)
				date->month = c - '0';
			else if (i == 3)
				date->month = date->month * 10 + (c - '0');
			else if (i == 4)
				date->year = c - '0';
			else if ((i >= 5) && (i <= 7))
				date->year = date->year * 10 + (c - '0');
			else
				break;
		}
		else if ((c == ',') || (c == '*'))
		{
			break;
		}
		else
		{
			*nmread = i;
			return navi_InvalidMessage;
		}
	}

	*nmread = i + 1;

	if (i == 0)
	{
		navierr_set_last(navi_NullField);
		return -1;
	}
	else if ((i < 6) || (i >= 8))
	{
		navierr_set_last(navi_InvalidMessage);
		return -1;
	}

	return navi_Ok;
}

//
// navi_parse_localzone
//

#define PARSE_LOCALZONE_INIT		0
#define PARSE_LOCALZONE_HOURS		1
#define PARSE_LOCALZONE_MINUTES		2
#define PARSE_LOCALZONE_FINI		3

int navi_parse_localzone(char *buffer, int *offset, int *nmread)
{
	int state = PARSE_LOCALZONE_INIT, i = 0, error = 0;
	int c, s = 1, h = 0, m = 0;

	for ( ; ; )
	{
		c = buffer[i++];

		switch (state)
		{
		case PARSE_LOCALZONE_INIT:
			if (isdigit(c))
			{
				state = PARSE_LOCALZONE_HOURS;
				h = c - '0';
			}
			else if (c == ',')
			{	// null field
				state = PARSE_LOCALZONE_FINI;
				navierr_set_last(navi_NullField);
				error = 1;
			}
			else if (c == '-')
			{
				state = PARSE_LOCALZONE_HOURS;
				s = -1;
			}
			else if (c == '+')
			{
				state = PARSE_LOCALZONE_HOURS;
			}
			else
			{
				error = 1;
				goto _Exit;
			}
			break;
		case PARSE_LOCALZONE_HOURS:
			if (isdigit(c))
			{
				h = h * 10 + c - '0';
			}
			else if (c == ',')
			{	// next field
				state = PARSE_LOCALZONE_MINUTES;
			}
			else
			{
				error = 1;
				goto _Exit;
			}
			break;
		case PARSE_LOCALZONE_MINUTES:
			if (isdigit(c))
			{
				m = m * 10 + c - '0';
			}
			else if ((c == ',') || (c == '*'))
			{
				goto _Exit;
			}
			else
			{
				error = 1;
				goto _Exit;
			}
			break;
		case PARSE_LOCALZONE_FINI:
			if ((c != ',') && (c != '*'))
			{
				navierr_set_last(navi_InvalidMessage);
			}
			goto _Exit;
		}
	}

_Exit:
	*offset = s * (h * 60 + m);
	*nmread = i + 1;

	if (error)
	{
		return navi_Error;
	}

	return navi_Ok;
}

#undef PARSE_LOCALZONE_INIT
#undef PARSE_LOCALZONE_SIGN
#undef PARSE_LOCALZONE_HOURS
#undef PARSE_LOCALZONE_MINUTES
#undef PARSE_LOCALZONE_FINI

// Talker identifier and sentence formatter
int navi_parse_address(char *buffer, int *tid, int *msgtype)
{
	int result, nmread;

	*tid = navi_parse_talkerid(buffer, &nmread);
	result = nmread;
	*msgtype = navi_parse_sentencefmt(buffer + result, &nmread);
	result += nmread;

	return result;
}

// Looks up Talker ID
int navi_parse_talkerid(char *buffer, int *nmread)
{
	*nmread = 2;

	if (strncmp("AG", buffer, 2) == 0)
		return navi_AG;
	else if (strncmp("AP", buffer, 2) == 0)
		return navi_AP;
	else if (strncmp("AI", buffer, 2) == 0)
		return navi_AI;
	else if (strncmp("CD", buffer, 2) == 0)
		return navi_CD;
	else if (strncmp("CR", buffer, 2) == 0)
		return navi_CR;
	else if (strncmp("CS", buffer, 2) == 0)
		return navi_CS;
	else if (strncmp("CT", buffer, 2) == 0)
		return navi_CT;
	else if (strncmp("CV", buffer, 2) == 0)
		return navi_CV;
	else if (strncmp("CX", buffer, 2) == 0)
		return navi_CX;
	else if (strncmp("DE", buffer, 2) == 0)
		return navi_DE;
	else if (strncmp("DF", buffer, 2) == 0)
		return navi_DF;
	else if (strncmp("EC", buffer, 2) == 0)
		return navi_EC;
	else if (strncmp("EI", buffer, 2) == 0)
		return navi_EI;
	else if (strncmp("EP", buffer, 2) == 0)
		return navi_EP;
	else if (strncmp("ER", buffer, 2) == 0)
		return navi_ER;
	else if (strncmp("GA", buffer, 2) == 0)
		return navi_GA;
	else if (strncmp("GP", buffer, 2) == 0)
		return navi_GP;
	else if (strncmp("GL", buffer, 2) == 0)
		return navi_GL;
	else if (strncmp("GN", buffer, 2) == 0)
		return navi_GN;
	else if (strncmp("GW", buffer, 2) == 0)
		return navi_GW;
	else if (strncmp("HC", buffer, 2) == 0)
		return navi_HC;
	else if (strncmp("HE", buffer, 2) == 0)
		return navi_HE;
	else if (strncmp("HN", buffer, 2) == 0)
		return navi_HN;
	else if (strncmp("II", buffer, 2) == 0)
		return navi_II;
	else if (strncmp("IN", buffer, 2) == 0)
		return navi_IN;
	else if (strncmp("LC", buffer, 2) == 0)
		return navi_LC;
	else if (strncmp("P", buffer, 1) == 0)
	{
		*nmread = 1;
		return navi_P;
	}
	else if (strncmp("RA", buffer, 2) == 0)
		return navi_RA;
	else if (strncmp("SD", buffer, 2) == 0)
		return navi_SD;
	else if (strncmp("SN", buffer, 2) == 0)
		return navi_SN;
	else if (strncmp("SS", buffer, 2) == 0)
		return navi_SS;
	else if (strncmp("TI", buffer, 2) == 0)
		return navi_TI;
	else if (strncmp("VD", buffer, 2) == 0)
		return navi_VD;
	else if (strncmp("VM", buffer, 2) == 0)
		return navi_VM;
	else if (strncmp("VW", buffer, 2) == 0)
		return navi_VW;
	else if (strncmp("VR", buffer, 2) == 0)
		return navi_VR;
	else if (strncmp("YX", buffer, 2) == 0)
		return navi_YX;
	else if (strncmp("ZA", buffer, 2) == 0)
		return navi_ZA;
	else if (strncmp("ZC", buffer, 2) == 0)
		return navi_ZC;
	else if (strncmp("ZQ", buffer, 2) == 0)
		return navi_ZQ;
	else if (strncmp("ZV", buffer, 2) == 0)
		return navi_ZV;
	else if (strncmp("WI", buffer, 2) == 0)
		return navi_WI;
	else
		return -1;
}

// Looks up sentence formatter
int navi_parse_sentencefmt(char *buffer, int *nmread)
{
	*nmread = 3;

	if (strncmp("AAM", buffer, 3) == 0)
		return navi_AAM;
	else if (strncmp("ACK", buffer, 3) == 0)
		return navi_ACK;
	else if (strncmp("ALM", buffer, 3) == 0)
		return navi_ALM;
	else if (strncmp("ALR", buffer, 3) == 0)
		return navi_ALR;
	else if (strncmp("APB", buffer, 3) == 0)
		return navi_APB;
	else if (strncmp("BEC", buffer, 3) == 0)
		return navi_BEC;
	else if (strncmp("BOD", buffer, 3) == 0)
		return navi_BOD;
	else if (strncmp("BWC", buffer, 3) == 0)
		return navi_BWC;
	else if (strncmp("BWR", buffer, 3) == 0)
		return navi_BWR;
	else if (strncmp("BWW", buffer, 3) == 0)
		return navi_BWW;
	else if (strncmp("DBT", buffer, 3) == 0)
		return navi_DBT;
	else if (strncmp("DCN", buffer, 3) == 0)
		return navi_DCN;
	else if (strncmp("DPT", buffer, 3) == 0)
		return navi_DPT;
	else if (strncmp("DSC", buffer, 3) == 0)
		return navi_DSC;
	else if (strncmp("DSE", buffer, 3) == 0)
		return navi_DSE;
	else if (strncmp("DSI", buffer, 3) == 0)
		return navi_DSI;
	else if (strncmp("DSR", buffer, 3) == 0)
		return navi_DSR;
	else if (strncmp("DTM", buffer, 3) == 0)
		return navi_DTM;
	else if (strncmp("FSI", buffer, 3) == 0)
		return navi_FSI;
	else if (strncmp("GBS", buffer, 3) == 0)
		return navi_GBS;
	else if (strncmp("GGA", buffer, 3) == 0)
		return navi_GGA;
	else if (strncmp("GLC", buffer, 3) == 0)
		return navi_GLC;
	else if (strncmp("GLL", buffer, 3) == 0)
		return navi_GLL;
	else if (strncmp("GNS", buffer, 3) == 0)
		return navi_GNS;
	else if (strncmp("GRS", buffer, 3) == 0)
		return navi_GRS;
	else if (strncmp("GSA", buffer, 3) == 0)
		return navi_GSA;
	else if (strncmp("GST", buffer, 3) == 0)
		return navi_GST;
	else if (strncmp("GSV", buffer, 3) == 0)
		return navi_GSV;
	else if (strncmp("HDG", buffer, 3) == 0)
		return navi_HDG;
	else if (strncmp("HDT", buffer, 3) == 0)
		return navi_HDT;
	else if (strncmp("HMR", buffer, 3) == 0)
		return navi_HMR;
	else if (strncmp("HMS", buffer, 3) == 0)
		return navi_HMS;
	else if (strncmp("HSC", buffer, 3) == 0)
		return navi_HSC;
	else if (strncmp("HTC", buffer, 3) == 0)
		return navi_HTC;
	else if (strncmp("HTD", buffer, 3) == 0)
		return navi_HTD;
	else if (strncmp("LCD", buffer, 3) == 0)
		return navi_LCD;
	else if (strncmp("MLA", buffer, 3) == 0)
		return navi_MLA;
	else if (strncmp("MSK", buffer, 3) == 0)
		return navi_MSK;
	else if (strncmp("MSS", buffer, 3) == 0)
		return navi_MSS;
	else if (strncmp("MTW", buffer, 3) == 0)
		return navi_MTW;
	else if (strncmp("MWD", buffer, 3) == 0)
		return navi_MWD;
	else if (strncmp("MWV", buffer, 3) == 0)
		return navi_MWV;
	else if (strncmp("OSD", buffer, 3) == 0)
		return navi_OSD;
	else if (strncmp("RMA", buffer, 3) == 0)
		return navi_RMA;
	else if (strncmp("RMB", buffer, 3) == 0)
		return navi_RMB;
	else if (strncmp("RMC", buffer, 3) == 0)
		return navi_RMC;
	else if (strncmp("ROT", buffer, 3) == 0)
		return navi_ROT;
	else if (strncmp("RPM", buffer, 3) == 0)
		return navi_RPM;
	else if (strncmp("RSA", buffer, 3) == 0)
		return navi_RSA;
	else if (strncmp("RSD", buffer, 3) == 0)
		return navi_RSD;
	else if (strncmp("RTE", buffer, 3) == 0)
		return navi_RTE;
	else if (strncmp("SFI", buffer, 3) == 0)
		return navi_SFI;
	else if (strncmp("STN", buffer, 3) == 0)
		return navi_STN;
	else if (strncmp("TLB", buffer, 3) == 0)
		return navi_TLB;
	else if (strncmp("TLL", buffer, 3) == 0)
		return navi_TLL;
	else if (strncmp("TTM", buffer, 3) == 0)
		return navi_TTM;
	else if (strncmp("TXT", buffer, 3) == 0)
		return navi_TXT;
	else if (strncmp("VBW", buffer, 3) == 0)
		return navi_VBW;
	else if (strncmp("VDR", buffer, 3) == 0)
		return navi_VDR;
	else if (strncmp("VHW", buffer, 3) == 0)
		return navi_VHW;
	else if (strncmp("VLW", buffer, 3) == 0)
		return navi_VLW;
	else if (strncmp("VPW", buffer, 3) == 0)
		return navi_VPW;
	else if (strncmp("VTG", buffer, 3) == 0)
		return navi_VTG;
	else if (strncmp("WCV", buffer, 3) == 0)
		return navi_WCV;
	else if (strncmp("WNC", buffer, 3) == 0)
		return navi_WNC;
	else if (strncmp("WPL", buffer, 3) == 0)
		return navi_WPL;
	else if (strncmp("XDR", buffer, 3) == 0)
		return navi_XDR;
	else if (strncmp("XTE", buffer, 3) == 0)
		return navi_XTE;
	else if (strncmp("XTR", buffer, 3) == 0)
		return navi_XTR;
	else if (strncmp("ZDA", buffer, 3) == 0)
		return navi_ZDA;
	else if (strncmp("ZDL", buffer, 3) == 0)
		return navi_ZDL;
	else if (strncmp("ZFO", buffer, 3) == 0)
		return navi_ZFO;
	else if (strncmp("ZTG", buffer, 3) == 0)
		return navi_ZTG;
	else
		return navi_Error;
}
