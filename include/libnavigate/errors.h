/*
 * errors.h - library error status utilities
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

/*! @file errors.h
 *  @brief Declares the structure and enumerations for error handling.
 *
 *  Contains declarations for structure, enumerations and typedefs
 *  for error handling utilities of the project.
 */

#ifndef INCLUDE_navi_errors_h
#define INCLUDE_navi_errors_h

#include "generic.h"

NAVI_BEGIN_DECL

//! @brief Return statuses for libnavigate methods
enum _navierr_status_t
{
	navi_Error = -1,		//!< error
	navi_Ok = 0				//!< no error
};

//! @brief Error classes
enum _navierr_class_t
{
	navi_Undefined = 0,			//!< undefined or no error
	navi_MsgExceedsMaxSize = 1,	//!< message too long
	navi_CrcEror = 2,			//!< crc error
	navi_NotEnoughBuffer = 3,	//!< not enough space in output buffer
	navi_MsgNotSupported = 4,	//!< message type not supported
	navi_NoValidMessage = 5,	//!< no valid message in buffer
	navi_NullField = 6,			//!< null field in a message, where it is not allowed
	navi_InvalidMessage = 7,	//!< message does not conform to protocol
	navi_NotImplemented = 8,	//!< method not implemented
	navi_InvalidParameter = 9	//!< invalid parameter to method
};

//! @brief Error status
typedef int navierr_status_t;
//! @brief Error classe
typedef int navierr_class_t;

//! @brief Holds last error information
//!
//! The structure holds the last error iformation.
typedef struct _navi_error_t
{
	navierr_class_t errclass;	//!< the class of last error
} navi_error_t;

//! @brief Returns the pointer to 'navi_error_t' object that holds the last library error
//! that occurred for this thread.
NAVI_EXTERN(const navi_error_t *) navierr_get_last(void);

//! @brief Sets the last error
NAVI_EXTERN(void) navierr_set_last(navierr_class_t errclass);

NAVI_END_DECL

#endif // INCLUDE_navi_errors_h
