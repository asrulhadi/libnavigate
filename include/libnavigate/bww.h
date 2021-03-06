/*
 * bww.h - generator and parser of BWW message
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

/*! @file bww.h
 *  @brief Declares the structure and handling utilities for BWW sentence.
 *
 *  Contains declarations for structure, initilizer, generator and parser
 *  of BWW sentence.
 */

#ifndef INCLUDE_navi_bww_h
#define INCLUDE_navi_bww_h

#include "sentence.h"

/*! @brief BWW - Bearing waypoint to waypoint
 *
 *  Bearing angle of the line, between the TO and the FROM waypoints,
 *  calculated at the FROM waypoint for any two arbitrary waypoints.
 *  $--BWW,x.x,T,x.x,M,c--c,c--c*hh[cr][lf]
 */
struct bww_t
{
	struct navi_offset_t bearingT;	//!< Bearing, degrees true
	struct navi_offset_t bearingM;	//!< Bearing, degrees magnetic
	char to_waypoint[21];			//!< TO waypoint ID
	char from_waypoint[21];			//!< FROM waypoint ID
};

NAVI_BEGIN_DECL

//! @brief Initializes BWW sentence structure with default values
NAVI_EXTERN(navierr_status_t) navi_init_bww(struct bww_t *msg);

//! @brief Creates BWW message
NAVI_EXTERN(navierr_status_t) navi_create_bww(const struct bww_t *msg,
	char *buffer, size_t maxsize, size_t *nmwritten);

//! @brief Parses BWW message
NAVI_EXTERN(navierr_status_t) navi_parse_bww(struct bww_t *msg, char *buffer);

NAVI_END_DECL

#endif // INCLUDE_navi_bww_h
