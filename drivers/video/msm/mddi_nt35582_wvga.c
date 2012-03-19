/* drivers/video/msm/mddi_nt35582_wvga.c
 * Created from the original mddi_toshiba_wvga.c and mddi_nt35582.c
 *
 * Credits to CodeAurora and Huawei.
 * Check http://github.com/Blefish for my other projects.
 *
 */

/* Copyright (c) 2009-2010, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "msm_fb.h"
#include "mddihost.h"

static int mddi_nt35582_wvga_enter_sleep(struct platform_device *pdev)
{
	int ret;
	ret = mddi_queue_register_write(0x1000, 0, 0, 0);
	return ret;
}

static int mddi_nt35582_wvga_exit_sleep(struct platform_device *pdev)
{
	int ret;
	ret = mddi_queue_register_write(0x1100, 0, 120, 0);
	return ret;
}

static int __devinit mddi_nt35582_wvga_probe(struct platform_device *pdev)
{
	msm_fb_add_device(pdev);
	return 0;
}

static struct platform_driver mddi_nt35582_wvga_driver = {
	.probe = mddi_nt35582_wvga_probe,
	.driver = {
		.name = "mddi_nt35582_wvga",
	},
};

static struct msm_fb_panel_data mddi_nt35582_wvga_panel_data = {
	.on = mddi_nt35582_wvga_exit_sleep,
	.off = mddi_nt35582_wvga_enter_sleep,
	//.set_backlight = pwm_set_backlight,
};

static struct platform_device mddi_nt35582_wvga_device = {
	.name = "mddi_nt35582_wvga",
	.id = 0,
	.dev = {
		.platform_data = &mddi_nt35582_wvga_panel_data,
	}
};

static int __init mddi_nt35582_wvga_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

#ifdef CONFIG_FB_MSM_MDDI_AUTO_DETECT
	if (msm_fb_detect_client("mddi_nt35582_wvga"))
		return 0;
#endif

	ret = platform_driver_register(&mddi_nt35582_wvga_driver);
	if (!ret)
	{
		pinfo = &mddi_nt35582_wvga_panel_data.panel_info;

		pinfo->xres = 480;
		pinfo->yres = 800;
		pinfo->pdest = DISPLAY_1;
		pinfo->type = MDDI_PANEL;
		pinfo->mddi.vdopkt = MDDI_DEFAULT_PRIM_PIX_ATTR;
		pinfo->wait_cycle = 0;
		pinfo->bpp = 16;
		pinfo->lcd.vsync_enable = TRUE;
		pinfo->lcd.refx100 = 4000;
		pinfo->lcd.v_back_porch = 0;
		pinfo->lcd.v_front_porch = 0;
		pinfo->lcd.v_pulse_width = 22;
		pinfo->lcd.hw_vsync_mode = TRUE;
		pinfo->lcd.vsync_notifier_period = 0;
		pinfo->bl_max = 255;
		pinfo->bl_min = 0;
		pinfo->clk_rate = 192000000;
		pinfo->clk_min = 192000000;
		pinfo->clk_max = 192000000;
		pinfo->fb_num = 2;

		ret = platform_device_register(&mddi_nt35582_wvga_device);
		if (ret) {
			platform_driver_unregister(&mddi_nt35582_wvga_driver);
			printk(KERN_ERR "%s: failed to register device!\n", __func__);
			return ret;
		}
	}

	return ret;
}

module_init(mddi_nt35582_wvga_init);
