/*MEMORY
{
	vectors : ORIGIN = 0x00000000, LENGTH = 0x00001000
	ram     : ORIGIN = 0x00001000, LENGTH = 0x00080000 - 0x00001000  
}*/
/* Linker script for OR1200 program */
OUTPUT_ARCH(or32)
__DYNAMIC = 0;

/* Linking for loading into external RAM */
MEMORY    
{
/*
	ld_info : ORIGIN = 0x00000000, LENGTH = 0x000000F0 
*/
	vectors : ORIGIN = 0x00000000, LENGTH = 0x2000 - 0x100
	ram : ORIGIN = 0x00002000, LENGTH = 0x00100000 - 0x00002000
}



SECTIONS
{
	.vectors :
	{
		_vec_start = .;
		*(.vectors)
		_vec_end = .;
	} > vectors

	.text ALIGN(4) :
	{
		*(.text)
	} 

	.rodata ALIGN(4) :
	{
		*(.rodata)
		*(.rodata.*)
	} 

	.icm ALIGN(4) :
	{
		_icm_start = .;
		*(.icm)
		_icm_end = .;
	} 

	.data ALIGN(4) :
	{
		_dst_beg = .;
		*(.data)
		_dst_end = .;
	} 

	.bss ALIGN(4) :
	{
		_bss_beg = .;
		*(.bss)
		_bss_end = .;
	} 

	PROVIDE(_stack_top = 0x00080000);
}
