/*
 * gga.h - generator and parser of GGA message
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

/*! @file gga.h
 *  @brief Declares the structure and handling utilities for GGA sentence.
 *
 *  Contains declarations for structure, initilizer, generator and parser
 *  of GGA sentence.
 */

#ifndef INCLUDE_navi_gga_h
#define INCLUDE_navi_gga_h

#include "sentence.h"

/*! @brief GGA - Global positioning system (GPS) fix data
 *
 *  Time, position and fix-related data for a GPS receiver.
 *  $--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh[cr][lf]
 */
struct gga_t
{
	struct navi_utc_t utc;				//!< UTC time
	struct navi_position_t fix;			//!< latitude, longitude fix
	navi_gpsindicator_t gpsindicator;	//!< GPS quality indicator

	/*! @brief Number of satellites in use (00-12)
	 *
	 *  -1, if null field
	 */
	int nmsatellites;

	double hdop;			//!< Horizontal dilution of precision
	double antaltitude;		//!< Antenna altitude above/below mean sea level (geoid)
	double geoidalsep;		//!< Geoidal separation

	/*! @brief Age of differential GPS data, seconds
	 *
	 *  -1, if null field
	 */
	int diffdata_age;

	/*! @brief Differential reference station ID, 1-1023
	 *
	 *  -1, if null field
	 */
	int station_id;
};

NAVI_BEGIN_DECL

//! @brief Initializes GGA sentence structure with default values
NAVI_EXTERN(navierr_status_t) navi_init_gga(struct gga_t *msg);

//! @brief Creates GGA message
NAVI_EXTERN(navierr_status_t) navi_create_gga(const struct gga_t *msg,
	char *buffer, size_t maxsize, size_t *nmwritten);

//! @brief Parses GGA message
NAVI_EXTERN(navierr_status_t) navi_parse_gga(struct gga_t *msg, char *buffer);

NAVI_END_DECL

#endif // INCLUDE_navi_gga_h
