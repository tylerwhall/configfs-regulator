#ifndef __CONFIGFS_REGULATOR_H__
#define __CONFIGFS_REGULATOR_H__

#include <linux/platform_device.h>
#include <linux/configfs.h>
#include <linux/regulator/driver.h>

extern struct platform_driver configfs_regulator_driver;

struct user_regulator {
	struct platform_device pdev;
	struct config_item config_item;
	struct regulator_desc rdesc;
	struct regulator_dev *rdev;
};

#endif
