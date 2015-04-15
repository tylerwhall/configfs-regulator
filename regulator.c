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
#include <linux/regulator/driver.h>

#include "configfs-regulator.h"

static inline struct user_regulator *pdev_to_user_regulator(struct platform_device *pdev)
{
	return container_of(pdev, struct user_regulator, pdev);
}

static inline struct user_regulator *dev_to_user_regulator(struct device *dev)
{
	return pdev_to_user_regulator(to_platform_device(dev));
}

static const struct regulator_ops configfs_reg_ops = {
};

static int configfs_regulator_probe(struct platform_device *pdev)
{
	struct user_regulator *ureg = pdev_to_user_regulator(pdev);
	struct regulator_config config = {
		.dev = &pdev->dev,
	};

	ureg->rdesc.name = config_item_name(&ureg->config_item);
	ureg->rdesc.id = -1;
	ureg->rdesc.type = REGULATOR_VOLTAGE;
	ureg->rdesc.owner = THIS_MODULE;
	ureg->rdesc.ops = &configfs_reg_ops;

	ureg->rdev = regulator_register(&ureg->rdesc, &config);
	if (IS_ERR(ureg->rdev)) {
		int rc = PTR_ERR(ureg->rdev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", rc);
		return rc;
	}

	return 0;
}

static int configfs_regulator_remove(struct platform_device *pdev)
{
	struct user_regulator *ureg = pdev_to_user_regulator(pdev);

	regulator_unregister(ureg->rdev);

	return 0;
}

struct platform_driver configfs_regulator_driver = {
	.probe	= configfs_regulator_probe,
	.remove	= configfs_regulator_remove,
	.driver	= {
		.name	= "configfs-regulator",
	},
};
