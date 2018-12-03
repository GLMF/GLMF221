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
 *   @(#)  [MB] cy_rpn_custom.c Version 1.27 du 18/09/17 - 
 */

#include  "rpn_header.h"
#include  "rpn_epri.h"
#include  "rpn_proto.h"
#include  <math.h>

/* Operators descriptors
   ~~~~~~~~~~~~~~~~~~~~~ */
struct rpn_operator      my_operators[]           = {
     {    RPN_OP(UNDEF),                     0,     0,        my_op_undef              },

     // Stack
     {    RPN_OP(ENTER),                     1,     0,        rpn_op_enter,          NULL                        },
     {    RPN_OP(CLX),                       1,     1,        rpn_op_clx,            NULL                        },
     {    RPN_OP(DELX),                      1,     1,        rpn_op_delx,           NULL                        },
     {    RPN_OP(CLST),                      0,     0,        rpn_op_clst,           NULL                        },
     {    RPN_OP(SWAP_XY),                   2,     2,        rpn_op_swap_xy,        NULL                        },
     {    RPN_OP(SWAP_XZ),                   2,     2,        rpn_op_swap_xz,        NULL                        },
     {    RPN_OP(SWAP_XT),                   2,     2,        rpn_op_swap_xt,        NULL                        },
     {    RPN_OP(SWAP_XL),                   2,     2,        rpn_op_swap_xl,        NULL                        },
     {    RPN_OP(RDN),                       0,     0,        rpn_op_roll_down,      NULL                        },
     {    RPN_OP(RUP),                       0,     0,        rpn_op_roll_up,        NULL                        },
     {    RPN_OP(LASTX),                     0,     1,        rpn_op_lastx,          NULL                        },
     {    RPN_OP(RCL_X),                     0,     1,        rpn_op_rcl_x,          NULL                        },
     {    RPN_OP(PRX),                       1,     1,        rpn_op_prx,            NULL                        },
     {    RPN_OP(PRSTK),                     1,     0,        rpn_op_prstk,          NULL                        },
//   {    RPN_OP(CATALOG),                   1,     0,        rpn_op_catalog,        NULL                        },

     // Math
     {    RPN_OP(ADD),                       2,     1,        my_op_generic2,        op_types_add                },
     {    RPN_OP(SUB),                       2,     1,        my_op_generic2,        op_types_sub                },
     {    RPN_OP(MUL),                       2,     1,        my_op_generic2,        op_types_mul                },
     {    RPN_OP(DIV),                       2,     1,        my_op_generic2,        op_types_div                },
     {    RPN_OP(MATH_MUL),                  2,     1,        my_op_math_mul,        op_types_mul                },
     {    RPN_OP(RECIPROCAL),                1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(POWER),                     1,     1,        my_op_generic2,        op_types_power              },
     {    RPN_OP(SQRT),                      1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(SQUARE),                    1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(LN),                        1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(EXP),                       1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(LOG10),                     1,     1,        rpn_op_log10 ,         NULL                        },
     {    RPN_OP(10_POWER_X),                1,     1,        rpn_op_10_power_x,     NULL                        },
     {    RPN_OP(PERCENT),                   1,     1,        rpn_op_percent,        NULL                        },
     {    RPN_OP(DELTA_PERCENT),             1,     1,        my_op_generic2,        op_types_delta_percent      },
     {    RPN_OP(SINE),                      1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(COSINE),                    1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(TANGENT),                   1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(SINH),                      1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(COSH),                      1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(TANH),                      1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(SIGMOID),                   1,     1,        my_op_generic1,        op_types_gen_math1          },
     {    RPN_OP(MATRIX),                    2,     1,        my_op_matrix,          NULL                        },
     {    RPN_OP(EXPLODE),                   1,     0,        my_op_explode,         NULL                        },
     {    RPN_OP(VEC2),                      2,     1,        my_op_vec2,            NULL                        },
     {    RPN_OP(VEC3),                      2,     1,        my_op_vec3,            NULL                        },
     {    RPN_OP(TRANSPOSE),                 1,     1,        my_op_transpose,       NULL                        },
     {    RPN_OP(CROSS_PRODUCT),             2,     1,        my_op_cross_prdct,     NULL                        },
     {    RPN_OP(SCALAR_MATRIX),             1,     1,        rpn_op_scalar_matrix,  NULL                        },
     {    RPN_OP(SEQ),                       2,    -1,        rpn_op_seq,            NULL                        },
     {    RPN_OP(DIM),                       1,     2,        rpn_op_dim,            NULL                        },

     // Strings
     {    RPN_OP(STR_LENGTH),                1,     1,        my_op_str_length,      NULL                        },
     {    RPN_OP(STR_CATENATE),              2,     1,        my_op_str_catenate,    NULL                        },
     {    RPN_OP(STR_DUPL),                  2,     1,        my_op_str_dupl,        NULL                        },

     // Statistics
     {    RPN_OP(SUM_ADD),                   2,     1,        rpn_op_sum_add,        NULL                        },
     {    RPN_OP(SUM_SUB),                   2,     1,        rpn_op_sum_sub,        NULL                        },
     {    RPN_OP(MEAN),                      0,     2,        rpn_op_mean,           NULL                        },
     {    RPN_OP(SDEV),                      0,     2,        rpn_op_sdev,           NULL                        },

     {    RPN_OP(NULL),                      0,     0,        0,                     0                           }
};

/******************************************************************************

                              MY_OP_UNDEF

******************************************************************************/
int my_op_undef(rpn_stack *stack, rpn_operator *op)
{
     RPN_INTERNAL_ERROR;
}

/******************************************************************************

                              MY_OP_GENERIC1

******************************************************************************/
int my_op_generic1(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg, _X_type;
     double                    _xf, _resultf;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _X_type                  = rpn_get_type(_stk_x);

     switch (_X_type) {

     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          switch (op->op_id) {

          case RPN_OP_ID_RECIPROCAL:
               _result                  = 1 / _x;
               break;

          case RPN_OP_ID_SQUARE:
               _result                  = _x * _x;
               break;

          case RPN_OP_ID_SQRT:
               _result                  = (int) sqrt((double) _x);
               break;

          case RPN_OP_ID_LN:
               _result                  = (int) log((double) _x);
               break;

          case RPN_OP_ID_EXP:
               _result                  = (int) exp((double) _x);
               break;

          case RPN_OP_ID_SINE:
               _result                  = (int) sin((double) _x);
               break;

          case RPN_OP_ID_COSINE:
               _result                  = (int) cos((double) _x);
               break;

          case RPN_OP_ID_TANGENT:
               _result                  = (int) tan((double) _x);
               break;

          case RPN_OP_ID_SINH:
               _result                  = (int) ((exp((double) _x) - exp((double) -_x)) / 2.0);
               break;

          case RPN_OP_ID_COSH:
               _result                  = (int) ((exp((double) _x) + exp((double) - _x)) / 2.0);
               break;

          case RPN_OP_ID_TANH:
               _result                  = (int) ((exp((double) _x * 2.0) - 1.0) / (exp((double) _x * 2.0) + 1.0));
               break;

          case RPN_OP_ID_SIGMOID:
               _result                  = (int) (1.0 / (1.0 + exp((double) -_x)));
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               RPN_INTERNAL_ERROR;
               break;
          }

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_INT;
          _stk_result->value.i     = _result;

          rpn_push(stack, _stk_result);
          break;
// }}}
     case RPN_TYPE_FLOAT:
// {{{
          _xf                      = _stk_x->value.f;

          switch (op->op_id) {

          case RPN_OP_ID_RECIPROCAL:
               _resultf                 = 1.0 / _xf;
               break;

          case RPN_OP_ID_SQUARE:
               _resultf                 = _xf * _xf;
               break;

          case RPN_OP_ID_SQRT:
               _resultf                 = sqrt(_xf);
               break;

          case RPN_OP_ID_LN:
               _resultf                 = log(_xf);
               break;

          case RPN_OP_ID_EXP:
               _resultf                 = exp(_xf);
               break;

          case RPN_OP_ID_SINE:
               _resultf                 = sin(_xf);
               break;

          case RPN_OP_ID_COSINE:
               _resultf                 = cos(_xf);
               break;

          case RPN_OP_ID_TANGENT:
               _resultf                 = tan(_xf);
               break;

          case RPN_OP_ID_SINH:
               _resultf                 = (exp(_xf) - exp(-_xf)) / 2.0;
               break;

          case RPN_OP_ID_COSH:
               _resultf                 = (exp(_xf) + exp(_xf)) / 2.0;
               break;

          case RPN_OP_ID_TANH:
               _resultf                 = (exp(_xf * 2.0) - 1.0) / (exp(_xf * 2.0) + 1.0);
               break;

          case RPN_OP_ID_SIGMOID:
               _resultf                 = 1.0 / (1.0 + exp((double) -_xf));
               break;

          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               RPN_INTERNAL_ERROR;
               break;
          }

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_FLOAT;
          _stk_result->value.f     = _resultf;

          rpn_push(stack, _stk_result);
          break;
// }}}
     case RPN_TYPE_MATRIX:
// {{{
          {
               struct rpn_matrix        *_mat_A, *_mat_C;
               int                       _n, _m, _p, _i, _j, _idx, _sum, _size;
               struct rpn_elt           *_elt, *_nil;
               struct rpn_stack         *_stack;

               _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;

               _n                       = _mat_A->n;
               _p                       = _mat_A->p;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();
               _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
               _mat_C                   = (struct rpn_matrix *) malloc(_size);
               _mat_C->n                = _n;
               _mat_C->p                = _p;
               _stk_result->value.obj   = _mat_C;
               rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

               _stack                   = rpn_new_stack(__func__);

               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _nil      = rpn_new_elt();
                         rpn_set_type(_nil, RPN_TYPE_INT);
                         rpn_push(_stack, _nil);

                         _elt      = rpn_mat_get_elt(_mat_A, _i, _j);
                         rpn_push(_stack, _elt);
                         if ((_retcode = my_op_generic1(_stack, op)) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }
                         _elt                = rpn_pop(_stack);
                         _idx                = ((_j - 1) * _n) + (_i - 1);
                         _mat_C->base[_idx]  = (void *) _elt;
                    }
               }

               if ((_mat_C->n == 1)
               &&  (_mat_C->p == 1)) {
                    _stk_result              = rpn_mat_get_elt(_mat_C, 1, 1);
               }

               rpn_push(stack, _stk_result);
          }
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
          break;
// }}}
     }

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_GENERIC2

******************************************************************************/
int my_op_generic2(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result, _lg;
     double                    _xf, _yf, _resultf;
     char                     *_s1, *_s2, *_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(rpn_get_X_type(stack)));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(rpn_get_Y_type(stack)));
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);


     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

     switch (_X_type) {
     
     case RPN_TYPE_INT:
// {{{
          _x                       = _stk_x->value.i;

          switch (_Y_type) {
          
          case RPN_TYPE_INT:
// {{{
               _y                       = _stk_y->value.i;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _result                  = _y + _x;
                    break;

               case RPN_OP_ID_SUB:
                    _result                  = _y - _x;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
                    _result                  = _y * _x;
                    break;

               case RPN_OP_ID_DIV:
                    _result                  = _y / _x;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _result                  = (_x - _y) * 100 / _y;
                    break;

               case RPN_OP_ID_POWER:
                    _result                  = (int) pow((double) _y, (double) _x);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_INT;
               _stk_result->value.i     = _result;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_FLOAT:
// {{{
               _yf                      = _stk_y->value.f;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _resultf                 = _yf + (double) _x;
                    break;

               case RPN_OP_ID_SUB:
                    _resultf                 = _yf - (double) _x;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
                    _resultf                 = _yf * (double) _x;
                    break;

               case RPN_OP_ID_DIV:
                    _resultf                 = _yf / (double) _x;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _resultf                 = (_xf - _yf) * 100.0 / _yf;
                    break;

               case RPN_OP_ID_POWER:
                    _resultf                 = pow(_yf, (double) _x);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_FLOAT;
               _stk_result->value.f     = _resultf;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_B                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_B->n;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_VECTOR_3:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          case RPN_TYPE_LITTERAL:
// {{{
               fprintf(stderr, "%s (op = %s) :\n", __func__, op->op_name);
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          case RPN_TYPE_NIL:
// {{{
               rpn_push(stack, _stk_x);
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_FLOAT:
// {{{
          _xf                      = _stk_x->value.f;

          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yf                       = (double) _stk_y->value.i;


               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _resultf                 = _yf + _xf;
                    break;

               case RPN_OP_ID_SUB:
                    _resultf                 = _yf - _xf;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
                    _resultf                 = _yf * _xf;
                    break;

               case RPN_OP_ID_DIV:
                    _resultf                 = _yf / _xf;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _resultf                 = (_xf - _yf) * 100.0 / _yf;
                    break;

               case RPN_OP_ID_POWER:
                    _resultf                 = pow(_yf, _xf);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_FLOAT;
               _stk_result->value.f     = _resultf;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_FLOAT:
// {{{
               _yf                      = _stk_y->value.f;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _resultf                 = _yf + _xf;
                    break;

               case RPN_OP_ID_SUB:
                    _resultf                 = _yf - _xf;
                    break;

               case RPN_OP_ID_MUL:
               case RPN_OP_ID_MATH_MUL:
                    _resultf                 = _yf * _xf;
                    break;

               case RPN_OP_ID_DIV:
                    _resultf                 = _yf / _xf;
                    break;

               case RPN_OP_ID_DELTA_PERCENT:
                    _resultf                 = (_xf - _yf) * 100.0 / _yf;
                    break;

               case RPN_OP_ID_POWER:
                    _resultf                 = pow(_yf, _xf);
                    break;

               default:
                    fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
                    fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_FLOAT;
               _stk_result->value.f     = _resultf;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_B                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_B->n;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_NIL:
// {{{
               rpn_push(stack, _stk_x);
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_STRING:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_STRING:
// {{{
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               switch (op->op_id) {

               case RPN_OP_ID_ADD:
                    _lg                      = strlen(_s1) + strlen(_s2);
                    _s                       = malloc(_lg + 1);

                    strcpy(_s, _s1);
                    strcat(_s, _s2);
                    break;

               case RPN_OP_ID_SUB:
               case RPN_OP_ID_MUL:
               case RPN_OP_ID_DIV:
               default:
                    RPN_INTERNAL_ERROR;
                    break;
               }

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_STRING;
               _stk_result->value.s     = _s;

               rpn_push(stack, _stk_result);
               break;
// }}}

          case RPN_TYPE_NIL:
               rpn_push(stack, _stk_x);
               break;

		case	RPN_TYPE_INT:
			fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
			fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
			break;

          default:
			fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
			fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;
// }}}
     case RPN_TYPE_LITTERAL:
// {{{
		switch (_Y_type) {

		case	RPN_TYPE_NIL:
			rpn_push(stack, _stk_x);
			break;

          case RPN_TYPE_LITTERAL:
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _s                       = malloc(_lg + 8);

               sprintf(_s, "(%s) %s (%s)", _s1, op->op_name, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_LITTERAL;
               _stk_result->value.s     = _s;

               rpn_push(stack, _stk_result);
               break;

		default:
			fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
			fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
			RPN_INTERNAL_ERROR;
			break;
		}
		break;
// }}}
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_y);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}

          case RPN_TYPE_FLOAT:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;
                    _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_y);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_push(stack, _stk_result);
               }
// }}}
               break;

          case RPN_TYPE_MATRIX:
// {{{
          {
               struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
               int                       _n, _m, _p, _i, _j, _k, _idx, _sum, _size;
               struct rpn_elt           *_elt, *_nil;
               struct rpn_stack         *_stack;

               _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
               _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

               /* Check dimensions : A : n x p;   B : n x p
                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
               if ((_mat_A->n != _mat_B->n)
               ||  (_mat_A->p != _mat_B->p)) {
                    // Incompatible dimensions
                    rpn_push(stack, _stk_y);
                    rpn_push(stack, _stk_x);

                    _retcode                 = RPN_RET_INCOMPATIBLE_ELTS;
                    goto end;
               }

               _n                       = _mat_A->n;
               _p                       = _mat_A->p;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();
               _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
               _mat_C                   = (struct rpn_matrix *) malloc(_size);
               _mat_C->n                = _n;
               _mat_C->p                = _p;
               _stk_result->value.obj   = _mat_C;
               rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

               _stack                   = rpn_new_stack(__func__);

               for (_i = 1; _i <= _n; _i++) {
                    for (_j = 1; _j <= _p; _j++) {
                         _elt      = rpn_mat_get_elt(_mat_A, _i, _j);
                         rpn_push(_stack, _elt);

                         _elt      = rpn_mat_get_elt(_mat_B, _i, _j);
                         rpn_push(_stack, _elt);

                         if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                              rpn_err_msg_op_error(op->op_name, _retcode);
                              exit(RPN_EXIT_OP_ERROR);
                         }

                         _elt                = rpn_pop(_stack);
                         _idx                = ((_j - 1) * _n) + (_i - 1);
                         _mat_C->base[_idx]  = (void *) _elt;
                    }
               }

               if ((_mat_C->n == 1)
               &&  (_mat_C->p == 1)) {
                    _stk_result              = rpn_mat_get_elt(_mat_C, 1, 1);
               }

               rpn_push(stack, _stk_result);
          }
          break;
// }}}
          case RPN_TYPE_NIL:
// {{{
               rpn_push(stack, _stk_x);
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_NIL:
// {{{
          rpn_push(stack, _stk_y);
          break;
// }}}
     default:
// {{{
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
          break;
// }}}
     }

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_MATH_MUL

******************************************************************************/
int my_op_math_mul(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _X_type, _Y_type;
     int                       _x, _y, _result, _lg, _n, _total_size, _i;
     double                    _xf, _yf, _resultf;
     char                     *_s1, *_s2, *_s, *_result_s;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     /* Check types
        ~~~~~~~~~~~ */
     if ((_retcode = rpn_check_types(op, stack)) != RPN_RET_OK) {
          goto end;
     }

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _X_type                  = rpn_get_type(_stk_x);
     _Y_type                  = rpn_get_type(_stk_y);

	fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
	fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
     switch (_X_type) {

     case RPN_TYPE_INT:
          _x                       = _stk_x->value.i;
// {{{
          switch (_Y_type) {
          
          case RPN_TYPE_INT:
// {{{
               _y                       = _stk_y->value.i;

               _result                  = _y * _x;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_INT;
               _stk_result->value.i     = _result;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_FLOAT:
// {{{
               _xf                      = (double) _x;;
               _yf                      = _stk_y->value.f;

               _resultf                 = _yf * _xf;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_FLOAT;
               _stk_result->value.f     = _resultf;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_STRING:
// {{{
               _n                       = _x;
               _s                       = _stk_y->value.s;

               _lg                      = strlen(_s);
               _total_size              = _lg * _n;

#define   MAX_SIZE       (10240)
               if (_total_size > MAX_SIZE) {
                    /* Result would not be reasonable !
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode                 = RPN_RET_TOO_BIG;
               }
               else {
                    _result_s                = malloc(_total_size + 1);

                    strcpy(_result_s, _s);
                    for (_i = 0; _i < (_n - 1); _i++) {
                         strcat(_result_s, _s);
                    }

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();

                    _stk_result->type        = RPN_TYPE_STRING;
                    _stk_result->value.s     = _result_s;

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;

                    _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              _elt      = rpn_mat_get_elt(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error("*", _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_FLOAT:
          _xf                      = _stk_x->value.f;
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
// {{{
               _yf                      = (double)_stk_y->value.i;

               _resultf                 = _yf * _xf;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_FLOAT;
               _stk_result->value.f     = _resultf;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_FLOAT:
// {{{
               _yf                      = _stk_y->value.f;

               _resultf                 = _yf * _xf;

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_FLOAT;
               _stk_result->value.f     = _resultf;

               rpn_push(stack, _stk_result);
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_C;
                    int                       _n, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;

                    _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _n                       = _mat_A->n;
                    _p                       = _mat_A->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _scalar   = rpn_clone_elt(_stk_x);
                              rpn_push(_stack, _scalar);

                              _elt      = rpn_mat_get_elt(_mat_A, _i, _j);
                              rpn_push(_stack, _elt);

                              if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_LITTERAL:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_LITTERAL:
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _s                       = malloc(_lg + 8);

               sprintf(_s, "(%s) * (%s)", _s1, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_LITTERAL;
               _stk_result->value.s     = _s;

               rpn_push(stack, _stk_result);
               break;

          default:
// {{{
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
// }}}
          }
          break;
// }}}
     case RPN_TYPE_MATRIX:
// {{{
          switch (_Y_type) {

          case RPN_TYPE_INT:
          case RPN_TYPE_FLOAT:
// {{{
               {
                    struct rpn_matrix        *_mat_B, *_mat_C;
                    int                       _n, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_scalar;
                    struct rpn_stack         *_stack;

                    _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;
                    _n                       = _mat_B->n;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);
                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _elt      = rpn_mat_get_elt(_mat_B, _i, _j);
                              rpn_push(_stack, _elt);

                              _scalar   = rpn_clone_elt(_stk_y);
                              rpn_push(_stack, _scalar);

                              if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                   rpn_err_msg_op_error(op->op_name, _retcode);
                                   exit(RPN_EXIT_OP_ERROR);
                              }

                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          case RPN_TYPE_MATRIX:
// {{{
               {
                    struct rpn_matrix        *_mat_A, *_mat_B, *_mat_C;
                    int                       _n, _m, _p, _i, _j, _k, _idx, _sum, _size;
                    struct rpn_elt           *_elt, *_nil;
                    struct rpn_stack         *_stack;

                    _mat_A                   = (struct rpn_matrix *) _stk_y->value.obj;
                    _mat_B                   = (struct rpn_matrix *) _stk_x->value.obj;

                    /* Check dimensions : A : n x m;   B : m x p
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    if (_mat_A->p != _mat_B->n) {
                         // Incompatible dimensions
                         rpn_push(stack, _stk_y);
                         rpn_push(stack, _stk_x);

                         _retcode                 = RPN_RET_INCOMPATIBLE_ELTS;
                         goto end;
                    }

                    _n                       = _mat_A->n;
                    _m                       = _mat_A->p;
                    _p                       = _mat_B->p;

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();
                    _size                    = sizeof(*_mat_C) + (((_n * _p) - 1) * sizeof (void *));
                    _mat_C                   = (struct rpn_matrix *) malloc(_size);
                    _mat_C->n                = _n;
                    _mat_C->p                = _p;
                    _stk_result->value.obj   = _mat_C;
                    rpn_set_type(_stk_result, RPN_TYPE_MATRIX);

                    _stack                   = rpn_new_stack(__func__);

                    for (_i = 1; _i <= _n; _i++) {
                         for (_j = 1; _j <= _p; _j++) {
                              _nil     = rpn_new_elt();
                              rpn_set_type(_nil, RPN_TYPE_NIL);
                              rpn_push(_stack, _nil);

                              for (_k = 1; _k <= _m; _k++) {
                                   _elt      = rpn_mat_get_elt(_mat_A, _i, _k);
                                   rpn_push(_stack, _elt);
                                   _elt      = rpn_mat_get_elt(_mat_B, _k, _j);
                                   rpn_push(_stack, _elt);
                                   if ((_retcode = my_op_generic2(_stack, op)) != 0) {
                                        rpn_err_msg_op_error(op->op_name, _retcode);
                                        exit(RPN_EXIT_OP_ERROR);
                                   }
                                   if ((_retcode = my_op_generic2(_stack, RPN_OP_DESC(ADD))) != 0) {
                                        rpn_err_msg_op_error(RPN_OP_DESC(ADD)->op_name, _retcode);
                                        exit(RPN_EXIT_OP_ERROR);
                                   }
                              }
                              _elt                = rpn_pop(_stack);
                              _idx                = ((_j - 1) * _n) + (_i - 1);
                              _mat_C->base[_idx]  = (void *) _elt;
                         }
                    }
                    rpn_op_clst(_stack, NULL);

                    if ((_mat_C->n == 1)
                    &&  (_mat_C->p == 1)) {
                         _stk_result              = rpn_mat_get_elt(_mat_C, 1, 1);
                    }

                    rpn_push(stack, _stk_result);
               }
               break;
// }}}
          default:
               fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
               fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
               RPN_INTERNAL_ERROR;
               break;
          }
          break;
// }}}
     default:
// {{{
          fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));
          RPN_INTERNAL_ERROR;
          break;
// }}}
     }

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_STR_LENGTH

******************************************************************************/
int my_op_str_length(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s;
     int                       _result;

     _stk_x                   = rpn_pop(stack);

     _s                       = _stk_x->value.s;

     _result                  = strlen(_s);

     rpn_set_lastx(stack, _stk_x);

     _stk_result              = rpn_new_elt();

     _stk_result->type        = RPN_TYPE_INT;
     _stk_result->value.i     = _result;

     rpn_push(stack, _stk_result);

     return RPN_RET_OK;
}

/******************************************************************************

                         MY_OP_STR_CATENATE

******************************************************************************/
int my_op_str_catenate(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s1, *_s2;
     char                     *_result;
     int                       _lg, _retcode      = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if (rpn_get_type(_stk_x) == RPN_TYPE_STRING) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s1                      = _stk_y->value.s;
               _s2                      = _stk_x->value.s;

               _lg                      = strlen(_s1) + strlen(_s2);
               _result                  = malloc(_lg + 1);

               strcpy(_result, _s1);
               strcat(_result, _s2);

               rpn_set_lastx(stack, _stk_x);

               _stk_result              = rpn_new_elt();

               _stk_result->type        = RPN_TYPE_STRING;
               _stk_result->value.s     = _result;

               rpn_push(stack, _stk_result);
          }
          else {
               _retcode                 = RPN_RET_INVALID_Y;
          }
     }
     else {
          _retcode                 = RPN_RET_INVALID_X;
     }

     if (_retcode != RPN_RET_OK) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
     }

     return _retcode;
}

/******************************************************************************

                         MY_OP_STR_DUPL

******************************************************************************/
int my_op_str_dupl(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     char                     *_s;
     int                       _n, _total_size, _i;
     char                     *_result;
     int                       _lg, _retcode      = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if (rpn_get_type(_stk_x) == RPN_TYPE_INT) {
          if (rpn_get_type(_stk_y) == RPN_TYPE_STRING) {
               _s                       = _stk_y->value.s;
               _n                       = _stk_x->value.i;

               _lg                      = strlen(_s);
               _total_size              = _lg * _n;

#define   MAX_SIZE       (10240)
               if (_total_size > MAX_SIZE) {
                    /* Result would not be reasonable !
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode                 = RPN_RET_TOO_BIG;
               }
               else {
                    _result                  = malloc(_total_size + 1);

                    strcpy(_result, _s);
                    for (_i = 0; _i < (_n - 1); _i++) {
                         strcat(_result, _s);
                    }

                    rpn_set_lastx(stack, _stk_x);

                    _stk_result              = rpn_new_elt();

                    _stk_result->type        = RPN_TYPE_STRING;
                    _stk_result->value.s     = _result;

                    rpn_push(stack, _stk_result);
               }
          }
          else {
               _retcode                 = RPN_RET_INVALID_Y;
          }
     }
     else {
          _retcode                 = RPN_RET_INVALID_X;
     }

     if (_retcode != RPN_RET_OK) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
     }

     return _retcode;
}

/******************************************************************************

                         MY_OP_MATRIX

******************************************************************************/
int my_op_matrix(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_n, *_stk_p, *_stk_matrix, *_stk_elt;
     int                       _n, _p, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_p                   = rpn_pop(stack);

     if ((_stk_p->type != RPN_TYPE_INT)
     || ((_p  = _stk_p->value.i) <= 0)) {
          /* p is invalid
             ~~~~~~~~~~~~ */
          rpn_push(stack, _stk_p);

          _retcode                 = RPN_RET_INVALID_X;
          goto end;
     }

     _stk_n                   = rpn_pop(stack);
     if ((_stk_n->type != RPN_TYPE_INT)
     || ((_n = _stk_n->value.i) <= 0)) {
          /* n is invalid
             ~~~~~~~~~~~~ */
          rpn_push(stack, _stk_n);
          rpn_push(stack, _stk_p);

          _retcode                 = RPN_RET_INVALID_Y;
          goto end;
     }

     /* Check number of elements in the stack
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     if (stack->num_elts < (_n * _p)) {
          /* Too few elements in the stack
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
          rpn_push(stack, _stk_n);
          rpn_push(stack, _stk_p);

          _retcode                 = RPN_RET_NOT_ENOUGH_ELTS;
          goto end;
     }

     /* Create matrix element
        ~~~~~~~~~~~~~~~~~~~~~ */
     _stk_matrix              = rpn_new_elt();
     _stk_matrix->type        = RPN_TYPE_MATRIX;

     _size                    = sizeof(*_matrix) + (((_n * _p) - 1) * sizeof (void *));
     _matrix                  = (struct rpn_matrix *) malloc(_size);
     _matrix->n               = _n;
     _matrix->p               = _p;
     _stk_matrix->value.obj   = _matrix;

     for (_i = _n; _i > 0; _i--) {
          for (_j = _p; _j > 0; _j--) {
               _stk_elt            = rpn_pop(stack);
               _idx                = ((_j - 1) * _n) + (_i - 1);
               _matrix->base[_idx] = (void *) _stk_elt;
          }
     }

     rpn_push(stack, _stk_matrix);

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_EXPLODE

******************************************************************************/
int my_op_explode(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_elt;
     int                       _n, _p, _size, _i, _j, _idx;
     struct rpn_matrix        *_matrix;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     if (_stk_x->type == RPN_TYPE_MATRIX) {
          _matrix                  = (struct rpn_matrix *) _stk_x->value.obj;
          _n                       = _matrix->n;
          _p                       = _matrix->p;

          for (_i = 1; _i <= _n; _i++) {
               for (_j = 1; _j <= _p; _j++) {
                     _idx          = ((_j - 1) * _n) + (_i - 1);
                     _elt          = (struct rpn_elt *) _matrix->base[_idx];
                    rpn_push(stack, _elt);
               }
          }
     }

     return _retcode;
}

/******************************************************************************

                         MY_OP_VEC2

******************************************************************************/
int my_op_vec2(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_n, *_stk_p;
     int                       _n, _p;

     /* Vectors of dimension 2 are defined as matrices 2x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt();
     rpn_set_type(_stk_n, RPN_TYPE_INT);
     _stk_n->value.i          = 2;      // 2 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt();
     rpn_set_type(_stk_p, RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return my_op_matrix(stack, RPN_OP_DESC(MATRIX));
}

/******************************************************************************

                         MY_OP_VEC3

******************************************************************************/
int my_op_vec3(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_n, *_stk_p;
     int                       _n, _p;

     /* Vectors of dimension 3 are defined as matrices 3x1
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_n                   = rpn_new_elt();
     rpn_set_type(_stk_n, RPN_TYPE_INT);
     _stk_n->value.i          = 3;      // 3 lines
     rpn_push(stack, _stk_n);

     _stk_p                   = rpn_new_elt();
     rpn_set_type(_stk_p, RPN_TYPE_INT);
     _stk_p->value.i          = 1;      // 1 column
     rpn_push(stack, _stk_p);

     return my_op_matrix(stack, RPN_OP_DESC(MATRIX));
}

/******************************************************************************

                         MY_OP_TRANSPOSE

******************************************************************************/
int my_op_transpose(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x;
     int                       _n, _p, _X_type;
     int                       _retcode;
     struct rpn_matrix        *_matrix;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
	_X_type				= rpn_get_type(_stk_x);
     if (_X_type != RPN_TYPE_MATRIX) {
		fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _matrix        = (struct rpn_matrix *) _stk_x->value.obj;
     if ((_matrix->n != 1) && (_matrix->p != 1)) {
		fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _n             = _matrix->n;
     _p             = _matrix->p;

     _matrix->n     = _p;
     _matrix->p     = _n;

     rpn_push(stack, _stk_x);

end:
     return _retcode;
}

/******************************************************************************

                         MY_OP_CROSS_PRDCT

******************************************************************************/
int my_op_cross_prdct(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _n, _p;
     int                       _retcode;
     struct rpn_matrix        *_mat_x, *_mat_y;
     struct rpn_elt           *_a1, *_a2, *_a3,
                              *_b1, *_b2, *_b3,
                              *_c1, *_c2, *_c3;
     struct rpn_stack         *_stack;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     if (rpn_get_type(_stk_x) != RPN_TYPE_MATRIX) {
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _mat_x         = (struct rpn_matrix *) _stk_x->value.obj;
     if ((_mat_x->n != 3) && (_mat_x->p != 1)) {
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_X;
          goto end;
     }

     _stk_y                   = rpn_pop(stack);
     if (rpn_get_type(_stk_y) != RPN_TYPE_MATRIX) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_Y;
          goto end;
     }

     _mat_y         = (struct rpn_matrix *) _stk_y->value.obj;
     if ((_mat_y->n != 3) && (_mat_y->p != 1)) {
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_x);
          _retcode       = RPN_RET_INVALID_Y;
          goto end;
     }

     _stack                   = rpn_new_stack(__func__);

     _a1  = rpn_mat_get_elt(_mat_y, 1, 1);
     _a2  = rpn_mat_get_elt(_mat_y, 2, 1);
     _a3  = rpn_mat_get_elt(_mat_y, 3, 1);

     _b1  = rpn_mat_get_elt(_mat_x, 1, 1);
     _b2  = rpn_mat_get_elt(_mat_x, 2, 1);
     _b3  = rpn_mat_get_elt(_mat_x, 3, 1);

     rpn_push(_stack, _a2);
     rpn_push(_stack, _b3);
     my_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_push(_stack, _a3);
     rpn_push(_stack, _b2);
     my_op_generic2(_stack, RPN_OP_DESC(MUL));
     my_op_generic2(_stack, RPN_OP_DESC(SUB));

     rpn_push(_stack, _a3);
     rpn_push(_stack, _b1);
     my_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_push(_stack, _a1);
     rpn_push(_stack, _b3);
     my_op_generic2(_stack, RPN_OP_DESC(MUL));
     my_op_generic2(_stack, RPN_OP_DESC(SUB));

     rpn_push(_stack, _a1);
     rpn_push(_stack, _b2);
     my_op_generic2(_stack, RPN_OP_DESC(MUL));
     rpn_push(_stack, _a2);
     rpn_push(_stack, _b1);
     my_op_generic2(_stack, RPN_OP_DESC(MUL));
     my_op_generic2(_stack, RPN_OP_DESC(SUB));

     my_op_vec3(_stack, RPN_OP_DESC(VEC3));

     rpn_set_lastx(stack, _stk_x);
     _stk_result              = rpn_pop(_stack);

     rpn_free_elt(_stk_x);
     rpn_free_elt(_stk_y);

     rpn_push(stack, _stk_result);

end:
     return _retcode;
}

/******************************************************************************

                              RPN_MAT_GET_I

******************************************************************************/
int rpn_mat_get_i(struct rpn_matrix *matrix, int i, int j)
{
     struct rpn_elt      *_elt;
     int                  _idx;

     _idx                = ((j - 1) * matrix->n) + (i - 1);
     _elt                = (struct rpn_elt *) matrix->base[_idx];

     return _elt->value.i;
}

/******************************************************************************

                              RPN_MAT_GET_ELT

******************************************************************************/
struct rpn_elt *rpn_mat_get_elt(struct rpn_matrix *matrix, int i, int j)
{
     struct rpn_elt      *_elt;
     int                  _idx;

     _idx                = ((j - 1) * matrix->n) + (i - 1);
     _elt                = (struct rpn_elt *) matrix->base[_idx];

     return _elt;
}

/******************************************************************************

                              RPN_OP_SEQ

******************************************************************************/
int rpn_op_seq(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _x, _y, _i;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((rpn_get_type(_stk_x) == RPN_TYPE_INT)
     &&  (rpn_get_type(_stk_y) == RPN_TYPE_INT)) {
          _x                       = _stk_x->value.i;
          _y                       = _stk_y->value.i;

          if (_y <= _x) {
               for (_i = _y; _i <= _x; _i++) {
                    _stk_result              = rpn_new_elt();

                    _stk_result->type        = RPN_TYPE_INT;
                    _stk_result->value.i     = _i;

                    rpn_push(stack, _stk_result);
               }
          }
          else {
               for (_i = _y; _i >= _x; _i--) {
                    _stk_result              = rpn_new_elt();

                    _stk_result->type        = RPN_TYPE_INT;
                    _stk_result->value.i     = _i;

                    rpn_push(stack, _stk_result);
               }
          }

          rpn_set_lastx(stack, _stk_x);
     }
     else {
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
     }

     return _retcode;
}

/******************************************************************************

                              RPN_OP_DIM

******************************************************************************/
int rpn_op_dim(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_dim_row, *_dim_col;
     int                       _x, _row, _col;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     switch (rpn_get_type(_stk_x)) {

     case RPN_TYPE_NIL:
          _row                     = 0;
          _col                     = 0;

          _dim_row                 = rpn_new_elt();
          _dim_row->type           = RPN_TYPE_INT;
          _dim_row->value.i        = _row;
          rpn_push(stack, _dim_row);

          _dim_col                 = rpn_new_elt();
          _dim_col->type           = RPN_TYPE_INT;
          _dim_col->value.i        = _col;
          rpn_push(stack, _dim_col);

          rpn_set_lastx(stack, _stk_x);
          break;

     case RPN_TYPE_MATRIX:
          {
               struct rpn_matrix        *_mat_A;
               int                       _idx, _sum, _size;
               struct rpn_elt           *_elt, *_nil;
               struct rpn_stack         *_stack;

               _mat_A                   = (struct rpn_matrix *) _stk_x->value.obj;

               _row                     = _mat_A->n;
               _col                     = _mat_A->p;

               _dim_row                 = rpn_new_elt();
               _dim_row->type           = RPN_TYPE_INT;
               _dim_row->value.i        = _row;
               rpn_push(stack, _dim_row);

               _dim_col                 = rpn_new_elt();
               _dim_col->type           = RPN_TYPE_INT;
               _dim_col->value.i        = _col;
               rpn_push(stack, _dim_col);

               rpn_set_lastx(stack, _stk_x);
          }
          break;

     case RPN_TYPE_INT:
     case RPN_TYPE_FLOAT:
          _row                     = 1;
          _col                     = 1;

          _dim_row                 = rpn_new_elt();
          _dim_row->type           = RPN_TYPE_INT;
          _dim_row->value.i        = _row;
          rpn_push(stack, _dim_row);

          _dim_col                 = rpn_new_elt();
          _dim_col->type           = RPN_TYPE_INT;
          _dim_col->value.i        = _col;
          rpn_push(stack, _dim_col);

          rpn_set_lastx(stack, _stk_x);
          break;

     default:
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
          break;
     }

     return _retcode;
}

#if 0
/******************************************************************************

                              RPN_OP_POWER

******************************************************************************/
int rpn_op_power(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result;
     int                       _x, _y, _result, _lg;
     double                    _xf, _yf, _resultf;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     if ((rpn_get_type(_stk_x) == RPN_TYPE_INT)
     &&  (rpn_get_type(_stk_y) == RPN_TYPE_INT)) {
          _x                       = _stk_x->value.i;
          _y                       = _stk_y->value.i;

          _result                  = (int) pow((double) _y, (double) _x);

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_INT;
          _stk_result->value.i     = _result;

          rpn_push(stack, _stk_result);
     }
     else if ((rpn_get_type(_stk_x) == RPN_TYPE_FLOAT)
          &&  (rpn_get_type(_stk_y) == RPN_TYPE_FLOAT)) {
          _xf                      = _stk_x->value.f;
          _yf                      = _stk_y->value.f;

          _resultf                 = pow(_yf, _xf);

          rpn_set_lastx(stack, _stk_x);

          _stk_result              = rpn_new_elt();

          _stk_result->type        = RPN_TYPE_FLOAT;
          _stk_result->value.f     = _resultf;

          rpn_push(stack, _stk_result);
     }
     else {
          rpn_push(stack, _stk_x);

          _retcode                 = RPN_RET_INVALID_TYPES;
     }

     return _retcode;
}
#endif

/******************************************************************************

                              RPN_OP_LOG10

******************************************************************************/
int rpn_op_log10(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xf, _resultf;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}

/******************************************************************************

                              RPN_OP_10_POWER_X

******************************************************************************/
int rpn_op_10_power_x(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xf, _resultf;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}

/******************************************************************************

                              RPN_OP_PERCENT

******************************************************************************/
int rpn_op_percent(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xf, _resultf;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}

/******************************************************************************

                              RPN_OP_SUM_ADD

******************************************************************************/
int rpn_op_sum_add(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_y, *_stk_result,
                              *_sum_x, *_sum_x2,
                              *_sum_y, *_sum_y2,
                              *_sum_xy, *_N, *_constant;
     rpn_stack                *_stack;
     int                       _retcode;
     struct rpn_operator      *_op;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);
     _stk_y                   = rpn_pop(stack);

     _stack                   = rpn_new_stack("sigma+ sum(x)");

     /* Sum(x)
        ~~~~~~ */
     _sum_x                   = G.sigma->reg[RPN_SIGMA_IDX_SUM_X];
     rpn_push(_stack, _sum_x);
     rpn_push(_stack, _stk_x);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);

     /* Sum(x^2)
        ~~~~~~ */
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     _sum_x2                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_X2];
     rpn_push(_stack, _sum_x2);
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));

     _op                      = RPN_OP_DESC(MATH_MUL);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_X2]);
     rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));

     /* Sum(y)
        ~~~~~~ */
     _sum_y                   = G.sigma->reg[RPN_SIGMA_IDX_SUM_Y];
     rpn_push(_stack, _sum_y);
     rpn_push(_stack, _stk_y);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);

     /* Sum(y^2)
        ~~~~~~ */
     rpn_op_swap_xy(_stack, RPN_OP_DESC(SWAP_XY));
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     _sum_y2                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2];
     rpn_push(_stack, _sum_y2);
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));
     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));

     _op                      = RPN_OP_DESC(MATH_MUL);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2]);
     rpn_op_roll_down(_stack, RPN_OP_DESC(RDN));

     /* Sum(x*y)
        ~~~~~~~~ */
     _op                      = RPN_OP_DESC(MATH_MUL);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _sum_xy                  = G.sigma->reg[RPN_SIGMA_IDX_SUM_XY];
     rpn_push(_stack, _sum_xy);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_SUM_XY]);

     /* N
        ~ */
     _N                       = G.sigma->reg[RPN_SIGMA_IDX_N];
     rpn_push(_stack, _N);
     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     _op                      = RPN_OP_DESC(ADD);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }
     rpn_sto(_stack, &G.sigma->reg[RPN_SIGMA_IDX_N]);

rpn_disp_sigma();

     rpn_op_clst(_stack, NULL);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/******************************************************************************

                              RPN_OP_SUM_SUB

******************************************************************************/
int rpn_op_sum_sub(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xf, _resultf;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}

/******************************************************************************

                              RPN_OP_MEAN

******************************************************************************/
int rpn_op_mean(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result,
                              *_sum_x, *_sum_y, *_N;
     rpn_stack                *_stack;
     int                       _retcode;
     struct rpn_operator      *_op;

     _retcode                 = RPN_RET_OK;

     _stack                   = rpn_new_stack("mean");

     /* Y mean
        ~~~~~~ */
     _sum_y                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);
     rpn_push(_stack, _sum_y);

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     /* X mean
        ~~~~~ */
     _sum_x                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);
     rpn_push(_stack, _sum_x);

     rpn_op_lastx(_stack, RPN_OP_DESC(LASTX));

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/******************************************************************************

                              RPN_OP_SDEV

******************************************************************************/
int rpn_op_sdev(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result,
                              *_sum_x, *_sum_y,
                              *_sum_x2, *_sum_y2, *_N,
                              *_constant;
     rpn_stack                *_stack;
     struct rpn_operator      *_op;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stack                   = rpn_new_stack("sdev");

     /* Y sdev
        ~~~~~~ */
     _sum_y2                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y2]);
     rpn_push(_stack, _sum_y2);

     _sum_y                    = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_Y]);
     rpn_push(_stack, _sum_y);

     _op                      = RPN_OP_DESC(SQUARE);
     if ((_retcode = my_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SQRT);
     if ((_retcode = my_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     /* X sdev
        ~~~~~~ */
     _sum_x2                   = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X2]);
     rpn_push(_stack, _sum_x2);

     _sum_x                    = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_SUM_X]);
     rpn_push(_stack, _sum_x);

     _op                      = RPN_OP_DESC(SQUARE);
     if ((_retcode = my_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _N                       = rpn_clone_elt(G.sigma->reg[RPN_SIGMA_IDX_N]);
     rpn_push(_stack, _N);

     _constant                = rpn_new_int(1);
     rpn_push(_stack, _constant);

     _op                      = RPN_OP_DESC(SUB);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(DIV);
     if ((_retcode = my_op_generic2(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _op                      = RPN_OP_DESC(SQRT);
     if ((_retcode = my_op_generic1(_stack, _op)) != 0) {
          rpn_err_msg_op_error(_op->op_name, _retcode);
          exit(RPN_EXIT_OP_ERROR);
     }

     _stk_result              = rpn_pop(_stack);
     rpn_push(stack, _stk_result);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/******************************************************************************

                              RPN_OP_SCALAR_MATRIX

******************************************************************************/
int rpn_op_scalar_matrix(rpn_stack *stack, rpn_operator *op)
{
     rpn_elt                  *_stk_x, *_stk_result;
     int                       _x, _result, _lg;
     double                    _xf, _resultf;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     _stk_x                   = rpn_pop(stack);

     rpn_push(stack, _stk_x);

     _retcode                 = RPN_RET_INVALID_TYPES;

     return _retcode;
}

/******************************************************************************

                              RPN_STO

******************************************************************************/
void rpn_sto(rpn_stack *stack, struct rpn_elt **dest_elt)
{
     struct rpn_elt           *_stk_x, *_X_clone;

     /* Pop X, clone element, Push X
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     _stk_x                   = rpn_pop(stack);
     _X_clone                 = rpn_clone_elt(_stk_x);
     rpn_push(stack, _stk_x);
     if (*dest_elt != NULL) {
          /* Free previous content
             ~~~~~~~~~~~~~~~~~~~~~ */
          rpn_free_elt(*dest_elt);
     }

     /* Store clone address into destination register
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     *dest_elt                = _X_clone;
}
