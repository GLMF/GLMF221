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
 *   rpn_header.h
 *
 *   @(#)  [MB] cy_rpn_header.h Version 1.15 du 18/06/25 - 
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  "rpn_types.h"

#define   X    fprintf(stderr, "%s (%d)\n", __FILE__, __LINE__);

#define   RPN_INTERNAL_ERROR       rpn_internal_error(__func__, __FILE__, __LINE__)

/* Boolean values
   ~~~~~~~~~~~~~~ */
#define   FALSE                      (0)
#define   TRUE                       (1)

/* Exit codes
   ~~~~~~~~~~ */
#define   RPN_EXIT_OK                (0)
#define   RPN_EXIT_USAGE             (1)
#define   RPN_EXIT_NO_MEM            (2)
#define   RPN_EXIT_OP_UNKNOWN        (3)
#define   RPN_EXIT_OP_ERROR          (4)
#define   RPN_EXIT_INVALID_ELT       (5)
#define   RPN_EXIT_SYNTAX            (6)
#define   RPN_EXIT_INTERNAL_ERR    (126)

/* Return codes
   ~~~~~~~~~~~~ */
#define   RPN_RET_INVALID_ELT       (-2)
#define   RPN_RET_ERROR             (-1)
#define   RPN_RET_OK                 (0)
#define   RPN_RET_INVALID_STACK      (1)
#define   RPN_RET_INVALID_STR        (2)
#define   RPN_RET_INVALID_X          (3)
#define   RPN_RET_INVALID_Y          (4)
#define   RPN_RET_TOO_BIG            (5)
#define   RPN_RET_INVALID_TYPES      (6)
#define   RPN_RET_NOT_ENOUGH_ELTS    (7)
#define   RPN_RET_INCOMPATIBLE_ELTS  (8)
#define   RPN_RET_INVALID_X_TYPE     (9)
#define   RPN_RET_INVALID_Y_TYPE    (10)

/* Arguments types
   ~~~~~~~~~~~~~~~ */
#define   RPN_TYPE_ANY_TYPE          (-2)
#define   RPN_TYPE_UNUSED            (-1)
#define   RPN_TYPE_UNDEFINED         (0)
#define   RPN_TYPE_NIL               (1)
#define   RPN_TYPE_INT               (2)
#define   RPN_TYPE_FLOAT             (3)
#define   RPN_TYPE_STRING            (4)
#define   RPN_TYPE_VECTOR_3          (5)
#define   RPN_TYPE_MATRIX            (6)
#define   RPN_TYPE_LITTERAL          (7)

/* Operators names
   ~~~~~~~~~~~~~~~ */
#define   RPN_OP_NAME_NULL           RPN_OP_NAME_UNDEF
#define   RPN_OP_NAME_UNDEF          "UNDEFINED OPERATOR NAME"
#define   RPN_OP_NAME_ENTER          "enter"
#define   RPN_OP_NAME_CLX            "clx"
#define   RPN_OP_NAME_DELX           "delx"
#define   RPN_OP_NAME_CLST           "clst"
#define   RPN_OP_NAME_SWAP_XY        "x<>y"
#define   RPN_OP_NAME_SWAP_XZ        "x<>z"
#define   RPN_OP_NAME_SWAP_XT        "x<>t"
#define   RPN_OP_NAME_SWAP_XL        "x<>l"
#define   RPN_OP_NAME_RDN            "rdn"
#define   RPN_OP_NAME_RUP            "rup"
#define   RPN_OP_NAME_LASTX          "lastx"
#define   RPN_OP_NAME_RCL_X          "rcl_x"
#define   RPN_OP_NAME_PRX            "prx"
#define   RPN_OP_NAME_PRSTK          "prstk"
#define   RPN_OP_NAME_CATALOG        "catalog"

// Math
#define   RPN_OP_NAME_ADD            "+"
#define   RPN_OP_NAME_SUB            "-"
#define   RPN_OP_NAME_MUL            "*"
#define   RPN_OP_NAME_DIV            "/"
#define   RPN_OP_NAME_MATH_MUL       "m*"
#define   RPN_OP_NAME_RECIPROCAL     "1/x"
#define   RPN_OP_NAME_POWER          "y^x"
#define   RPN_OP_NAME_SQRT           "sqrt"
#define   RPN_OP_NAME_SQUARE         "x^2"
#define   RPN_OP_NAME_LN             "ln"
#define   RPN_OP_NAME_EXP            "exp"
#define   RPN_OP_NAME_LOG10          "log"
#define   RPN_OP_NAME_10_POWER_X     "10^x"
#define   RPN_OP_NAME_PERCENT        "%"
#define   RPN_OP_NAME_DELTA_PERCENT  "delta%"
#define   RPN_OP_NAME_SINE           "sin"
#define   RPN_OP_NAME_COSINE         "cos"
#define   RPN_OP_NAME_TANGENT        "tan"
#define   RPN_OP_NAME_SINH           "sinh"
#define   RPN_OP_NAME_COSH           "cosh"
#define   RPN_OP_NAME_TANH           "tanh"
#define   RPN_OP_NAME_SIGMOID        "sigmoid"
#define   RPN_OP_NAME_MATRIX         "mat"
#define   RPN_OP_NAME_EXPLODE        "expl"
#define   RPN_OP_NAME_VEC2           "vec2"
#define   RPN_OP_NAME_VEC3           "vec3"
#define   RPN_OP_NAME_TRANSPOSE      "t"
#define   RPN_OP_NAME_CROSS_PRODUCT  "^"
#define   RPN_OP_NAME_SCALAR_MATRIX  "scalar_matrix"
#define   RPN_OP_NAME_SEQ            "seq"
#define   RPN_OP_NAME_DIM            "dim"

// Strings
#define   RPN_OP_NAME_STR_LENGTH     "length"
#define   RPN_OP_NAME_STR_CATENATE   "cat"
#define   RPN_OP_NAME_STR_DUPL       "dupl"


// Statistics
#define   RPN_OP_NAME_SUM_ADD        "sigma+"
#define   RPN_OP_NAME_SUM_SUB        "sigma-"
#define   RPN_OP_NAME_MEAN           "mean"
#define   RPN_OP_NAME_SDEV           "sdev"

/* Operators ID
   ~~~~~~~~~~~~ */
#define   RPN_OP_ID_NULL                RPN_OP_ID_UNDEF
#define   RPN_OP_ID_UNDEF               (0)

// Stack
#define   RPN_OP_BASE_STACK             (RPN_OP_ID_UNDEF + 1)
#define   RPN_OP_END_STACK               RPN_OP_ID_PRSTK

#define   RPN_OP_ID_ENTER               (RPN_OP_BASE_STACK + 0)
#define   RPN_OP_ID_CLX                 (RPN_OP_BASE_STACK + 1)
#define   RPN_OP_ID_DELX                (RPN_OP_BASE_STACK + 2)
#define   RPN_OP_ID_CLST                (RPN_OP_BASE_STACK + 3)
#define   RPN_OP_ID_SWAP_XY             (RPN_OP_BASE_STACK + 4)
#define   RPN_OP_ID_SWAP_XZ             (RPN_OP_BASE_STACK + 5)
#define   RPN_OP_ID_SWAP_XT             (RPN_OP_BASE_STACK + 6)
#define   RPN_OP_ID_SWAP_XL             (RPN_OP_BASE_STACK + 7)
#define   RPN_OP_ID_RDN                 (RPN_OP_BASE_STACK + 8)
#define   RPN_OP_ID_RUP                 (RPN_OP_BASE_STACK + 9)
#define   RPN_OP_ID_LASTX               (RPN_OP_BASE_STACK + 10)
#define   RPN_OP_ID_RCL_X               (RPN_OP_BASE_STACK + 11)
#define   RPN_OP_ID_PRX                 (RPN_OP_BASE_STACK + 12)
#define   RPN_OP_ID_PRSTK               (RPN_OP_BASE_STACK + 13)
//#define   RPN_OP_ID_CATALOG             (RPN_OP_BASE_STACK + 13)

// Math
#define   RPN_OP_BASE_MATH              (RPN_OP_END_STACK + 1)
#define   RPN_OP_END_MATH                RPN_OP_ID_DIM

#define   RPN_OP_ID_ADD                 (RPN_OP_BASE_MATH + 0)
#define   RPN_OP_ID_SUB                 (RPN_OP_BASE_MATH + 1)
#define   RPN_OP_ID_MUL                 (RPN_OP_BASE_MATH + 2)
#define   RPN_OP_ID_DIV                 (RPN_OP_BASE_MATH + 3)
#define   RPN_OP_ID_MATH_MUL            (RPN_OP_BASE_MATH + 4)
#define   RPN_OP_ID_RECIPROCAL          (RPN_OP_BASE_MATH + 5)
#define   RPN_OP_ID_POWER               (RPN_OP_BASE_MATH + 6)
#define   RPN_OP_ID_SQRT                (RPN_OP_BASE_MATH + 7)
#define   RPN_OP_ID_SQUARE              (RPN_OP_BASE_MATH + 8)
#define   RPN_OP_ID_LN                  (RPN_OP_BASE_MATH + 9)
#define   RPN_OP_ID_EXP                 (RPN_OP_BASE_MATH + 10)
#define   RPN_OP_ID_LOG10               (RPN_OP_BASE_MATH + 11)
#define   RPN_OP_ID_10_POWER_X          (RPN_OP_BASE_MATH + 12)
#define   RPN_OP_ID_PERCENT             (RPN_OP_BASE_MATH + 13)
#define   RPN_OP_ID_DELTA_PERCENT       (RPN_OP_BASE_MATH + 14)
#define   RPN_OP_ID_SINE                (RPN_OP_BASE_MATH + 15)
#define   RPN_OP_ID_COSINE              (RPN_OP_BASE_MATH + 16)
#define   RPN_OP_ID_TANGENT             (RPN_OP_BASE_MATH + 17)
#define   RPN_OP_ID_SINH                (RPN_OP_BASE_MATH + 18)
#define   RPN_OP_ID_COSH                (RPN_OP_BASE_MATH + 19)
#define   RPN_OP_ID_TANH                (RPN_OP_BASE_MATH + 20)
#define   RPN_OP_ID_SIGMOID             (RPN_OP_BASE_MATH + 21)
#define   RPN_OP_ID_MATRIX              (RPN_OP_BASE_MATH + 22)
#define   RPN_OP_ID_EXPLODE             (RPN_OP_BASE_MATH + 23)
#define   RPN_OP_ID_VEC2                (RPN_OP_BASE_MATH + 24)
#define   RPN_OP_ID_VEC3                (RPN_OP_BASE_MATH + 25)
#define   RPN_OP_ID_TRANSPOSE           (RPN_OP_BASE_MATH + 26)
#define   RPN_OP_ID_CROSS_PRODUCT       (RPN_OP_BASE_MATH + 27)
#define   RPN_OP_ID_SCALAR_MATRIX       (RPN_OP_BASE_MATH + 28)
#define   RPN_OP_ID_SEQ                 (RPN_OP_BASE_MATH + 29)
#define   RPN_OP_ID_DIM                 (RPN_OP_BASE_MATH + 30)

// Strings
#define   RPN_OP_BASE_STRINGS           (RPN_OP_END_MATH + 1)
#define   RPN_OP_END_STRINGS             RPN_OP_ID_STR_DUPL

#define   RPN_OP_ID_STR_LENGTH          (RPN_OP_BASE_STRINGS + 0)
#define   RPN_OP_ID_STR_CATENATE        (RPN_OP_BASE_STRINGS + 1)
#define   RPN_OP_ID_STR_DUPL            (RPN_OP_BASE_STRINGS + 2)

// Statistics
#define   RPN_OP_BASE_STATS             (RPN_OP_END_STRINGS + 1)
#define   RPN_OP_END_STATS               RPN_OP_ID_STR_DUPL

#define   RPN_OP_ID_SUM_ADD             (RPN_OP_BASE_STATS + 0)
#define   RPN_OP_ID_SUM_SUB             (RPN_OP_BASE_STATS + 1)
#define   RPN_OP_ID_MEAN                (RPN_OP_BASE_STATS + 2)
#define   RPN_OP_ID_SDEV                (RPN_OP_BASE_STATS + 3)

/* Sigma constants
   ~~~~~~~~~~~~~~~ */
#define   RPN_NB_SIGMA                  (6)

#define   RPN_SIGMA_IDX_SUM_X           (0)
#define   RPN_SIGMA_IDX_SUM_X2          (1)
#define   RPN_SIGMA_IDX_SUM_Y           (2)
#define   RPN_SIGMA_IDX_SUM_Y2          (3)
#define   RPN_SIGMA_IDX_SUM_XY          (4)
#define   RPN_SIGMA_IDX_N               (5)

#define   RPN_OP(fct)                   RPN_OP_NAME_ ## fct, RPN_OP_ID_ ## fct
#define   RPN_OP_DESC(fct)              (&my_operators[RPN_OP_ID_ ## fct])

/* Structures definitions
   ~~~~~~~~~~~~~~~~~~~~~~ */
struct global_struct {
     char                           *progname;
     rpn_stack                      *stack;
     struct rpn_sigma               *sigma;
     char                           *CSV_sep;
};

struct rpn_elt {
     uint32_t                        type;
     union {
          int                        i;
          long                       l;
          long long                 ll;
          float                      f;
          double                     d;
          long double               ld;
          bool                       b;
          char                      *s;
          void                      *obj;
     }                               value;
     struct rpn_elt                 *previous_elt,
                                    *next_elt;
};

struct rpn_operator {
     char                           *op_name;
     int                             op_id;
     uint32_t                        nb_args_in;
     uint32_t                        nb_args_out;
     int                            (*func)(rpn_stack *, rpn_operator *);
     struct rpn_st_types            *st_types;
//   AJOUTER alias
};
typedef struct rpn_operator         rpn_operator;

struct rpn_stack {
     const char                     *name;
     uint32_t                        num_elts;
     struct rpn_elt                 *lastx;
     int                             clear_x;
     int                             new_value;
     rpn_elt                        *base_elt,
                                    *top_elt;
//   AJOUTER name, print_mode
};

struct rpn_matrix {
     uint32_t                        n;
     uint32_t                        p;
     void                           *base[1];
};

struct rpn_st_types {
     int                             nb_operands;
     int                             nb_results;
     int                             X_type;
     int                             Y_type;
};

struct rpn_sigma {
     char                           *sigma_name;
     struct rpn_elt                 *reg[RPN_NB_SIGMA];
};

#define   T(nb_ops, nb_res, X_type, Y_type)       \
     {    nb_ops,   nb_res, RPN_TYPE_## X_type,   RPN_TYPE_ ## Y_type      }

/* External variables
   ~~~~~~~~~~~~~~~~~~ */
extern struct global_struct         G;
