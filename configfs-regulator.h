#ifndef __CONFIGFS_REGULATOR_H__
#define __CONFIGFS_REGULATOR_H__

#include <linux/platform_device.h>

extern struct platform_driver configfs_regulator_driver;

struct user_regulator {
	struct platform_device pdev;
	struct config_item config_item;
};

#endif
