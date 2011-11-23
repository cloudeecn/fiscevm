/**
 *  Copyright 2010-2011 Yuxuan Huang. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FY_DATA_H
#define	FY_DATA_H

#include "../fiscestu.h"
#include "../fisceprt.h"

#ifdef	__cplusplus
extern "C" {
#endif

fy_ubyte fy_dataRead(fy_data *data);
fy_char fy_dataRead2(fy_data *data);
fy_uint fy_dataRead4(fy_data *data);
fy_ulong fy_dataRead8(fy_data *data);
float fy_dataReadf(fy_data *data);
double fy_dataReadd(fy_data *data);
void fy_dataSkip(fy_data *data, int size);

#ifdef	__cplusplus
}
#endif

#endif	/* FY_DATA_H */

