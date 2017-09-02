/*
	2DArray.inc - 2 dimensional string support
	This include is only for storing strings in 2D arrays!

	A very useful include which uses packed strings for saving memory.
	You can create 2D arrays that store strings without having to worry
	about a long compilation time or memory usage.

    This Source Code Form is subject to the terms of the Mozilla Public
 	License, v. 2.0. If a copy of the MPL was not distributed with this
 	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <a_samp>

#define Dim2D_Create(%0,%1,%2)      new %0[%1][%2 char]
#define Dim2D_Set(%0,%1,%2)         strpack(%0[%1], %2)
#define Dim2D_Get(%0,%1,%2,%3)      strunpack(%2, %0[%1], %3)
#define Dim2D_Size(%0)           	sizeof(%0)
#define Dim2D_Size2D(%0)            sizeof(%0[])
#define Dim2D_Remove(%0,%1)         strdel(%0[%1], 0, strlen(%0[%1]))
#define Dim2D_Move(%0,%1,%2)        %0[%2] = %0[%1], Dim2D_Remove(%0, %1)
#define Dim2D_Copy(%0,%1,%2)        %0[%2] = %0[%1]
#define Dim2D_ValidIndex(%0,%1)     %0[%1]{0}
#define Dim2D_Foreach(%0,%1)        for (new %1; %1 < sizeof(%0); %1++) if (Dim2D_ValidIndex(%0,%1))
#define Dim2D_SendData(%0,%1,%2)    CallRemoteFunction(%2, "dss", %1, Dim2D_Return(%0,%1), #%0)
#define Dim2D:%0[%1,%2]             %0[%1][%2 char]

stock Dim2D_Return(array2D[][], arrIndex)
{
	new strDest[256];
	Dim2D_Get(array2D, arrIndex, strDest, sizeof(strDest));
	return strDest;
}

stock Dim2D_Next(const array2D[][], const size = Dim2D_Size(array2D))
{
	for (new iIter = 0; iIter < size; iIter ++)
	{
		if (!array2D[iIter]{0}) return iIter;
	}
	return -1;
}

stock Dim2D_ListArray(const array2D[][], const skipinvalid = true, const size = sizeof(array2D))
{
	print("_____________");
	print("Array Listing");
	for (new iIter = 0, strArray[256]; iIter < size; iIter ++)
	{
	    if (!array2D[iIter]{0} && skipinvalid) continue;
	    Dim2D_Get(array2D, iIter, strArray, sizeof(strArray));
	    printf("Index %d: %s", iIter, strArray);
	}
	print("_____________");
	return 1;
}