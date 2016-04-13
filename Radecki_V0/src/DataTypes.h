#ifndef DataTypes_h
#define DataTypes_h

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
