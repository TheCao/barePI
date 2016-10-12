//
// types.h
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _uspienv_types_h
#define _uspienv_types_h

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef int		boolean;
#define FALSE		0
#define TRUE		1

//typedef unsigned long	size_t;
typedef __SIZE_TYPE__ size_t;
typedef long		ssize_t;


// dodane
typedef void							VOID;
typedef void *							APTR;

typedef signed char						INT8;
typedef signed short					INT16;
typedef signed int						INT32;
typedef signed long long int			INT64;

typedef unsigned char					UINT8;
typedef unsigned short					UINT16;
typedef unsigned int					UINT32;
typedef unsigned long long int			UINT64;


typedef float							FLOAT;
typedef	double							DOUBLE;

typedef volatile signed char			VINT8;
typedef volatile signed short int		VINT16;
typedef volatile signed int				VINT32;
typedef volatile signed long long int	VINT64;

typedef volatile unsigned char			VUINT8;
typedef volatile unsigned short int		VUINT16;
typedef volatile unsigned int			VUINT32;
typedef volatile unsigned long long int	VUINT64;

typedef UINT32							UINTPTR;

typedef unsigned int					IPTR;
typedef unsigned char*					STRPTR;
typedef unsigned int					BOOL;

#endif
