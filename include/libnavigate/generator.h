/*
 * generator.h - interface to IEC message generator
 *
 * Copyright (C) 2012, 2013 I. S. Gorbunov <igor.genius at gmail.com>
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

/*! @file generator.h
 *  @brief Declares API-calls for generating messages.
 *
 *  Contains declarations for the main generator function and some helpful
 *  utilities for use in generation of messages.
 */

#ifndef INCLUDE_navi_generator_h
#define INCLUDE_navi_generator_h

#include "sentence.h"

NAVI_BEGIN_DECL

/*! @brief Generator for IEC 61162-1 (2000-07) messages
 *
 * Generates IEC sentence by its description given by type and msg.
 * Stores the result to buffer which has the maximum size of maxsize.
 * The number of stored characters is stored to nmwritten.
 * Returns 0 if succeeded, -1 otherwise
 * Call navierr_get_last() to get the error description
 *
 * @param[in] type the type of address field
 * @param[in] address the target address
 * @param[in] msg the message body
 * @param[out] buffer the output buffer
 * @param[in] maxsize the size of the output buffer
 * @param[out] nmwritten the amount of bytes written to the output buffer
 * @return the status of operation
 */
NAVI_EXTERN(navierr_status_t) navi_create_msg(navi_addrfield_t type,
	const void *address, const void *msg, char *buffer,
	size_t maxsize, size_t *nmwritten);

//! @brief Returns the string representation of talker identifier
NAVI_EXTERN(const char *) navi_talkerid_str(navi_talkerid_t tid);

//! @brief Returns the string representation of approved sentence
NAVI_EXTERN(const char *) navi_sentencefmt_str(navi_approved_fmt_t fmt);

//! @brief Returns the string representation of geodetic datum
NAVI_EXTERN(const char *) navi_datum_str(navi_datum_t datum);

//! @brief Returns the string representation of geodetic datum subdivision code
NAVI_EXTERN(const char *) navi_datumsubdiv_str(navi_datum_subdivision_t datumsub);

//! @brief Returns the offset or position fix sign
NAVI_EXTERN(const char *) navi_fixsign_str(navi_offset_sign_t fixsign);

//! @brief Returns the status
NAVI_EXTERN(const char *) navi_status_str(navi_status_t status);

//! @brief Returns the mode indicator
NAVI_EXTERN(const char *) navi_modeindicator_str(navi_modeindicator_t mi);

//! @brief Returns the extended mode indicator
NAVI_EXTERN(const char *) navi_modeindicator_extended_str(navi_modeindicator_t mi);

//! @brief Returns the GSA 2D/3D switching mode
NAVI_EXTERN(const char *) navi_gsamode_str(navi_gsaswitchmode_t mode);

//! @brief Prints offset 'x.x,a', or null fields
//!
//! @return the number of printed characters
NAVI_EXTERN(size_t) navi_print_offset(const struct navi_offset_t *offset,
	char *buffer, size_t maxsize);

//! @brief Prints position fix 'llll.ll,a,yyyyy.yy,a', or null fields
//!
//! @return the number of printed characters
NAVI_EXTERN(size_t) navi_print_position_fix(const struct navi_position_t *fix,
	char *buffer, size_t maxsize);

//! @brief Prints variable numbers
//!
//! @return the number of printed characters
NAVI_EXTERN(size_t) navi_print_number(double value, char *buffer, size_t maxsize);

//! @brief Prints fixed length hexadecimal numbers, MSB on the left.
//!
//! If fieldwidth equals 0, the field is considered to be null.
//! @return the number of printed characters
NAVI_EXTERN(size_t) navi_print_hexfield(const char bytes[], int fieldwidth,
	char *buffer, size_t maxsize);

//! @brief Prints fixed length decimal numbers
//!
//! If fieldwidth equals 0, the field is considered to be null.
//! @return the number of printed characters
NAVI_EXTERN(size_t) navi_print_decfield(const char bytes[], int fieldwidth,
	char *buffer, size_t maxsize);

//! @brief Print fixed length field with given radix
NAVI_EXTERN(size_t) navi_print_fixedfield(const char bytes[], int fieldwidth, int radix,
	char *buffer, size_t maxsize);

//! @brief Prints UTC time
//!
//! @return the number of printed characters
NAVI_EXTERN(size_t) navi_print_utc(const struct navi_utc_t *utc,
	char *buffer, size_t maxsize);

//! @brief Prints array of mode indicators
//!
//! @return the number of printed characters
NAVI_EXTERN(size_t) navi_print_miarray(const navi_modeindicator_t mi[], int miquant,
	char *buffer);

//! @brief Fills a buffer with valid characters from the given string,
//! translating reserved characters to triples '^xx'.
//!
//! @return navi_Error, if the conversion is not possible
NAVI_EXTERN(navierr_status_t) navi_print_character_field(const char *from,
	char *to, size_t maxsize);

NAVI_END_DECL

#endif // INCLUDE_navi_generator_h
