/*
 * bec.h - generator and parser of BEC message
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

/*! @file bec.h
 *  @brief Declares the structure and handling utilities for BEC sentence.
 *
 *  Contains declarations for structure, initilizer, generator and parser
 *  of BEC sentence.
 */

#ifndef INCLUDE_navi_bec_h
#define INCLUDE_navi_bec_h

#include "sentence.h"

/*! @brief BEC - Bearing and distance to waypoint - dead reckoning
 *
 * Time (UTC) and distance and bearing to, and location of,
 * a specified waypoint from the dead-reckoned present position.
 * $--BEC,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x.x,T,x.x,M,x.x,N,c--c*hh[cr][lf]
 */
struct bec_t
{
	struct navi_utc_t utc;				//!< UTC of observation
	struct navi_position_t waypointfix;	//!< Waypoint coordinates
	struct navi_offset_t bearingT;		//!< Bearing, degrees true
	struct navi_offset_t bearingM;		//!< Bearing, degrees magnetic
	double distance;					//!< Distance, nautical miles
	char waypoint_id[21];				//!< Waipoint ID
};

NAVI_BEGIN_DECL

//! @brief Initializes BEC sentence structure with default values
NAVI_EXTERN(navierr_status_t) navi_init_bec(struct bec_t *msg);

//! @brief Creates BEC message
NAVI_EXTERN(navierr_status_t) navi_create_bec(const struct bec_t *msg,
	char *buffer, size_t maxsize, size_t *nmwritten);

//! @brief Parses BEC message
NAVI_EXTERN(navierr_status_t) navi_parse_bec(struct bec_t *msg, char *buffer);

NAVI_END_DECL

#endif // INCLUDE_navi_bec_h
