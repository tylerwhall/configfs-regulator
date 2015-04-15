/*
 * Platform driver user-space regulators
 *
 * Copyright 2015 Tyler Hall
 *
 * Author: Tyler Hall <tylerwhall@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <linux/module.h>
#include <linux/platform_device.h>

static int configfs_regulator_probe(struct platform_device *pdev)
{
	return 0;
}

static int configfs_regulator_remove(struct platform_device *pdev)
{
	return 0;
}

struct platform_driver configfs_regulator_driver = {
	.probe	= configfs_regulator_probe,
	.remove	= configfs_regulator_remove,
	.driver	= {
		.name	= "configfs-regulator",
	},
};
