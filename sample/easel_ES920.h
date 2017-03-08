/*
 * easel_es920.h
 *
 *  Created on: 2016/01/05
 *      Author: contec
 */

#ifndef EASEL_ES920_H_
#define EASEL_ES920_H_

#define SettingRequest()	easel_ES920_set_mode_config(EASEL_ES920_MODE_CONFIG)

#define WirelessSettingRequest	easel_ES920_set_wireless_default
#define RunRequest	easel_ES920_set_mode_operation

#define SaveRequest	easel_ES920_save_param

#define LoadRequest	easel_ES920_load_param

#endif /* EASEL_ES920_H_ */
