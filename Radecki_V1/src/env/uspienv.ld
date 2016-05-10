/*
 * uspienv.ld
 */

SECTIONS
{
	. = 0x10000;
	
	.text : {
		*(.text*)
	}

	_etext = .;

	.rodata : {
		*(.rodata*)
	}

	.data : {
		*(.data*)
	}

	__bss_start = .;

	.bss : {
		*(.bss*)
	}

	_end = .;
}
