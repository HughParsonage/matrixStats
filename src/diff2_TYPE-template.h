/***********************************************************************
 TEMPLATE:
  double diff2_<Integer|Real>(X_C_TYPE *x, R_xlen_t nx, R_xlen_t lag, R_xlen_t differences, X_C_TYPE *ans, R_xlen_t nans)

 Arguments:
   The following macros ("arguments") should be defined for the
   template to work as intended.

  - METHOD_NAME: the name of the resulting function
  - X_TYPE: 'i' or 'r'

 Copyright: Henrik Bengtsson, 2014
 ***********************************************************************/
#include "types.h"

/* Expand arguments:
    X_TYPE => (X_C_TYPE, X_IN_C, [METHOD_NAME])
 */
#include "templates-types.h"
#include <R_ext/Error.h>

void METHOD_NAME(X_C_TYPE *x, R_xlen_t nx, R_xlen_t lag, R_xlen_t differences, X_C_TYPE *ans, R_xlen_t nans) {
  int ii, tt, uu;
  X_C_TYPE *tmp = NULL;

  /* Nothing to do? */
  if (nans <= 0) return;

  /* Special case (difference == 1) */
  if (differences == 1) {
    uu = lag;
    tt = 0;
    for (ii=0; ii < nans; ii++) {
      ans[ii] = x[uu++] - x[tt++];
    }
  } else {
    /* Allocate temporary work vector (to hold intermediate differences) */
    tmp = Calloc(nx-lag, double);

    /* (a) First order of differences */
    uu = lag;
    tt = 0;
    for (ii=0; ii < nx-lag; ii++) {
      tmp[ii] = x[uu++] - x[tt++];
    }

    /* (b) All other orders of differences but the last */
    while (--differences > 1) {
      uu = lag;
      tt = 0;
      for (ii=0; ii < nx-lag; ii++) {
        tmp[ii] = tmp[uu++] - tmp[tt++];
      }
      nx--;
    }

    /* (c) Last order of differences */
    uu = lag;
    tt = 0;
    for (ii=0; ii < nans; ii++) {
      ans[ii] = tmp[uu++] - tmp[tt++];
    }

    /* Deallocate temorary work vector */
    Free(tmp);
  } /* if (differences ...) */
}

/* Undo template macros */
#include "templates-types_undef.h"

/***************************************************************************
 HISTORY:
 2014-12-29 [HB]
 o Created.
 **************************************************************************/