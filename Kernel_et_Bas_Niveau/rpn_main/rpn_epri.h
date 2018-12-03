/*
 * Copyright (C) 2017, Martial Bornet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   rpn_epri.h
 *
 *   @(#)  [MB] cy_rpn_epri.h Version 1.3 du 18/06/10 - 
 */

#if ! defined(RPNA_EPRI_H)
#define   _RPN_EPRI_H

/* External variables
   ~~~~~~~~~~~~~~~~~~ */
extern struct global_struct         G;

extern struct rpn_st_types          op_types_add[],
                                    op_types_sub[],
                                    op_types_mul[],
                                    op_types_div[],
                                    op_types_math_mul[],
                                    op_types_delta_percent[],
                                    op_types_power[],
							 
							 op_types_gen_math1[];

extern struct rpn_operator          my_operators[];

#endif    // _RPN_EPRI_H
