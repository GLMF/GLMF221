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
 *   rpn_proto.h
 *
 *   @(#)  [MB] cy_rpn_proto.h Version 1.7 du 18/08/18 - 
 */


/* Functions prototypes
   ~~~~~~~~~~~~~~~~~~~~ */
rpn_stack                           *rpn_new_stack(const char *);
rpn_elt                             *rpn_new_elt(void);
rpn_elt						 *rpn_new_int(int);
void                                 rpn_free_elt(rpn_elt *);
rpn_elt                             *rpn_clone_elt(rpn_elt *);
struct rpn_sigma                    *rpn_new_sigma(char *);
void                                 rpn_sto(rpn_stack *, struct rpn_elt **);

int                                  rpn_push(rpn_stack *, rpn_elt *);
rpn_elt                             *rpn_pop(rpn_stack *);

int                                  rpn_get_type(rpn_elt *);
int                                  rpn_set_type(rpn_elt *, int);

rpn_operator                        *rpn_search_op(rpn_operator *, char *);

int                                  rpn_op(rpn_stack *, rpn_operator *);

void                                 rpn_err_msg_no_mem(void);
void                                 rpn_err_msg_op_not_found(char *);
void                                 rpn_err_msg_op_error(char *, int);
char                                *rpn_err_to_string(int errcode);
void                                 rpn_err_msg_invalid_string(char *);
void                                 rpn_internal_error(const char *, char *, int);

int                                  rpn_strip_quotes(char **);
int                                  rpn_strip_single_quotes(char **);
int                                  rpn_check_types(rpn_operator *, struct rpn_stack *);
char                                *rpn_type_to_string(int);

int                                  my_op_undef(rpn_stack *, rpn_operator *);

/* Strings
   ~~~~~~~ */
int                                  my_op_str_length(rpn_stack *, rpn_operator *);
int                                  my_op_str_catenate(rpn_stack *, rpn_operator *);
int                                  my_op_str_dupl(rpn_stack *, rpn_operator *);


int                                  rpn_op_roll_down(rpn_stack *, rpn_operator *);
int                                  rpn_op_roll_up(rpn_stack *, rpn_operator *);
int                                  rpn_op_lastx(rpn_stack *, rpn_operator *);
int                                  rpn_op_rcl_x(rpn_stack *, rpn_operator *);

int                                  rpn_op_swap_xy(rpn_stack *, rpn_operator *);
int                                  rpn_op_swap_xz(rpn_stack *, rpn_operator *);
int                                  rpn_op_swap_xt(rpn_stack *, rpn_operator *);
int                                  rpn_op_swap_xl(rpn_stack *, rpn_operator *);
int                                  rpn_op_enter(rpn_stack *, rpn_operator *);
int                                  rpn_op_clx(rpn_stack *, rpn_operator *);
int                                  rpn_op_delx(rpn_stack *, rpn_operator *);
int                                  rpn_op_clst(rpn_stack *, rpn_operator *);
int                                  rpn_op_prx(rpn_stack *, rpn_operator *);
int                                  rpn_op_prstk(rpn_stack *, rpn_operator *);


/* Math
   ~~~~ */
int                                  my_op_generic1(rpn_stack *, rpn_operator *);
int                                  my_op_generic2(rpn_stack *, rpn_operator *);
int                                  my_op_math_mul(rpn_stack *, rpn_operator *);
int                                  my_op_div(rpn_stack *, rpn_operator *);

int                                  rpn_op_reciprocal(rpn_stack *, rpn_operator *);
int                                  rpn_op_power(rpn_stack *, rpn_operator *);
int                                  rpn_op_sqrt(rpn_stack *, rpn_operator *);
int                                  rpn_op_square(rpn_stack *, rpn_operator *);
int                                  rpn_op_ln(rpn_stack *, rpn_operator *);
int                                  rpn_op_exp(rpn_stack *, rpn_operator *);
int                                  rpn_op_log10(rpn_stack *, rpn_operator *);
int                                  rpn_op_10_power_x(rpn_stack *, rpn_operator *);
int                                  rpn_op_percent(rpn_stack *, rpn_operator *);
int                                  rpn_op_delta_percent(rpn_stack *, rpn_operator *);
int                                  rpn_op_sine(rpn_stack *, rpn_operator *);
int                                  rpn_op_cosine(rpn_stack *, rpn_operator *);
int                                  rpn_op_tan(rpn_stack *, rpn_operator *);
int                                  my_op_matrix(rpn_stack *, rpn_operator *);
int                                  my_op_explode(rpn_stack *, rpn_operator *);
int                                  my_op_vec2(rpn_stack *, rpn_operator *);
int                                  my_op_vec3(rpn_stack *, rpn_operator *);
int                                  my_op_transpose(rpn_stack *, rpn_operator *);
int                                  my_op_cross_prdct(rpn_stack *, rpn_operator *);
int                                  rpn_op_scalar_matrix(rpn_stack *, rpn_operator *);
int                                  rpn_op_seq(rpn_stack *, rpn_operator *);
int                                  rpn_op_dim(rpn_stack *, rpn_operator *);

/* Statistics
   ~~~~~~~~~~ */
int                                  rpn_op_sum_add(rpn_stack *, rpn_operator *);
int                                  rpn_op_sum_sub(rpn_stack *, rpn_operator *);
int                                  rpn_op_mean(rpn_stack *, rpn_operator *);
int                                  rpn_op_sdev(rpn_stack *, rpn_operator *);

int                                  rpn_mat_get_i(struct rpn_matrix *, int, int);
struct rpn_elt                      *rpn_mat_get_elt(struct rpn_matrix *, int, int);

void                                 rpn_disp_elt(rpn_elt *, int, int);
// void                                 rpn_disp_stack(rpn_stack *, char *);
void                                 rpn_disp_stack(rpn_stack *);
void                                 rpn_disp_sigma(void);

