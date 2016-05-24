/*
 * uspienv.ld
 */

ENTRY (main)

MEMORY
{
	ram : ORIGIN = 0x10000, LENGTH = 0x1000000
}

SECTIONS
{
	. = 0x10000;
	
	.text : {
		*(.text*)
	} > ram

	_etext = .;

	.rodata : {
		*(.rodata*)
	} > ram

	.data : {
		*(.data*)
	} > ram

	__bss_start = .;

	.bss : {
		*(.bss*)
	} > ram

	_end = .;
}
