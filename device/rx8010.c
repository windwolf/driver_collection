// #include "rx8010.h"

// #define ADDRESS 0x64

// static void rx8010_i2c_read_cplt(I2CDevice *i2c);
// static void rx8010_i2c_write_cplt(I2CDevice *i2c);
// static void rx8010_i2c_error(I2CDevice *i2c, OP_RESULT rst);

// static void rx8010_i2c_read_cplt(I2CDevice *i2c){

// };
// static void rx8010_i2c_write_cplt(I2CDevice *i2c){

// };
// static void rx8010_i2c_error(I2CDevice *i2c, OP_RESULT rst){

// };

// OP_RESULT rx8010_init(RX8010 *rx8010, I2CDevice *i2c)
// {
//     rx8010->i2c = i2c;
//     _i2c_device_register(i2c, rx8010, &rx8010_i2c_read_cplt, &rx8010_i2c_write_cplt, &rx8010_i2c_error);
//     return OP_RESULT_OK;
// };
// OP_RESULT rx8010_datetime_Get(RX8010 *rx8010, DateTime *datetime)
// {
//     uint8_t data[7];
//     i2c_device_mem_read(rx8010->i2c, ADDRESS, 0x10, data, 7, DEVICE_DATAWIDTH_8);
//     i2c_device
// };

// /**
//  * @description:  Write rx8010 register
//  * @param {type}
//  * @return:
//  */
// static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data)
// {
//     rt_uint8_t buf[2];

//     buf[0] = reg; // cmd
//     buf[1] = data;
//     // buf[2] = data[1];

//     if (rt_i2c_master_send(bus, RX8010_WRITE_ADDR, 0, buf, 2) == 2)
//         return RT_EOK;
//     else
//         return -RT_ERROR;
// }

// /**
//  * @description: Read rx8010 register
//  * @param {type}
//  * @return:
//  */
// static rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t date)
// {
//     struct rt_i2c_msg msgs[2];

//     msgs[0].addr = RX8010_WRITE_ADDR;
//     msgs[0].flags = RT_I2C_WR;
//     msgs[0].buf = reg;
//     msgs[0].len = 1;

//     msgs[1].addr = RX8010_READ_ADDR;
//     msgs[1].flags = RT_I2C_RD;
//     msgs[1].buf = date;
//     msgs[1].len = 1;

//     if (rt_i2c_transfer(bus, &msgs, 2) == 1)
//     {
//         return RT_EOK;
//     }
//     else
//     {
//         return -RT_ERROR;
//     }
// }
// #if 0
// static irqreturn_t rx8010_irq_1_handler(int irq, void *dev_id)
// {
// 	struct i2c_client *client = dev_id;
// 	struct rx8010_data *rx8010 = i2c_get_clientdata(client);
// 	int flagreg;

// 	mutex_lock(&rx8010->rtc->ops_lock);

// 	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);

// 	if (flagreg <= 0) {
// 		mutex_unlock(&rx8010->rtc->ops_lock);
// 		return IRQ_NONE;
// 	}

// 	if (flagreg & RX8010_FLAG_VLF)
// 		dev_warn(&client->dev, "Frequency stop detected\n");

// 	if (flagreg & RX8010_FLAG_TF) {
// 		flagreg &= ~RX8010_FLAG_TF;
// 		rtc_update_irq(rx8010->rtc, 1, RTC_PF | RTC_IRQF);
// 	}

// 	if (flagreg & RX8010_FLAG_AF) {
// 		flagreg &= ~RX8010_FLAG_AF;
// 		rtc_update_irq(rx8010->rtc, 1, RTC_AF | RTC_IRQF);
// 	}

// 	if (flagreg & RX8010_FLAG_UF) {
// 		flagreg &= ~RX8010_FLAG_UF;
// 		rtc_update_irq(rx8010->rtc, 1, RTC_UF | RTC_IRQF);
// 	}

// 	i2c_smbus_write_byte_data(client, RX8010_FLAG, flagreg);

// 	mutex_unlock(&rx8010->rtc->ops_lock);
// 	return IRQ_HANDLED;
// }

// /**
//  * @description: get time from rx8010
//  * @param {type} 
//  * @return: 
//  */
// static int rx8010_get_time(struct device *dev, struct rtc_time *dt)
// {
// 	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
// 	u8 date[7];
// 	int flagreg;
// 	int err;

// 	flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
// 	if (flagreg < 0)
// 		return flagreg;

// 	if (flagreg & RX8010_FLAG_VLF) {
// 		dev_warn(dev, "Frequency stop detected\n");
// 		return -EINVAL;
// 	}

// 	err = i2c_smbus_read_i2c_block_data(rx8010->client, RX8010_SEC,
// 					    7, date);
// 	if (err != 7)
// 		return err < 0 ? err : -EIO;

// 	dt->tm_sec = bcd2bin(date[RX8010_SEC - RX8010_SEC] & 0x7f);
// 	dt->tm_min = bcd2bin(date[RX8010_MIN - RX8010_SEC] & 0x7f);
// 	dt->tm_hour = bcd2bin(date[RX8010_HOUR - RX8010_SEC] & 0x3f);
// 	dt->tm_mday = bcd2bin(date[RX8010_MDAY - RX8010_SEC] & 0x3f);
// 	dt->tm_mon = bcd2bin(date[RX8010_MONTH - RX8010_SEC] & 0x1f) - 1;
// 	dt->tm_year = bcd2bin(date[RX8010_YEAR - RX8010_SEC]) + 100;
// 	dt->tm_wday = ffs(date[RX8010_WDAY - RX8010_SEC] & 0x7f);

// 	return 0;
// }

// static int rx8010_set_time(struct device *dev, struct rtc_time *dt)
// {
// 	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
// 	u8 date[7];
// 	int ctrl, flagreg;
// 	int ret;

// 	if ((dt->tm_year < 100) || (dt->tm_year > 199))
// 		return -EINVAL;

// 	/* set STOP bit before changing clock/calendar */
// 	ctrl = i2c_smbus_read_byte_data(rx8010->client, RX8010_CTRL);
// 	if (ctrl < 0)
// 		return ctrl;
// 	rx8010->ctrlreg = ctrl | RX8010_CTRL_STOP;
// 	ret = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
// 					rx8010->ctrlreg);
// 	if (ret < 0)
// 		return ret;

// 	date[RX8010_SEC - RX8010_SEC] = bin2bcd(dt->tm_sec);
// 	date[RX8010_MIN - RX8010_SEC] = bin2bcd(dt->tm_min);
// 	date[RX8010_HOUR - RX8010_SEC] = bin2bcd(dt->tm_hour);
// 	date[RX8010_MDAY - RX8010_SEC] = bin2bcd(dt->tm_mday);
// 	date[RX8010_MONTH - RX8010_SEC] = bin2bcd(dt->tm_mon + 1);
// 	date[RX8010_YEAR - RX8010_SEC] = bin2bcd(dt->tm_year - 100);
// 	date[RX8010_WDAY - RX8010_SEC] = bin2bcd(1 << dt->tm_wday);

// 	ret = i2c_smbus_write_i2c_block_data(rx8010->client,
// 					     RX8010_SEC, 7, date);
// 	if (ret < 0)
// 		return ret;

// 	/* clear STOP bit after changing clock/calendar */
// 	ctrl = i2c_smbus_read_byte_data(rx8010->client, RX8010_CTRL);
// 	if (ctrl < 0)
// 		return ctrl;
// 	rx8010->ctrlreg = ctrl & ~RX8010_CTRL_STOP;
// 	ret = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
// 					rx8010->ctrlreg);
// 	if (ret < 0)
// 		return ret;

// 	flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
// 	if (flagreg < 0) {
// 		return flagreg;
// 	}

// 	if (flagreg & RX8010_FLAG_VLF)
// 		ret = i2c_smbus_write_byte_data(rx8010->client, RX8010_FLAG,
// 						flagreg & ~RX8010_FLAG_VLF);

// 	return 0;
// }
// #endif
// /**
//  * @description: Init rx8010
//  * @param {type}
//  * @return:
//  */
// static int rx8010_init(char const *devicename)
// {
//     int err;
//     rt_uint8_t flag;
//     rt_uint8_t need_clear = 0;

//     RT_ASSERT(devicename);

//     /* 查找I2C总线设备，获取I2C总线设备句柄 */
//     rx8010 = (struct rt_i2c_bus_device *)rt_i2c_bus_device_find(devicename);
//     RT_ASSERT(rx8010);
//     if (RT_NULL == rx8010)
//     {
//         LOG_E("RX8010 Init devicename %s not found!", devicename);
//     }

//     /* Initialize reserved registers as specified in datasheet */
//     err = write_reg(rx8010, RX8010_RESV17, 0xD8);
//     if (err < 0)
//         return err;

//     err = write_reg(rx8010, RX8010_RESV30, 0x00);
//     if (err < 0)
//         return err;

//     err = write_reg(rx8010, RX8010_RESV31, 0x08);
//     if (err < 0)
//         return err;

//     err = write_reg(rx8010, RX8010_IRQ, 0x00);
//     if (err < 0)
//         return err;

//     err = read_regs(rx8010, RX8010_FLAG, flag);
//     if (err < 0)
//         return err;

//     if (flag & RX8010_FLAG_VLF)
//     {
//         LOG_W("Frequency stop was detected");
//     }

//     if (flag & RX8010_FLAG_AF)
//     {
//         LOG_W("Alarm was detected");
//         need_clear = 1;
//     }

//     if (flag & RX8010_FLAG_TF)
//         need_clear = 1;

//     if (flag & RX8010_FLAG_UF)
//         need_clear = 1;

//     if (need_clear)
//     {
//         flag &= ~(RX8010_FLAG_AF | RX8010_FLAG_TF | RX8010_FLAG_UF);
//         err = write_reg(rx8010, RX8010_FLAG, flag);
//         if (err < 0)
//             return err;
//     }

//     // rx8010->ctrlreg = (ctrl[1] & ~RX8010_CTRL_TEST);

//     return 0;
// }
// #if 0
// static int rx8010_read_alarm(struct device *dev, struct rtc_wkalrm *t)
// {
// 	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
// 	struct i2c_client *client = rx8010->client;
// 	u8 alarmvals[3];
// 	int flagreg;
// 	int err;

// 	err = i2c_smbus_read_i2c_block_data(client, RX8010_ALMIN, 3, alarmvals);
// 	if (err != 3)
// 		return err < 0 ? err : -EIO;

// 	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);
// 	if (flagreg < 0)
// 		return flagreg;

// 	t->time.tm_sec = 0;
// 	t->time.tm_min = bcd2bin(alarmvals[0] & 0x7f);
// 	t->time.tm_hour = bcd2bin(alarmvals[1] & 0x3f);

// 	if (!(alarmvals[2] & RX8010_ALARM_AE))
// 		t->time.tm_mday = bcd2bin(alarmvals[2] & 0x7f);

// 	t->enabled = !!(rx8010->ctrlreg & RX8010_CTRL_AIE);
// 	t->pending = (flagreg & RX8010_FLAG_AF) && t->enabled;

// 	return 0;
// }

// static int rx8010_set_alarm(struct device *dev, struct rtc_wkalrm *t)
// {
// 	struct i2c_client *client = to_i2c_client(dev);
// 	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
// 	u8 alarmvals[3];
// 	int extreg, flagreg;
// 	int err;

// 	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);
// 	if (flagreg < 0) {
// 		return flagreg;
// 	}

// 	if (rx8010->ctrlreg & (RX8010_CTRL_AIE | RX8010_CTRL_UIE)) {
// 		rx8010->ctrlreg &= ~(RX8010_CTRL_AIE | RX8010_CTRL_UIE);
// 		err = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
// 						rx8010->ctrlreg);
// 		if (err < 0) {
// 			return err;
// 		}
// 	}

// 	flagreg &= ~RX8010_FLAG_AF;
// 	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_FLAG, flagreg);
// 	if (err < 0)
// 		return err;

// 	alarmvals[0] = bin2bcd(t->time.tm_min);
// 	alarmvals[1] = bin2bcd(t->time.tm_hour);
// 	alarmvals[2] = bin2bcd(t->time.tm_mday);

// 	err = i2c_smbus_write_i2c_block_data(rx8010->client, RX8010_ALMIN,
// 					     2, alarmvals);
// 	if (err < 0)
// 		return err;

// 	extreg = i2c_smbus_read_byte_data(client, RX8010_EXT);
// 	if (extreg < 0)
// 		return extreg;

// 	extreg |= RX8010_EXT_WADA;
// 	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_EXT, extreg);
// 	if (err < 0)
// 		return err;

// 	if (alarmvals[2] == 0)
// 		alarmvals[2] |= RX8010_ALARM_AE;

// 	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_ALWDAY,
// 					alarmvals[2]);
// 	if (err < 0)
// 		return err;

// 	if (t->enabled) {
// 		if (rx8010->rtc->uie_rtctimer.enabled)
// 			rx8010->ctrlreg |= RX8010_CTRL_UIE;
// 		if (rx8010->rtc->aie_timer.enabled)
// 			rx8010->ctrlreg |=
// 				(RX8010_CTRL_AIE | RX8010_CTRL_UIE);

// 		err = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
// 						rx8010->ctrlreg);
// 		if (err < 0)
// 			return err;
// 	}

// 	return 0;
// }

// static int rx8010_alarm_irq_enable(struct device *dev,
// 				   unsigned int enabled)
// {
// 	struct i2c_client *client = to_i2c_client(dev);
// 	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
// 	int flagreg;
// 	u8 ctrl;
// 	int err;

// 	ctrl = rx8010->ctrlreg;

// 	if (enabled) {
// 		if (rx8010->rtc->uie_rtctimer.enabled)
// 			ctrl |= RX8010_CTRL_UIE;
// 		if (rx8010->rtc->aie_timer.enabled)
// 			ctrl |= (RX8010_CTRL_AIE | RX8010_CTRL_UIE);
// 	} else {
// 		if (!rx8010->rtc->uie_rtctimer.enabled)
// 			ctrl &= ~RX8010_CTRL_UIE;
// 		if (!rx8010->rtc->aie_timer.enabled)
// 			ctrl &= ~RX8010_CTRL_AIE;
// 	}

// 	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);
// 	if (flagreg < 0)
// 		return flagreg;

// 	flagreg &= ~RX8010_FLAG_AF;
// 	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_FLAG, flagreg);
// 	if (err < 0)
// 		return err;

// 	if (ctrl != rx8010->ctrlreg) {
// 		rx8010->ctrlreg = ctrl;
// 		err = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
// 						rx8010->ctrlreg);
// 		if (err < 0)
// 			return err;
// 	}

// 	return 0;
// }

// static int rx8010_ioctl(struct device *dev, unsigned int cmd, unsigned long arg)
// {
// 	struct i2c_client *client = to_i2c_client(dev);
// 	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
// 	int ret, tmp;
// 	int flagreg;

// 	switch (cmd) {
// 	case RTC_VL_READ:
// 		flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
// 		if (flagreg < 0)
// 			return flagreg;

// 		tmp = !!(flagreg & RX8010_FLAG_VLF);
// 		if (copy_to_user((void __user *)arg, &tmp, sizeof(int)))
// 			return -EFAULT;

// 		return 0;

// 	case RTC_VL_CLR:
// 		flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
// 		if (flagreg < 0) {
// 			return flagreg;
// 		}

// 		flagreg &= ~RX8010_FLAG_VLF;
// 		ret = i2c_smbus_write_byte_data(client, RX8010_FLAG, flagreg);
// 		if (ret < 0)
// 			return ret;

// 		return 0;

// 	default:
// 		return -ENOIOCTLCMD;
// 	}
// }

// static struct rtc_class_ops rx8010_rtc_ops = {
// 	.read_time = rx8010_get_time,
// 	.set_time = rx8010_set_time,
// 	.ioctl = rx8010_ioctl,
// };

// static int rx8010_probe(struct i2c_client *client,
// 			const struct i2c_device_id *id)
// {
// 	struct i2c_adapter *adapter = client->adapter;
// 	struct rx8010_data *rx8010;
// 	int err = 0;

// 	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA
// 		| I2C_FUNC_SMBUS_I2C_BLOCK)) {
// 		dev_err(&adapter->dev, "doesn't support required functionality\n");
// 		return -EIO;
// 	}

// 	rx8010 = devm_kzalloc(&client->dev, sizeof(struct rx8010_data),
// 			      GFP_KERNEL);
// 	if (!rx8010)
// 		return -ENOMEM;

// 	rx8010->client = client;
// 	i2c_set_clientdata(client, rx8010);

// 	err = rx8010_init_client(client);
// 	if (err)
// 		return err;

// 	if (client->irq > 0) {
// 		dev_info(&client->dev, "IRQ %d supplied\n", client->irq);
// 		err = devm_request_threaded_irq(&client->dev, client->irq, NULL,
// 						rx8010_irq_1_handler,
// 						IRQF_TRIGGER_LOW | IRQF_ONESHOT,
// 						"rx8010", client);

// 		if (err) {
// 			dev_err(&client->dev, "unable to request IRQ\n");
// 			client->irq = 0;
// 		} else {
// 			rx8010_rtc_ops.read_alarm = rx8010_read_alarm;
// 			rx8010_rtc_ops.set_alarm = rx8010_set_alarm;
// 			rx8010_rtc_ops.alarm_irq_enable = rx8010_alarm_irq_enable;
// 		}
// 	}

// 	rx8010->rtc = devm_rtc_device_register(&client->dev, client->name,
// 		&rx8010_rtc_ops, THIS_MODULE);

// 	if (IS_ERR(rx8010->rtc)) {
// 		dev_err(&client->dev, "unable to register the class device\n");
// 		return PTR_ERR(rx8010->rtc);
// 	}

// 	rx8010->rtc->max_user_freq = 1;

// 	return err;
// }