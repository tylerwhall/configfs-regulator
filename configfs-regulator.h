#ifndef __CONFIGFS_REGULATOR_H__
#define __CONFIGFS_REGULATOR_H__

#include <linux/platform_device.h>

struct user_regulator {
	struct platform_device pdev;
	struct config_item config_item;
};

#endif
