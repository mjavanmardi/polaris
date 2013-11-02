#pragma once

//this file mimicing gg_endian.c from gaiageo library

int
import32 (char *p, int little_endian, int little_endian_arch)
{
/* fetches a 32bit int from BLOB respecting declared endiannes */
	union cvt
	{
	unsigned char byte[4];
	int int_value;
	} convert;
	if (little_endian_arch)
	  {
	  /* Litte-Endian architecture [e.g. x86] */
	  if (!little_endian)
		{
		/* Big Endian data */
		convert.byte[0] = *(p + 3);
		convert.byte[1] = *(p + 2);
		convert.byte[2] = *(p + 1);
		convert.byte[3] = *(p + 0);
		}
	  else
		{
		/* Little Endian data */
		convert.byte[0] = *(p + 0);
		convert.byte[1] = *(p + 1);
		convert.byte[2] = *(p + 2);
		convert.byte[3] = *(p + 3);
		}
	  }
	else
	  {
	  /* Big Endian architecture [e.g. PPC] */
	  if (!little_endian)
		{
		/* Big Endian data */
		convert.byte[0] = *(p + 0);
		convert.byte[1] = *(p + 1);
		convert.byte[2] = *(p + 2);
		convert.byte[3] = *(p + 3);
		}
	  else
		{
		/* Little Endian data */
		convert.byte[0] = *(p + 3);
		convert.byte[1] = *(p + 2);
		convert.byte[2] = *(p + 1);
		convert.byte[3] = *(p + 0);
		}
	  }
	return convert.int_value;
}

double import64(char *p, int little_endian, int little_endian_arch)
	{
/* fetches a 64bit double from BLOB respecting declared endiannes */
	union cvt
	{
	unsigned char byte[8];
	double double_value;
	} convert;
	if (little_endian_arch)
	  {
/* Litte-Endian architecture [e.g. x86] */
	  if (!little_endian)
		{
		/* Big Endian data */
		convert.byte[0] = *(p + 7);
		convert.byte[1] = *(p + 6);
		convert.byte[2] = *(p + 5);
		convert.byte[3] = *(p + 4);
		convert.byte[4] = *(p + 3);
		convert.byte[5] = *(p + 2);
		convert.byte[6] = *(p + 1);
		convert.byte[7] = *(p + 0);
		}
	  else
		{
		/* Little Endian data */
		convert.byte[0] = *(p + 0);
		convert.byte[1] = *(p + 1);
		convert.byte[2] = *(p + 2);
		convert.byte[3] = *(p + 3);
		convert.byte[4] = *(p + 4);
		convert.byte[5] = *(p + 5);
		convert.byte[6] = *(p + 6);
		convert.byte[7] = *(p + 7);
		}
	  }
	else
	  {
	  /* Big Endian architecture [e.g. PPC] */
	  if (!little_endian)
		{
		/* Big Endian data */
		convert.byte[0] = *(p + 0);
		convert.byte[1] = *(p + 1);
		convert.byte[2] = *(p + 2);
		convert.byte[3] = *(p + 3);
		convert.byte[4] = *(p + 4);
		convert.byte[5] = *(p + 5);
		convert.byte[6] = *(p + 6);
		convert.byte[7] = *(p + 7);
		}
	  else
		{
		/* Little Endian data */
		convert.byte[0] = *(p + 7);
		convert.byte[1] = *(p + 6);
		convert.byte[2] = *(p + 5);
		convert.byte[3] = *(p + 4);
		convert.byte[4] = *(p + 3);
		convert.byte[5] = *(p + 2);
		convert.byte[6] = *(p + 1);
		convert.byte[7] = *(p + 0);
		}
	  }
	return convert.double_value;
}