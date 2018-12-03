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
 *   rpn_utils.c
 *
 *   @(#)  [MB] cy_rpn_utils.c Version 1.15 du 18/09/17 - 
 */

#include  "rpn_header.h"
#include  "rpn_epri.h"
#include  "rpn_proto.h"

struct global_struct     G         = { 0 };

struct rpn_st_types       op_types_gen_math1[] = {
     T(1, 1, INT,        UNUSED),
     T(1, 1, FLOAT,      UNUSED),
     T(1, 1, MATRIX,     UNUSED),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_sqrt[] = {
     T(1, 1, INT,        UNUSED),
     T(1, 1, FLOAT,      UNUSED),
     T(1, 1, MATRIX,     UNUSED),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_add[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        FLOAT),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, FLOAT,      INT),
     T(2, 1, FLOAT,      FLOAT),
     T(2, 1, FLOAT,      VECTOR_3),
     T(2, 1, FLOAT,      MATRIX),

     T(2, 1, STRING,     STRING),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   FLOAT),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     FLOAT),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),
     T(2, 1, LITTERAL,   LITTERAL),

     T(2, 1, NIL,        ANY_TYPE),
     T(2, 1, ANY_TYPE,   NIL),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_sub[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        FLOAT),
     T(2, 1, INT,        STRING),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, FLOAT,      INT),
     T(2, 1, FLOAT,      FLOAT),
     T(2, 1, FLOAT,      VECTOR_3),
     T(2, 1, FLOAT,      MATRIX),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   FLOAT),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     FLOAT),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_mul[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        FLOAT),
     T(2, 1, INT,        STRING),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, FLOAT,      INT),
     T(2, 1, FLOAT,      FLOAT),
     T(2, 1, FLOAT,      VECTOR_3),
     T(2, 1, FLOAT,      MATRIX),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   FLOAT),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     FLOAT),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),
     T(2, 1, LITTERAL,   LITTERAL),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_div[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        FLOAT),
     T(2, 1, INT,        STRING),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, FLOAT,      INT),
     T(2, 1, FLOAT,      FLOAT),
     T(2, 1, FLOAT,      VECTOR_3),
     T(2, 1, FLOAT,      MATRIX),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   FLOAT),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     FLOAT),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_delta_percent[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        FLOAT),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
     T(2, 1, INT,        LITTERAL),

     T(2, 1, FLOAT,      INT),
     T(2, 1, FLOAT,      FLOAT),
     T(2, 1, FLOAT,      VECTOR_3),
     T(2, 1, FLOAT,      MATRIX),

     T(2, 1, STRING,     STRING),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   FLOAT),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     FLOAT),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

     T(2, 1, LITTERAL,   INT),
     T(2, 1, LITTERAL,   LITTERAL),

     T(2, 1, NIL,        ANY_TYPE),
     T(2, 1, ANY_TYPE,   NIL),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

struct rpn_st_types       op_types_power[] = {
     T(2, 1, INT,        INT),
     T(2, 1, INT,        FLOAT),
     T(2, 1, INT,        VECTOR_3),
     T(2, 1, INT,        MATRIX),
//   T(2, 1, INT,        LITTERAL),

     T(2, 1, FLOAT,      INT),
     T(2, 1, FLOAT,      FLOAT),
     T(2, 1, FLOAT,      VECTOR_3),
     T(2, 1, FLOAT,      MATRIX),

//   T(2, 1, STRING,     STRING),

     T(2, 1, VECTOR_3,   INT),
     T(2, 1, VECTOR_3,   FLOAT),
     T(2, 1, VECTOR_3,   VECTOR_3),
     T(2, 1, VECTOR_3,   MATRIX),

     T(2, 1, MATRIX,     INT),
     T(2, 1, MATRIX,     FLOAT),
     T(2, 1, MATRIX,     VECTOR_3),
     T(2, 1, MATRIX,     MATRIX),

//   T(2, 1, LITTERAL,   INT),
//   T(2, 1, LITTERAL,   LITTERAL),

     T(2, 1, NIL,        ANY_TYPE),
     T(2, 1, ANY_TYPE,   NIL),

     T(0, 0, UNDEFINED,  UNDEFINED)
};

/******************************************************************************

                         RPN_NEW_STACK

******************************************************************************/
rpn_stack *rpn_new_stack(const char *stack_name)
{
     rpn_stack           *_stack;

     if ((_stack = (rpn_stack *) malloc(sizeof(*_stack))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _stack->name        = strdup(stack_name);
     _stack->num_elts    = 0;
     _stack->lastx       = rpn_new_elt();
     _stack->clear_x     = FALSE;
     _stack->new_value   = FALSE;
     rpn_set_type(_stack->lastx, RPN_TYPE_INT);
     _stack->base_elt    = 0;
     _stack->top_elt     = 0;

     return _stack;
}

/******************************************************************************

                         RPN_NEW_ELT

******************************************************************************/
rpn_elt *rpn_new_elt()
{
     rpn_elt             *_elt;

     if ((_elt = (rpn_elt *) malloc(sizeof(*_elt))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _elt->type          = RPN_TYPE_UNDEFINED;
     _elt->value.i       = 0;
     _elt->previous_elt  = NULL;
     _elt->next_elt      = NULL;

     return _elt;
}

/******************************************************************************

                         RPN_FREE_ELT

******************************************************************************/
void rpn_free_elt(rpn_elt *elt)
{
     int                  _type;

     switch ((_type = rpn_get_type(elt))) {

     case RPN_TYPE_INT:
     case RPN_TYPE_FLOAT:
     case RPN_TYPE_STRING:
          free(elt);
          break;

     case RPN_TYPE_MATRIX:
     default:
//          fprintf(stderr, "%s : (%s) element type %d : not managed yet !\n",
//                  G.progname, __func__, _type);
          break;
     }
}

/******************************************************************************

                         RPN_NEW_INT

******************************************************************************/
rpn_elt *rpn_new_int(int i)
{
     rpn_elt             *_elt;

     _elt                = rpn_new_elt();
     _elt->type          = RPN_TYPE_INT;
     _elt->value.i       = i;

     return _elt;
}

/******************************************************************************

                         RPN_CLONE_ELT

******************************************************************************/
rpn_elt *rpn_clone_elt(rpn_elt *elt)
{
     rpn_elt             *_clone;
     int                  _idx;

     _clone              = rpn_new_elt();
     _clone->type        = elt->type;
     _clone->value.f     = elt->value.f;

     switch (elt->type) {

     case RPN_TYPE_MATRIX:
          {
               struct rpn_elt           *_sub_elt, *_clone_sub;
               struct rpn_matrix        *_mat, *_clone_mat;
               int                       _size;

               _mat                = (struct rpn_matrix *) elt->value.obj;
               _size               = sizeof(struct rpn_matrix)
                                   + (((_mat->n *_mat->p) - 1) * sizeof(struct rpn_elt *));
               _clone->value.obj   = malloc(_size);
               _clone_mat          = _clone->value.obj;
               _clone_mat->n       = _mat->n;
               _clone_mat->p       = _mat->p;

               for (_idx = 0; _idx < (_mat->n * _mat->p); _idx++) {
                    _sub_elt                 = (struct rpn_elt *) _mat->base[_idx];
                    _clone_sub               = rpn_clone_elt(_sub_elt);
                    _clone_mat->base[_idx]   = _clone_sub;
               }
          }
          break;

     case RPN_TYPE_NIL:
     case RPN_TYPE_INT:
     case RPN_TYPE_FLOAT:
     case RPN_TYPE_STRING:
     case RPN_TYPE_LITTERAL:
          break;

     default:
          fprintf(stderr, "%s : unknown type (%d)\n", __func__, elt->type);
          break;
     }

     return _clone;
}

/******************************************************************************

                         RPN_PUSH

******************************************************************************/
int rpn_push(rpn_stack *stack, rpn_elt *elt)
{
     int             _retcode;
     rpn_elt        *_stk_x;

     /* Consistency check
        ~~~~~~~~~~~~~~~~~ */
     if ((elt->previous_elt != 0)
     ||  (elt->next_elt     != 0)) {
          RPN_INTERNAL_ERROR;
     }

     if (stack != 0) {
          if (stack->clear_x) {
               stack->clear_x      = FALSE;
               _stk_x         = rpn_pop(stack);
               rpn_free_elt(_stk_x);
          }

          if (stack->base_elt == 0) {
               stack->base_elt          = elt;
               stack->top_elt           = elt;

               elt->previous_elt        = 0;
               elt->next_elt            = 0;
          }
          else {
               stack->top_elt->next_elt = elt;

               elt->previous_elt        = stack->top_elt;
               elt->next_elt            = 0;

               stack->top_elt           = elt;
          }
          stack->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_STACK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_POP

******************************************************************************/
rpn_elt *rpn_pop(rpn_stack *stack)
{
     rpn_elt        *_elt     = 0;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt                     = stack->top_elt;
          stack->top_elt           = _elt->previous_elt;
          if (stack->top_elt != 0) {
               stack->top_elt->next_elt = 0;
          }
          else {
               stack->base_elt     = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          stack->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt();
//        rpn_set_type(_elt, RPN_TYPE_INT);
          rpn_set_type(_elt, RPN_TYPE_NIL);
     }

     return _elt;
}

/******************************************************************************

                         RPN_BASE_PUSH

******************************************************************************/
int rpn_base_push(rpn_stack *stack, rpn_elt *elt)
{
     int             _retcode;

     if (stack != 0) {
          if (stack->base_elt == 0) {
               stack->base_elt               = elt;
               stack->top_elt                = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = 0;
          }
          else {
               stack->base_elt->previous_elt = elt;

               elt->previous_elt             = 0;
               elt->next_elt                 = stack->base_elt;

               stack->base_elt                = elt;
          }
          stack->num_elts++;
          _retcode                 = RPN_RET_OK;
     }
     else {
          _retcode                 = RPN_RET_INVALID_STACK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_BASE_POP

******************************************************************************/
rpn_elt *rpn_base_pop(rpn_stack *stack)
{
     rpn_elt        *_elt     = 0;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt                     = stack->base_elt;
          stack->base_elt          = _elt->next_elt;
          if (stack->base_elt != 0) {
               stack->base_elt->previous_elt = 0;
          }
          else {
               stack->top_elt      = 0;
          }

          _elt->previous_elt       = 0;
          _elt->next_elt           = 0;

          stack->num_elts--;
     }
     else {
          _elt                     = rpn_new_elt();
// TODO : Remplacer type INT par NIL ???
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
          rpn_set_type(_elt, RPN_TYPE_INT);
     }

     return _elt;
}

/******************************************************************************

                         RPN_ROLL_DOWN

******************************************************************************/
int rpn_op_roll_down(rpn_stack *stack, rpn_operator *op)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_pop(stack);
     rpn_base_push(stack, _elt);
     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/******************************************************************************

                         RPN_ROLL_UP

******************************************************************************/
int rpn_op_roll_up(rpn_stack *stack, rpn_operator *op)
{
     int                       _retcode;
     struct rpn_elt           *_elt;

     _elt                     = rpn_base_pop(stack);
     rpn_push(stack, _elt);

     _retcode                 = RPN_RET_OK;

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XY

******************************************************************************/
int rpn_op_swap_xy(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_stk_y;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 2) {
          _stk_x        = rpn_pop(stack);
          _stk_y        = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_y);
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XZ

******************************************************************************/
int rpn_op_swap_xz(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_stk_y,
                              *_stk_z;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 3) {
          _stk_x    = rpn_pop(stack);
          _stk_y    = rpn_pop(stack);
          _stk_z    = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_z);
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XT

******************************************************************************/
int rpn_op_swap_xt(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_stk_y,
                              *_stk_z, *_stk_t;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 4) {
          _stk_x    = rpn_pop(stack);
          _stk_y    = rpn_pop(stack);
          _stk_z    = rpn_pop(stack);
          _stk_t    = rpn_pop(stack);

          rpn_push(stack, _stk_x);
          rpn_push(stack, _stk_z);
          rpn_push(stack, _stk_y);
          rpn_push(stack, _stk_t);
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_SWAP_XL

******************************************************************************/
int rpn_op_swap_xl(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);

          rpn_push(stack, stack->lastx);
          stack->lastx   = _stk_x;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP

******************************************************************************/
int rpn_op(rpn_stack *stack, rpn_operator *op)
{
     int                       _retcode;

     stack->clear_x      = FALSE;

     if (stack->new_value) {
          rpn_disp_elt(stack->top_elt, FALSE, FALSE);
          printf("%15s\n", op->op_name);
     }
     else {
          rpn_disp_elt(NULL, FALSE, FALSE);
          printf("%15s\n", op->op_name);
     }
     stack->new_value    = FALSE;

     _retcode            =  (*op->func)(stack, op);

     if (op->nb_args_out > 0) {
          rpn_disp_elt(stack->top_elt, FALSE, FALSE);
          printf("%15s\n", "***");
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_ENTER

******************************************************************************/
int rpn_op_enter(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);
          _elt      = rpn_clone_elt(_stk_x);
          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
     }

     stack->clear_x = TRUE;

     return _retcode;
}

/******************************************************************************

                         RPN_OP_CLX

******************************************************************************/
int rpn_op_clx(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_nil;

     _stk_x         = rpn_pop(stack);
     rpn_free_elt(_stk_x);

     _nil           = rpn_new_elt();
     rpn_set_type(_nil, RPN_TYPE_NIL);
     rpn_push(stack, _nil);

     stack->clear_x = TRUE;

     return RPN_RET_OK;
}

/******************************************************************************

                         RPN_OP_DELX

******************************************************************************/
int rpn_op_delx(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_nil;

     _stk_x         = rpn_pop(stack);
     rpn_free_elt(_stk_x);

     return RPN_RET_OK;
}

/******************************************************************************

                         RPN_OP_CLST

******************************************************************************/
int rpn_op_clst(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_nil;

// TODO : Revoir la liberation complete de la pile : dernier elt non libere ?
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     for (_stk_x = rpn_pop(stack); stack->num_elts != 0;_stk_x = rpn_pop(stack)) {
          rpn_free_elt(_stk_x);
     }

     _nil           = rpn_new_elt();
     rpn_set_type(_nil, RPN_TYPE_NIL);
     rpn_push(stack, _nil);

     stack->clear_x = TRUE;

     return RPN_RET_OK;
}

/******************************************************************************

                         RPN_OP_PRX

******************************************************************************/
int rpn_op_prx(rpn_stack *stack, rpn_operator *op)
{
     rpn_disp_elt(stack->top_elt, FALSE, FALSE);
     printf("%15s\n", op->op_name);

     return RPN_RET_OK;
}

/******************************************************************************

                         RPN_OP_PRSTK

******************************************************************************/
int rpn_op_prstk(rpn_stack *stack, rpn_operator *op)
{
     /* Display stack
        ~~~~~~~~~~~~~ */
     rpn_disp_stack(stack);

     return RPN_RET_OK;
}

/******************************************************************************

                         RPN_SET_LASTX

******************************************************************************/
void rpn_set_lastx(rpn_stack *stack, rpn_elt *elt)
{
     if (stack->lastx) {
          rpn_free_elt(stack->lastx);
     }

     stack->lastx        = rpn_clone_elt(elt);
}

/******************************************************************************

                         RPN_OP_LASTX

******************************************************************************/
int rpn_op_lastx(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          rpn_push(stack, rpn_clone_elt(stack->lastx));
     }

     return _retcode;
}

/******************************************************************************

                         RPN_OP_RCL_X

******************************************************************************/
int rpn_op_rcl_x(rpn_stack *stack, rpn_operator *op)
{
     struct rpn_elt           *_stk_x, *_elt;
     int                       _retcode;

     _retcode                 = RPN_RET_OK;

     if (stack->num_elts >= 1) {
          _stk_x    = rpn_pop(stack);
          _elt      = rpn_clone_elt(_stk_x);
          rpn_push(stack, _stk_x);
          rpn_push(stack, _elt);
     }

     return _retcode;
}

/******************************************************************************

                         RPN_GET_TYPE

******************************************************************************/
int rpn_get_type(rpn_elt *elt)
{
     int        _retcode;

     if (elt) {
          _retcode  = elt->type;
     }
     else {
          _retcode  = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_SET_TYPE

******************************************************************************/
int rpn_set_type(rpn_elt *elt, int type)
{
     int             _retcode;

     if (elt) {
          elt->type      = type;
          _retcode       = RPN_RET_OK;
     }
     else {
          _retcode       = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_GET_X_TYPE

******************************************************************************/
int rpn_get_X_type(struct rpn_stack *stack)
{
     int             _retcode;
     struct rpn_elt *_elt;

     if ((stack != 0) && (stack->num_elts != 0)) {
          _elt             = stack->top_elt;
          _retcode         = _elt->type;
     }
     else {
          _retcode       = RPN_RET_INVALID_ELT;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_GET_Y_TYPE

******************************************************************************/
int rpn_get_Y_type(struct rpn_stack *stack)
{
     int             _retcode;
     struct rpn_elt *_elt;

     if ((stack != 0) && (stack->num_elts >= 2)) {
          _elt             = stack->top_elt->previous_elt;
          _retcode         = _elt->type;
     }
     else {
          _retcode       = RPN_TYPE_NIL;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_SEARCH_OP

******************************************************************************/
rpn_operator *rpn_search_op(rpn_operator *operators, char *operator)
{
     rpn_operator        *_op;

     for (_op = operators; _op->func != 0; _op++) {
          if (!strcmp(_op->op_name, operator)) {
               /* Operator found
                  ~~~~~~~~~~~~~~ */
               break;
          }
     }

     if (_op->func == 0) {
          _op       = 0;
     }

     return _op;
}

/******************************************************************************

                         RPN_CATALOG

******************************************************************************/
int rpn_catalog(struct rpn_operator *ops)
{
     struct rpn_operator      *_op;
     char                     *_fmt_s, *_fmt_d;

     _fmt_s    = "%-25s %10s %10s\n";
     _fmt_d    = "%-25s %10d %10d\n";

     printf(_fmt_s, "=== OPERATOR ===", "IN PARAMS", "OUT PARAMS");
     for (_op = ops; (_op->op_id != 0) || (_op == ops); _op++) {
          printf(_fmt_d, _op->op_name, _op->nb_args_in, _op->nb_args_out);
     }
}

/******************************************************************************

                         RPN_ERR_MSG_NO_MEM

******************************************************************************/
void rpn_err_msg_no_mem()
{
     fprintf(stderr, "%s: not enough memory !\n", G.progname);
}

/******************************************************************************

                         RPN_ERR_MSG_OP_NOT_FOUND

******************************************************************************/
void rpn_err_msg_op_not_found(char *operator)
{
     fprintf(stderr, "%s: operator \"%s\" not found !\n", G.progname, operator);
}

/******************************************************************************

                         RPN_ERR_MSG_OP_ERROR

******************************************************************************/
void rpn_err_msg_op_error(char *operator, int retcode)
{
     fprintf(stderr, "%s: operator \"%s\" : %s !\n",
             G.progname, operator, rpn_err_to_string(retcode));
}

/******************************************************************************

                         RPN_ERR_TO_STRING

******************************************************************************/
char *rpn_err_to_string(int errcode)
{
     static char          _mesg[32];
     char                *_err_mesg;

     switch (errcode) {

     case RPN_RET_INVALID_ELT:
          _err_mesg      =  "invalid element";
          break;

     case RPN_RET_ERROR:
          _err_mesg      = "error";
          break;

     case RPN_RET_OK:
          _err_mesg      = "OK";
          break;

     case RPN_RET_INVALID_STACK:
          _err_mesg      = "invalid stack";
          break;

     case RPN_RET_INVALID_STR:
          _err_mesg      = "invalid string";
          break;

     case RPN_RET_INVALID_X:
          _err_mesg      = "invalid X";
          break;

     case RPN_RET_INVALID_Y:
          _err_mesg      = "invalid Y";
          break;

     case RPN_RET_TOO_BIG:
          _err_mesg      = "too big";
          break;

     case RPN_RET_INVALID_TYPES:
          _err_mesg      = "invalid types";
          break;

     case RPN_RET_NOT_ENOUGH_ELTS:
          _err_mesg      = "not enough elements";
          break;

     case RPN_RET_INCOMPATIBLE_ELTS:
          _err_mesg      = "incompatible elements";
          break;

     case RPN_RET_INVALID_X_TYPE:
          _err_mesg      = "invalid X type";
          break;

     case RPN_RET_INVALID_Y_TYPE:
          _err_mesg      = "invalid Y type";
          break;

     default:
          sprintf(_mesg, "UNKNOWN ERROR CODE (%d)", errcode);
          _err_mesg      = _mesg;
          break;
     }

     return _err_mesg;
}

/******************************************************************************

                         RPN_ERR_MSG_INVALID_STRING

******************************************************************************/
void rpn_err_msg_invalid_string(char *str)
{
     fprintf(stderr, "%s: invalid string (%s)\n", G.progname, str);
}

/******************************************************************************

                         RPN_INTERNAL_ERROR

******************************************************************************/
void rpn_internal_error(const char *funct, char *file, int line)
{
     fprintf(stderr, "%s: INTERNAL ERROR !!! (%s() : %s(%d))\n",
             G.progname, funct, file, line);
     exit(RPN_EXIT_INTERNAL_ERR);
}
/******************************************************************************

                         RPN_STRIP_QUOTES

******************************************************************************/
int rpn_strip_quotes(char **str)
{
     int                 _lg, _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '"' || (*str)[_lg - 1] != '"') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;

          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_STRIP_SINGLEQUOTES

******************************************************************************/
int rpn_strip_single_quotes(char **str)
{
     int                 _lg, _retcode;
     char                *_new_str;

     _lg            = strlen(*str);
     if ((*str)[0] != '\'' || (*str)[_lg - 1] != '\'') {
          _retcode       = RPN_RET_INVALID_STR;
     }
     else {
          _new_str            = strdup(*str);
          _new_str[_lg - 1]   = 0;
          _new_str++;

          *str                = _new_str;

          _retcode            = RPN_RET_OK;
     }

     return _retcode;
}

/******************************************************************************

                         RPN_CHECK_TYPES

******************************************************************************/
int rpn_check_types(rpn_operator *op, struct rpn_stack *stack)
{
     struct rpn_st_types      *_t;
     int                       _X_type = -1, _Y_type   = -1,
                               _retcode;
     bool                      _good_X = FALSE, _good_Y = FALSE;

     _retcode       = RPN_RET_INVALID_TYPES;

     _t             = op->st_types;
     if (_t->nb_operands != 0) {
          switch (_t->nb_operands) {

          case 1:
               _X_type   = rpn_get_X_type(stack);

               for (_t = op->st_types; _t->nb_operands != 0; _t++) {
//fprintf(stderr, "_X_type = %d  _t->X_type = %d\n", _X_type, _t->X_type);
                    if ((_X_type == _t->X_type)
                    || (_t->X_type == RPN_TYPE_ANY_TYPE)) {
                         /* X type validated
                            ~~~~~~~~~~~~~~~~ */
                         _retcode       = RPN_RET_OK;
                         goto end;
                    }
               }

               _retcode       = RPN_RET_INVALID_X_TYPE;
               break;

          case 2:
               _X_type   = rpn_get_X_type(stack);
               _Y_type   = rpn_get_Y_type(stack);
// fprintf(stderr, "X_type = %s\n", rpn_type_to_string(_X_type));
// fprintf(stderr, "Y_type = %s\n", rpn_type_to_string(_Y_type));

               for (_t = op->st_types; _t->nb_operands != 0; _t++) {
                    if ((_X_type == _t->X_type)
                    ||  (_t->X_type == RPN_TYPE_ANY_TYPE)) {
                         /* X type validated
                            ~~~~~~~~~~~~~~~~ */
                         _good_X        = TRUE;

                         if ((_Y_type == _t->Y_type)
                         ||  (_t->Y_type == RPN_TYPE_ANY_TYPE)) {
                              /* Y type validated
                                 ~~~~~~~~~~~~~~~~ */
                              _retcode       = RPN_RET_OK;
                              goto end;
                         }
                    }
               }

               if (_good_X) {
                    /* If X has a good type, then Y has not
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
                    _retcode       = RPN_RET_INVALID_Y_TYPE;
               }
               else {
                    _retcode       = RPN_RET_INVALID_X_TYPE;
               }
               break;

          default:
               RPN_INTERNAL_ERROR;
               break;
          }

     }
     else {
          _retcode       = RPN_RET_OK;
     }

end:
     if (_retcode != RPN_RET_OK) {
          fprintf(stderr, "%s: authorized types for %s :\n",
                  G.progname, op->op_name);

          for (_t = op->st_types; _t->nb_operands != 0; _t++) {
               switch (_t->nb_operands) {

               case 1:
                    fprintf(stderr, "     %s\n", rpn_type_to_string(_t->X_type));
                    break;

               case 2:
                    fprintf(stderr, "     %-15s %-15s\n",
                            rpn_type_to_string(_t->X_type),
                            rpn_type_to_string(_t->Y_type));
                    break;

               default:
                    RPN_INTERNAL_ERROR;
                    break;
               }
          }
     }
     return _retcode;
}

/******************************************************************************

                         RPN_TYPE_TO_STRING

******************************************************************************/
char *rpn_type_to_string(int type)
{
     char                     *_str_type;
     static char               _err_mesg[32];

     switch (type) {

     case RPN_TYPE_ANY_TYPE:
          _str_type      = "ANY_TYPE";
          break;

     case RPN_TYPE_UNUSED:
          _str_type      = "UNUSED";
          break;

     case RPN_TYPE_UNDEFINED:
          _str_type      = "UNDEFINED";
          break;

     case RPN_TYPE_NIL:
          _str_type      = "NIL";
          break;

     case RPN_TYPE_INT:
          _str_type      = "INT";
          break;

     case RPN_TYPE_FLOAT:
          _str_type      = "FLOAT";
          break;

     case RPN_TYPE_STRING:
          _str_type      = "STRING";
          break;

     case RPN_TYPE_VECTOR_3:
          _str_type      = "VECTOR_3";
          break;

     case RPN_TYPE_MATRIX:
          _str_type      = "MATRIX";
          break;

     case RPN_TYPE_LITTERAL:
          _str_type      = "LITTERAL";
          break;

     default:
          sprintf(_err_mesg, "??? (%d)", type);
          _str_type      = _err_mesg;
          break;
     }

     return _str_type;
}

/******************************************************************************

                         RPN_CHECK_OPS

******************************************************************************/
void rpn_check_ops()
{
     int                       _i, _error;
     rpn_operator             *_op;

     _error         = FALSE;

     for (_i = 0; my_operators[_i].func != 0; _i++) {
          _op            = &my_operators[_i];

          if (_op->op_id != _i) {
               _error         = TRUE;
               fprintf(stderr, "*** Inconsistency : %-15s : ID = %3d, should be %3d !\n",
                       _op->op_name, _op->op_id, _i);
          }
     }

     if (_error) {
          RPN_INTERNAL_ERROR;
     }
}

/******************************************************************************

                         RPN_NEW_SIGMA

******************************************************************************/
struct rpn_sigma *rpn_new_sigma(char *name)
{
     struct rpn_sigma         *_sigma;
     int                       _i;

     if ((_sigma = (struct rpn_sigma *) malloc(sizeof(*_sigma))) == 0) {
          rpn_err_msg_no_mem();
          exit(RPN_EXIT_NO_MEM);
     }

     _sigma->sigma_name       = strdup(name);

     for (_i = 0; _i < RPN_NB_SIGMA; _i++) {
          _sigma->reg[_i]          = rpn_new_elt();
          rpn_set_type(_sigma->reg[_i], RPN_TYPE_INT);
     }

     return _sigma;
}

