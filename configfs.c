/*
 * Configfs interface for creating user-space regulators
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
#include <linux/slab.h>
#include <linux/configfs.h>

#include "configfs-regulator.h"

#define UREG_MAX_PLATFORM_IDS 128
static unsigned long platform_ids[BITS_TO_LONGS(UREG_MAX_PLATFORM_IDS)];

static ssize_t user_regulator_attr_show(struct config_item *item,
					struct configfs_attribute *attr,
					char *page)
{
	return 0;
}

static ssize_t user_regulator_attr_store(struct config_item *item,
					 struct configfs_attribute *attr,
					 const char *page, size_t count)
{
	return count;
}

static inline struct user_regulator *to_user_regulator(struct config_item *item)
{
	return item ? container_of(item, struct user_regulator, config_item) : NULL;
}

static void user_regulator_release(struct config_item *item)
{
	struct user_regulator *ureg = to_user_regulator(item);

	clear_bit(ureg->pdev.id, platform_ids);
	platform_device_unregister(&ureg->pdev);
	kfree(ureg);
}

static struct configfs_item_operations user_regulator_item_ops = {
	.release		= user_regulator_release,
	.show_attribute		= user_regulator_attr_show,
	.store_attribute	= user_regulator_attr_store,
};

static struct config_item_type user_regulator_type = {
	.ct_item_ops	= &user_regulator_item_ops,
	.ct_owner	= THIS_MODULE,
};

static struct config_item *user_regulators_make_item(struct config_group *group, const char *name)
{
	int rc;
	struct user_regulator *ureg = kzalloc(sizeof(struct user_regulator), GFP_KERNEL);

	if (!ureg)
		return ERR_PTR(-ENOMEM);

	config_item_init_type_name(&ureg->config_item, name, &user_regulator_type);

	ureg->pdev.name = "configfs-regulator";
	do {
		ureg->pdev.id = find_first_zero_bit(platform_ids, UREG_MAX_PLATFORM_IDS);
		if (ureg->pdev.id >= UREG_MAX_PLATFORM_IDS) {
			pr_err("%s: no platform device ids free\n", __func__);
			rc = -ENOSPC;
			goto err;
		}
	} while (test_and_set_bit(ureg->pdev.id, platform_ids));

	rc = platform_device_register(&ureg->pdev);
	if (rc)
		goto err;

	return &ureg->config_item;
err:
	kfree(ureg);
	return ERR_PTR(rc);
}

static struct configfs_group_operations user_regulators_group_ops = {
	.make_item	= user_regulators_make_item,
};

static struct config_item_type group_user_regulator_type = {
	.ct_group_ops	= &user_regulators_group_ops,
	.ct_owner	= THIS_MODULE,
};

static struct configfs_subsystem group_user_regulator = {
	.su_group = {
		.cg_item = {
			.ci_namebuf = "regulator",
			.ci_type = &group_user_regulator_type,
		},
	},
};

static int __init user_regulator_init(void)
{
	config_group_init(&group_user_regulator.su_group);
	mutex_init(&group_user_regulator.su_mutex);

	return configfs_register_subsystem(&group_user_regulator);
}

static void __exit user_regulator_exit(void)
{
	configfs_unregister_subsystem(&group_user_regulator);
}

module_init(user_regulator_init);
module_exit(user_regulator_exit);
MODULE_LICENSE("GPL");
