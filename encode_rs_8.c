/* Reed-Solomon encoder
 * Copyright 2004, Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#include <string.h>
#include "fixed.h"


static void encode_rs_8_c(data_t *data, data_t *parity,int pad);

void encode_rs_8(data_t *data, data_t *parity,int pad){

    encode_rs_8_c(data,parity,pad);
}

/* Portable C version */
static void encode_rs_8_c(data_t *data, data_t *parity,int pad){

#include "encode_rs.h"

}
