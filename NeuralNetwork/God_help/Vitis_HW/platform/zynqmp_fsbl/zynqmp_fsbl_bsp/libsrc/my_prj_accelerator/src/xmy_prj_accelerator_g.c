#include "xmy_prj_accelerator.h"

XMy_prj_accelerator_Config XMy_prj_accelerator_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,my-prj-accelerator-1.0", /* compatible */
		0xa0020000 /* reg */
	},
	 {
		 NULL
	}
};