#include "VDES.h"
#include "BoundingBox.h"
#include "VDESConfigure.h"
#include "UtilityInterface.h"
#include "AISBitsManager.h"
#include "spdlog/spdlog.h"
#include "TimerManager.h"
#include "ThreadPool.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

static void VDESBusinessNotifyHandle(VDES::VDESManager::EventType type, int retCode)
{
	if (type == VDES::VDESManager::EventType::MESSAGE_RECEIVE)
	{
		auto &manager = VDES::VDESManager::GetInstance();
		auto messgaes = manager.GetMessages(VDES::VDESManager::MailBox::INBOX);
		int a;
		a = 10;
	}
}

static void GenerateAtoNs(const uint32_t count = 10, const double latitude = 24.0345, 
						  const double longitude = 118.07853)
{
	for (auto i = 0; i < count; i++)
	{
		VDES::AISBitsManager aisBitsManager;

		aisBitsManager.Encode(2, 16);
		aisBitsManager.Encode(3, 16);
		aisBitsManager.Encode(994121001 + i, 30);
		aisBitsManager.Encode(1 + i, 5);
		aisBitsManager.Encode(static_cast<uint32_t>((longitude + i * 1.0 / count) * 600000), 28);
		aisBitsManager.Encode(static_cast<uint32_t>((latitude + i * 1.0 / count) * 600000), 27);
		aisBitsManager.Encode(0, 8);
		aisBitsManager.Encode(0, 1);
		aisBitsManager.Encode(0, 1);
		std::string atonName = fmt::format("ATON TEST {}", i);
		aisBitsManager.Encode(atonName.c_str(), 1, 0);
		auto spareBitsNum = 8 - ((676 + atonName.length() * 6) % 8);
		aisBitsManager.Encode(0, spareBitsNum);
		auto timestamp = VDES::UtilityInterface::GetCurrentTimeStamp();
		aisBitsManager.Encode(timestamp + i * 10, 32);
		aisBitsManager.Encode(0, 512);

		auto payload = aisBitsManager.GetEncodedVDMPayload();
		auto fillNum = aisBitsManager.GetFillBitsNumberToEncode();
		auto str = fmt::format("!AIVEM,1,1,0,412999001,412999002,{},{}", payload, fillNum);
		VDES::UtilityInterface::AddChecksum(str);
		std::cout << str << std::endl;
	}
}

static void GenerateObstacles(const uint32_t count = 10, const double latitude = 24.0345,
							  const double longitude = 118.07853)
{
    for (auto i = 0; i < count; i++)
    {
        VDES::AISBitsManager aisBitsManager;

        aisBitsManager.Encode(2, 16);
        aisBitsManager.Encode(8, 16);

        aisBitsManager.Encode(0, 3);
        aisBitsManager.Encode(1 + i % 13, 4);
		aisBitsManager.Encode(static_cast<uint32_t>((longitude + i * 1.0 / count) * 600000), 28);
		aisBitsManager.Encode(static_cast<uint32_t>((latitude + i * 1.0 / count) * 600000), 27);
        aisBitsManager.Encode(20 + i % 50, 7);
		
        aisBitsManager.Encode(8, 4);
        aisBitsManager.Encode(8, 5);
        aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(12, 6);

		aisBitsManager.Encode(9, 4);
		aisBitsManager.Encode(8, 5);
		aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(16, 6);

		aisBitsManager.Encode(1, 3);
		aisBitsManager.Encode(0, 512);

        auto payload = aisBitsManager.GetEncodedVDMPayload();
        auto fillNum = aisBitsManager.GetFillBitsNumberToEncode();
        auto str = fmt::format("!AIVEM,1,1,1,412999001,412999002,{},{}", payload, fillNum);
        VDES::UtilityInterface::AddChecksum(str);
		std::cout << str << std::endl;
    }
}

static void GenerateMaritimeOperations(const uint32_t count = 10, const double latitude = 24.0345,
	const double longitude = 118.07853)
{
	for (auto i = 0; i < count; i++)
	{
        VDES::AISBitsManager aisBitsManager;
		aisBitsManager.Encode(2, 16);
        aisBitsManager.Encode(8, 16);
		aisBitsManager.Encode(1, 3);
		aisBitsManager.Encode(1 + i % 4, 4);
		aisBitsManager.Encode(1, 3);
		aisBitsManager.Encode(412000001 + i, 30);
		aisBitsManager.Encode(static_cast<uint32_t>((longitude + 0.5 + i * 1.0 / count) * 600000), 28);
		aisBitsManager.Encode(static_cast<uint32_t>((latitude + 0.5 + i * 1.0 / count) * 600000), 27);
		aisBitsManager.Encode(static_cast<uint32_t>((longitude - 0.5 + i * 1.0 / count) * 600000), 28);
		aisBitsManager.Encode(static_cast<uint32_t>((latitude - 0.5 + i * 1.0 / count) * 600000), 27);
		aisBitsManager.Encode(20 + i % 50, 7);
		
		aisBitsManager.Encode(8, 4);
		aisBitsManager.Encode(8, 5);
		aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(12, 6);

		aisBitsManager.Encode(9, 4);
		aisBitsManager.Encode(8, 5);
		aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(16, 6);
		aisBitsManager.Encode(0, 3);
        aisBitsManager.Encode(0, 512);

		auto payload = aisBitsManager.GetEncodedVDMPayload();
		auto fillNum = aisBitsManager.GetFillBitsNumberToEncode();
		auto str = fmt::format("!AIVEM,1,1,2,412999001,412999002,{},{}", payload, fillNum);
		VDES::UtilityInterface::AddChecksum(str);
		std::cout << str << std::endl;
	}
}

static void GenerateMilitaryActivities(const uint32_t count = 10, const double latitude = 24.0345,
	const double longitude = 118.07853)
{
	for (auto i = 0; i < count; i++)
	{
        VDES::AISBitsManager aisBitsManager;
        aisBitsManager.Encode(2, 16);
        aisBitsManager.Encode(8, 16);
        aisBitsManager.Encode(2, 3);
        aisBitsManager.Encode(1 + i % 8, 4);
		aisBitsManager.Encode(0, 1);

		auto pointNum = 3 + rand() % 10;
		aisBitsManager.Encode(pointNum, 5);
		for (auto i = 0; i < pointNum; i++)
		{
			auto longitudeTemp = longitude + ((i % 2 == 0) ? i * 1.0 / count : i * -1.0 / count);
			auto latitudeTemp = latitude + ((i % 2 == 0) ? i * 2.0 / count : i * -2.0 / count);
			aisBitsManager.Encode(static_cast<uint32_t>(longitudeTemp * 600000), 28);
			aisBitsManager.Encode(static_cast<uint32_t>(latitudeTemp * 600000), 27);
		}
		aisBitsManager.Encode(8, 4);
		aisBitsManager.Encode(8, 5);
		aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(12, 6);

		aisBitsManager.Encode(9, 4);
		aisBitsManager.Encode(8, 5);
		aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(16, 6);
		aisBitsManager.Encode(0, 3);
		aisBitsManager.Encode(0, 512);

		aisBitsManager.Encode(1 + i % 4, 4);

		auto spareBits = 8 - ((45 + 55 * pointNum + 40 + 4 + 512) % 8);
		aisBitsManager.Encode(0, spareBits);
		aisBitsManager.Encode(0, 512);
		auto payload = aisBitsManager.GetEncodedVDMPayload();
		auto fillNum = aisBitsManager.GetFillBitsNumberToEncode();
		auto str = fmt::format("!AIVEM,1,1,3,412999001,412999002,{},{}", payload, fillNum);
		VDES::UtilityInterface::AddChecksum(str);
		std::cout << str << std::endl;
	}
}

static void GenerateDistress(const uint32_t count = 10, const double latitude = 24.0345,
	const double longitude = 118.07853)
{
	for (auto i = 0; i < count; i++)
	{
		VDES::AISBitsManager aisBitsManager;
        aisBitsManager.Encode(2, 16);
        aisBitsManager.Encode(8, 16);
        aisBitsManager.Encode(3, 3);
        aisBitsManager.Encode(1 + i % 11, 4);
		aisBitsManager.Encode(1 + i % 11, 4);
		aisBitsManager.Encode(1 + i % 4, 3);
		aisBitsManager.Encode(1 + i % 6, 4);
		aisBitsManager.Encode(static_cast<uint32_t>((longitude + i * 1.0 / count) * 600000), 28);
		aisBitsManager.Encode(static_cast<uint32_t>((latitude + i * 1.0 / count) * 600000), 27);

		aisBitsManager.Encode(8, 4);
		aisBitsManager.Encode(8, 5);
		aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(12, 6);

		aisBitsManager.Encode(9, 4);
		aisBitsManager.Encode(8, 5);
		aisBitsManager.Encode(10, 5);
		aisBitsManager.Encode(16, 6);
		aisBitsManager.Encode(0, 3);
		aisBitsManager.Encode(0, 512);

		auto payload = aisBitsManager.GetEncodedVDMPayload();
		auto fillNum = aisBitsManager.GetFillBitsNumberToEncode();
		auto str = fmt::format("!AIVEM,1,1,4,412999001,412999002,{},{}", payload, fillNum);
		VDES::UtilityInterface::AddChecksum(str);
		std::cout << str << std::endl;
	}
}



static void TestParse(void)
{
	static std::ifstream file("VDES_VDM.txt", std::ios::in);
	static uint32_t timerID;

	timerID = VDES::TimerManager::GetInstance().AddTimer(50, true, [&] {
		if (file.is_open())
		{
			std::string line;
			auto &instance = VDES::VDESManager::GetInstance();
			if (std::getline(file, line))
			{
				std::cout << line << std::endl;
				line.append("\r\n");
				instance.Parse(line.c_str(), line.length());
				return;
			}
		}
		VDES::TimerManager::GetInstance().RemoveTimer(timerID);
		file.close();
	});
}

static void GenerateDAC_412_FI_26(void)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);

	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(26, 6);
	// 发布时间
	bitsManager.Encode(22, 5);
	// 预报时间
	bitsManager.Encode(1, 7);
	// 天气现象
	bitsManager.Encode(1, 5);
	// 坐标
	bitsManager.Encode(118.12345 * 60, 15);
	bitsManager.Encode(24.12345 * 60, 14);
	// 风速
	bitsManager.Encode(10, 7);
	// 风向
	bitsManager.Encode(60, 9);
	// 气温
	bitsManager.Encode(200, 11);
	// 气压
	bitsManager.Encode(40, 10);
	// 能见度
	bitsManager.Encode(50, 8);
	// 信息来源
	bitsManager.Encode(1, 3);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
	}
}

// 海洋气象预报 (海区，带参数)
static std::vector<std::string> GenerateDAC_412_FI_27_WithArea(uint8_t areaCode)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(27, 6);

	// 发布时间
	bitsManager.Encode(21, 5);
	// 预报时间
	bitsManager.Encode(1, 7);

	// 海区
	bitsManager.Encode(areaCode, 7);
	// 天气现象
	bitsManager.Encode(4, 5);
	// 风向
	bitsManager.Encode(150, 9);
	// 风力级低值 
	bitsManager.Encode(8, 5);
	// 风力级高值
	bitsManager.Encode(12, 5);
	// 能见度
	bitsManager.Encode(10, 5);

	// 信息来源
	bitsManager.Encode(2, 3);
	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	return bitsManager.BuildPacket();
}

// 海洋气象预报 (海区)
static void GenerateDAC_412_FI_27(void)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(27, 6);

	// 发布时间
	bitsManager.Encode(21, 5);
	// 预报时间
	bitsManager.Encode(1, 7);

	// 海区
	bitsManager.Encode(3, 7);
	// 天气现象
	bitsManager.Encode(4, 5);
	// 风向
	bitsManager.Encode(150, 9);
	// 风力级低值 
	bitsManager.Encode(8, 5);
	// 风力级高值
	bitsManager.Encode(12, 5);
	// 能见度
	bitsManager.Encode(10, 5);

	// 信息来源
	bitsManager.Encode(2, 3);
	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
	}
}

// 海洋环境预报(坐标)
static void GenerateDAC_412_FI_28(void)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(28, 6);

	// 发布时间
	bitsManager.Encode(22, 5);
	// 预报时间
	bitsManager.Encode(1, 7);

	// 坐标
	bitsManager.Encode(118.58 * 60, 15);
	bitsManager.Encode(24.58 * 60, 14);
	// 流速
	bitsManager.Encode(50, 10);
	// 流向
	bitsManager.Encode(230, 9);
	// 浪高
	bitsManager.Encode(20, 8);
	// 浪向
	bitsManager.Encode(145, 9);
	// 海温
	bitsManager.Encode(40, 10);
	
	// 信息来源
	bitsManager.Encode(2, 3);
	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
	}
}

// 海洋环境预报 (海区，带参数)
static std::vector<std::string> GenerateDAC_412_FI_29_WithArea(uint8_t areaCode)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);

	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(29, 6);
	// 发布时间
	bitsManager.Encode(22, 5);
	// 预报时间
	bitsManager.Encode(1, 7);

	// 海区
	bitsManager.Encode(areaCode, 5);
	// 海温低值
	bitsManager.Encode(30, 9);
	// 海温高值
	bitsManager.Encode(10, 8);
	// 流向平均
	bitsManager.Encode(150, 9);
	// 流向最大
	bitsManager.Encode(225, 9);
	// 流速平均
	bitsManager.Encode(50, 8);
	// 流速最大
	bitsManager.Encode(100, 8);
	// 浪高
	bitsManager.Encode(20, 8);
	// 涌浪
	bitsManager.Encode(30, 8);
	// 信息来源
	bitsManager.Encode(2, 3);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	return bitsManager.BuildPacket();
}

// 海洋环境预报(海区)
static void GenerateDAC_412_FI_29(void)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);

	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(29, 6);
	// 发布时间
	bitsManager.Encode(22, 5);
	// 预报时间
	bitsManager.Encode(1, 7);

	// 海区
	bitsManager.Encode(2, 5);
	// 海温低值
	bitsManager.Encode(30, 9);
	// 海温高值
	bitsManager.Encode(10, 8);
	// 流向平均
	bitsManager.Encode(150, 9);
	// 流向最大
	bitsManager.Encode(225, 9);
	// 流速平均
	bitsManager.Encode(50, 8);
	// 流速最大
	bitsManager.Encode(100, 8);
	// 浪高
	bitsManager.Encode(20, 8);
	// 涌浪
	bitsManager.Encode(30, 8);
	// 信息来源
	bitsManager.Encode(2, 3);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
	}
}


static std::vector<std::string> GenerateDAC_412_FI_30(void)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(30, 6);
	// 发布时间
	bitsManager.Encode(20, 5);
	// 预报时间
	bitsManager.Encode(1, 7);

	// 近岸水域编码
	bitsManager.Encode(1, 8);
	// 海温低值 
	bitsManager.Encode(50, 9);
	// 海温高值
	bitsManager.Encode(40, 8);
	// 浪高低值
	bitsManager.Encode(150, 8);
	// 浪高高值
	bitsManager.Encode(200, 8);
	// 高潮时间
	bitsManager.Encode(11, 5);
	bitsManager.Encode(4, 5);
	bitsManager.Encode(30, 6);
	// 高潮潮高 (10 bits, raw 860cm tide height with -100cm base is 960)
	bitsManager.Encode(960, 10);
	// 低潮时间
	bitsManager.Encode(11, 5);
	bitsManager.Encode(8, 5);
	bitsManager.Encode(38, 6);
	// 低潮潮高 (9 bits, raw 200cm tide height with -100cm base is 300)
	bitsManager.Encode(300, 9);

	// 信息来源
	bitsManager.Encode(3, 3);
	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		SPDLOG_DEBUG("VDM = {}", vdm);
		std::cout << "GEN VDM (FI=30 along shore): " << sentence;
	}
	return results;
}

// 潮汐
static std::vector<std::string> GenerateDAC_412_FI_32(void)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(32, 6);

	// 发布时间
	bitsManager.Encode(10, 5);

	// 经度 #1
	bitsManager.Encode(119.113868 * 60, 15);
	// 纬度 #1
	bitsManager.Encode(24.35993 * 60, 14);
	// 潮位基准面 #1
	bitsManager.Encode(500, 9);
	// 高潮时间 #1
	bitsManager.Encode(12, 5);
	bitsManager.Encode(2, 5);
	bitsManager.Encode(10, 6);
	// 高潮潮高 #1
	bitsManager.Encode(860, 10);
	// 低潮时间 #1
	bitsManager.Encode(12, 5);
	bitsManager.Encode(3, 5);
	bitsManager.Encode(18, 6);
	// 低潮潮高 #1
	bitsManager.Encode(20, 9);

	// 经度 #2
	bitsManager.Encode(30, 7);
	// 纬度 #2
	bitsManager.Encode(20, 7);
	// 潮位基准面 #2
	bitsManager.Encode(500, 9);
	// 高潮时间 #2
	bitsManager.Encode(12, 5);
	bitsManager.Encode(5, 5);
	bitsManager.Encode(32, 6);
	// 高潮潮高 #2
	bitsManager.Encode(750, 10);
	// 低潮时间 #2
	bitsManager.Encode(12, 5);
	bitsManager.Encode(6, 5);
	bitsManager.Encode(34, 6);
	// 低潮潮高 #2
	bitsManager.Encode(10, 9);
	// 信息来源
	bitsManager.Encode(2, 3);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
		std::cout << "GEN VDM (FI=32 tide forecast): " << vdm << std::endl;
	}
	return vdms;
}

// 碍航物
static std::vector<std::string> GenerateDAC_412_FI_35(uint8_t geometryType)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(35, 6);
	
	// MRN
	bitsManager.Encode(100, 17);
	// 片段描述
	bitsManager.Encode(0, 2);
	// 碍航物类型
	bitsManager.Encode(2, 5);
	// 几何图形类型
	bitsManager.Encode(geometryType, 2);

	if (geometryType == 0)
	{
		// 点、圆形 (Table 96, 62 bits)
		// 中心点经度 (119.029)
		bitsManager.Encode(static_cast<uint32_t>(119.029365 * 600000.0), 28);
		// 中心点纬度 (24.6888)
		bitsManager.Encode(static_cast<uint32_t>(24.688816 * 600000.0), 27);
		// 半径 (20 * 100m = 2000m)
		bitsManager.Encode(20, 7);
	}
	else if (geometryType == 1)
	{
		// 扇形 (Table 98, 80 bits)
		// 中心点经度 (119.029)
		bitsManager.Encode(static_cast<uint32_t>(119.029365 * 600000.0), 28);
		// 中心点纬度 (24.6888)
		bitsManager.Encode(static_cast<uint32_t>(24.688816 * 600000.0), 27);
		// 起始边角度 (30 degrees)
		bitsManager.Encode(30, 9);
		// 终止边角度 (120 degrees)
		bitsManager.Encode(120, 9);
		// 半径 (50 * 0.1nm = 5.0nm)
		bitsManager.Encode(50, 7);
	}
	else if (geometryType == 2)
	{
		// 多边形 (Table 100, 76 + 33*(n-2) = 109 bits for n=3)
		// 经度 #1 (119.02)
		bitsManager.Encode(static_cast<uint32_t>(119.02 * 6000.0), 22);
		// 纬度 #1 (24.68)
		bitsManager.Encode(static_cast<uint32_t>(24.68 * 6000.0), 21);
		// 经度 #2 (offset +0.02)
		bitsManager.Encode(static_cast<uint32_t>(0.02 * 6000.0), 17);
		// 纬度 #2 (offset +0.01)
		bitsManager.Encode(static_cast<uint32_t>(0.01 * 6000.0), 16);
		// 经度 #3 (offset -0.01)
		bitsManager.Encode(static_cast<uint32_t>(static_cast<int32_t>(-0.01 * 6000.0) & 0x1FFFF), 17);
		// 纬度 #3 (offset -0.02)
		bitsManager.Encode(static_cast<uint32_t>(static_cast<int32_t>(-0.02 * 6000.0) & 0xFFFF), 16);
	}

	// 生效时间 (month=9, day=11, hour=4, minute=30)
	bitsManager.Encode(9, 4);
	bitsManager.Encode(11, 5);
	bitsManager.Encode(4, 5);
	bitsManager.Encode(30, 6);
	// 有效期 (1: 1天)
	bitsManager.Encode(1, 2);
	// 注意事项 (1: 禁止驶入)
	bitsManager.Encode(1, 4);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
		std::cout << "GEN VDM (FI=35 Obstruction, geometryType=" << (int)geometryType << "): " << vdm << std::endl;
	}
	return vdms;
}

// 水上水下活动
static std::vector<std::string> GenerateDAC_412_FI_36(int geometryType)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(36, 6);

	// MRN
	bitsManager.Encode(101, 17);
	// 片段描述
	bitsManager.Encode(0, 2);
	// 活动类型
	bitsManager.Encode(2, 6);
	// 施工船MMSI
	bitsManager.Encode(412001002, 30);
	// 几何图形
	bitsManager.Encode(geometryType, 2);

	if (geometryType == 0)
	{
		// 中心点经度
		bitsManager.Encode(static_cast<int64_t>(119.1399683 * 6000.0), 22);
		// 中心点纬度
		bitsManager.Encode(static_cast<int64_t>(24.523708 * 6000.0), 21);
		// 半径 (10 = 1.0 nm)
		bitsManager.Encode(10, 7);
	}
	else if (geometryType == 1)
	{
		// 中心点经度
		bitsManager.Encode(static_cast<int64_t>(119.1399683 * 6000.0), 22);
		// 中心点纬度
		bitsManager.Encode(static_cast<int64_t>(24.523708 * 6000.0), 21);
		// 起始边角度
		bitsManager.Encode(45, 9);
		// 终止边角度
		bitsManager.Encode(135, 9);
		// 半径 (30 = 3.0 nm)
		bitsManager.Encode(30, 7);
	}
	else if (geometryType == 2 || geometryType == 3)
	{
		// Point #1
		bitsManager.Encode(static_cast<int64_t>(119.12 * 6000.0), 22);
		bitsManager.Encode(static_cast<int64_t>(24.52 * 6000.0), 21);
		// Point #2 relative offsets
		bitsManager.Encode(static_cast<int64_t>(0.02 * 6000.0), 17);
		bitsManager.Encode(static_cast<int64_t>(0.01 * 6000.0), 16);
		// Point #3 relative offsets
		bitsManager.Encode(static_cast<int64_t>(-0.01 * 6000.0), 17);
		bitsManager.Encode(static_cast<int64_t>(-0.02 * 6000.0), 16);
	}

	// 时间类型
	bitsManager.Encode(0, 1);

	// 起始时间
	bitsManager.Encode(9, 4);
	bitsManager.Encode(11, 5);
	bitsManager.Encode(5, 5);
	bitsManager.Encode(26, 6);

	// 结束时间
	bitsManager.Encode(9, 4);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(5, 5);
	bitsManager.Encode(50, 6);

	// 注意事项
	bitsManager.Encode(1, 4);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
		std::cout << "GEN VDM (FI=36 activities, geometryType=" << geometryType << "): " << vdm << std::endl;
	}
	return vdms;
}

// 军事活动
// 军事活动
static std::vector<std::string> GenerateDAC_412_FI_38(uint8_t geometryType)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(38, 6);

	// MRN
	bitsManager.Encode(12345, 17);
	// 片段描述
	bitsManager.Encode(0, 2);
	// 活动类型
	bitsManager.Encode(2, 4); // theme=2 (军事演习)
	// 几何图形类型
	bitsManager.Encode(geometryType, 2);

	if (geometryType == 0)
	{
		// 中心点经度
		bitsManager.Encode(static_cast<int64_t>(119.12345 * 6000.0), 22);
		// 中心点纬度
		bitsManager.Encode(static_cast<int64_t>(24.54321 * 6000.0), 21);
		// 半径 (15 = 1.5 nm)
		bitsManager.Encode(15, 7);
	}
	else if (geometryType == 1)
	{
		// 中心点经度
		bitsManager.Encode(static_cast<int64_t>(119.12345 * 6000.0), 22);
		// 中心点纬度
		bitsManager.Encode(static_cast<int64_t>(24.54321 * 6000.0), 21);
		// 起始边角度
		bitsManager.Encode(60, 9);
		// 终止边角度
		bitsManager.Encode(120, 9);
		// 半径 (25 = 2.5 nm)
		bitsManager.Encode(25, 7);
	}
	else if (geometryType == 2 || geometryType == 3)
	{
		// Point #1
		bitsManager.Encode(static_cast<int64_t>(119.12345 * 6000.0), 22);
		bitsManager.Encode(static_cast<int64_t>(24.54321 * 6000.0), 21);
		// Point #2 relative offsets
		bitsManager.Encode(static_cast<int64_t>(0.03 * 6000.0), 17);
		bitsManager.Encode(static_cast<int64_t>(0.02 * 6000.0), 16);
		// Point #3 relative offsets
		bitsManager.Encode(static_cast<int64_t>(-0.02 * 6000.0), 17);
		bitsManager.Encode(static_cast<int64_t>(-0.03 * 6000.0), 16);
	}

	// 时间类型 (timeType)
	bitsManager.Encode(0, 1);

	// 起始时间
	bitsManager.Encode(9, 4);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(5, 5);
	bitsManager.Encode(26, 6);

	// 结束时间
	bitsManager.Encode(9, 4);
	bitsManager.Encode(14, 5);
	bitsManager.Encode(5, 5);
	bitsManager.Encode(50, 6);

	// 注意事项
	bitsManager.Encode(3, 4); // cautionCode=3 (航道封航)

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=38 military, geometryType=" << (int)geometryType << "): " << sentence;
	}
	return results;
}

static std::vector<std::string> GenerateDAC_412_FI_39(void)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(39, 6);

	// MRN
	bitsManager.Encode(54321, 17);
	// 片段描述
	bitsManager.Encode(0, 2);
	// 类型
	bitsManager.Encode(2, 4); // 搁浅
	// 状况描述: 落水+失踪 (Bit 3 & Bit 4) -> 24
	bitsManager.Encode(24, 16);
	// 判断位
	bitsManager.Encode(1, 3);
	// 经度 (118.07853)
	bitsManager.Encode(static_cast<int64_t>(118.07853 * 6000.0), 22);
	// 纬度 (24.0345)
	bitsManager.Encode(static_cast<int64_t>(24.0345 * 6000.0), 21);
	// 位置说明
	bitsManager.Encode(0, 1);
	// 时间: 9月12日5时26分
	bitsManager.Encode(9, 4);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(5, 5);
	bitsManager.Encode(26, 6);
	// 发布时效 (12h)
	bitsManager.Encode(12, 6);
	// 注意事项 (1 = 请求援助)
	bitsManager.Encode(1, 4);
	// 备用填充位
	bitsManager.Encode(0, 4);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=39 distress): " << sentence;
	}
	return results;
}

static std::vector<std::string> GenerateDAC_412_FI_40(uint8_t testCase)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4124001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(40, 6);

	// MRN
	bitsManager.Encode(65432, 17);
	// 片段描述
	bitsManager.Encode(0, 2);
	// 类型
	bitsManager.Encode(1, 5); // 禁航区

	if (testCase == 0) // 圆形
	{
		bitsManager.Encode(0, 2); // 圆形
		// 经度 (118.1234)
		bitsManager.Encode(static_cast<int64_t>(118.1234 * 6000.0), 22);
		// 纬度 (24.4567)
		bitsManager.Encode(static_cast<int64_t>(24.4567 * 6000.0), 21);
		// 半径 (2.5 nm -> 25)
		bitsManager.Encode(25, 7);
	}
	else if (testCase == 1) // 扇形
	{
		bitsManager.Encode(1, 2); // 扇形
		// 经度 (118.1234)
		bitsManager.Encode(static_cast<int64_t>(118.1234 * 6000.0), 22);
		// 纬度 (24.4567)
		bitsManager.Encode(static_cast<int64_t>(24.4567 * 6000.0), 21);
		// 起始角 (30°)
		bitsManager.Encode(30, 9);
		// 终止角 (120°)
		bitsManager.Encode(120, 9);
		// 半径 (5.0 nm -> 50)
		bitsManager.Encode(50, 7);
	}
	else if (testCase == 2) // 矩形 (geoType = 2, points count = 2)
	{
		bitsManager.Encode(2, 2); // 多边形
		// 经度 #1 (118.1)
		bitsManager.Encode(static_cast<int64_t>(118.1 * 6000.0), 22);
		// 纬度 #1 (24.5)
		bitsManager.Encode(static_cast<int64_t>(24.5 * 6000.0), 21);

		// Relative Point #2: Lon -0.1 (-600), Lat -0.1 (-600)
		bitsManager.Encode(static_cast<int64_t>(-600), 17);
		bitsManager.Encode(static_cast<int64_t>(-600), 16);
	}
	else // 三角形 (geoType = 2, points count = 3)
	{
		bitsManager.Encode(2, 2); // 多边形
		// 经度 #1 (118.1)
		bitsManager.Encode(static_cast<int64_t>(118.1 * 6000.0), 22);
		// 纬度 #1 (24.5)
		bitsManager.Encode(static_cast<int64_t>(24.5 * 6000.0), 21);

		// Point #2 relative to #1: Lon +0.1 (+600), Lat -0.1 (-600)
		bitsManager.Encode(static_cast<int64_t>(600), 17);
		bitsManager.Encode(static_cast<int64_t>(-600), 16);

		// Point #3 relative to #2: Lon -0.2 (-1200), Lat 0 (0)
		bitsManager.Encode(static_cast<int64_t>(-1200), 17);
		bitsManager.Encode(static_cast<int64_t>(0), 16);
	}

	// 时间类型 (0: 连续)
	bitsManager.Encode(0, 1);
	// 开始时间 (9月12日5时26分)
	bitsManager.Encode(9, 4);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(5, 5);
	bitsManager.Encode(26, 6);
	// 结束时间 (9月15日8时30分)
	bitsManager.Encode(9, 4);
	bitsManager.Encode(15, 5);
	bitsManager.Encode(8, 5);
	bitsManager.Encode(30, 6);
	// 注意事项 (2: 禁止驶入)
	bitsManager.Encode(2, 4);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=40 designated area testCase=" << (int)testCase << "): " << sentence;
	}
	return results;
}

// 船舶遇险
static std::vector<std::string> GenerateDAC_412_FI_33(uint8_t status)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(33, 6);

	// MRN
	bitsManager.Encode(12345, 17);
	// 片段描述
	bitsManager.Encode(0, 2);
	// 状态
	bitsManager.Encode(status, 5);

	if (status == 1)
	{
		// 新设: Table 62 (112 bits)
		// MRN
		bitsManager.Encode(67890, 17);
		// 片段描述 (1: 需要额外的文本补充片段)
		bitsManager.Encode(1, 2);
		// 航标类型
		bitsManager.Encode(5, 5);
		// 经度 (step 1/10000', 119.5432 degrees)
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度 (24.3210 degrees)
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
		// 节奏名称编码
		bitsManager.Encode(2, 5);
		// 节奏参数编码
		bitsManager.Encode(3, 5);
		// 标体颜色
		bitsManager.Encode(1, 5);
		// 灯光颜色
		bitsManager.Encode(2, 4);
		// 灯质周期
		bitsManager.Encode(6, 4);
		// 作用距离
		bitsManager.Encode(12, 5);
		// 莫尔斯编码
		bitsManager.Encode(4, 5);
	}
	else if (status == 5)
	{
		// 故障: Table 75 (83 bits)
		// MRN
		bitsManager.Encode(67890, 17);
		// 片段描述
		bitsManager.Encode(1, 2);
		// 故障类型
		bitsManager.Encode(2, 4); // 灯光熄灭
		// 航标类型
		bitsManager.Encode(5, 5);
		// 经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
	}
	else if (status == 8)
	{
		// 改变灯质: Table 77 (102 bits)
		// MRN
		bitsManager.Encode(67890, 17);
		// 片段描述
		bitsManager.Encode(1, 2);
		// 航标类型
		bitsManager.Encode(5, 5);
		// 经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
		// 改变后节奏名称
		bitsManager.Encode(4, 5);
		// 改变后节奏参数
		bitsManager.Encode(5, 5);
		// 改变后灯光颜色
		bitsManager.Encode(3, 4);
		// 改变后灯质周期
		bitsManager.Encode(8, 4);
		// 改变后作用距离
		bitsManager.Encode(15, 5);
	}
	else if (status == 9)
	{
		// 莫尔斯编码变更: Table 79 (84 bits)
		// MRN
		bitsManager.Encode(67890, 17);
		// 片段描述
		bitsManager.Encode(1, 2);
		// 航标类型
		bitsManager.Encode(5, 5);
		// 经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
		// 改变后莫尔斯编码
		bitsManager.Encode(6, 5);
	}
	else if (status == 10)
	{
		// 冰标变更等: Table 81 (74 bits)
		// MRN
		bitsManager.Encode(67890, 17);
		// 片段描述
		bitsManager.Encode(1, 2);
		// 经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
	}
	else if (status == 14)
	{
		// 临时撤除: Table 83 (119 bits)
		// MRN
		bitsManager.Encode(67890, 17);
		// 片段描述
		bitsManager.Encode(1, 2);
		// 临时撤除起始时间 (月/日/时/分)
		bitsManager.Encode(10, 4); // 10月
		bitsManager.Encode(15, 5); // 15日
		bitsManager.Encode(8, 5);  // 8时
		bitsManager.Encode(30, 6); // 30分
		// 恢复日期与时间 (月/日/时/分)
		bitsManager.Encode(12, 4); // 12月
		bitsManager.Encode(31, 5); // 31日
		bitsManager.Encode(23, 5); // 23时
		bitsManager.Encode(59, 6); // 59分
		// 航标类型
		bitsManager.Encode(5, 5);
		// 经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
	}
	else if (status == 15)
	{
		// 临时位置调整: Table 85 (174 bits)
		// MRN
		bitsManager.Encode(67890, 17);
		// 片段描述
		bitsManager.Encode(1, 2);
		// 临时位置调整起始时间 (月/日/时/分)
		bitsManager.Encode(10, 4); // 10月
		bitsManager.Encode(15, 5); // 15日
		bitsManager.Encode(8, 5);  // 8时
		bitsManager.Encode(30, 6); // 30分
		// 恢复日期与时间 (月/日/时/分)
		bitsManager.Encode(12, 4); // 12月
		bitsManager.Encode(31, 5); // 31日
		bitsManager.Encode(23, 5); // 23时
		bitsManager.Encode(59, 6); // 59分
		// 航标类型
		bitsManager.Encode(5, 5);
		// 改变前位置经度
		bitsManager.Encode(static_cast<uint32_t>(118.5432 * 600000.0), 28);
		// 改变前位置纬度
		bitsManager.Encode(static_cast<uint32_t>(23.3210 * 600000.0), 27);
		// 改变后位置经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 改变后位置纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
	}
	else
	{
		// Fallback for other statuses to prevent empty body
		bitsManager.Encode(67890, 17);
		bitsManager.Encode(0, 2);
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
	}

	// 注意事项 (precaution)
	bitsManager.Encode(3, 4);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
		std::cout << "GEN VDM (FI=33 AtoN dynamics, status=" << (int)status << "): " << vdm << std::endl;
	}
	return vdms;
}

static std::vector<std::string> GenerateDAC_412_FI_34(uint8_t status)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(34, 6);

	// 航标属性
	bitsManager.Encode(1, 3);
	// 状态
	bitsManager.Encode(status, 4);

	if (status == 1)
	{
		// 新设: Table 88 (115 bits)
		// MMSI #1
		bitsManager.Encode(994121001, 30);
		// 航标 MRN #1
		bitsManager.Encode(67890, 17);
		// 片段描述 #1
		bitsManager.Encode(0, 2);
		// 航标类型 #1
		bitsManager.Encode(5, 6);
		// 经度 (step 1/10000', 119.5432 degrees)
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度 (24.3210 degrees)
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
		// 作用距离
		bitsManager.Encode(12, 5);
	}
	else if (status == 2 || status == 5 || status == 6)
	{
		// 撤除、故障、恢复工作: Table 91 (110 bits)
		// MMSI #1
		bitsManager.Encode(994121001, 30);
		// 航标 MRN #1
		bitsManager.Encode(67890, 17);
		// 片段描述 #1
		bitsManager.Encode(0, 2);
		// 航标类型 #1
		bitsManager.Encode(5, 6);
		// 经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
	}
	else if (status == 3 || status == 4)
	{
		// 移位、位置调整: Table 93 (166 bits)
		// MMSI #1
		bitsManager.Encode(994121001, 30);
		// 航标 MRN #1
		bitsManager.Encode(67890, 17);
		// 片段描述 #1
		bitsManager.Encode(0, 2);
		// 航标类型 #1
		bitsManager.Encode(5, 6);
		// 改变前位置经度 (119.5400)
		bitsManager.Encode(static_cast<uint32_t>(119.5400 * 600000.0), 28);
		// 改变前位置纬度 (24.3200)
		bitsManager.Encode(static_cast<uint32_t>(24.3200 * 600000.0), 27);
		// 改变后位置说明
		bitsManager.Encode(1, 1); // 概位
		// 改变后位置经度
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 改变后位置纬度
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
	}
	else
	{
		// Fallback
	}

	// 注意事项 (precaution)
	bitsManager.Encode(3, 4);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
		std::cout << "GEN VDM (FI=34 AIS AtoN dynamics, status=" << (int)status << "): " << vdm << std::endl;
	}
	return vdms;
}

// 桥梁
static std::vector<std::string> GenerateDAC_412_FI_41(void)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(41, 6);

	// MRN
	bitsManager.Encode(12345, 17);
	// 片段描述
	bitsManager.Encode(0, 2);
	// 流速 (流速为 123 cm/s)
	bitsManager.Encode(123, 9);
	// 流向 (流向为 145°)
	bitsManager.Encode(145, 9);

	// 发布时间: 日 (15), 时 (10), 分 (30), 秒 (45) -> 共22位
	bitsManager.Encode(15, 5);
	bitsManager.Encode(10, 5);
	bitsManager.Encode(30, 6);
	bitsManager.Encode(45, 6);

	// Span #1:
	// 中心经度 (118.0793)
	bitsManager.Encode(static_cast<int64_t>(118.0793 * 600000.0), 28);
	// 中心纬度 (24.580496)
	bitsManager.Encode(static_cast<int64_t>(24.580496 * 600000.0), 27);
	// 净高 (45.5 m -> 455)
	bitsManager.Encode(455, 10);
	// 净宽 (300 m -> 300)
	bitsManager.Encode(300, 10);
	// 通行能力 (双向 -> 3)
	bitsManager.Encode(3, 2);
	// 通行方向 (通航角 120°)
	bitsManager.Encode(120, 9);
	// 允许会遇 (允许 -> 1)
	bitsManager.Encode(1, 1);
	// 允许超越 (允许 -> 1)
	bitsManager.Encode(1, 1);

	// Span #2:
	// 中心经度 (118.0893)
	bitsManager.Encode(static_cast<int64_t>(118.0893 * 600000.0), 28);
	// 中心纬度 (24.590496)
	bitsManager.Encode(static_cast<int64_t>(24.590496 * 600000.0), 27);
	// 净高 (50.0 m -> 500)
	bitsManager.Encode(500, 10);
	// 净宽 (400 m -> 400)
	bitsManager.Encode(400, 10);
	// 通行能力 (双向 -> 3)
	bitsManager.Encode(3, 2);
	// 通行方向 (通航角 130°)
	bitsManager.Encode(130, 9);
	// 允许会遇 (允许 -> 1)
	bitsManager.Encode(1, 1);
	// 允许超越 (允许 -> 1)
	bitsManager.Encode(1, 1);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=41 bridge): " << sentence;
	}
	return results;
}

static std::vector<std::string> GenerateDAC_412_FI_45(void)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(45, 6);

	// 渔网标识类型
	bitsManager.Encode(3, 4);
	// 关联
	bitsManager.Encode(1, 1);

	// MRN #1
	bitsManager.Encode(100001, 20);
	// 经度 1
	bitsManager.Encode(static_cast<int64_t>(118.5399 * 60000), 25);
	// 纬度 1
	bitsManager.Encode(static_cast<int64_t>(24.4186 * 60000), 24);

	// MRN #2
	bitsManager.Encode(100002, 20);
	// 经度 2
	bitsManager.Encode(static_cast<int64_t>(0.12 * 60000), 15);
	// 纬度 2
	bitsManager.Encode(static_cast<int64_t>(0.06 * 60000), 14);

	// MRN #3
	bitsManager.Encode(100003, 20);
	// 经度 3
	bitsManager.Encode(static_cast<int64_t>(0.05 * 60000), 15);
	// 纬度 3
	bitsManager.Encode(static_cast<int64_t>(0.03 * 60000), 14);
	
	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	for (auto &vdm : vdms)
	{
		SPDLOG_DEBUG("VDM = {}", vdm);
		std::cout << "GEN VDM (FI=45 net sounder): " << vdm << std::endl;
	}
	return vdms;
}

// Marine Meteorology & Environmental Warning (FI = 31)
static std::vector<std::string> GenerateDAC_412_FI_31(uint8_t warningType)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(31, 6);

	// Warning Type
	bitsManager.Encode(warningType, 4);

	if (warningType == 1)
	{
		// Cyclone MRN
		bitsManager.Encode(100, 17);
		// Fragment
		bitsManager.Encode(0, 2);

		// Path Point 1
		// Time (day:5, hour:5, minute:6)
		bitsManager.Encode(23, 5);
		bitsManager.Encode(10, 5);
		bitsManager.Encode(30, 6);
		// Coordinates
		bitsManager.Encode(static_cast<uint32_t>(118.5432 * 6000), 22);
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 6000), 21);
		bitsManager.Encode(2, 3); // Cyclone type
		bitsManager.Encode(150, 10); // Radius wind 7
		bitsManager.Encode(80, 8); // Radius wind 10
		bitsManager.Encode(40, 7); // Radius wind 12
		bitsManager.Encode(25, 6); // Move speed
		bitsManager.Encode(180, 9); // Move direction
		bitsManager.Encode(12, 5); // Max wind scale
		bitsManager.Encode(160, 9); // Center pressure

		// Path Point 2
		// Time
		bitsManager.Encode(23, 5);
		bitsManager.Encode(16, 5);
		bitsManager.Encode(0, 6);
		// Coordinates
		bitsManager.Encode(static_cast<uint32_t>(119.1234 * 6000), 22);
		bitsManager.Encode(static_cast<uint32_t>(25.0123 * 6000), 21);
		bitsManager.Encode(2, 3); // Cyclone type
		bitsManager.Encode(160, 10); // Radius wind 7
		bitsManager.Encode(90, 8); // Radius wind 10
		bitsManager.Encode(45, 7); // Radius wind 12
		bitsManager.Encode(30, 6); // Move speed
		bitsManager.Encode(190, 9); // Move direction
		bitsManager.Encode(13, 5); // Max wind scale
		bitsManager.Encode(150, 9); // Center pressure
	}
	else if (warningType == 2 || warningType == 3 || warningType == 4)
	{
		// General Warnings
		// Element 1
		bitsManager.Encode(201, 17); // MRN
		bitsManager.Encode(5, 7); // Sea Area Code
		bitsManager.Encode(2, 2); // Warning Level
		// Element 2
		bitsManager.Encode(202, 17); // MRN
		bitsManager.Encode(12, 7); // Sea Area Code
		bitsManager.Encode(3, 2); // Warning Level
	}
	else if (warningType == 5)
	{
		// Storm Surge
		// Element 1
		bitsManager.Encode(301, 17); // MRN
		bitsManager.Encode(15, 6); // City Code
		bitsManager.Encode(25, 5); // Surge Height (raw 25 * 0.1 = 2.5m)
		bitsManager.Encode(1, 2); // Warning Level
		// Element 2
		bitsManager.Encode(302, 17); // MRN
		bitsManager.Encode(32, 6); // City Code
		bitsManager.Encode(12, 5); // Surge Height (raw 12 * 0.1 = 1.2m)
		bitsManager.Encode(3, 2); // Warning Level
	}
	else if (warningType == 6)
	{
		// Ice Warning
		bitsManager.Encode(401, 17); // MRN
		bitsManager.Encode(4, 7); // Region Code
		bitsManager.Encode(2, 2); // Warning Level
		bitsManager.Encode(0, 5); // Spare (5 bits)
	}

	// Publish Time (month:4, day:5, hour:5, minute:6 = 20 bits)
	bitsManager.Encode(6, 4); // June
	bitsManager.Encode(23, 5); // 23rd
	bitsManager.Encode(10, 5); // 10:00
	bitsManager.Encode(30, 6); // 30m

	// Start Time
	bitsManager.Encode(6, 4);
	bitsManager.Encode(23, 5);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(0, 6);

	// End Time
	bitsManager.Encode(6, 4);
	bitsManager.Encode(24, 5);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(0, 6);

	// Duration
	bitsManager.Encode(24, 6);
	// Info Source
	bitsManager.Encode(1, 3);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=31 WarningType=" << (int)warningType << "): " << sentence;
	}
	return results;
}

// Maritime Towing (FI = 37)
static std::vector<std::string> GenerateDAC_412_FI_37(void)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(37, 6);

	// MRN
	bitsManager.Encode(201, 17);
	// Fragment
	bitsManager.Encode(0, 2);
	// MMSI
	bitsManager.Encode(412001234, 30);

	// Node 1
	bitsManager.Encode(static_cast<uint32_t>(118.456 * 6000), 22);
	bitsManager.Encode(static_cast<uint32_t>(24.123 * 6000), 21);

	// Node 2
	bitsManager.Encode(static_cast<uint32_t>(118.789 * 6000), 22);
	bitsManager.Encode(static_cast<uint32_t>(24.345 * 6000), 21);

	// Towing Method (2 bits)
	bitsManager.Encode(1, 2); // 1 = 傍拖 (Side towing)

	// Length
	bitsManager.Encode(350, 12);
	// Width
	bitsManager.Encode(45, 7);
	// Speed
	bitsManager.Encode(125, 8); // 12.5 knots

	// Start Time
	bitsManager.Encode(6, 4);
	bitsManager.Encode(23, 5);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(0, 6);

	// End Time
	bitsManager.Encode(6, 4);
	bitsManager.Encode(23, 5);
	bitsManager.Encode(18, 5);
	bitsManager.Encode(0, 6);

	// Caution Code
	bitsManager.Encode(2, 4);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=37 Towing): " << sentence;
	}
	return results;
}

// Navigational Channel Centerline (FI = 42)
static std::vector<std::string> GenerateDAC_412_FI_42(void)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(42, 6);

	// MRN
	bitsManager.Encode(301, 17);
	// Fragment
	bitsManager.Encode(0, 2);

	// Node 1 (fixed point 28/27 bits)
	bitsManager.Encode(static_cast<uint32_t>(118.123456 * 600000), 28);
	bitsManager.Encode(static_cast<uint32_t>(24.654321 * 600000), 27);

	// Node 2 (offset 24/23 bits)
	bitsManager.Encode(static_cast<uint32_t>(0.02 * 600000), 24);
	bitsManager.Encode(static_cast<uint32_t>(0.01 * 600000), 23);

	// Width
	bitsManager.Encode(150, 9);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=42 Channel Centerline): " << sentence;
	}
	return results;
}

// Navigational Channel Boundary (FI = 43/44)
static std::vector<std::string> GenerateDAC_412_FI_43(uint8_t edgeType)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(edgeType == 0 ? 43 : 44, 6);

	// MRN
	bitsManager.Encode(302, 17);
	// Fragment
	bitsManager.Encode(0, 2);

	// Node 1 (fixed point 28/27 bits)
	bitsManager.Encode(static_cast<uint32_t>(118.234567 * 600000), 28);
	bitsManager.Encode(static_cast<uint32_t>(24.765432 * 600000), 27);

	// Node 2 (offset 24/23 bits)
	bitsManager.Encode(static_cast<uint32_t>(0.03 * 600000), 24);
	bitsManager.Encode(static_cast<uint32_t>(0.02 * 600000), 23);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (FI=" << (edgeType == 0 ? 43 : 44) << " Channel Boundary, edgeType=" << (int)edgeType << "): " << sentence;
	}
	return results;
}

// Frontend Prompt Text (FI = 5)
static std::vector<std::string> GenerateDAC_413_FI_5(void)
{
	VDES::AISBitsManager bitsManager;

	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4135001, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(413, 10);
	// FI	
	bitsManager.Encode(5, 6);

	// Prompt Content (提示内容 = 3: 恶劣天气预警, 注意安全航行)
	bitsManager.Encode(3, 6);
	// Display Duration (显示时长 = 5 minutes)
	bitsManager.Encode(5, 4);

	// Text: "DANGER AREA WARNING"
	std::string text = "DANGER AREA WARNING";
	bitsManager.Encode(text, 413, 1);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	if (spareBits != 8)
	{
		bitsManager.Encode(0, spareBits);
	}
	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		auto sentence = vdm + "\r\n";
		results.push_back(sentence);
		std::cout << "GEN VDM (DAC=413 FI=5 Frontend Prompt): " << sentence;
	}
	return results;
}

static void GenerateASM_DAC_413_FI_5(void)
{
	VDES::AISBitsManager bitsManager;

	// DAC
	bitsManager.Encode(413, 10);
	// FI	
	bitsManager.Encode(5, 6);

	// Prompt Content (提示内容 = 3: 恶劣天气预警, 注意安全航行)
	bitsManager.Encode(2, 6);
	// Display Duration (显示时长 = 5 minutes)
	bitsManager.Encode(5, 4);

	// Text: "DANGER AREA WARNING"
	std::string text = "前方浅水作业区域";
	auto content = VDES::UtilityInterface::UTF8ToGBK(text);
	bitsManager.Encode(content, 413, 1);

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto byteFillBitsNum = 8 - (bitsNum % 8);
	bitsManager.Encode(0, 6);

	auto payload = bitsManager.GetEncodedVDMPayload();
	auto fillNum = bitsManager.GetFillBitsNumberToEncode();

	int a;
	a = 10;
}


static std::vector<std::string> GenerateDAC_413_FI_8(uint32_t mrn, uint16_t mainDAC, uint8_t mainFI, const std::string &desc)
{
	VDES::AISBitsManager bitsManager;

	// Message ID (6 bits) = 8
	bitsManager.Encode(8, 6);
	// Repeat indicator (2 bits) = 0
	bitsManager.Encode(0, 2);
	// Source ID (30 bits) = 4123001
	bitsManager.Encode(4123001, 30);
	// Spare (2 bits) = 0
	bitsManager.Encode(0, 2);
	// DAC (10 bits) = 413
	bitsManager.Encode(413, 10);
	// FI (6 bits) = 8
	bitsManager.Encode(8, 6);

	// MRN (17 bits)
	bitsManager.Encode(mrn & 0x1FFFF, 17);
	// mainDAC (10 bits)
	bitsManager.Encode(mainDAC & 0x3FF, 10);
	// mainFI (6 bits)
	bitsManager.Encode(mainFI & 0x3F, 6);
	// encodingType (2 bits) = 0 (6-bit ASCII)
	bitsManager.Encode(0, 2);
	// textLength (11 bits)
	bitsManager.Encode(static_cast<uint32_t>(desc.length()), 11);

	// Encode string in 6-bit ASCII
	static std::map<char, uint32_t> map6Bit = {
		{'@', 0},  {'A', 1},  {'B', 2},  {'C', 3},  {'D', 4},
		{'E', 5},  {'F', 6},  {'G', 7},  {'H', 8},  {'I', 9},
		{'J', 10}, {'K', 11}, {'L', 12}, {'M', 13}, {'N', 14},
		{'O', 15}, {'P', 16}, {'Q', 17}, {'R', 18}, {'S', 19},
		{'T', 20}, {'U', 21}, {'V', 22}, {'W', 23}, {'X', 24},
		{'Y', 25}, {'Z', 26}, {'[', 27}, {'\\', 28},{']', 29},
		{'^', 30}, {'_', 31}, {' ', 32}, {'!', 33}, {'\"', 34},
		{'#', 35}, {'$', 36}, {'%', 37}, {'&', 38}, {'\'', 39},
		{'(', 40}, {')', 41}, {'*', 42}, {'+', 43}, {',', 44},
		{'-', 45}, {'.', 46}, {'/', 47}, {'0', 48}, {'1', 49},
		{'2', 50}, {'3', 51}, {'4', 52}, {'5', 53}, {'6', 54},
		{'7', 55}, {'8', 56}, {'9', 57}, {':', 58}, {';', 59},
		{'<', 60}, {'=', 61}, {'>', 62}, {'?', 63}
	};

	for (char c : desc)
	{
		char upperC = toupper(c);
		auto iter = map6Bit.find(upperC);
		uint32_t code = (iter != map6Bit.end()) ? iter->second : 32;
		bitsManager.Encode(code, 6);
	}

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	if (spareBits < 8)
	{
		bitsManager.Encode(0, spareBits);
	}

	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		results.push_back(vdm + "\r\n");
	}
	return results;
}


int main(void)
{
	SetConsoleOutputCP(CP_UTF8);
	spdlog::default_logger()->set_level(spdlog::level::trace);
	//GenerateASM_DAC_413_FI_5();
#if 0
	GenerateDAC_412_FI_26();
	GenerateDAC_412_FI_27();
	GenerateDAC_412_FI_28();
	GenerateDAC_412_FI_29();
	GenerateDAC_412_FI_30();
	GenerateDAC_412_FI_32();
	GenerateDAC_412_FI_35(0);
	GenerateDAC_412_FI_36(0);
	GenerateDAC_412_FI_38(0);
	GenerateDAC_412_FI_39();
	GenerateDAC_412_FI_40(0);
	GenerateDAC_412_FI_40(1);
	GenerateDAC_412_FI_40(2);
	GenerateDAC_412_FI_40(3);
	GenerateDAC_412_FI_38(1);
	GenerateDAC_412_FI_38(2);
	GenerateDAC_412_FI_41();
	GenerateDAC_412_FI_44();
	GenerateDAC_412_FI_31(1);
	GenerateDAC_412_FI_31(2);
	GenerateDAC_412_FI_31(5);
	GenerateDAC_412_FI_31(6);
	GenerateDAC_412_FI_37();
	GenerateDAC_412_FI_42();
	GenerateDAC_412_FI_43(0);
	GenerateDAC_412_FI_43(1);
#endif
	auto &vdesManager = VDES::VDESManager::GetInstance();

	VDES::ConfigureManager::GetInstance().SetStoragePath(".");
	vdesManager.Initialize();
	vdesManager.EmptyDatabase();
#if 0
	// Parse new standard early warning messages (FI=31)
	std::cout << "\n=== Testing DAC=412, FI=31 (Warnings) ===" << std::endl;
	for (uint8_t type : {1, 2, 5, 6})
	{
		auto sentences = GenerateDAC_412_FI_31(type);
		for (const auto &vdm : sentences)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Query from Database and print
	auto cyclones = vdesManager.GetMewTropicalCyclones(0, 100);
	std::cout << "Successfully retrieved " << cyclones.size() << " tropical cyclone warnings from DB:" << std::endl;
	for (const auto &ew : cyclones)
	{
		std::cout << "  Warning ID: " << ew.dataID
				  << ", MRN: " << ew.MRN
				  << ", Published: " << ew.timestampPublished
				  << ", Start: " << ew.timestampStart
				  << ", End: " << ew.timestampEnd
				  << ", Duration: " << (int)ew.warningDuration
				  << ", Info Source: " << (int)ew.infoSource << std::endl;
		std::cout << "    Cyclone path points count: " << ew.pathPoints.size() << std::endl;
		for (size_t i = 0; i < ew.pathPoints.size(); ++i)
		{
			const auto &pt = ew.pathPoints[i];
			std::cout << "      Pt " << i << ": TS: " << pt.timestamp
					  << ", Lat: " << pt.centerLatitude
					  << ", Lon: " << pt.centerLongitude
					  << ", Type: " << (int)pt.cycloneType
					  << ", Wind Scale Max: " << (int)pt.maxWindScale
					  << ", Pressure: " << pt.centerPressure << std::endl;
		}
	}

	auto gales = vdesManager.GetMewGales(0, 100);
	std::cout << "Successfully retrieved " << gales.size() << " gale warnings from DB:" << std::endl;
	for (const auto &ew : gales)
	{
		std::cout << "  Warning ID: " << ew.dataID
				  << ", MRN: " << ew.MRN
				  << ", Area Code: " << (int)ew.areaCode
				  << ", Warning Level: " << (int)ew.warningLevel
				  << ", Published: " << ew.timestampPublished << std::endl;
	}

	auto waves = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "Successfully retrieved " << waves.size() << " large wave warnings from DB:" << std::endl;

	auto fogs = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "Successfully retrieved " << fogs.size() << " sea fog warnings from DB:" << std::endl;

	auto surges = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "Successfully retrieved " << surges.size() << " storm surge warnings from DB:" << std::endl;
	for (const auto &ew : surges)
	{
		std::cout << "  Warning ID: " << ew.dataID
				  << ", MRN: " << ew.MRN
				  << ", City Code: " << (int)ew.cityCode
				  << ", Surge Height: " << ew.surgeHeight
				  << ", Warning Level: " << (int)ew.warningLevel
				  << ", Published: " << ew.timestampPublished << std::endl;
	}

	auto ices = vdesManager.GetMewSeaIces(0, 100);
	std::cout << "Successfully retrieved " << ices.size() << " sea ice warnings from DB:" << std::endl;
	for (const auto &ew : ices)
	{
		std::cout << "  Warning ID: " << ew.dataID
				  << ", MRN: " << ew.MRN
				  << ", Region Code: " << (int)ew.regionCode
				  << ", Warning Level: " << (int)ew.warningLevel
				  << ", Published: " << ew.timestampPublished << std::endl;
	}
	std::cout << "=========================================\n" << std::endl;

	// Parse tide forecast messages (FI=32)
	std::cout << "\n=== Testing DAC=412, FI=32 (Tides) ===" << std::endl;
	auto tideSentences = GenerateDAC_412_FI_32();
	for (const auto &vdm : tideSentences)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Query from Database and print
	auto tides = vdesManager.GetTideForecasts(0, 100);
	std::cout << "Successfully retrieved " << tides.size() << " tide forecasts from DB:" << std::endl;
	for (const auto &tide : tides)
	{
		std::cout << "  Tide ID: " << tide.dataID
				  << ", Hour Publish: " << (int)tide.hourPublish
				  << ", Info Source: " << (int)tide.infoSource
				  << ", Stations count: " << tide.stations.size() << std::endl;
		for (size_t i = 0; i < tide.stations.size(); ++i)
		{
			const auto &station = tide.stations[i];
			std::cout << "    Station " << i << ": Lat: " << station.coordinate.GetLatitude()
					  << ", Lon: " << station.coordinate.GetLongitude()
					  << ", Tidal Datum: " << station.tidalDatum << " cm"
					  << ", Tide High: " << station.tideHigh << " cm at " << station.timestampTideHigh
					  << ", Tide Low: " << station.tideLow << " cm at " << station.timestampTideLow << std::endl;
		}
	}
	std::cout << "========================================\n" << std::endl;

	// Parse obstacle messages (FI=35)
	std::cout << "\n=== Testing DAC=412, FI=35 (Obstacles) ===" << std::endl;
	for (uint8_t geomType : {0, 1})
	{
		auto obsSentences = GenerateDAC_412_FI_35(geomType);
		for (const auto &vdm : obsSentences)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Query from Database and print
	auto obstacles = vdesManager.GetObstacles(0, 100);
	std::cout << "Successfully retrieved " << obstacles.size() << " obstacles from DB:" << std::endl;
	for (const auto &obs : obstacles)
	{
		std::cout << "  Obstacle ID: " << obs.dataID
				  << ", Type: " << (int)obs.type
				  << ", GeometryType: " << (int)obs.geometryType
				  << ", Lat: " << obs.coordinate.GetLatitude()
				  << ", Lon: " << obs.coordinate.GetLongitude()
				  << ", Range/Radius: " << (int)obs.range << std::endl;
		if (obs.geometryType == 1)
		{
			std::cout << "    Sector Angles: Start=" << obs.sectorStartAngle
					  << ", End=" << obs.sectorEndAngle << std::endl;
		}
	}
	std::cout << "========================================\n" << std::endl;
#endif

#if 0 
	std::cout << "\n==============================================" << std::endl;
	std::cout << "RUNNING AMK & SHIP-TO-SHORE REQUEST INTEGRATION TEST..." << std::endl;
	std::cout << "==============================================\n" << std::endl;

	// Capture sendEvent output
	std::vector<std::string> sentAABSentences;
	vdesManager.sendEvent.append([&](const VDES::CommunicationType type, const char *data, size_t size) {
		std::string sent(data, size);
		std::cout << "[SENT NMEA] " << sent;
		if (sent.find("AAB") != std::string::npos) {
			sentAABSentences.push_back(sent);
		}
	});

	// Register event listener
	int lastRetCode = -999;
	vdesManager.notifyEvent.append([&](const VDES::VDESManager::EventType type, const int retCode) {
		if (type == VDES::VDESManager::EventType::MESSAGE_SEND) {
			lastRetCode = retCode;
			std::cout << "[EVENT] Received MESSAGE_SEND with retCode: " << retCode << std::endl;
		}
	});

	// 1. Send Route Recommendation Request
	VDES::RouteRecommendationRequest routeReq;
	routeReq.grossTonnage = 12000;
	routeReq.maxStaticDraft = 12.5;
	routeReq.cargoType = 70;
	routeReq.month = 7;
	routeReq.day = 4;
	routeReq.hour = 10;
	routeReq.minute = 30;
	
	VDES::Coordinate start(38.5, 118.2);
	VDES::Coordinate dest(39.1, 119.5);
	routeReq.startCoordinate = start;
	routeReq.destCoordinate = dest;

	std::cout << "\n1. Sending Route Recommendation Request..." << std::endl;
	vdesManager.SendRouteRecommendationRequest(routeReq);

	// 2. Send Hydrometeorology Request
	VDES::HydrometeorologyRequest hydroReq;
	hydroReq.MRN = 12345;
	hydroReq.windSpeed = true;
	hydroReq.windDirection = true;
	hydroReq.visibility = true;
	hydroReq.requestTime = 1783130000; // Mock UTC timestamp
	hydroReq.coordinates.push_back(start);
	hydroReq.coordinates.push_back(dest);

	std::cout << "\n2. Sending Hydrometeorology Request..." << std::endl;
	vdesManager.SendHydrometeorologyRequest(hydroReq);

	// Verify database solidification
	std::cout << "\n3. Checking history requests from DB..." << std::endl;
	auto routeHistory = vdesManager.GetRouteRecommendationRequests(0, 10);
	auto hydroHistory = vdesManager.GetHydrometeorologyRequests(0, 10);

	std::cout << "Route Request History Count: " << routeHistory.size() << std::endl;
	if (!routeHistory.empty()) {
		auto r = routeHistory.front();
		std::cout << "  dataID: " << r.dataID << std::endl;
		std::cout << "  tonnage: " << r.grossTonnage << " (Expected: 12000)" << std::endl;
		std::cout << "  sequenceNum: " << r.sequenceNum << std::endl;
		std::cout << "  sendStatus: " << r.sendStatus << " (Expected: -1)" << std::endl;
	}

	std::cout << "Hydrometeorology Request History Count: " << hydroHistory.size() << std::endl;
	if (!hydroHistory.empty()) {
		auto h = hydroHistory.front();
		std::cout << "  dataID: " << h.dataID << std::endl;
		std::cout << "  MRN: " << h.MRN << " (Expected: 12345)" << std::endl;
		std::cout << "  sequenceNum: " << h.sequenceNum << std::endl;
		std::cout << "  sendStatus: " << h.sendStatus << " (Expected: -1)" << std::endl;
		std::cout << "  coordinates count: " << h.coordinates.size() << " (Expected: 2)" << std::endl;
	}

	// 3. Simulate AMK sentence receipt for Route Recommendation request (sequence number from routeHistory.front().sequenceNum)
	if (!routeHistory.empty()) {
		uint32_t routeSeq = routeHistory.front().sequenceNum;
		std::cout << fmt::format("\n4. Simulating AMK sentence for Route Recommendation (Seq={})...", routeSeq) << std::endl;
		
		// $AIAMK,004129999,1,,<seq>,0
		std::string amkSentence = fmt::format("$AIAMK,004129999,1,,{},0", routeSeq);
		VDES::UtilityInterface::AddChecksum(amkSentence);
		amkSentence += "\r\n";
		
		vdesManager.Parse(amkSentence.c_str(), amkSentence.length());
		
		// Query again to check if status was updated
		auto updatedRouteHistory = vdesManager.GetRouteRecommendationRequests(0, 10);
		if (!updatedRouteHistory.empty()) {
			std::cout << "  Updated Route Request sendStatus: " << updatedRouteHistory.front().sendStatus << " (Expected: 0)" << std::endl;
			std::cout << "  lastRetCode from event: " << lastRetCode << " (Expected: 1)" << std::endl;
		}
	}

	// 4. Simulate AMK sentence receipt for Hydrometeorology request with failure ack type 2 (Seq = hydroHistory.front().sequenceNum)
	if (!hydroHistory.empty()) {
		uint32_t hydroSeq = hydroHistory.front().sequenceNum;
		std::cout << fmt::format("\n5. Simulating failed AMK sentence for Hydrometeorology (Seq={})...", hydroSeq) << std::endl;
		
		// $AIAMK,004129999,1,,<seq>,2
		std::string amkSentence = fmt::format("$AIAMK,004129999,1,,{},2", hydroSeq);
		VDES::UtilityInterface::AddChecksum(amkSentence);
		amkSentence += "\r\n";
		
		vdesManager.Parse(amkSentence.c_str(), amkSentence.length());
		
		// Query again to check if status was updated
		auto updatedHydroHistory = vdesManager.GetHydrometeorologyRequests(0, 10);
		if (!updatedHydroHistory.empty()) {
			std::cout << "  Updated Hydro Request sendStatus: " << updatedHydroHistory.front().sendStatus << " (Expected: 2)" << std::endl;
			std::cout << "  lastRetCode from event: " << lastRetCode << " (Expected: 2)" << std::endl;
		}
	}

	std::cout << "\n==============================================" << std::endl;
	std::cout << "AMK & SHIP REQUEST INTEGRATION TEST COMPLETE!" << std::endl;
	std::cout << "==============================================\n" << std::endl;
	exit(0);
#endif
#if 0
	std::cout << "\n==============================================" << std::endl;
	std::cout << "RUNNING EXTENDED STATIC INFO INTEGRATION TEST..." << std::endl;
	std::cout << "==============================================\n" << std::endl;

	// Mock own vessel config
	{
		std::string epv = "$AIEPV,1,1,1,106,123456789";
		VDES::UtilityInterface::AddChecksum(epv);
		epv += "\r\n";
		vdesManager.Parse(epv.c_str(), epv.length());

		std::string vsd = "$AIVSD,70,12.5,120,SHANGHAI,1200,04,07,1,0";
		VDES::UtilityInterface::AddChecksum(vsd);
		vsd += "\r\n";
		vdesManager.Parse(vsd.c_str(), vsd.length());
	}

	// Capture sendEvent output
	std::vector<std::string> sentSentences;
	vdesManager.sendEvent.append([&](const VDES::CommunicationType type, const char *data, size_t size) {
		std::string sent(data, size);
		std::cout << "[SENT NMEA] " << sent << std::endl;
		sentSentences.push_back(sent);
	});

	// Register event listener
	bool gotPartA = false;
	bool gotPartB = false;
	vdesManager.notifyEvent.append([&](const VDES::VDESManager::EventType type, const int retCode) {
		if (type == VDES::VDESManager::EventType::ASM_EXTENDED_VESSEL_A) {
			gotPartA = true;
			std::cout << "[EVENT] Received Extended Vessel Info Part A!" << std::endl;
		}
		if (type == VDES::VDESManager::EventType::ASM_EXTENDED_VESSEL_B) {
			gotPartB = true;
			std::cout << "[EVENT] Received Extended Vessel Info Part B!" << std::endl;
		}
	});

	// Test SetExtendedVesselInfo
	VDES::ExtendedVesselInfo info;
	info.extendedVesselType = 12;
	info.autonomousLevel = 3;
	info.portCode = "CNYTN";
	info.chineseName = "向阳红09";

	std::cout << "\n--- Debug ConfigureManager own vessel ---" << std::endl;
	auto confVessel = VDES::ConfigureManager::GetInstance().GetOwnVesselInfo();
	std::cout << "MMSI in ConfigureManager: " << confVessel.mmsi << std::endl;
	std::cout << "Draught in ConfigureManager: " << confVessel.draught << std::endl;
	std::cout << "CrewNum in ConfigureManager: " << confVessel.crewNum << std::endl;
	std::cout << "ETA Month in ConfigureManager: " << (int)confVessel.eta.month << std::endl;
	std::cout << "-------------------------------------------\n" << std::endl;

	std::cout << "Testing SetExtendedVesselInfo..." << std::endl;
	vdesManager.SetExtendedVesselInfo(info);

	// Retrieve set info
	auto checkInfo = vdesManager.GetExtendedVesselInfo();
	std::cout << "\n--- Verify GetExtendedVesselInfo (Own Vessel) ---" << std::endl;
	std::cout << "Own Vessel Type: " << (int)checkInfo.extendedVesselType << " (Expected: 12)" << std::endl;
	std::cout << "Own Autonomous Level: " << (int)checkInfo.autonomousLevel << " (Expected: 3)" << std::endl;
	std::cout << "Own Port Code: " << checkInfo.portCode << " (Expected: CNYTN)" << std::endl;
	std::cout << "Own Chinese Name: " << checkInfo.chineseName << " (Expected: 向阳红09)" << std::endl;

	// Loop back sent NMEA sentences to Parse() to simulate receiving them from other vessels
	std::cout << "\nLooping back sent NMEA sentences to simulate reception..." << std::endl;
	for (const auto &s : sentSentences) {
		auto parts = VDES::UtilityInterface::SplitString(s, std::vector<std::string>{",", "*"});
		if (parts.size() >= 8 && parts[0] == "!AIBBM") {
			std::string total = parts[1];
			std::string num = parts[2];
			std::string payload = parts[6];
			std::string fill = parts[7];
			std::string asmSentence = fmt::format("$AIASM,,{},{},,1,8,0,987654321,,{},{}", total, num, payload, fill);
			VDES::UtilityInterface::AddChecksum(asmSentence);
			asmSentence += "\r\n";
			std::cout << "[LOOPBACK RECEIVED ASM] " << asmSentence;
			vdesManager.Parse(asmSentence.c_str(), asmSentence.length());
		}
	}

	// Verify received database table
	std::cout << "\n--- Verify GetOtherVesselExtendedInfos ---" << std::endl;
	auto receivedList = vdesManager.GetOtherVesselExtendedInfos(0, 10);
	std::cout << "Received Other Vessel Info Count: " << receivedList.size() << std::endl;
	for (const auto &oth : receivedList) {
		time_t etaTime = static_cast<time_t>(oth.eta);
		struct tm *timeUTC = gmtime(&etaTime);
		int etaMonth = timeUTC ? timeUTC->tm_mon + 1 : 0;

		std::cout << "  MMSI: " << oth.mmsi << " (Expected: 987654321)" << std::endl;
		std::cout << "  Type: " << (int)oth.extendedVesselType << " (Expected: 12)" << std::endl;
		std::cout << "  Autonomous Level: " << (int)oth.autonomousLevel << " (Expected: 3)" << std::endl;
		std::cout << "  Port Code: " << oth.portCode << " (Expected: CNYTN)" << std::endl;
		std::cout << "  Chinese Name: " << oth.chineseName << " (Expected: 向阳红09)" << std::endl;
		std::cout << "  Draught: " << oth.draught << " (Expected: 12.5)" << std::endl;
		std::cout << "  ETA Month: " << etaMonth << " (Expected: 7)" << std::endl;
		std::cout << "  Crew Num: " << oth.crewNum << " (Expected: 120)" << std::endl;
	}

	std::cout << "\n==============================================" << std::endl;
	std::cout << "INTEGRATION TEST COMPLETE!" << std::endl;
	std::cout << "==============================================\n" << std::endl;
	exit(0);
#endif

	std::vector<std::string> senteces{
		// 海洋气象预报坐标 DAC = 412, FI = 26
		//"$AIASM,1783156663,1,1,,1,2,0,666666666,,Iia1P9hA9CQR77<kRP@,2*1C\r\n",
		//"$AIASM,1783156881,1,1,,2,2,0,666666666,,Iia1RihW9DgEbe?T81gv:tnu;>AOK0,4*3D\r\n",
		//"$AIASM,1783152841,1,1,,2,2,0,666666666,,Iia1P9hA9CQR77<kRP@,2*15\r\n",
		//"$AIASM,1782890582,1,1,,1,2,0,666666666,,Iia1P9jn7A1R77<kRP@,2*53\r\n",
		//"$AIASM,1782890623,1,1,,1,2,0,666666666,,Iibwv`0000?5WBiT7l@,2*5C\r\n",
		//"$AIASM,1782890651,1,1,,1,2,0,666666666,,Iia1Sqjn0005W7=T2Qt0t0FLLn@:30,4*2D\r\n",
		// 海洋气象预报海区 DAC = 412, FI = 27
		//"$AIASM,1783221328,1,1,,2,2,0,666666666,,Iid1R7FkQ><5L0DVQ62lVEE82lVEI0,4*1A\r\n",
		//"$AIASM,1783219666,1,1,,2,2,0,666666666,,Iid1Qeu3P?AP,0*35\r\n",
		//"$AIASM,1782890679,1,1,,1,2,0,666666666,,Iid1Qeu3P?AP,0*32\r\n",
		//"$AIASM,1782890701,1,1,,2,2,0,666666666,,Iid1R7FkP?B9FkP?B;FkP?B=FkP?B?FkP?BAFkP?BCFkP?BEFkP?A0,4*26\r\n",
		//"$AIASM,1782890721,1,1,,2,2,0,666666666,,Iie1P213U>PP,0*3B\r\n",
		//"$AIASM,1782890739,1,1,,2,2,0,666666666,,Iie1P253U?DP,0*23\r\n",
		// 海洋环境预报坐标 DAC = 412, FI = 28
		//"$AIASM,1782890906,1,1,,1,2,0,666666666,,Iii00003`Wv`L0drb08Wv`L0drb08Wv`L0drb80,2*15\r\n",
		//"$AIASM,1782890930,1,1,,1,2,0,666666666,,IijqPqK00008L0dp0<0,2*76\r\n",
		//"$AIASM,1782890950,1,1,,2,2,0,666666666,,Iii1PqK3`Ww8L?@rb40,2*30\r\n",
		//"$AIASM,1782890973,1,1,,1,2,0,666666666,,Iii1PqK3`PNFP?@rb40,2*6E\r\n",
		// 海洋环境预报海区 DAC = 412, FI = 29
		//"$AIASM,1782891000,1,1,,1,2,0,666666666,,IinqPp17P0UP3rvgaP,4*5B\r\n",
		//"$AIASM,1782891025,1,1,,1,2,0,666666666,,IinqPp17P0UP3rvg`817P0UP3rvgaP,4*37\r\n",
		//"$AIASM,1782891073,1,1,,2,2,0,666666666,,Iim1P=DjD>UP`?3h`P,4*35\r\n",
		//"$AIASM,1782891102,1,1,,2,2,0,666666666,,Iip02l080007`00037P0@0,4*33\r\n",
		// 海洋环境预报近岸 DAC = 412, FI = 30
		//"$AIASM,1783255188,2,1,8,2,2,0,666666666,,Iirk410;6?9iIpBh37Td1C9TOOnI>?m0iquP0oAWquVESu8<NODTAPDKb1VH,0*56\r\n",
		//"$AIASM,,2,2,8,2,,0,666666666,,wF37WoH,2*00\r\n",
		//"$AIASM,1782968761,1,1,,2,2,0,666666666,,Iip02l080007`00037P0@0,4*37\r\n",
		//"$AIASM,1782968901,1,1,,1,2,0,666666666,,Iip0010>@5OEhgO@37Wl80,4*6E\r\n",
		//"$AIASM,1782891146,1,1,,1,2,0,666666666,,Iiq1P1wpt52Uhg9H37Pj80,4*2D\r\n",
		//"$AIASM,1782891172,1,1,,1,2,0,666666666,,Iiq1P1bVB52Uhg9H37Pj80,4*2F\r\n",
		// 海洋气象环境预警 DAC = 412, FI = 31
		//"$AIASM,1783402405,1,1,,1,2,0,666666666,,IitD=VQVn5?Lh6@j567=kKeB9HqP0>I@3WD02@,4*5B\r\n",
		//"$AIASM,1783256655,1,1,,1,2,0,666666666,,IiuT=i01k:0Lp07@`050,0*5F\r\n",
		//"$AIASM,1783256514,1,1,,1,2,0,666666666,,IitD=VQVn5?Lh6@j567=kKeB9HqP0>I@3WD02@,4*5D\r\n",
		//"$AIASM,1783256597,1,1,,1,2,0,666666666,,IitT=`1E3J8H@nTU4=ag53JP1LjP7>01l>0180,4*45\r\n",
		// 潮汐预报 DAC = 412, FI = 32
		//"$AIASM,1783320688,2,1,1,1,2,0,666666666,,Ij0Qk87wP0Da805DB0?c7r5:Eu1E5O@GcviBawDEBOnUrgbDbWlUFauFgcsU,0*50\r\n",
		//"$AIASM,,2,2,1,1,,0,666666666,,:`B9E:8L00j1BbOFEBWo80,4*26\r\n",
		//"$AIASM,1783303109,2,1,0,1,2,0,666666666,,Ij0Qk87wP0Da805DB0?c7r5:Eu1E5O@GcviBawDEBOnUrgbDbWlUFauFgcsU,0*5E\r\n",
		//"$AIASM,,2,2,0,1,,0,666666666,,:`B9E:8L00j1BbOFEBWo80,4*27\r\n",
		//"$AIASM,1783252387,2,1,6,2,2,0,666666666,,Ij0Qk87wP0Da805DB0?c7r5:Eu1E5O@GcviBawDEBOnUrgbDbWlUFauFgcsU,0*5A\r\n",
		//"$AIASM,,2,2,6,2,,0,666666666,,:`B9E:8L00j1BbOFEBWo80,4*22\r\n",

		//"$AIASM,1782977822,1,1,,2,2,0,666666666,,Ij01jn0001r07lG2t060,0*49\r\n",
		//"$AIASM,1782977845,2,1,3,1,2,0,666666666,,Ij2qjnpv?auwJFOP1u03krOOnUWp0O@0tvWouaIv07l0??auwJFOP1u03krO,0*39\r\n",
		//"$AIASM,,2,2,3,1,,0,666666666,,OnUWp0OA00,4*26\r\n",
		//"$AIASM,1782977859,1,1,,1,2,0,666666666,,Ij31jnpg7mL;jFG2t<R0,0*23\r\n",
		//"$AIASM,1782977882,1,1,,1,2,0,666666666,,Ij11jnpg?eL;jFG2t<R0,0*27\r\n",
		// 航标动态（非AIS) DAC = 412, FI = 33
		//"$AIASM,1782978051,1,1,,1,2,0,666666666,,IlBNC2`TQrE9r?saRjIwc9dwHCum<Vt?Aw9N?ed,2*2C\r\n",
		//"$AIASM,1782978012,1,1,,2,2,0,666666666,,Ij4Gmr40IpDHHEP8o`80@14GP0039665H0000008008D,0*39\r\n",
		//"$AIASM,1782978015,1,1,,1,2,0,666666666,,IlBNC2`TQrE9r?saRjIwc9w43O?=bMtVssgmTSV5Ad>WtI>LHVvsnn0,2*04\r\n",
		//"$AIASM,1782978051,1,1,,1,2,0,666666666,,IlBNC2`TQrE9r?saRjIwc9dwHCum<Vt?Aw9N?ed,2*2C\r\n",
		//"$AIASM,1782978054,2,1,8,2,2,0,666666666,,Ij42JB816qTH>@<8Ma2P2>38hLG0@s3`04LrAPq0hPe3N08r<S00gQ3NfL0A,0*4D\r\n",
		//"$AIASM,,2,2,8,2,,0,666666666,,nI601O26t8P0Shj<02v4=q`Ph0,4*3D\r\n",
		// AIS航标动态 DAC = 412, FI = 34
		//"$AIASM,1783258502,1,1,,1,2,0,666666666,,Ij:3nPU;n0040A<suP`hQD08,0*65\r\n",
		// 碍航物 DAC = 412, FI = 35
		//"$AIASM,1783253640,1,1,,2,2,0,666666666,,Ij<=@b6:kDdLDu06hww`0HOw6Os4ww?wTh06f8R:80,4*44\r\n",
		// 海上拖带 DAC = 412, FI = 37
		//"$AIASM,1783579913,1,1,,1,2,0,666666666,,IjD8s0i>t:DF5L`r>?;:M0Id``003:O@07tt0P,4*1A\r\n",
		//"$AIASM,1783403099,1,1,,2,2,0,666666666,,IjD8s0i>t:DF5L`r>?;4V0L`8`003:O@07tt0P,4*52\r\n",
		//"$AIASM,1783580797,1,1,,2,2,0,666666666,,IjD0sh0OMJ0FFbPog<;>@8L@:l06@jO9f7lKPP,4*49\r\n",
		// 船舶遇险 DAC = 412, FI = 39
		//"$AIASM,1783658954,1,1,,2,2,0,666666666,,IjLuoH0000a5d1akp06?030,2*0D\r\n",
		//"$AIASM,1783658811,1,1,,1,2,0,666666666,,IjL6;r@0T4`V30q93LAbi10,2*0B\r\n",
		//"$AIASM,1783259385,1,1,,1,2,0,666666666,,IjLuo`P004a5d1akpLAbh30,2*4F\r\n",
		//"$AIASM,1783259374,1,1,,1,2,0,666666666,,IjL6;r@0T4`V30q93LAbi10,2*06\r\n",
		//"$AIASM,1783254079,1,1,,2,2,0,666666666,,Ij<=@HL@pcB0d?cFOo4A5<0,2*1A\r\n",
		//"$AIASM,1783253640,1,1,,2,2,0,666666666,,Ij<=@b6:kDdLDu06hww`0HOw6Os4ww?wTh06f8R:80,4*44\r\n",
		// 桥梁 DAC = 412, FI = 41
		//"$AIASM,1783259551,1,1,,1,2,0,666666666,,IjT0?H>7JTP021>@p3@gH0I05@jP,0*5C\r\n",
		// 航道边线左侧 DAC = 412, FI = 43
		//"$AIASM,1783259645,1,1,,2,2,0,666666666,,Ijd1sPRc?@1>g400rV0v`P01LiQpbN06@I3k;d0,2*4F\r\n",
		// 航线推荐 DAC = 412, FI = 46
		//"$AIASM,1783646310,1,1,,1,4,0,666666666,210210210,Ijwi5M<<2LTwivS>P1`OP8da0?@T00,4*26\r\n",
		//"$AIASM,1783646512,1,1,,2,4,0,666666666,210210210,IjtQ6JmP2KelP00003Fn000006ed7uSh0G<H00,4*72\r\n",
		//"$AIASM,1783646310,1,1,,1,4,0,666666666,210210210,Ijwi5M<<2LTwivS>P1`OP8da0?@T00,4*26\r\n",
		// 中文短信 DAC = 413, FI = 04
		//"$AIASM,1782977984,1,1,,1,2,0,666666666,,IlBNC2`TQrE9r?saRjIwc9w43O?=bMtVssgmTSV5Ad>WtI>LHVvsnn0,2*0A\r\n",
		// 前端提示文字 DAC = 413, FI = 5
		//"$AIASM,1783260323,1,1,,1,2,0,666666666,,IlD9@P4L72TT04T50`2p4T>4SPp,2*1C\r\n",
		//"$AIASM,1783063312,1,1,,1,2,0,666666666,,IlD9Hs2rMSdqJrgoa=HwRWj0,0*62\r\n",

		//"$AIASM,1782890906,1,1,,1,2,0,666666666,,Iii00003`Wv`L0drb08Wv`L0drb08Wv`L0drb80,2*15\r\n",
		//"$AIASM,1782890930,1,1,,1,2,0,666666666,,IijqPqK00008L0dp0<0,2*76\r\n",
		//"$AIASM,1782890950,1,1,,2,2,0,666666666,,Iii1PqK3`Ww8L?@rb40,2*30\r\n",
		//"$AIASM,1782890973,1,1,,1,2,0,666666666,,Iii1PqK3`PNFP?@rb40,2*6E\r\n",
		//"$AIASM,1782891000,1,1,,1,2,0,666666666,,IinqPp17P0UP3rvgaP,4*5B\r\n",
		//"$AIASM,1782891025,1,1,,1,2,0,666666666,,IinqPp17P0UP3rvg`817P0UP3rvgaP,4*37\r\n",
		//"$AIASM,1782891073,1,1,,2,2,0,666666666,,Iim1P=DjD>UP`?3h`P,4*35\r\n",
		//"$AIASM,1782891102,1,1,,2,2,0,666666666,,Iip02l080007`00037P0@0,4*33\r\n",
		//"$AIASM,1782891119,1,1,,1,2,0,666666666,,Iip0010>@5OEhgO@37Wl80,4*68\r\n",
		//"$AIASM,1782891146,1,1,,1,2,0,666666666,,Iiq1P1wpt52Uhg9H37Pj80,4*2D\r\n",
		//"$AIASM,1782891172,1,1,,1,2,0,666666666,,Iiq1P1bVB52Uhg9H37Pj80,4*2F\r\n"
		//"$AIASM,1782881505,1,1,,1,2,0,666666666,,Iia1P9jn7A1R77<kRP@,2*5C\r\n",
		//"$AIASM,1782794617,2,1,3,2,2,0,666666666,,Iia1P9jnE605WBiT2P4qK:KP2kaHj1@2LeU:01IldI0`1>FjS80drF<PD0W;,0*6F\r\n",
		//"$AIASM,,2,2,3,2,,0,666666666,,I@`0FM;6@:0CUdWn0;>US8509jnCd05WBiT2P4qK9fP2kaHj1@H,2*69\r\n",
	};

	for (const auto &sen : senteces)
	{
		vdesManager.Parse(sen.c_str(), sen.length());
	}
#if 0
	auto vdm45 = GenerateDAC_412_FI_45();
	for (const auto &vdm : vdm45)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	auto vdm32 = GenerateDAC_412_FI_32();
	for (const auto &vdm : vdm32)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	auto vdm37 = GenerateDAC_412_FI_37();
	for (const auto &vdm : vdm37)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	auto vdm30 = GenerateDAC_412_FI_30();
	for (const auto &vdm : vdm30)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	auto vdm41_active = GenerateDAC_412_FI_41();
	for (const auto &vdm : vdm41_active)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Parse mock Channel Boundary (FI=43/44) messages
	auto vdm43_left = GenerateDAC_412_FI_43(0);
	for (const auto &vdm : vdm43_left)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	auto vdm43_right = GenerateDAC_412_FI_43(1);
	for (const auto &vdm : vdm43_right)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}
#endif
	int a;
	a = 10;
#if 0
	

	std::vector<std::string> senteces{
		"!AIVDM,1,1,,A,803saNAW6c0@VrtFW53hI0D6A0,4*23\r\n", // DAC_412_FI_26 (Meteorology Coordinate)
		"!AIVDM,1,1,,A,803saNAW6rP@I4dQRT0,2*7F\r\n", // DAC_412_FI_27 (Meteorology Sea Area)
		"!AIVDM,1,1,,A,803saNAW7;0CND;Q1TqQA8Q@P0,4*01\r\n", // DAC_412_FI_28 (Environment Coordinate)
		"!AIVDM,1,1,,A,803saNAW7K0A1p:BkQ<V@D7T,0*54\r\n", // DAC_412_FI_29 (Environment Sea Area)
		"!AIVDM,1,1,,A,803saNAW7b0@4I54eT;8up2lCBiP,0*5F\r\n", // DAC_412_FI_30 (Along shore Environment)
		"!AIVDM,1,1,,A,803saNAW856v`FmvS15JphJ@D?5?`hd5o<=42T0,2*0E\r\n", // DAC_412_FI_32 (Tide)
		"!AIVDM,1,1,,A,803saNAW8h0j0T@KtC744=9BETNA00,4*6E\r\n", // DAC_412_FI_35 (Obstruction)
		"!AIVDM,1,1,,A,803saNAW900jPPHSb;b2fQG4OHhbEUJUQLQ0,0*2C\r\n", // DAC_412_FI_36 (Activities)
		"!AIVDM,1,1,,A,803saNAW;0011l?D?T6w7`cQ0p@GL>40,0*05\r\n" // DAC_412_FI_44 (Net Sounder)
	};

	for (const auto &vdm : senteces)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Parse new standard early warning messages (FI=31, FI=37, FI=43)
	auto vdm31_1 = GenerateDAC_412_FI_31(1);
	for (const auto &vdm : vdm31_1) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm31_2 = GenerateDAC_412_FI_31(2);
	for (const auto &vdm : vdm31_2) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm31_5 = GenerateDAC_412_FI_31(5);
	for (const auto &vdm : vdm31_5) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm31_6 = GenerateDAC_412_FI_31(6);
	for (const auto &vdm : vdm31_6) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm37 = GenerateDAC_412_FI_37();
	for (const auto &vdm : vdm37) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm38_0 = GenerateDAC_412_FI_38(0);
	for (const auto &vdm : vdm38_0) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm38_1 = GenerateDAC_412_FI_38(1);
	for (const auto &vdm : vdm38_1) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm38_2 = GenerateDAC_412_FI_38(2);
	for (const auto &vdm : vdm38_2) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm42 = GenerateDAC_412_FI_42();
	for (const auto &vdm : vdm42) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm43_0 = GenerateDAC_412_FI_43(0);
	for (const auto &vdm : vdm43_0) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm43_1 = GenerateDAC_412_FI_43(1);
	for (const auto &vdm : vdm43_1) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm5 = GenerateDAC_413_FI_5();
	for (const auto &vdm : vdm5) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm32 = GenerateDAC_412_FI_32();
	for (const auto &vdm : vdm32) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm33_1 = GenerateDAC_412_FI_33(1);
	for (const auto &vdm : vdm33_1) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm33_5 = GenerateDAC_412_FI_33(5);
	for (const auto &vdm : vdm33_5) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm33_14 = GenerateDAC_412_FI_33(14);
	for (const auto &vdm : vdm33_14) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm34_1 = GenerateDAC_412_FI_34(1);
	for (const auto &vdm : vdm34_1) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm34_5 = GenerateDAC_412_FI_34(5);
	for (const auto &vdm : vdm34_5) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm34_3 = GenerateDAC_412_FI_34(3);
	for (const auto &vdm : vdm34_3) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm35_0 = GenerateDAC_412_FI_35(0);
	for (const auto &vdm : vdm35_0) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm35_1 = GenerateDAC_412_FI_35(1);
	for (const auto &vdm : vdm35_1) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm36_0 = GenerateDAC_412_FI_36(0);
	for (const auto &vdm : vdm36_0) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm36_1 = GenerateDAC_412_FI_36(1);
	for (const auto &vdm : vdm36_1) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm36_2 = GenerateDAC_412_FI_36(2);
	for (const auto &vdm : vdm36_2) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto vdm39 = GenerateDAC_412_FI_39();
	for (const auto &vdm : vdm39) vdesManager.Parse(vdm.c_str(), vdm.length());

	for (int tc = 0; tc < 4; ++tc)
	{
		auto vdm40 = GenerateDAC_412_FI_40(tc);
		for (const auto &vdm : vdm40) vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	auto vdm41 = GenerateDAC_412_FI_41();
	for (const auto &vdm : vdm41) vdesManager.Parse(vdm.c_str(), vdm.length());

#if 0
	auto &msiOverviews = vdesManager.GetMSIOverviews();
	for (auto &info : msiOverviews)
	{
		switch (info.type)
		{
		case 0:
			vdesManager.DeleteObstacle(info.dataID);
			break;
		case 1:
			vdesManager.DeleteMaritimeOperation(info.dataID);
			break;
		case 2:
			vdesManager.DeleteMilitaryActivity(info.dataID);
			break;
		case 3:
			vdesManager.DeleteMaritimeDistress(info.dataID);
			break;
		case 4:
			vdesManager.DeleteDesignatedArea(info.dataID);
			break;
		}
	}
#endif
	//vdesManager.SendAISMessage(VDES::VDESManager::AISMessageType::MESSAGE_14, 0, "TEST");
	auto envs = vdesManager.GetMarineEnvironmentFCSTs(0, 100);
	std::cout << "Parsed " << envs.size() << " environment coordinates (FI=28):" << std::endl;
	for (const auto &env : envs)
	{
		std::cout << "  Lat: " << env.coordinate.GetLatitude()
				  << ", Lon: " << env.coordinate.GetLongitude()
				  << ", Flow Vel: " << env.flowVelocity
				  << ", Flow Dir: " << env.flowDirection
				  << ", Wave H: " << env.waveHeight
				  << ", Wave Dir: " << env.waveDirection
				  << ", Temp: " << env.temperature
				  << ", TS: " << env.timestampFCST << std::endl;
	}
	// --- Sea Area Environment Forecasts (FI = 29) ---
	auto envAreas = vdesManager.GetMarineEnvironmentFCSTAreas(0, 100);
	std::cout << "Parsed " << envAreas.size() << " environment areas (FI=29):" << std::endl;
	for (const auto &area : envAreas)
	{
		std::cout << "  Area Code: " << (int)area.areaCode
				  << ", Temp Low: " << area.temperatureLow
				  << ", Temp High: " << area.temperatureHigh
				  << ", Flow Dir Avg: " << area.flowDirectionAvg
				  << ", Flow Dir Max: " << area.flowDirctionMax
				  << ", Flow Vel Avg: " << area.flowVelocityAvg
				  << ", Flow Vel Max: " << area.flowVelocityMax
				  << ", Wave Height: " << area.waveHeight
				  << ", Swell Height: " << area.swellHeight
				  << ", Info Source: " << (int)area.infoSource
				  << ", TS: " << area.timestampFCST << std::endl;
	}

	// --- Maritime Distress (FI = 39) ---
	auto distresses = vdesManager.GetMaritimeDistresses(0, 100);
	std::cout << "Parsed " << distresses.size() << " maritime distresses (FI=39):" << std::endl;
	for (const auto &d : distresses)
	{
		std::cout << "  Type: " << (int)d.type
				  << ", StatusDescription: " << d.statusDescription
				  << ", Judgment: " << (int)d.judgment
				  << ", LocationInstruction: " << (int)d.locationInstruction
				  << ", Duration: " << (int)d.duration
				  << ", CautionCode: " << (int)d.cautionCode
				  << ", Lat: " << d.coordinate.GetLatitude()
				  << ", Lon: " << d.coordinate.GetLongitude()
				  << ", Valid TS: " << d.timestampValid
				  << ", Certified: " << d.isCertified << std::endl;
	}

	// --- Designated Areas (FI = 40) ---
	auto designatedAreas = vdesManager.GetDesignatedAreas(0, 100);
	std::cout << "Parsed " << designatedAreas.size() << " designated areas (FI=40):" << std::endl;
	for (const auto &area : designatedAreas)
	{
		std::cout << "  MRN: " << area.MRN
				  << ", Fragment: " << (int)area.fragment
				  << ", AreaType: " << (int)area.areaType
				  << ", GeometryType: " << (int)area.geometryType
				  << ", CenterLat: " << area.coordinateNE.GetLatitude()
				  << ", CenterLon: " << area.coordinateNE.GetLongitude()
				  << ", Range: " << area.range
				  << ", StartAngle: " << area.sectorStartAngle
				  << ", EndAngle: " << area.sectorEndAngle
				  << ", TimeType: " << (int)area.timeType
				  << ", Start TS: " << area.timestampStart
				  << ", End TS: " << area.timestampEnd
				  << ", Caution: " << (int)area.cautionCode
				  << ", Points: " << area.polygon.size() << std::endl;
		for (size_t i = 0; i < area.polygon.size(); ++i)
		{
			std::cout << "    Pt " << i << ": Lat=" << area.polygon[i].GetLatitude()
					  << ", Lon=" << area.polygon[i].GetLongitude() << std::endl;
		}
	}

	// --- Obstacles (FI = 35) ---
	auto obstacles = vdesManager.GetObstacles(0, 100);
	std::cout << "Parsed " << obstacles.size() << " obstacles (FI=35):" << std::endl;
	for (const auto &obs : obstacles)
	{
		std::cout << "  Type: " << (int)obs.type
				  << ", GeometryType: " << (int)obs.geometryType
				  << ", Longitude: " << obs.coordinate.GetLongitude()
				  << ", Latitude: " << obs.coordinate.GetLatitude()
				  << ", Range: " << (int)obs.range
				  << ", Comment: " << (int)obs.comment
				  << ", Start: " << obs.timestampStart
				  << ", End: " << obs.timestampEnd
				  << ", Duration: " << (int)obs.duration << std::endl;
		if (obs.geometryType == 1)
		{
			std::cout << "    Sector Start Angle: " << obs.sectorStartAngle
					  << ", End Angle: " << obs.sectorEndAngle << std::endl;
		}
		else if (obs.geometryType == 2 || obs.geometryType == 3)
		{
			std::cout << "    Polygon/Polyline coordinates count: " << obs.polygon.size() << std::endl;
			for (const auto &pt : obs.polygon)
			{
				std::cout << "      Lat: " << pt.GetLatitude() << ", Lon: " << pt.GetLongitude() << std::endl;
			}
		}
	}

	// --- Bridges (FI = 41) ---
	auto bridges = vdesManager.GetBridges(0, 100);
	std::cout << "Parsed " << bridges.size() << " bridges (FI=41):" << std::endl;
	for (const auto &bridge : bridges)
	{
		std::cout << "  MRN: " << bridge.MRN
				  << ", Fragment: " << (int)bridge.fragment
				  << ", Flow Velocity: " << bridge.flowVelocity
				  << ", Flow Direction: " << bridge.flowDirection
				  << ", Center Longitude: " << bridge.center.GetLongitude()
				  << ", Center Latitude: " << bridge.center.GetLatitude()
				  << ", Height: " << bridge.height
				  << ", Width: " << bridge.width
				  << ", Pass Dir: " << (int)bridge.directionToPass
				  << ", Pass Ability: " << (int)bridge.passAbility
				  << ", Enable Meeting: " << bridge.enableMeeting
				  << ", Enable Overtaking: " << bridge.enableOvertaking << std::endl;
	}

	// --- Channel Centerlines (FI = 42) ---
	auto centerlines = vdesManager.GetChannelCenterlines(0, 100);
	std::cout << "Parsed " << centerlines.size() << " channel centerlines (FI=42):" << std::endl;
	for (const auto &cc : centerlines)
	{
		std::cout << "  MRN: " << cc.MRN
				  << ", Fragment: " << (int)cc.fragment
				  << ", Width: " << cc.width
				  << ", Coordinates count: " << cc.coordinates.size() << std::endl;
		for (size_t i = 0; i < cc.coordinates.size(); ++i)
		{
			std::cout << "    Pt " << i << ": Lat=" << cc.coordinates[i].GetLatitude()
					  << ", Lon=" << cc.coordinates[i].GetLongitude() << std::endl;
		}
	}

	// --- Channel Boundaries (FI = 43/44) ---
	auto boundaries = vdesManager.GetChannelBoundaries(0, 100);
	std::cout << "Parsed " << boundaries.size() << " channel boundaries (FI=43/44):" << std::endl;
	for (const auto &cb : boundaries)
	{
		std::cout << "  MRN: " << cb.MRN
				  << ", Fragment: " << (int)cb.fragment
				  << ", Left Coordinates count: " << cb.leftCoordinates.size()
				  << ", Right Coordinates count: " << cb.rightCoordinates.size() << std::endl;
		for (size_t i = 0; i < cb.leftCoordinates.size(); ++i)
		{
			std::cout << "    Left Pt " << i << ": Lat=" << cb.leftCoordinates[i].GetLatitude()
					  << ", Lon=" << cb.leftCoordinates[i].GetLongitude() << std::endl;
		}
		for (size_t i = 0; i < cb.rightCoordinates.size(); ++i)
		{
			std::cout << "    Right Pt " << i << ": Lat=" << cb.rightCoordinates[i].GetLatitude()
					  << ", Lon=" << cb.rightCoordinates[i].GetLongitude() << std::endl;
		}
	}

	// --- Along shore Environmental Forecast (FI = 30) ---
	auto environmentAlongshores = vdesManager.GetMarineEnvironmentFCSTAlongshores(0, 100);
	std::cout << "Parsed " << environmentAlongshores.size() << " along shore environment forecasts (FI=30):" << std::endl;
	for (const auto &alongshore : environmentAlongshores)
	{
		std::cout << "  Area Code: " << (int)alongshore.areaCode
				  << ", Temp Low: " << alongshore.temperatureLow
				  << ", Temp High: " << alongshore.temperatureHigh
				  << ", Wave Low: " << alongshore.waveHeightLow
				  << ", Wave High: " << alongshore.waveHeightHigh
				  << ", Tide High: " << alongshore.tideHigh
				  << ", TS Tide High: " << alongshore.timestampTideHigh
				  << ", Tide Low: " << alongshore.tideLow
				  << ", TS Tide Low: " << alongshore.timestampTideLow
				  << ", TS FCST: " << alongshore.timestampFCST
				  << ", Info Source: " << (int)alongshore.infoSource << std::endl;
	}

	// --- Maritime Operations (FI = 36) ---
	auto maritimeOperations = vdesManager.GetMaritimeOperations();
	std::cout << "Parsed " << maritimeOperations.size() << " maritime operations (FI=36):" << std::endl;
	for (const auto &op : maritimeOperations)
	{
		std::cout << "  MMSI: " << op.mmsi
				  << ", Type: " << (int)op.type
				  << ", GeometryType: " << (int)op.geometryType
				  << ", Lat: " << op.coordinateNE.GetLatitude()
				  << ", Lon: " << op.coordinateNE.GetLongitude()
				  << ", Range: " << op.range
				  << ", Start: " << op.timestampStart
				  << ", End: " << op.timestampEnd 
				  << ", TimeType: " << (int)op.timeType
				  << ", Caution: " << (int)op.cautionCode << std::endl;
		if (op.geometryType == 1)
		{
			std::cout << "    Sector Start Angle: " << op.sectorStartAngle
					  << ", End Angle: " << op.sectorEndAngle << std::endl;
		}
		else if (op.geometryType == 2 || op.geometryType == 3)
		{
			std::cout << "    Polygon/Polyline coordinates count: " << op.polygon.size() << std::endl;
			for (const auto &pt : op.polygon)
			{
				std::cout << "      Lat: " << pt.GetLatitude() << ", Lon: " << pt.GetLongitude() << std::endl;
			}
		}
	}

	// --- Maritime Towings (FI = 37) ---
	auto maritimeTowings = vdesManager.GetMaritimeTowings();
	std::cout << "Parsed " << maritimeTowings.size() << " maritime towings (FI=37):" << std::endl;
	for (const auto &towing : maritimeTowings)
	{
		std::cout << "  MMSI: " << towing.mmsi
				  << ", Start Coord: Lat: " << towing.coordinateStart.GetLatitude() << ", Lon: " << towing.coordinateStart.GetLongitude()
				  << ", End Coord: Lat: " << towing.coordinateEnd.GetLatitude() << ", Lon: " << towing.coordinateEnd.GetLongitude()
				  << ", Length: " << towing.length << " m, Width: " << (int)towing.width << " m"
				  << ", Speed: " << towing.speed << " kn"
				  << ", Start Time: " << towing.timestampStart
				  << ", End Time: " << towing.timestampEnd
				  << ", Caution: " << (int)towing.cautionCode << std::endl;
	}

	// --- Net Sounders (FI = 45) ---
	auto netSounders = vdesManager.GetNetSounders(0, 100);
	std::cout << "Parsed " << netSounders.size() << " net sounders (FI=45):" << std::endl;
	for (const auto &ns : netSounders)
	{
		std::cout << "  MRN: " << ns.MRN
				  << ", Fragment: " << (int)ns.fragment
				  << ", Type: " << (int)ns.type
				  << ", Continous: " << (int)ns.isContinous
				  << ", Nets Count: " << ns.nets.size()
				  << ", Description: " << ns.description << std::endl;
		for (size_t i = 0; i < ns.nets.size(); ++i)
		{
			std::cout << "    Net " << i << ": MRN=" << ns.nets[i].MRN
					  << ", FragmentDesc=" << (int)ns.nets[i].fragmentDesc
					  << ", Lat=" << ns.nets[i].latitude
					  << ", Lon=" << ns.nets[i].longitude << std::endl;
		}
	}

	// --- Frontend Prompts (DAC = 413, FI = 5) ---
	auto prompts = vdesManager.GetFrontendPrompts(0, 100);
	std::cout << "Parsed " << prompts.size() << " frontend prompts (DAC=413, FI=5):" << std::endl;
	for (const auto &pr : prompts)
	{
		std::cout << "  PromptContent: " << (int)pr.promptContent
				  << ", DisplayDuration: " << (int)pr.displayDuration << " min"
				  << ", Text: " << pr.text << std::endl;
	}

	// --- Marine Meteorology & Environmental Warnings (FI = 31) ---
	auto cyclones = vdesManager.GetMewTropicalCyclones(0, 100);
	std::cout << "Parsed " << cyclones.size() << " tropical cyclone warnings (FI=31):" << std::endl;
	for (const auto &ew : cyclones)
	{
		std::cout << "  MRN: " << ew.MRN
				  << ", Published: " << ew.timestampPublished
				  << ", Start: " << ew.timestampStart
				  << ", End: " << ew.timestampEnd
				  << ", Duration: " << (int)ew.warningDuration
				  << ", Info Source: " << (int)ew.infoSource << std::endl;
		std::cout << "    Cyclone path points count: " << ew.pathPoints.size() << std::endl;
	}

	auto gales = vdesManager.GetMewGales(0, 100);
	std::cout << "Parsed " << gales.size() << " gale warnings (FI=31):" << std::endl;
	for (const auto &ew : gales)
	{
		std::cout << "  MRN: " << ew.MRN
				  << ", Area Code: " << (int)ew.areaCode
				  << ", Warning Level: " << (int)ew.warningLevel
				  << ", Published: " << ew.timestampPublished << std::endl;
	}

	auto waves = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "Parsed " << waves.size() << " large wave warnings (FI=31):" << std::endl;

	auto fogs = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "Parsed " << fogs.size() << " sea fog warnings (FI=31):" << std::endl;

	auto surges = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "Parsed " << surges.size() << " storm surge warnings (FI=31):" << std::endl;

	auto ices = vdesManager.GetMewSeaIces(0, 100);
	std::cout << "Parsed " << ices.size() << " sea ice warnings (FI=31):" << std::endl;

	// --- Military Activities (FI = 38) ---
	auto militaryActivities = vdesManager.GetMilitaryActivitys();
	std::cout << "Parsed " << militaryActivities.size() << " military activities (FI=38):" << std::endl;
	for (const auto &act : militaryActivities)
	{
		std::cout << "  Theme: " << (int)act.theme
				  << ", GeometryType: " << (int)act.geometryType
				  << ", Center/Start: Lat: " << act.coordinateNE.GetLatitude() << ", Lon: " << act.coordinateNE.GetLongitude()
				  << ", Range: " << act.range << " nm"
				  << ", Sector Start Angle: " << act.sectorStartAngle << ", End Angle: " << act.sectorEndAngle
				  << ", Start: " << act.timestampStart
				  << ", End: " << act.timestampEnd
				  << ", TimeType: " << (int)act.timeType
				  << ", Caution: " << (int)act.cautionCode << std::endl;
		if (act.geometryType == 2 || act.geometryType == 3)
		{
			std::cout << "    Polygon/Polyline coordinates count: " << act.polygon.size() << std::endl;
			for (const auto &pt : act.polygon)
			{
				std::cout << "      Lat: " << pt.GetLatitude() << ", Lon: " << pt.GetLongitude() << std::endl;
			}
		}
	}

	// --- Tide Forecasts (FI = 32) ---
	auto tides = vdesManager.GetTideForecasts(0, 100);
	std::cout << "Parsed " << tides.size() << " tide forecasts (FI=32):" << std::endl;
	for (const auto &tide : tides)
	{
		std::cout << "  Hour Publish: " << (int)tide.hourPublish
				  << ", Info Source: " << (int)tide.infoSource
				  << ", Stations count: " << tide.stations.size() << std::endl;
		for (const auto &station : tide.stations)
		{
			std::cout << "    Station: Lat: " << station.coordinate.GetLatitude()
					  << ", Lon: " << station.coordinate.GetLongitude()
					  << ", Tidal Datum: " << station.tidalDatum << " cm"
					  << ", Tide High: " << station.tideHigh << " cm at " << station.timestampTideHigh
					  << ", Tide Low: " << station.tideLow << " cm at " << station.timestampTideLow << std::endl;
		}
	}

	// --- AtoN Dynamics (FI = 33) ---
	auto atonDynamicsList = vdesManager.GetAtoNDynamics(0, 100);
	std::cout << "Parsed " << atonDynamicsList.size() << " AtoN Dynamics (FI=33):" << std::endl;
	for (const auto &atonDyn : atonDynamicsList)
	{
		std::cout << "  MRN: " << atonDyn.MRN
				  << ", Fragment: " << (int)atonDyn.fragment
				  << ", Status: " << (int)atonDyn.status
				  << ", Precaution: " << (int)atonDyn.precaution
				  << ", Elements count: " << atonDyn.elements.size() << std::endl;
		for (const auto &elem : atonDyn.elements)
		{
			std::cout << "    Element MRN: " << elem.MRN
					  << ", FragmentDesc: " << (int)elem.fragmentDesc
					  << ", Type: " << (int)elem.type
					  << ", Desc: \"" << elem.description << "\""
					  << ", Coordinate: Lat: " << elem.coordinate.GetLatitude() << ", Lon: " << elem.coordinate.GetLongitude()
					  << ", rhythmNameCode: " << (int)elem.rhythmNameCode
					  << ", rhythmParamCode: " << (int)elem.rhythmParamCode
					  << ", bodyColor: " << (int)elem.bodyColor
					  << ", lightColor: " << (int)elem.lightColor
					  << ", lightPeriod: " << (int)elem.lightPeriod
					  << ", range: " << (int)elem.range
					  << ", morseCode: " << (int)elem.morseCode
					  << ", prevCoordinate: Lat: " << elem.prevCoordinate.GetLatitude() << ", Lon: " << elem.prevCoordinate.GetLongitude()
					  << ", isRoughPosition: " << (elem.isRoughPosition ? "True" : "False")
					  << ", failureType: " << (int)elem.failureType
					  << ", timestampStart: " << elem.timestampStart
					  << ", timestampEnd: " << elem.timestampEnd << std::endl;
		}
	}

	// --- AIS AtoN Dynamics (FI = 34) ---
	auto aisAtonDynamicsList = vdesManager.GetAISAtoNDynamics(0, 100);
	std::cout << "Parsed " << aisAtonDynamicsList.size() << " AIS AtoN Dynamics (FI=34):" << std::endl;
	for (const auto &atonDyn : aisAtonDynamicsList)
	{
		std::cout << "  MRN: " << atonDyn.MRN
				  << ", Fragment: " << (int)atonDyn.fragment
				  << ", AtoNAttribute: " << (int)atonDyn.atonAttribute
				  << ", Status: " << (int)atonDyn.status
				  << ", Precaution: " << (int)atonDyn.precaution
				  << ", Elements count: " << atonDyn.elements.size() << std::endl;
		for (const auto &elem : atonDyn.elements)
		{
			std::cout << "    Element MMSI: " << elem.mmsi
					  << ", MRN: " << elem.MRN
					  << ", Type: " << (int)elem.type
					  << ", Coordinate: Lat: " << elem.coordinate.GetLatitude() << ", Lon: " << elem.coordinate.GetLongitude()
					  << ", range: " << (int)elem.range
					  << ", prevCoordinate: Lat: " << elem.prevCoordinate.GetLatitude() << ", Lon: " << elem.prevCoordinate.GetLongitude()
					  << ", isRoughPosition: " << (elem.isRoughPosition ? "True" : "False") << std::endl;
		}
	}

	//auto messages = vdesManager.GetMessages(VDES::VDESManager::MailBox::INBOX);
	//auto messages = vdesManager.GetLastInteractionMessages(444444444);
	//auto requests = vdesManager.GetVTSRequests();

	//vdesManager.SendInformation(412000001, 412000002, "HELLO", false);

	/*vdesManager.SendRequestToVTS(999712001, 8);

	vdesManager.SendInformation(0, 0, "HELLO", false);

	auto hydrometeorologies = vdesManager.GetHydrometeorology();*/

	

	/*VDES::BoundingBox::Rect rect;
	rect.left = 118;
	rect.right = 119;
	rect.top = 25;
	rect.bottom = 23;

	auto bbox = VDES::BoundingBox::Build(rect);
	auto atons = vdesManager.GetAtoNs(bbox);
	auto distresses = vdesManager.GetMaritimeDistresses(bbox);
	auto func = std::bind(VDESBusinessNotifyHandle, std::placeholders::_1, std::placeholders::_2);

	vdesManager.notifyEvent.append(func);

    auto &instance = VDES::VDESManager::GetInstance();

	instance.DeleteMessages(VDES::VDESManager::MailBox::OUTBOX, 0, 1);

	auto value = instance.GetMilitaryActivitys(0, 100);

	auto views = instance.GetMSIOverviews(0, 100);*/

	//instance.DeleteAtoN(994136008);
	//TestParse();
#if 0

	std::vector<std::string> senteces{
		"$AIVEM,1,1,12,999712001,666666666,00802p0V0PHV?D3BaDjHIUtC700000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*02\r\n",
		"$AIVEM,1,1,59,000000111,000000000,00802Ol=DuTqqaVd:0iP45L`ldF50Hh28T00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*7E\r\n",
		"$AIVEM,1,1,48,000000111,000000000,00802P<EqW>8eEg@F;PKFGq=`;`R;WUSs3aWIWVP87Tgr7VPJwWc:KUT8SUfbKUg8;Uf;P00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*2B\r\n",
		"$AIVEM,1,1,95,008888888,799999990,0080006vffff::::00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*0C\r\n",
		"$AIVEM,1,1,88,008888888,000000000,0080006nl<2<228000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*40\r\n",
		"$AIVEM,1,1,99,008888888,000000000,00802Q2HUWs`>2K2HH3h6JU6H3WU;GVU`OVTboUSq7a`a7WS`OWc:KUT8SUfbKUg8;Uf;SUTHc`cJH00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*1D\r\n",
		"$AIVEM,1,1,29,999712001,799999990,00800P0101@0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*5B\r\n",
		"$XNTDB,1,1,0,,999712001,0,0,17,00800H08HG=kI65kHG=THG=QLnAW89H000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*32\r\n",
		"!ABTDB,1,1,52,999712001,799999990,0,0,11,00800P0100h0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*55\r\n",
		//"!ABTDB,1,1,44,999712001,799999990,0,0,11,00801h28000000000000000000000000000006LdQ6Ll97u5w=td;BP:2>T::00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*0C\r\n",
		//"$AIVEM,1,1,45,999712001,799999990,00800J1`qJNDq`VHq`RAIi2w=h0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*54\r\n",
		//"$XNTDB,1,1,0,,999712001,0,0,17,00800P010=@0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*6A\r\n",
		//"!ABTSB,1,1,19,93,999712001,0,0,0,17,00800m9bT=7dogfN=Tw>h24I752:36bnV`00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*74\r\n",
		//"$ABVEM,1,1,43,799999990,999712001,00801h0h0o;8AW=2Ap19rF00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*05\r\n",
		//"$XNTSB,1,1,0,93,,,0,0,17,00800@0:IGMUIGMVInQkI7=VRuWW00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*29\r\n",
		//"$XNTSB,1,1,0,93,,,0,0,17,00800@0>qJBlqHvAq`v@qrBrq;vQq`6gInIVInKVRJ?VRJ=VRuem00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*0C\r\n",
		//"$XNTDB,1,1,0,,987654321,0,0,17,00800008PDhht0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*23\r\n",
		//"!ABTSB,1,1,80,92,999712001,799999990,0,0,19,00801h18000000000000000000000000000006?;Bo<s8hg;Bo<s8hPHte;LkdS2te;LkdS200000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*49\r\n",
		//"!ABTSB,1,1,58,93,999712001,0,0,0,17,008020R6j4h5TWs@8000P00900000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*6F\r\n",
		//"$AIVEM,1,1,86,999712001,000000000,00800vqc9v:`K7stF>`702AG5B1VrVIt00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*5C\r\n",
		//"!ABTDB,1,1,5,999712001,0,0,0,11,0080003l@4hT4qP4U5E`PtpMPTqVodMh00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*33\r\n",
		//"!ABTDB,1,1,7,999712001,666666666,0,0,11,0080083uQDq<PT<E<PUVocjQwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwh,4*7C\r\n",
		//"!ABTDB,1,1,9,999712001,0,0,0,11,00802h44He0@TaW1j@dd5:b0Ies7P00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*45\r\n",
		//"!ABTDB,1,1,13,999712001,666666666,0,0,11,00802p4DHe0@TaAHEVtd5:b0Ies7r00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*74\r\n",
		//"!ABTDB,1,1,15,999712001,0,0,0,11,00800@4LEEA6;CSUfKwVTboVeHg`cqEVodPP00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*47\r\n",
		//"!ABTDB,1,1,17,999712001,666666666,0,0,11,00800J4TEEA6;CSUcsgUWH3VeHg`cqEVodQR00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*09\r\n",
		//"!ABTDB,1,1,19,999712001,0,0,0,11,00800i;DLBQ8RLTjEr71P2DDD6KNj9H0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*03\r\n",
		//"!ABTDB,1,1,21,999712001,666666666,0,0,11,00801h5bNtQb`9:j`au4AEM5gGtt5LtlUFtdO8c:9T0W3V:R4PW17S0P003@R33JAB8Mnh@C=gI:J136bfAwc`F4dkKnCD00hc:RWlA5ElFuOkhEkkBEKjitSkFdGlEtb1:db:IJQ`bVT980000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*2C\r\n",

		//"!ABTDB,1,1,1,999712001,0,0,0,11,00802P1w:1E8STbpF?lQFKFUu3`R;WUSs3aWIWVP87Tgr7VPJwWc:KUT8SUfbKUg8;Uf;P00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*27\r\n",
		//"!ABTDB,1,1,2,999712001,0,0,0,11,00802P1w:1E8STbpF?lQFKFUu3Ug8;Uf;SUTHc`cJKTgr7VPJt00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*00\r\n",
		//"!ABTDB,1,1,14,999712001,0,0,0,11,00802PCoT=E8SWppF:19FKFUu3aWIWVP87Wc:KUT8SUfbKUg8;Uf;SVeHg`cqD00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*46\r\n",
		//"!ABTDB,1,1,80,111,0,0,0,11,00802RaRg<qi5ber2iL3Jjw9e11Rg<qm5BM`jgAHPjw9fNSTf83`R;WUa9cW`87Ug8;Uf;P0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*33\r\n",
		//"!ABTDB,1,1,85,111,0,0,0,11,00802R<EqW>2AJcNPdG0ndgjK@A5BM`jgAHPjw9fNfRrbvBsgNV7SNFTSNR8fNF?d>FtPfFpf00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*72\r\n",
		//"!ABTDB,1,1,90,111,0,0,0,11,00802P<EqW>8eEg@F;PKFGq=`;`R;WUSs3aWIWVP87Tgr7VPJwWc:KUT8SUfbKUg8;Uf;P00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*05\r\n",
		//"$AIVEM,1,1,5,999712001,799999990,00800J0>EmMGEnJJ7@00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*7F\r\n",
		//"$AIVEM,1,1,87,999712001,000000000,0080229HaUeSAHbt0e0o0<tSF0l4Q@1;gP0t8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*42\r\n",
		/*"$AIVEM,1,1,73,008888888,000000000,008022IHVbds@wcrPdG4000000000003bW`rh000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*71\r\n",
		"$AIVEM,1,1,34,008888888,000000000,008022AHUoBC@viDPdG4000000000003bUNs0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*2A\r\n",
		"$AIVEM,1,1,35,008888888,000000000,00800@2jr8Nb=vJLR3WVUrDi<30hqaNnr8Nk<CPh<>JGefvtS>NDdNJlaNRmek7`gJsUW:SUa:OVdcoUSr?Vf:wUS;cUa:WVe:GTf;CVf:wVf:wURb7aVpKUVr;VW8WaVI3UQJkUSsPi<fF?evJkRfBuSNF9SNJjgk4m<>NidvJhe>FOWvR<PvFKe>F6QNvtS>RwVvRQS>JnafJIb3;`gJs`ccsUa8OUT8c``pGTgIkTf9cSP8;Uf8kUT8C`R:c`R;WVdrSVQ8wSP89VSLup00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*62\r\n",
		"$AIVEM,1,1,76,008888888,000000000,00802Ol?<<4qTRIE01D066LC>lK00d038400000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*22\r\n",
		"$AIVEM,1,1,63,008888888,000000000,00802RURCeI12VRUjO3KhkAEo01UMoL0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*2A\r\n",
		"$AIVEM,1,1,45,008888888,000000000,0080248HIVp006RTP<je=P3BQ06IG2@2VBgN`07r00P0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*4C\r\n",
		"$AIVEM,1,1,76,008888888,000000000,00802Ol?<<4qTRIE01D066LC>lK00d038400000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*22\r\n",
		"$AIVEM,1,1,76,008888888,000000000,00802Ol?<<4qTRIE01D066LC>lK00d038400000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*22\r\n",
		"$AIEDM,1,1,57,14,G@1L08NR>5<00000008026`,2*25\r\n",
		"$AIVEM,1,1,96,008888888,799999990,00800@0>qcRaqKbVqHNTqHNhqsnAJGEVT78000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*7A\r\n",
		"$AIVEM,1,1,35,008888888,799999990,00801i4bvg1wH6LleLtmW:tdS5M5O4aSaP00000000000000000PDBb808UD@0000000001wAMEOCEk7=<Ew;BCG;HB81bJ>aP0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*42\r\n",
		"$AIVEM,1,1,55,008888888,000000000,00802h6rslfu50wO`>JBM7@L6tqTPSDH4q1QtpMb3gBslD79qjq`=goKn@HT>9:PrUqci1oaMr:1vw@LlTr>Pp=qk916`h9j33qhsD7NUo``>CkUk@KOfodPi8LBE1m;kGR3gBslD3uvPqa9lM1hKkVB2=APCT67kQn`>u;gA@LWW;VPnwMgI1R@pTb3bGVg47NUo``7su1kBC`r3PoW<T4JS0W8<?W3e@MrGNRPq?>G=1evsNj34Qi9D7Dg=N8>u;gA@?or3VTWAl71g>I88m61>@HO>7JPslfu51jNLfJ3KunuT693RB`>aNJt@MrGNRPOgl7=9>S`>3NLj@Ab<2LPhvL>m1oaMr:3TtqLl6oses8<B74U@MBtmpPslfu50wO`>JBM7@L6tqTPSDH4q1QtpMb3gBslD79qjq`=goKn@HT>9:PrUqci1oaMr:1vw@LlTr>Pp=qk916`h9j33qhsD7NUo``>CkUk@KOfodPi8LBE1m;kGR3gBslD3uvPqa9lM1hKkVB2=APCT67kQn`>u;gA@LWW;VPnwMgI1R@pTb3bGVg47NUo``7su1kBC`r3PoW<T4JS0W8<?W3e@MrGNRPq?>G=1evsNj34Qi9D7Dg=N8>u;gA@?or3VTWAl71g>I88m61>@HO>7JPslfu51jNLfJ3KunuT693RB`>aNJt@MrGNRPOgl7=9>S`>3NLj@Ab<2LPhvL>m1oaMr:3TtqLl6oses8<B74U@MBtmpPslfu50wO`>JBM7@L6tqTPSDH4q1QtpMb3gBslD79qjq`=goKn@HT>9:PrUqci1oaMr:0EhKkVB2=APCT67kQn`>u;gA@LWW;VPnwMgI1R@pTb3bGVg47NUo``7su1kBC`r3PoW<T4JS0W8<?W3e@MrGNRPq?>G=0:slfu50wO`>JBM7@L6tqTPSDH4q1QtpMb3gBslD79qjq`=goKn@HT>9:PrUqci1oaMr:1vw@LlTr>Pp=qk916`h9j33qhsD7NUo``>CkUk@KOfodPi8LBE1m;kGR3gBslD3uvPqa9lM1hKkVB2=APCT67kQn`>u;gA@LWW;VPnwMgI1R@pTb0D152PI`jI`00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*45\r\n",
		"$AIVEM,1,1,36,008888888,799999990,00801hi:vg1wH6Ltq5ttq5td?@Lt5Lmmmj00000000000000000hTCwTg<KBKpI3=;uD00IQrD3<?BP00kC0=@00cjlU?jGdokEtalGl7kCeukE53jkdf1:8S8:8S8:8R880000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*72\r\n",
		"$AIVEM,1,1,42,008888888,000000000,0080248J9qch4G?615WT@2r:40S99CqlFBOM`07L00@0000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*68\r\n",
		"$AIVEM,1,1,11,008888888,000000000,00800vl59c>`WVg0ALtH021116J@LP00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*25\r\n",
		"$AIVEM,1,1,45,008888888,799999990,00800@6R@E=6@NJ8TNNLPNF5c>F?f>J9TvFoaNNJQ>BrfVJ=kh00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,0*62\r\n",
		"$AIVEM,1,1,73,008888888,000000000,00802h4rPPICFVBU=JhBAtVPI<vd4KB<vLo;usS59os2MqoNAoVuqH1VbGr000000000000000000000000000000000000000000000000000000000000000000000000000000000000000,4*63\r\n",
		"$AIVEM,1,1,97,008888888,000000000,00802h56slfu50wOgp`;VTWAl71g>IA@DJS0WA@F7kQni@GNUo`i@G9qjqi@FosesA@F93RBi@GDg=N0IbUvP00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*3A\r\n",
		"$AIVEM,1,1,97,008888888,000000000,00802h56slfu50wOgp`;VTWAl71g>IA@DJS0WA@F7kQni@GNUo`i@G9qjqi@FosesA@F93RBi@GDg=N0IbUvP00000000000000000000000000000000000000000000000000000000000000000000000000000000000000,2*3A\r\n",*/
	};

	for (auto &sentece : senteces)
	{
		vdesManager.Parse(sentece.c_str(), sentece.size());
	}
#endif
	
#if 0
	auto obs = vdesManager.GetObstacles();
	auto ships = vdesManager.GetAbnormalShips();

	auto operations = vdesManager.GetMaritimeOperations();

	vdesManager.SetInformationRead(VDES::InformationType::MESSAGE_RECEIVED, 1, true);
	auto fcsts = vdesManager.GetMarineMeteorologyFCSTs(0);
	fcsts = vdesManager.GetMarineMeteorologyFCSTs(VDES::Coordinate(38.7, 118.1), 0.5);
	
	VDES::BoundingBox::Rect rectMeteorology;
	rectMeteorology.left = 118;
	rectMeteorology.right = 119;
	rectMeteorology.top = 39;
	rectMeteorology.bottom = 38;

	auto bboxMeteorology = VDES::BoundingBox::Build(rectMeteorology);
	fcsts = vdesManager.GetMarineMeteorologyFCSTs(bboxMeteorology);
	vdesManager.DeleteMarineMeteorologyFCSTs(0, 100);
	std::vector<uint32_t> dataIDS { 0, 1, 2, 3 };
	vdesManager.DeleteMarineMeteorologyFCSTs(dataIDS);
	auto enviroments = vdesManager.GetMarineEnvironmentFCSTs(0);

	vdesManager.DeleteMarineEnvironmentFCSTs(dataIDS);

	/*auto ships = vdesManager.GetAbnormalShips();
	auto operation = vdesManager.GetMaritimeOperation(24.038732753828867, 118.0749478536279, 0.90713570668490295);
	auto opreations = vdesManager.GetMaritimeOperations();
	vdesManager.SendInformation(413123456, 412123567, "TEST");*/
	//instance.EmptyDatabase();
#endif
#endif

	// Test parsing left and right channel boundaries (MRN=302)
	{
		std::cout << "\n--- Simulating Left Boundary (FI=43) parsing... ---" << std::endl;
		auto leftVDMs = GenerateDAC_412_FI_43(0);
		for (const auto &vdm : leftVDMs)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}

		std::cout << "\n--- Simulating Right Boundary (FI=44) parsing... ---" << std::endl;
		auto rightVDMs = GenerateDAC_412_FI_43(1);
		for (const auto &vdm : rightVDMs)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Verify Channel Boundaries (FI = 43/44) from database
	std::cout << "\n=== Verification: Querying Channel Boundaries (FI=43/44) from DB ===" << std::endl;
	auto boundaries = vdesManager.GetChannelBoundaries(0, 100);
	std::cout << "Parsed and saved Channel Boundaries count: " << boundaries.size() << std::endl;
	for (const auto &cb : boundaries)
	{
		std::cout << "  MRN: " << cb.MRN
				  << ", Fragment: " << (int)cb.fragment
				  << ", Left Coordinates count: " << cb.leftCoordinates.size()
				  << ", Right Coordinates count: " << cb.rightCoordinates.size() << std::endl;
		for (size_t i = 0; i < cb.leftCoordinates.size(); ++i)
		{
			std::cout << "    Left Pt " << i << ": Lat=" << cb.leftCoordinates[i].GetLatitude()
					  << ", Lon=" << cb.leftCoordinates[i].GetLongitude() << std::endl;
		}
		for (size_t i = 0; i < cb.rightCoordinates.size(); ++i)
		{
			std::cout << "    Right Pt " << i << ": Lat=" << cb.rightCoordinates[i].GetLatitude()
					  << ", Lon=" << cb.rightCoordinates[i].GetLongitude() << std::endl;
		}
	}
	std::cout << "=========================================================\n" << std::endl;

	// Verify Maritime Towing (FI=37) from database
	std::cout << "\n=== Verification: Querying Maritime Towings (FI=37) from DB ===" << std::endl;
	auto towings = vdesManager.GetMaritimeTowings(0, 100);
	std::cout << "Parsed and saved Maritime Towings count: " << towings.size() << std::endl;
	for (const auto &t : towings)
	{
		std::cout << "  MMSI: " << t.mmsi
				  << ", TowingMethod: " << (int)t.towingMethod
				  << ", Length: " << t.length
				  << ", Width: " << (int)t.width
				  << ", Speed: " << (int)t.speed
				  << ", StartTime: " << t.timestampStart
				  << ", EndTime: " << t.timestampEnd << std::endl;
		std::cout << "    StartPt: Lat=" << t.coordinateStart.GetLatitude()
				  << ", Lon=" << t.coordinateStart.GetLongitude() << std::endl;
		std::cout << "    EndPt: Lat=" << t.coordinateEnd.GetLatitude()
				  << ", Lon=" << t.coordinateEnd.GetLongitude() << std::endl;
	}
	std::cout << "=========================================================\n" << std::endl;

	// Verify NetSounder (FI=45) from database
	std::cout << "\n=== Verification: Querying Net Sounders (FI=45) from DB ===" << std::endl;
	auto netSounders = vdesManager.GetNetSounders(0, 100);
	std::cout << "Parsed and saved Net Sounders count: " << netSounders.size() << std::endl;
	for (const auto &ns : netSounders)
	{
		std::cout << "  MRN (First): " << ns.MRN
				  << ", Type: " << (int)ns.type
				  << ", Continuous: " << (int)ns.isContinous
				  << ", Nets Count: " << ns.nets.size()
				  << ", Description: " << ns.description << std::endl;
		for (size_t i = 0; i < ns.nets.size(); ++i)
		{
			std::cout << "    Net " << i << ": MRN=" << ns.nets[i].MRN
					  << ", FragmentDesc=" << (int)ns.nets[i].fragmentDesc
					  << ", Lat=" << ns.nets[i].latitude
					  << ", Lon=" << ns.nets[i].longitude << std::endl;
		}
	}
	std::cout << "=========================================================\n" << std::endl;

	// Verify Tide Forecast (FI=32) from database
	std::cout << "\n=== Verification: Querying Tide Forecasts (FI=32) from DB ===" << std::endl;
	auto tides = vdesManager.GetTideForecasts(0, 100);
	std::cout << "Parsed and saved Tide Forecasts count: " << tides.size() << std::endl;
	for (const auto &t : tides)
	{
		std::cout << "  Hour Publish: " << (int)t.hourPublish
				  << ", Info Source: " << (int)t.infoSource
				  << ", Timestamp (Publication): " << t.timestamp
				  << ", Stations Count: " << t.stations.size() << std::endl;
		for (size_t i = 0; i < t.stations.size(); ++i)
		{
			const auto &station = t.stations[i];
			std::cout << "    Station " << i << ": Lat=" << station.coordinate.GetLatitude()
					  << ", Lon=" << station.coordinate.GetLongitude()
					  << ", Datum=" << station.tidalDatum << " cm"
					  << ", High Tide: " << station.tideHigh << " cm at " << station.timestampTideHigh
					  << ", Low Tide: " << station.tideLow << " cm at " << station.timestampTideLow << std::endl;
		}
	}
	std::cout << "===========================================================\n" << std::endl;

	// Verify Maritime Towing (FI=37) from database
	std::cout << "\n=== Verification: Querying Maritime Towings (FI=37) from DB ===" << std::endl;
	auto maritimeTowings = vdesManager.GetMaritimeTowings();
	std::cout << "Parsed and saved Maritime Towings count: " << maritimeTowings.size() << std::endl;
	for (const auto &t : maritimeTowings)
	{
		std::cout << "  MMSI: " << t.mmsi
				  << ", Towing Method: " << (int)t.towingMethod
				  << ", Start Coord: Lat: " << t.coordinateStart.GetLatitude() << ", Lon: " << t.coordinateStart.GetLongitude()
				  << ", End Coord: Lat: " << t.coordinateEnd.GetLatitude() << ", Lon: " << t.coordinateEnd.GetLongitude()
				  << ", Length: " << t.length << " m, Width: " << (int)t.width << " m"
				  << ", Speed: " << t.speed << " kn"
				  << ", Start Time: " << t.timestampStart
				  << ", End Time: " << t.timestampEnd
				  << ", Caution: " << (int)t.cautionCode << std::endl;
	}
	std::cout << "==============================================================\n" << std::endl;

	// Verify Alongshore Forecast (FI=30) and deletion from database
	std::cout << "\n=== Verification: Querying Alongshore Forecasts (FI=30) from DB ===" << std::endl;
	auto alongshoresBefore = vdesManager.GetMarineEnvironmentFCSTAlongshores(0, 100);
	std::cout << "Initial Alongshore Forecasts count: " << alongshoresBefore.size() << std::endl;
	std::vector<uint32_t> idsToDelete;
	for (const auto &a : alongshoresBefore)
	{
		std::cout << "  Area Code: " << (int)a.areaCode
				  << ", Temp Low: " << a.temperatureLow << " C"
				  << ", Temp High: " << (int)a.temperatureHigh << " C"
				  << ", dataID: " << a.dataID << std::endl;
		idsToDelete.push_back(a.dataID);
	}

	if (!idsToDelete.empty())
	{
		std::cout << "\n--- Testing DeleteMarineEnvironmentFCSTAlongshores by IDs ---" << std::endl;
		bool success = vdesManager.DeleteMarineEnvironmentFCSTAlongshores(idsToDelete);
		std::cout << "Deletion result: " << (success ? "SUCCESS" : "FAILED") << std::endl;

		auto alongshoresAfter = vdesManager.GetMarineEnvironmentFCSTAlongshores(0, 100);
		std::cout << "Post-deletion Alongshore Forecasts count: " << alongshoresAfter.size() << std::endl;
	}
	std::cout << "==================================================================\n" << std::endl;

	// Verify SendNetSounder (FI=45) active broadcast
	std::cout << "\n=== Verification: Sending Net Sounder Broadcast (FI=45) ===" << std::endl;
	
	// Temporarily capture sendEvent output
	std::string capturedNMEA;
	auto handle = vdesManager.sendEvent.append([&](const VDES::CommunicationType type, const char *data, size_t size) {
		std::string sentence(data, size);
		if (sentence.find("AAB") != std::string::npos) {
			capturedNMEA = sentence;
		}
	});

	VDES::NetSounder ownNetSounder;
	ownNetSounder.MRN = 12345;
	ownNetSounder.fragment = 0;
	ownNetSounder.type = 3;
	ownNetSounder.isContinous = true;
	ownNetSounder.timestamp = VDES::UtilityInterface::GetCurrentTimeStamp();
	ownNetSounder.description = "Vessel own active net sounder";

	VDES::NetSounder::NetInfo net1;
	net1.MRN = 12345;
	net1.fragmentDesc = 1; // Needs text supplement!
	net1.latitude = 24.4186;
	net1.longitude = 118.54;
	ownNetSounder.nets.push_back(net1);

	VDES::NetSounder::NetInfo net2;
	net2.MRN = 12346;
	net2.fragmentDesc = 0;
	net2.latitude = 24.4786;
	net2.longitude = 118.66;
	ownNetSounder.nets.push_back(net2);

	bool sendSuccess = vdesManager.SendNetSounder(ownNetSounder);
	std::cout << "SendNetSounder result: " << (sendSuccess ? "SUCCESS" : "FAILED") << std::endl;
	if (sendSuccess) {
		std::cout << "Captured Broadcast NMEA: " << capturedNMEA;
		
		// Parse it back to simulate reception and verify parser changes
		vdesManager.Parse(capturedNMEA.c_str(), capturedNMEA.length());
	}

	// Verify database persistence and fragmentDesc
	auto dbNets = vdesManager.GetNetSounders(0, 100);
	bool found = false;
	for (const auto &n : dbNets) {
		if (n.MRN == 12345) {
			found = true;
			std::cout << "Persisted Net Sounder found in DB. MRN: " << n.MRN 
					  << ", Type: " << (int)n.type 
					  << ", Continuous: " << n.isContinous 
					  << ", IsOwnShip: " << n.isOwn 
					  << ", Nets count: " << n.nets.size() 
					  << ", Desc: " << n.description << std::endl;
			for (size_t i = 0; i < n.nets.size(); ++i) {
				std::cout << "    Net " << i << ": MRN=" << n.nets[i].MRN 
						  << ", FragmentDesc=" << (int)n.nets[i].fragmentDesc 
						  << ", Lat=" << n.nets[i].latitude 
						  << ", Lon=" << n.nets[i].longitude << std::endl;
			}
		}
	}
	if (!found) {
		std::cout << "ERROR: Persisted Net Sounder NOT found in database!" << std::endl;
	}

	// Now simulate receiving description supplement fragment (DAC 413, FI 8)
	std::cout << "\n--- Simulating reception of Supplementary Description (FI 8) for MRN = 12345 ---" << std::endl;
	auto suppVDMs = GenerateDAC_413_FI_8(12345, 412, 45, "Updated description from supplementary fragment!");
	for (const auto &vdm : suppVDMs)
	{
		std::cout << "Injecting FI 8 NMEA: " << vdm;
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Re-query database to verify update
	std::cout << "Re-querying database to check for updated description..." << std::endl;
	dbNets = vdesManager.GetNetSounders(0, 100);
	found = false;
	for (const auto &n : dbNets) {
		if (n.MRN == 12345) {
			found = true;
			std::cout << "Persisted Net Sounder after update: MRN: " << n.MRN 
					  << ", Desc: \"" << n.description << "\"" << std::endl;
		}
	}
	std::cout << "==========================================================\n" << std::endl;

	// Verify Tide Forecast deletion, BBox retrieval, and picking APIs
	std::cout << "\n=== Verification: Tide Forecast BBox Retrieval, Picking, and Deletion ===" << std::endl;
	
	// Query current Tide Forecasts in DB to get an ID for testing
	auto currentTideForecasts = vdesManager.GetTideForecasts(0, 10);
	std::cout << "Initial Tide Forecasts count in DB: " << currentTideForecasts.size() << std::endl;
	
	if (!currentTideForecasts.empty()) {
		// Test BBox retrieval
		// In previous test run, we had mock tide stations around Xiamen (Lat: 24.35, Lon: 119.1) and (Lat: 24.6833, Lon: 119.6)
		// We'll build a BoundingBox targeting this region
		VDES::BoundingBox::Rect rect;
		rect.left = 119.0;
		rect.right = 119.7;
		rect.bottom = 24.3;
		rect.top = 24.7;
		auto searchBBox = VDES::BoundingBox::Build(rect);
		
		auto bboxStations = vdesManager.GetTideStations(searchBBox);
		std::cout << "Tide Stations found in BBox [119.0, 119.7, 24.3, 24.7]: " << bboxStations.size() << std::endl;
		for (size_t i = 0; i < bboxStations.size(); ++i) {
			std::cout << "    BBox Station " << i << ": Lat=" << bboxStations[i].coordinate.GetLatitude() 
					  << ", Lon=" << bboxStations[i].coordinate.GetLongitude() 
					  << ", TidalDatum=" << bboxStations[i].tidalDatum 
					  << ", HighTide=" << bboxStations[i].tideHigh << std::endl;
		}
		
		// Test Picking API: Pick station nearest to (Lat: 24.35, Lon: 119.1) within 10 NM
		auto pickedStation = vdesManager.GetTideStation(24.35, 119.1, 10.0);
		if (pickedStation) {
			std::cout << "Tide Station successfully picked near (24.35, 119.1) within 10 NM: Lat=" 
					  << pickedStation->coordinate.GetLatitude() << ", Lon=" << pickedStation->coordinate.GetLongitude() 
					  << ", HighTide=" << pickedStation->tideHigh << std::endl;
		} else {
			std::cout << "WARNING: No Tide Station picked near (24.35, 119.1) within 10 NM" << std::endl;
		}

		// Test Deletion API (Delete by list of IDs)
		std::vector<uint32_t> idsToDelete;
		for (const auto &forecast : currentTideForecasts) {
			idsToDelete.push_back(forecast.dataID);
		}
		
		std::cout << "Deleting Tide Forecasts with IDs: ";
		for (auto id : idsToDelete) std::cout << id << " ";
		std::cout << std::endl;
		
		bool deleteSuccess = vdesManager.DeleteTideForecasts(idsToDelete);
		std::cout << "DeleteTideForecasts result: " << (deleteSuccess ? "SUCCESS" : "FAILED") << std::endl;
		
		auto postDeleteTideForecasts = vdesManager.GetTideForecasts(0, 10);
		std::cout << "Post-deletion Tide Forecasts count in DB: " << postDeleteTideForecasts.size() << std::endl;
		
		auto postDeleteStations = vdesManager.GetTideStations(searchBBox);
		std::cout << "Post-deletion Tide Stations in BBox: " << postDeleteStations.size() << std::endl;
	} else {
		std::cout << "WARNING: No initial Tide Forecasts in DB to perform verification!" << std::endl;
	}
	std::cout << "=========================================================================\n" << std::endl;

	// Verify MSIObstacle duration update
	std::cout << "\n=== Verification: Querying MSIObstacles (FI=35) from DB ===" << std::endl;
	auto msiObstacles = vdesManager.GetObstacles(0, 10);
	std::cout << "MSI Obstacles count: " << msiObstacles.size() << std::endl;
	for (const auto &obs : msiObstacles)
	{
		std::cout << "  Obstacle ID: " << obs.dataID 
				  << ", Type: " << (int)obs.type 
				  << ", Lat: " << obs.coordinate.GetLatitude() 
				  << ", Lon: " << obs.coordinate.GetLongitude() 
				  << ", Start: " << obs.timestampStart 
				  << ", End: " << obs.timestampEnd 
				  << ", Duration: " << (int)obs.duration << std::endl;
	}
	std::cout << "===========================================================\n" << std::endl;

	// Verify Automatic Expiration and Cleanup
	std::cout << "\n=== Verification: Automatic Expiration and Cleanup ===" << std::endl;
	try {
		VDES::SQLite::Database db(VDES::ConfigureManager::GetInstance().GetStoragePath() + "/VDES.db", VDES::SQLite::OPEN_READWRITE);
		// Clean up any existing ID 9999 first
		db.exec("DELETE FROM MSIObstacle WHERE ID = 9999");
		db.exec("DELETE FROM MSIObstacleBBox WHERE ID = 9999 OR ID = -9999");

		{
			// Insert manually
			db.exec("INSERT INTO MSIObstacle (ID, Type, Latitude, Longitude, Range, [Timestamp Start], [Timestamp End], Comment, Certified, [Timestamp Receive], Read, GeometryType, SectorStartAngle, SectorEndAngle, Polygon, Duration) "
					"VALUES (9999, 2, 24.5, 118.5, 5, 1000, 2000, 0, 1, 1000, 0, 0, 0, 0, NULL, 1)");
			db.exec("INSERT INTO MSIObstacleBBox VALUES (9999, 118.4, 118.6, 24.4, 24.6)");
			std::cout << "Manually inserted mock expired obstacle ID 9999 (Duration=1, End=2000)." << std::endl;

			// Verify it is inside the database before clean
			VDES::SQLite::Statement queryCheck(db, "SELECT COUNT(*) FROM MSIObstacle WHERE ID = 9999");
			if (queryCheck.executeStep()) {
				std::cout << "Mock obstacle count in DB before API call: " << queryCheck.getColumn(0).getInt() << std::endl;
			}
		}

		// Trigger the cleanup via GetObstacles API call
		std::cout << "Calling GetObstacles() to trigger automatic cleanup..." << std::endl;
		auto listAfterCleanup = vdesManager.GetObstacles(0, 10);

		{
			// Verify it is no longer inside the database after clean
			VDES::SQLite::Statement queryCheckAfter(db, "SELECT COUNT(*) FROM MSIObstacle WHERE ID = 9999");
			if (queryCheckAfter.executeStep()) {
				int count = queryCheckAfter.getColumn(0).getInt();
				std::cout << "Mock obstacle count in DB after API call: " << count << std::endl;
				if (count == 0) {
					std::cout << "SUCCESS: Expired obstacle ID 9999 was automatically cleaned up from database!" << std::endl;
				} else {
					std::cout << "ERROR: Expired obstacle ID 9999 was NOT cleaned up!" << std::endl;
				}
			}
		}

		{
			// Also verify BBox is cleaned up
			VDES::SQLite::Statement queryBBoxCheck(db, "SELECT COUNT(*) FROM MSIObstacleBBox WHERE ID = 9999");
			if (queryBBoxCheck.executeStep()) {
				std::cout << "Mock BBox count in DB after API call: " << queryBBoxCheck.getColumn(0).getInt() << std::endl;
			}
		}

	} catch (const std::exception &e) {
		std::cout << "WARNING: Failed to run automatic expiration cleanup test: " << e.what() << std::endl;
	}
	std::cout << "=======================================================\n" << std::endl;

	// Verify Bridge (FI=41) updates
	std::cout << "\n=== Verification: Querying Bridges (FI=41) from DB ===" << std::endl;
	auto bridges = vdesManager.GetBridges(0, 10);
	std::cout << "Bridges count: " << bridges.size() << std::endl;
	for (const auto &bridge : bridges)
	{
		std::cout << "  Bridge MRN: " << bridge.MRN 
				  << ", FlowVelocity: " << bridge.flowVelocity 
				  << ", FlowDirection: " << bridge.flowDirection 
				  << ", Timestamp (PubTime): " << bridge.timestamp 
				  << ", Spans Count: " << bridge.spans.size() << std::endl;
		for (size_t i = 0; i < bridge.spans.size(); ++i)
		{
			const auto &span = bridge.spans[i];
			std::cout << "    Span " << i << ": Lat=" << span.center.GetLatitude()
					  << ", Lon=" << span.center.GetLongitude()
					  << ", Height=" << span.height
					  << ", Width=" << span.width
					  << ", DirectionToPass=" << span.directionToPass
					  << ", PassAbility=" << (int)span.passAbility
					  << ", EnableMeeting=" << span.enableMeeting
					  << ", EnableOvertaking=" << span.enableOvertaking << std::endl;
		}
	}
	std::cout << "======================================================" << std::endl;

	// Verify non-AIS & AIS AtoN Dynamics Deletion APIs
	std::cout << "\n=== Verification: AtoNDynamics (FI=33) and AISAtoNDynamics (FI=34) Deletion ===" << std::endl;
	
	// Parse some mock AtoN Dynamics messages to populate the database
	std::cout << "Parsing mock AtoN Dynamics (FI=33) messages..." << std::endl;
	for (uint8_t status : {1, 5, 8, 9, 10, 14, 15})
	{
		auto sentences = GenerateDAC_412_FI_33(status);
		for (const auto &vdm : sentences)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Now simulate receiving description supplement fragment (DAC 413, FI 8) for AtoN Dynamics (MRN = 67890)
	std::cout << "\n--- Simulating reception of Supplementary Description (FI 8) for AtoN MRN = 67890 ---" << std::endl;
	auto atonSuppVDMs = GenerateDAC_413_FI_8(67890, 412, 33, "Updated AtoN description from supplementary fragment!");
	for (const auto &vdm : atonSuppVDMs)
	{
		std::cout << "Injecting FI 8 NMEA for AtoN: " << vdm;
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Re-query database to verify update
	std::cout << "Re-querying database to check for updated AtoN description..." << std::endl;
	auto dbAtons = vdesManager.GetAtoNDynamics(0, 100);
	bool atonFound = false;
	for (const auto &n : dbAtons) {
		for (const auto &elem : n.elements) {
			if (elem.MRN == 67890) {
				atonFound = true;
				std::cout << "Persisted AtoNDynamics element after update: Element MRN: " << elem.MRN 
						  << ", FragmentDesc: " << (int)elem.fragmentDesc
						  << ", Desc: \"" << elem.description << "\""
						  << ", Lat: " << elem.coordinate.GetLatitude() << ", Lon: " << elem.coordinate.GetLongitude()
						  << ", PrevLat: " << elem.prevCoordinate.GetLatitude() << ", PrevLon: " << elem.prevCoordinate.GetLongitude()
						  << ", RhythmName: " << (int)elem.rhythmNameCode
						  << ", RhythmParam: " << (int)elem.rhythmParamCode
						  << ", LightColor: " << (int)elem.lightColor
						  << ", LightPeriod: " << (int)elem.lightPeriod
						  << ", Range: " << (int)elem.range
						  << ", MorseCode: " << (int)elem.morseCode << std::endl;
			}
		}
	}
	if (!atonFound) {
		std::cout << "ERROR: Persisted AtoNDynamics Element NOT found after update!" << std::endl;
	}
	std::cout << "==========================================================\n" << std::endl;

	std::cout << "Parsing mock AIS AtoN Dynamics (FI=34) messages..." << std::endl;
	for (uint8_t status : {1, 3, 5})
	{
		auto sentences = GenerateDAC_412_FI_34(status);
		for (const auto &vdm : sentences)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Query initial counts
	auto atons = vdesManager.GetAtoNDynamics(0, 100);
	std::cout << "Initial AtoN Dynamics count in DB: " << atons.size() << std::endl;
	if (!atons.empty())
	{
		std::vector<uint32_t> idsToDelete;
		for (const auto &item : atons)
		{
			idsToDelete.push_back(item.dataID);
			std::cout << "  AtoNDynamic Record ID to delete: " << item.dataID << " (MRN=" << item.MRN << ")" << std::endl;
		}

		bool deleteSuccess = vdesManager.DeleteAtoNDynamics(idsToDelete);
		std::cout << "DeleteAtoNDynamics result: " << (deleteSuccess ? "SUCCESS" : "FAILED") << std::endl;

		auto postDeleteAtons = vdesManager.GetAtoNDynamics(0, 100);
		std::cout << "Post-deletion AtoN Dynamics count in DB: " << postDeleteAtons.size() << std::endl;
	}

	auto aisAtons = vdesManager.GetAISAtoNDynamics(0, 100);
	std::cout << "Initial AIS AtoN Dynamics count in DB: " << aisAtons.size() << std::endl;
	if (!aisAtons.empty())
	{
		std::vector<uint32_t> idsToDelete;
		for (const auto &item : aisAtons)
		{
			idsToDelete.push_back(item.dataID);
			std::cout << "  AISAtoNDynamic Record ID to delete: " << item.dataID << " (MRN=" << item.MRN << ")" << std::endl;
		}

		bool deleteSuccess = vdesManager.DeleteAISAtoNDynamics(idsToDelete);
		std::cout << "DeleteAISAtoNDynamics result: " << (deleteSuccess ? "SUCCESS" : "FAILED") << std::endl;

		auto postDeleteAisAtons = vdesManager.GetAISAtoNDynamics(0, 100);
		std::cout << "Post-deletion AIS AtoN Dynamics count in DB: " << postDeleteAisAtons.size() << std::endl;
	}
	std::cout << "================================================================================" << std::endl;

	// Verify Marine Meteorology Forecast Sea Area (FI=27) Deletion APIs
	std::cout << "\n=== Verification: MarineMeteorologyFCSTAreas (FI=27) Deletion ===" << std::endl;
	
	std::cout << "Parsing mock Marine Meteorology Forecast (Sea Area, FI=27) messages..." << std::endl;
	for (uint8_t areaCode : {10, 20, 30})
	{
		auto sentences = GenerateDAC_412_FI_27_WithArea(areaCode);
		for (const auto &vdm : sentences)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Query initial counts
	auto fcstAreas = vdesManager.GetMarineMeteorologyFCSTAreas(0, 100);
	std::cout << "Initial MarineMeteorologyFCSTAreas count in DB: " << fcstAreas.size() << std::endl;
	for (const auto &item : fcstAreas)
	{
		std::cout << "  Area Code: " << (int)item.areaCode << ", Weather Code: " << (int)item.weatherCode << std::endl;
	}

	if (!fcstAreas.empty())
	{
		// 1. Test deletion by dataIDs (which are areaCodes for this type)
		std::vector<uint32_t> idsToDelete { 10, 20 };
		std::cout << "Deleting area codes 10 and 20..." << std::endl;
		bool deleteSuccess = vdesManager.DeleteMarineMeteorologyFCSTAreas(idsToDelete);
		std::cout << "Delete result: " << (deleteSuccess ? "SUCCESS" : "FAILED") << std::endl;

		auto postDelete = vdesManager.GetMarineMeteorologyFCSTAreas(0, 100);
		std::cout << "Post-deletion count in DB: " << postDelete.size() << std::endl;
		for (const auto &item : postDelete)
		{
			std::cout << "  Remaining Area Code: " << (int)item.areaCode << std::endl;
		}

		// 2. Test deletion by index/number
		std::cout << "Deleting remaining by index and count..." << std::endl;
		deleteSuccess = vdesManager.DeleteMarineMeteorologyFCSTAreas(0, 10);
		std::cout << "Delete result: " << (deleteSuccess ? "SUCCESS" : "FAILED") << std::endl;

		auto finalCheck = vdesManager.GetMarineMeteorologyFCSTAreas(0, 100);
		std::cout << "Final MarineMeteorologyFCSTAreas count in DB: " << finalCheck.size() << std::endl;
	}
	std::cout << "================================================================================" << std::endl;

	// Verify Marine Environment Forecast Sea Area (FI=29) Deletion APIs
	std::cout << "\n=== Verification: MarineEnvironmentFCSTAreas (FI=29) Deletion ===" << std::endl;
	
	std::cout << "Parsing mock Marine Environment Forecast (Sea Area, FI=29) messages..." << std::endl;
	for (uint8_t areaCode : {11, 22, 33})
	{
		auto sentences = GenerateDAC_412_FI_29_WithArea(areaCode);
		for (const auto &vdm : sentences)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Query initial counts
	auto envAreas = vdesManager.GetMarineEnvironmentFCSTAreas(0, 100);
	std::cout << "Initial MarineEnvironmentFCSTAreas count in DB: " << envAreas.size() << std::endl;
	for (const auto &item : envAreas)
	{
		std::cout << "  Area Code: " << (int)item.areaCode << ", Temperature Low: " << (int)item.temperatureLow << std::endl;
	}

	if (!envAreas.empty())
	{
		// 1. Test deletion by dataIDs (which are areaCodes for this type)
		std::vector<uint32_t> idsToDelete { 11, 22 };
		std::cout << "Deleting area codes 11 and 22..." << std::endl;
		bool deleteSuccess = vdesManager.DeleteMarineEnvironmentFCSTAreas(idsToDelete);
		std::cout << "Delete result: " << (deleteSuccess ? "SUCCESS" : "FAILED") << std::endl;

		auto postDelete = vdesManager.GetMarineEnvironmentFCSTAreas(0, 100);
		std::cout << "Post-deletion count in DB: " << postDelete.size() << std::endl;
		for (const auto &item : postDelete)
		{
			std::cout << "  Remaining Area Code: " << (int)item.areaCode << std::endl;
		}

		// 2. Test deletion by index/number
		std::cout << "Deleting remaining by index and count..." << std::endl;
		deleteSuccess = vdesManager.DeleteMarineEnvironmentFCSTAreas(0, 10);
		std::cout << "Delete result: " << (deleteSuccess ? "SUCCESS" : "FAILED") << std::endl;

		auto finalCheck = vdesManager.GetMarineEnvironmentFCSTAreas(0, 100);
		std::cout << "Final MarineEnvironmentFCSTAreas count in DB: " << finalCheck.size() << std::endl;
	}
	std::cout << "================================================================================" << std::endl;

	return 0;
}
