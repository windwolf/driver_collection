#include "inc/w25qxx/w25qxx_base.h"

DEVICE_STATUS w25qxx_create(W25QXX_BASE *instance, Buffer buffer)
{
	instance->buffer = buffer;
	tx_event_flags_create(&instance->events, "w25qxx");
}

void w25qxx_calc_parameter(W25QXX_BASE *instance)
{
	switch (instance->flashId)
	{
	case W25QXX_FLASH_ID_SST25VF016B:
		strcpy(instance->flashName, "SST25VF016B");
		instance->flashSize = 2 * 1024 * 1024;
		instance->sectorSize = 4 * 1024;
		break;

	case W25QXX_FLASH_ID_MX25L1606E:
		strcpy(instance->flashName, "MX25L1606E");
		instance->flashSize = 2 * 1024 * 1024;
		instance->sectorSize = 4 * 1024;
		break;

	case W25QXX_FLASH_ID_W25Q64BV:
		strcpy(instance->flashName, "W25Q64");
		instance->flashSize = 8 * 1024 * 1024;
		instance->sectorSize = 4 * 1024;
		break;

	case W25QXX_FLASH_ID_W25Q128:
		strcpy(instance->flashName, "W25Q128");
		instance->flashSize = 16 * 1024 * 1024;
		instance->sectorSize = 4 * 1024;
		break;

	default:
		strcpy(instance->flashName, "Unknow Flash");
		instance->flashSize = 2 * 1024 * 1024;
		instance->sectorSize = 4 * 1024;
		break;
	}
}