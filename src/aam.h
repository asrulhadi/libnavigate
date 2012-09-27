/*
 * aam.h - generator and parser of AAM message
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

#ifndef INCLUDE_navi_aam_h
#define INCLUDE_navi_aam_h

#include <libnavigate/errors.h>
#include <libnavigate/sentence.h>

NAVI_BEGIN_DECL

//
// Initializes AAM sentence structure with default values
NAVI_EXTERN(navierr_status_t) navi_init_aam(struct aam_t *aam, navi_talkerid_t tid);

NAVI_END_DECL

#endif // INCLUDE_navi_aam_h
