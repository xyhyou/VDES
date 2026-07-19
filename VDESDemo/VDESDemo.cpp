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
#include <iomanip>
#include <sstream>
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

static std::vector<std::string> GenerateDAC_412_FI_33_WithMRN(uint8_t status, uint32_t headerMRN, uint32_t elementMRN)
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

	// 状态
	bitsManager.Encode(status, 5);

	if (status == 1)
	{
		// 新设: Table 62 (112 bits)
		// MRN
		bitsManager.Encode(elementMRN, 17);
		// 片段描述 (1: 需要额外的文本补充片段)
		bitsManager.Encode(1, 2);
		// 航标类型
		bitsManager.Encode(5, 5);
		// 经度 (step 1/10000', 119.5432 degrees)
		bitsManager.Encode(static_cast<uint32_t>(119.5432 * 600000.0), 28);
		// 纬度 (24.3210 degrees)
		bitsManager.Encode(static_cast<uint32_t>(24.3210 * 600000.0), 27);
		// 航标助航状态
		bitsManager.Encode(1, 5);
		// 灯质名称代号
		bitsManager.Encode(0, 4);
		// 灯质参数代号
		bitsManager.Encode(0, 4);
		// 灯光颜色代号
		bitsManager.Encode(0, 3);
		// 闪光周期
		bitsManager.Encode(0, 6);
		// 射程
		bitsManager.Encode(0, 5);
		// 备用
		bitsManager.Encode(0, 6);
	}
	else
	{
		bitsManager.Encode(elementMRN, 17);
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

static std::vector<std::string> GenerateDAC_412_FI_48(void)
{
	VDES::AISBitsManager bitsManager;
	// Message ID
	bitsManager.Encode(8, 6);
	// Repeat indicator
	bitsManager.Encode(0, 2);
	// Source ID
	bitsManager.Encode(4123005, 30);
	// Spare
	bitsManager.Encode(0, 2);
	// DAC
	bitsManager.Encode(412, 10);
	// FI	
	bitsManager.Encode(48, 6);

	// 航线版本号 (Route version): 6 bits
	bitsManager.Encode(12, 6);

	// 起始时间 (Start time): 22 bits
	bitsManager.Encode(14, 5); // Day
	bitsManager.Encode(10, 5); // Hour
	bitsManager.Encode(30, 6); // Minute
	bitsManager.Encode(45, 6); // Second

	// Waypoint #1
	bitsManager.Encode(static_cast<int64_t>(120.1234 * 600000), 28);
	bitsManager.Encode(static_cast<int64_t>(24.5678 * 600000), 27);
	bitsManager.Encode(1, 1);  // minutes
	bitsManager.Encode(60, 12); // duration

	// Waypoint #2 (incremental)
	bitsManager.Encode(738, 13); // deltaLon = +738
	bitsManager.Encode(3760, 12); // deltaLat = -336 (12-bit 2's complement)
	bitsManager.Encode(0, 1);  // seconds
	bitsManager.Encode(45, 12); // duration

	// Waypoint #3 (incremental)
	bitsManager.Encode(7682, 13); // deltaLon = -510 (13-bit 2's complement)
	bitsManager.Encode(252, 12); // deltaLat = +252
	bitsManager.Encode(1, 1);  // minutes
	bitsManager.Encode(15, 12); // duration

	auto bitsNum = bitsManager.GetBitsNumberToDecode();
	auto spareBits = 8 - (bitsNum % 8);
	bitsManager.Encode(0, spareBits);
	auto vdms = bitsManager.BuildPacket();
	return vdms;
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
		uint32_t mrnBase = 200 + (warningType - 2) * 10;
		// General Warnings
		// Element 1
		bitsManager.Encode(mrnBase + 1, 17); // MRN
		bitsManager.Encode(1, 2); // Fragment Description
		bitsManager.Encode(5, 7); // Sea Area Code
		bitsManager.Encode(2, 2); // Warning Level
		// Element 2
		bitsManager.Encode(mrnBase + 2, 17); // MRN
		bitsManager.Encode(0, 2); // Fragment Description
		bitsManager.Encode(12, 7); // Sea Area Code
		bitsManager.Encode(3, 2); // Warning Level
	}
	else if (warningType == 5)
	{
		// Storm Surge
		// Element 1
		bitsManager.Encode(301, 17); // MRN
		bitsManager.Encode(0, 2); // Fragment Description
		bitsManager.Encode(15, 6); // City Code
		bitsManager.Encode(25, 5); // Surge Height (raw 25 * 0.1 = 2.5m)
		bitsManager.Encode(1, 2); // Warning Level
		// Element 2
		bitsManager.Encode(302, 17); // MRN
		bitsManager.Encode(1, 2); // Fragment Description
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

	// MRN (20 bits)
	bitsManager.Encode(mrn, 20);
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

static std::vector<std::string> GenerateDAC_413_FI_7(uint32_t mrn, uint16_t mainDAC, uint8_t mainFI, const std::vector<VDES::Coordinate> &deltas)
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
	// FI (6 bits) = 7
	bitsManager.Encode(7, 6);

	// MRN (20 bits)
	bitsManager.Encode(mrn, 20);
	// mainDAC (10 bits)
	bitsManager.Encode(mainDAC, 10);
	// mainFI (6 bits)
	bitsManager.Encode(mainFI, 6);

	if (mainDAC == 412 && (mainFI == 42 || mainFI == 43 || mainFI == 44))
	{
		for (const auto &delta : deltas)
		{
			int32_t lonInc = static_cast<int32_t>(round(delta.GetLongitude() * 600000.0));
			int32_t latInc = static_cast<int32_t>(round(delta.GetLatitude() * 600000.0));

			uint32_t compLon = VDES::UtilityInterface::ConvertIntegerToComplementCode(lonInc, 24);
			uint32_t compLat = VDES::UtilityInterface::ConvertIntegerToComplementCode(latInc, 23);

			bitsManager.Encode(compLon, 24);
			bitsManager.Encode(compLat, 23);
		}
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

static void EncodeTime16(VDES::AISBitsManager &bitsManager, uint64_t timestamp)
{
	tm timeUTC = { 0 };
	timeUTC = *gmtime((time_t *)&timestamp);
	bitsManager.Encode(timeUTC.tm_mday, 5);
	bitsManager.Encode(timeUTC.tm_hour, 5);
	bitsManager.Encode(timeUTC.tm_min, 6);
}

static std::vector<std::string> GenerateDAC_413_FI_7_Cyclone(uint32_t mrn, const std::vector<VDES::MewTropicalCyclone::PathPoint> &points)
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
	// FI (6 bits) = 7
	bitsManager.Encode(7, 6);

	// MRN (20 bits)
	bitsManager.Encode(mrn, 20);
	// mainDAC (10 bits) = 412
	bitsManager.Encode(412, 10);
	// mainFI (6 bits) = 31
	bitsManager.Encode(31, 6);

	for (const auto &pt : points)
	{
		EncodeTime16(bitsManager, pt.timestamp);

		int32_t lonInt = static_cast<int32_t>(round(pt.centerLongitude * 6000.0));
		int32_t latInt = static_cast<int32_t>(round(pt.centerLatitude * 6000.0));
		uint32_t compLon = VDES::UtilityInterface::ConvertIntegerToComplementCode(lonInt, 22);
		uint32_t compLat = VDES::UtilityInterface::ConvertIntegerToComplementCode(latInt, 21);

		bitsManager.Encode(compLon, 22);
		bitsManager.Encode(compLat, 21);
		bitsManager.Encode(pt.cycloneType, 3);
		bitsManager.Encode(pt.radiusWindScale7, 10);
		bitsManager.Encode(pt.radiusWindScale10, 8);
		bitsManager.Encode(pt.radiusWindScale12, 7);
		bitsManager.Encode(pt.moveSpeed, 6);
		bitsManager.Encode(pt.moveDirection, 9);
		bitsManager.Encode(pt.maxWindScale, 5);
		bitsManager.Encode(pt.centerPressure, 9);
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

struct MockGeneralWarningElement
{
	uint32_t MRN = 0;
	uint8_t  fragment = 0;
	uint8_t  seaAreaCode = 0;
	uint8_t  warningLevel = 0;
};

static std::vector<std::string> GenerateDAC_413_FI_7_Gale(uint32_t mrn, const std::vector<MockGeneralWarningElement> &warnings)
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
	// FI (6 bits) = 7
	bitsManager.Encode(7, 6);

	// MRN (20 bits)
	bitsManager.Encode(mrn, 20);
	// mainDAC (10 bits) = 412
	bitsManager.Encode(412, 10);
	// mainFI (6 bits) = 31
	bitsManager.Encode(31, 6);

	for (const auto &w : warnings)
	{
		bitsManager.Encode(w.MRN, 17);
		bitsManager.Encode(w.fragment, 2);
		bitsManager.Encode(w.seaAreaCode, 7);
		bitsManager.Encode(w.warningLevel, 2);
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

struct MockStormSurgeElement
{
	uint32_t MRN = 0;
	uint8_t  fragment = 0;
	uint8_t  cityCode = 0;
	uint32_t surgeHeight = 0;
	uint8_t  warningLevel = 0;
};

static std::vector<std::string> GenerateDAC_413_FI_7_Surge(uint32_t mrn, const std::vector<MockStormSurgeElement> &warnings)
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
	// FI (6 bits) = 7
	bitsManager.Encode(7, 6);

	// MRN (20 bits)
	bitsManager.Encode(mrn, 20);
	// mainDAC (10 bits) = 412
	bitsManager.Encode(412, 10);
	// mainFI (6 bits) = 31
	bitsManager.Encode(31, 6);

	for (const auto &w : warnings)
	{
		bitsManager.Encode(w.MRN, 17);
		bitsManager.Encode(w.fragment, 2);
		bitsManager.Encode(w.cityCode, 6);
		bitsManager.Encode(w.surgeHeight, 5);
		bitsManager.Encode(w.warningLevel, 2);
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

static std::vector<std::string> GenerateDAC_412_FI_50(uint32_t destMmsi, uint32_t srcMmsi)
{
	VDES::AISBitsManager bitsManager;

	// Message ID (6 bits) = 6 (Addressed ASM)
	bitsManager.Encode(6, 6);
	// Repeat indicator (2 bits) = 0
	bitsManager.Encode(0, 2);
	// Source ID (30 bits) = srcMmsi
	bitsManager.Encode(srcMmsi, 30);
	// Sequence number (2 bits) = 0
	bitsManager.Encode(0, 2);
	// Destination ID (30 bits) = destMmsi
	bitsManager.Encode(destMmsi, 30);
	// Retransmit flag (1 bit) = 0
	bitsManager.Encode(0, 1);
	// Spare (1 bit) = 0
	bitsManager.Encode(0, 1);

	// DAC (10 bits) = 412
	bitsManager.Encode(412, 10);
	// FI (6 bits) = 50
	bitsManager.Encode(50, 6);

	// responseInfo (14 bits) = 0x3F (all 6 parameters enabled)
	bitsManager.Encode(0x3F, 14);

	// forecastTime (20 bits): month=7, day=13, hour=12, min=0
	bitsManager.Encode(7, 4);
	bitsManager.Encode(13, 5);
	bitsManager.Encode(12, 5);
	bitsManager.Encode(0, 6);

	// Point 1 (for Coordinate 1)
	bitsManager.Encode(10, 6);   // windSpeed
	bitsManager.Encode(180, 9);  // windDirection
	bitsManager.Encode(100, 8);  // visibility
	bitsManager.Encode(20, 8);   // waveHeight
	bitsManager.Encode(190, 9);  // waveDirection
	bitsManager.Encode(15, 8);   // swellHeight

	// Point 2 (for Coordinate 2)
	bitsManager.Encode(12, 6);   // windSpeed
	bitsManager.Encode(200, 9);  // windDirection
	bitsManager.Encode(120, 8);  // visibility
	bitsManager.Encode(25, 8);   // waveHeight
	bitsManager.Encode(210, 9);  // waveDirection
	bitsManager.Encode(18, 8);   // swellHeight

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

static std::vector<std::string> GenerateDAC_412_FI_47(uint32_t destMmsi, uint32_t srcMmsi)
{
	VDES::AISBitsManager bitsManager;

	// Message ID (6 bits) = 6 (Addressed ASM)
	bitsManager.Encode(6, 6);
	// Repeat indicator (2 bits) = 0
	bitsManager.Encode(0, 2);
	// Source ID (30 bits) = srcMmsi
	bitsManager.Encode(srcMmsi, 30);
	// Sequence number (2 bits) = 0
	bitsManager.Encode(0, 2);
	// Destination ID (30 bits) = destMmsi
	bitsManager.Encode(destMmsi, 30);
	// Retransmit flag (1 bit) = 0
	bitsManager.Encode(0, 1);
	// Spare (1 bit) = 0
	bitsManager.Encode(0, 1);

	// DAC (10 bits) = 412
	bitsManager.Encode(412, 10);
	// FI (6 bits) = 47
	bitsManager.Encode(47, 6);

	// rawTime (6 bits) = 63 (meaning 31.5h)
	bitsManager.Encode(63, 6);

	// Coordinates (28 bits lon, 27 bits lat)
	// Point 1: 38.5, 118.2
	int32_t lon1 = static_cast<int32_t>(::round(118.2 * 600000.0));
	int32_t lat1 = static_cast<int32_t>(::round(38.5 * 600000.0));
	bitsManager.Encode(VDES::UtilityInterface::ConvertIntegerToComplementCode(lon1, 28), 28);
	bitsManager.Encode(VDES::UtilityInterface::ConvertIntegerToComplementCode(lat1, 27), 27);

	// Point 2: 39.1, 119.5 (expressed as offset from Point 1)
	// Offset: 119.5 - 118.2 = 1.3, 39.1 - 38.5 = 0.6
	int32_t lonOffset = static_cast<int32_t>(::round(1.3 * 600000.0));
	int32_t latOffset = static_cast<int32_t>(::round(0.6 * 600000.0));
	bitsManager.Encode(VDES::UtilityInterface::ConvertIntegerToComplementCode(lonOffset, 24), 24);
	bitsManager.Encode(VDES::UtilityInterface::ConvertIntegerToComplementCode(latOffset, 23), 23);

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


struct MockRevocationElement
{
	uint16_t dac;
	uint8_t  fi;
	uint32_t mrn;
};

static std::vector<std::string> GenerateDAC_413_FI_9(const std::vector<MockRevocationElement> &elements)
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
	// FI (6 bits) = 9
	bitsManager.Encode(9, 6);

	for (const auto &elem : elements)
	{
		bitsManager.Encode(elem.dac, 10);
		bitsManager.Encode(elem.fi, 6);
		bitsManager.Encode(elem.mrn, 20);
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

static std::vector<std::string> GenerateDAC_413_FI_10(uint16_t targetDAC, uint8_t targetFI, uint32_t startMRN, uint32_t endMRN)
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
	// FI (6 bits) = 10
	bitsManager.Encode(10, 6);

	bitsManager.Encode(targetDAC, 10);
	bitsManager.Encode(targetFI, 6);
	bitsManager.Encode(startMRN, 20);
	bitsManager.Encode(endMRN, 20);
	bitsManager.Encode(0, 24); // Spare 24 bits

	auto vdms = bitsManager.BuildPacket();
	std::vector<std::string> results;
	for (auto &vdm : vdms)
	{
		results.push_back(vdm + "\r\n");
	}
	return results;
}

static void NotifyHandle(const VDES::VDESManager::EventType eventType, const int retCode)
{
	auto &vdesManager = VDES::VDESManager::GetInstance();
	
	if (eventType == VDES::VDESManager::EventType::ASM_ATON_DYNAMICS)
	{
		auto dynamics = vdesManager.GetAtoNDynamics(0, 100);
		int a;
		a = 10;
	}
	else if (eventType == VDES::VDESManager::EventType::ASM_CHANNEL_CENTERLINE)
	{
		auto infos = vdesManager.GetChannelCenterlines(0, 100);
		int a;
		a = 10;
	}
	else if (eventType == VDES::VDESManager::EventType::ASM_CHANNEL_BOUNDARY)
	{
		auto infos = vdesManager.GetChannelBoundaries(0, 100);
		int a;
		a = 10;
	}
	else if (eventType == VDES::VDESManager::EventType::MSI_MILITARY_ACTIVITY)
	{
		auto infos = vdesManager.GetMilitaryActivitys(0, 100);
		int a;
		a = 10;
	}
}

static void TestABBTextMessages()
{
	std::cout << "\n=== Testing ABB Text Messages (FI=3 and FI=4) ===" << std::endl;
	auto &vdesManager = VDES::VDESManager::GetInstance();

	// Clear inbox first
	auto originalMsgs = vdesManager.GetMessages(VDES::VDESManager::MailBox::INBOX, 0, 100);
	std::cout << "Original Inbox messages count: " << originalMsgs.size() << std::endl;

	// 1. Encode a test text using 14-bit Chinese (FI=3)
	{
		std::string gbkText = VDES::UtilityInterface::UTF8ToGBK("测试14位中英文123A");

		VDES::AISBitsManager encoder;
		encoder.Encode(413, 10);
		encoder.Encode(3, 6);
		encoder.Encode(gbkText, 413, 3);
		auto payload = encoder.GetEncodedVDMPayload();
		auto fillBits = encoder.GetFillBitsNumberToEncode();

		std::ostringstream sentenceStream;
		sentenceStream << "!AIABB,01,01,1.0,412123456,1,03,1," << payload << "," << fillBits;
		
		std::string sentence = sentenceStream.str();
		auto xorVal = VDES::UtilityInterface::GetXOR(sentence);
		sentenceStream << "*" << std::hex << std::setw(2) << std::uppercase << std::setfill('0')
					   << static_cast<int>(xorVal) << "\r\n";

		std::cout << "Feeding ABB FI=3 sentence: " << sentenceStream.str();
		vdesManager.Parse(sentenceStream.str().c_str(), sentenceStream.str().length());
	}

	// 2. Encode a test text using 13-bit Chinese (FI=4)
	{
		std::string gbkText = VDES::UtilityInterface::UTF8ToGBK("测试13位中英文567B");

		VDES::AISBitsManager encoder;
		encoder.Encode(413, 10);
		encoder.Encode(4, 6);
		encoder.Encode(gbkText, 413, 4);
		auto payload = encoder.GetEncodedVDMPayload();
		auto fillBits = encoder.GetFillBitsNumberToEncode();

		std::ostringstream sentenceStream;
		sentenceStream << "!AIABB,01,01,1.0,412789123,1,04,1," << payload << "," << fillBits;
		
		std::string sentence = sentenceStream.str();
		auto xorVal = VDES::UtilityInterface::GetXOR(sentence);
		sentenceStream << "*" << std::hex << std::setw(2) << std::uppercase << std::setfill('0')
					   << static_cast<int>(xorVal) << "\r\n";

		std::cout << "Feeding ABB FI=4 sentence: " << sentenceStream.str();
		vdesManager.Parse(sentenceStream.str().c_str(), sentenceStream.str().length());
	}

	// Retrieve from database and print
	auto currentMsgs = vdesManager.GetMessages(VDES::VDESManager::MailBox::INBOX, 0, 100);
	std::cout << "Inbox messages count after test: " << currentMsgs.size() << std::endl;
	for (const auto &msg : currentMsgs)
	{
		if (msg.mmsiSource == 412123456 || msg.mmsiSource == 412789123)
		{
			std::cout << "  Retrieved Message from DB: Source MMSI = " << msg.mmsiSource 
					  << ", Dest MMSI = " << msg.mmsiDestination
					  << ", Content = \"" << msg.content << "\"" 
					  << ", Type = " << (msg.messageType == VDES::MessageType::VDES ? "VDES" : "AIS") << std::endl;
		}
	}
	std::cout << "=================================================" << std::endl;
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
	VDES::ConfigureManager::GetInstance().SetBaseStationMMSI(4129999);
	vdesManager.Initialize();
	//vdesManager.EmptyDatabase();
	vdesManager.notifyEvent.append(NotifyHandle);
#if 0 
	VDES::HydrometeorologyRequest hydroRequest;
	hydroRequest.coordinates.push_back(VDES::Coordinate(38.720, 119.02));
	hydroRequest.coordinates.push_back(VDES::Coordinate(38.65, 120.10));
	hydroRequest.coordinates.push_back(VDES::Coordinate(38.525, 120.20));

	hydroRequest.requestTime = VDES::UtilityInterface::GetCurrentTimeStamp();
	hydroRequest.visibility = true;
	hydroRequest.waveHeight = true;
	hydroRequest.waveDirection = true;
	vdesManager.SendHydrometeorologyRequest(hydroRequest);
#endif
#if 0
	VDES::RouteRecommendationRequest request;
	request.grossTonnage = 10000;
	request.maxStaticDraft = 23.0;
	request.cargoType = 2;
	request.month = 7;
	request.day = 12;
	request.hour = 2;
	request.minute = 0;
	request.startCoordinate.SetLatitude(35.912496);
	request.startCoordinate.SetLongitude(122.913825);
	request.destCoordinate.SetLatitude(27.2832616666666667);
	request.destCoordinate.SetLongitude(124.33780);
	auto ret = vdesManager.SendRouteRecommendationRequest(request);
#endif

	auto info = vdesManager.GetChannelCenterlines(0, 100);
	int b;
	b = 100;
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
				  << ", Fragment: " << (int)ew.fragment
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
				  << ", Fragment: " << (int)ew.fragment
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
		// FI = 26
		"!AIABB,02,01,0,,1,,0,Ii`p41gO8jhUn7vjiL0202GHOs;5h0@09MQwddG00P0Un7vjiL020,0*12\r\n",
		"!AIABB,02,02,0,,1,,0,2GHOs;5j0,4*35\r\n",
		// 13bit中文短信
		//"!AIABB,12,01,2,412999999,1,04,1,IlCDaDHVEk:roWPjt?vr`@LqrvTO?DaDHVEvnuBUAPdD9sOtIDdSHMTeVS@I,0*5B\r\n",
		//"!AIABB,12,02,2,412999999,1,04,1,aVldS6EvdtUkjHvm7SUQN7ktM;<Ge02WebDb<5RQ?uA=BuDp:HviH@6@0i31,0*46\r\n",
		//"!AIABB,12,03,2,412999999,1,04,1,kvawF@KrJWebAwCDueGQrJVOSaIRu`0DueBUAPdD9wb9bGbW1C7n;20j068H,0*19\r\n",
		//"!AIABB,12,04,2,412999999,1,04,1,e?m?rj>G<c`tFMqUNOSaIRu`0Duc>tjddD9wb9bGbW1C7n;20j068Hd?m?eg,0*65\r\n",
		//"!AIABB,12,05,2,412999999,1,04,1,B7l:S2@k>fFbrMG:jQRB0eM4Sv3=<no@wb97uC5RiLBG>v?rW45vdQ;L2uA>,0*61\r\n",
		//"!AIABB,12,06,2,412999999,1,04,1,8;MabDb<5RQ>3=<nuehO9UNJ7OQRAt=LnIq?rk5smjT6C:t@C9ag1k>lN6Vt,0*02\r\n",
		//"!AIABB,12,07,2,412999999,1,04,1,7<sflqdkjNnV>O7v5:0OKOTfNC7ng9AQ8IVr3hm?9lAs3V;:`1vdpVm8IG<1,0*40\r\n",
		//"!AIABB,12,08,2,412999999,1,04,1,;6o2vWTknHTnR?no64QVG18J70faRF:4t27Vr5umjt6B?;PTPrwKL4wFHCbE,0*13\r\n",
		//"!AIABB,12,09,2,412999999,1,04,1,W0f335@;20j068Heb?6r;W0F:4qbGbW1C7nI==p>InjEhHt4qi?KOC3`E:5d,0*5F\r\n",
		//"!AIABB,12,10,2,412999999,1,04,1,prU<=WebtaC3Tf@ms;E5kF3:Db=DNLGMMSuIRudPDO=Elrdd8380HQRn`0Du,0*7A\r\n",
		//"!AIABB,12,11,2,412999999,1,04,1,eBUAPdD9lG8HDLcDaDHWGqGM3MDki9gTT>l6UKQdeI<Uc9boH=V;5QsKtbod,0*3D\r\n",
		//"!AIABB,12,12,2,412999999,1,04,1,aq>w20USKueP,5*07\r\n",
		// 14 bit中文编码
		//"!AIABB,12,01,1,412999999,1,03,1,Il>IdsTdAg4Kw6bkR9?VVeaJVeP=:anIdsTdAf:3VK>q8dD9kodOiN;CUDSs,0*35\r\n",
		//"!AIABB,12,02,1,412999999,1,03,1,S;AA6<=N;Aw4lIFsrCeH1i8DbTBO;GDvDOGc:MR4kIo95RQ>J8jQ:wN<mc02,0*1C\r\n",
		//"!AIABB,12,03,1,412999999,1,03,1,hP<P1R63VJ9VS8a7b>RPc8gb>RP`Twb>UcbO:?cmU>i2IdsTRi@W=4I@UOg6,0*42\r\n",
		//"!AIABB,12,04,1,412999999,1,03,1,JmP1H@6@0i35a=4kAU<WR=tB46kR=jmm?U7mrjWHQ1dpSIH`CVR<`BgoS=Jh,0*15\r\n",
		//"!AIABB,12,05,1,412999999,1,03,1,0d8380HQRhVRH`>Gqal:8jB@bFabS>Mg098Rd<Vpc8f:8iQauvJ8dRw8mRiL,0*68\r\n",
		//"!AIABB,12,06,1,412999999,1,03,1,FQ5Go=4in4lI6<D=Lk37HCvDkIo95RQ>:8iQ``>:Ni6qCFQ@SgrlJUulWI`j,0*27\r\n",
		//"!AIABB,12,07,1,412999999,1,03,1,?cbFbiK4KSd6CBPQU<N91a@@jV<wUBvrCeEu8Kj9pD`1RProOBMc08UbR<TT,0*06\r\n",
		//"!AIABB,12,08,1,412999999,1,03,1,9ure6lWIMVLbHdfibJ<`kvqft7PTa2fB2lWM@nJ<UnH`>R<TT8JbBD`Jw8ld,0*38\r\n",
		//"!AIABB,12,09,1,412999999,1,03,1,D9iOEMq91E;EIiI4@mEHi5chmDlI6eLhU3G51kRiH@6@0i35ei8Ct5=<F:4t,0*45\r\n",
		//"!AIABB,12,10,1,412999999,1,03,1,`BgoS=Jh1Tl`8IC62d`N:kFEHRPrOJW@eTH=>IdSN8`:9LqVs@RBKPg=q:Js,0*33\r\n",
		//"!AIABB,12,11,1,412999999,1,03,1,fPfqCRASrOqJJ<Srp:;RDVb<qld8380HQRnmU>i2IdsTRi@Wg9B@m<U<nMjF,0*00\r\n",
		//"!AIABB,12,12,1,412999999,1,03,1,T79Wjr?`AF`D8UL?e8:o3IJjI;FCEfhK<F;2:3c`pWcBM`D89:@cRPh,4*62\r\n",
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
		//"$AIASM,1783406998,1,1,,1,2,0,666666666,,IiuT=i003VD0qh0>Q@0:,0*01\r\n",
		//"$AIASM,1783768074,1,1,,1,2,0,666666666,,IiuD=g0@Q3Kp=q@o051T=hP0=k:0Lp07@p04P0,4*2A\r\n",
		//"$AIASM,1783406998,1,1,,1,2,0,666666666,,IiuT=i003VD0qh0>Q@0:,0*01\r\n",
		//"$AIASM,1783741085,1,1,,1,2,0,666666666,,IiuD=g0@Q3Kp=q@o0;QT=hP0=k:0Lp07@p04P0,4*41\r\n",
		//"$AIASM,1783741063,1,1,,2,2,0,666666666,,Iitl=bP8@nd1E3JpUT=d3g@nj0=k:0Lp07@p2mP,2*5D\r\n",
		//"$AIASM,1783740907,1,1,,1,2,0,666666666,,IitT=`0E@nR1Q3J@U4=aSi@n`05k:0Lp07@p04P,2*33\r\n",
		//"$AIASM,1783740861,1,1,,1,2,0,666666666,,Iit@00QVn5?Lh6@j567=qKe4tTqP0037P0ip4@,4*6E\r\n",
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
		//"$AIASM,1783836671,2,1,2,1,2,0,666666666,,Ij4@2=hj<7864>lQ@170I63Rp27HM00STlS1j1Q1J6t0Al6AP0GhQgG>08s3,0*17\r\n",
		//"$AIASM,,2,2,2,1,,0,666666666,,8h0;p@oQ404N1TH05t8KkA1P,5*04\r\n",
		//"$AIASM,1783831989,1,1,,2,2,0,666666666,,Ij480I@j6a0B5Rq5Q@@Ra@01V38JPP8F;S;634U40P0,2*23\r\n",
		//"$AIASM,1783764815,1,1,,2,2,0,666666666,,Ij480I@j6a0B5Rq5Q@@Ra@01V38JPP8F;S;634U40P0,2*28\r\n",
		//"$AIASM,1783743301,1,1,,1,2,0,666666666,,Ij5p0GAm4BMH038bA`PEns@4EE1P:tH>10,4*57\r\n",
		//"$AIASM,1783759233,1,1,,2,2,0,666666666,,Ij5h07im;6MJiS8b7oPEns@00@3bN<roS6ADnk0csAp2,0*76\r\n",
		//"$AIASM,1783743292,1,1,,1,2,0,666666666,,Ij5h07im;6MH038b7oPEns@00@3bN<rh06ADnk0csAp2,0*2A\r\n",
		//"$AIASM,1783749982,1,1,,1,2,0,666666666,,Ij5@00A5?dQ2etl0P0,4*79\r\n",
		//"$AIASM,1783749975,1,1,,1,2,0,666666666,,Ij584lPj:umN4PI20H0,2*37\r\n",
		//"$AIASM,1783749966,1,1,,2,2,0,666666666,,Ij50050j:vqL4P@fiPj<P05@j:umN4PI21@D@`05Pj:wrB4P8riQ2D1P,0*3C\r\n",
		//"$AIASM,1783749958,1,1,,2,2,0,666666666,,Ij4gwwi:Wkp<0;B2t?ww0VA1UIPbcLPOwu2<R3io1EIE@wwp6I49Hr2bqP08,0*37\r\n",
		//"$AIASM,1783749951,1,1,,2,2,0,666666666,,Ij4P1F@j6r9t5Rb9BQfVj1Hbit8,2*1D\r\n",
		//"$AIASM,1783756370,2,1,7,1,2,0,666666666,,Ij4@2=hj<7864>lQ@170I63Rp27HM00STlS1j1Q1J6t0Al6AP0GhQgG>08s3,0*1F\r\n",
		//"$AIASM,,2,2,7,1,,0,666666666,,8h0;p@oQ404N1TH05t8KkA1P,5*01\r\n",
		//"$AIASM,1783749926,1,1,,1,2,0,666666666,,Ij480I@j6a0B5Rq5Q@@Ra@01V38JPP8F;S;634U40P0,2*25\r\n",
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
		// 航道中线 DAC = 412, FI = 42
		//"$AIASM,1783849837,2,1,2,2,2,0,666666666,,Ij`1ulRVuo1K9elwwU;wn<f071qwtO`0>qswflOwqC?vluwwK9OvR4OwloOv,0*05\r\n",
		//"$AIASM,,2,2,2,2,,0,666666666,,0n0101wtuB03LIwuK007RKwtwP0CFh05LC80,0*3D\r\n",
		// 航道边线左侧 DAC = 412, FI = 43
		//"$AIASM,1783259645,1,1,,2,2,0,666666666,,Ijd1sPRc?@1>g400rV0v`P01LiQpbN06@I3k;d0,2*4F\r\n",
		// 航线推荐 DAC = 412, FI = 47
		//"$AIASM,1783259820,1,1,,1,4,0,666666666,210210210,Ijwi5M<<2LTwivS>P1`OP8da0?@T00,4*24\r\n",
		//"$AIASM,1783646310,1,1,,1,4,0,666666666,210210210,Ijwi5M<<2LTwivS>P1`OP8da0?@T00,4*26\r\n",
		//"$AIASM,1783646512,1,1,,2,4,0,666666666,210210210,IjtQ6JmP2KelP00003Fn000006ed7uSh0G<H00,4*72\r\n",
		//"$AIASM,1783646310,1,1,,1,4,0,666666666,210210210,Ijwi5M<<2LTwivS>P1`OP8da0?@T00,4*26\r\n",
		// 航线交换 DAC = 412, FI = 48
		"$ABASM,1784009334,1,1,,2,4,0,210210210,666666666,Ik032DvlEQ>h9j>808`OAPh4g7lH<663r464040,2*4D\r\n",
		"$ABASM,1784005238,1,1,,1,4,0,210210210,666666666,Ik032DvlEQ>h9j>808`OAPh4g7lH<663r464040,2*4F\r\n",
		// 水文气象响应(岸基) DAC = 412, FI = 50
		//"$AIASM,1783858247,1,1,,1,4,0,666666666,210210210,Ik80LMr03hR5gt:F`NwSh0,4*1E\r\n",
		// 中文短信 DAC = 413, FI = 04
		//"$AIASM,1782977984,1,1,,1,2,0,666666666,,IlBNC2`TQrE9r?saRjIwc9w43O?=bMtVssgmTSV5Ad>WtI>LHVvsnn0,2*0A\r\n",
		// 前端提示文字 DAC = 413, FI = 5
		//"$AIASM,1783260323,1,1,,1,2,0,666666666,,IlD9@P4L72TT04T50`2p4T>4SPp,2*1C\r\n",
		//"$AIASM,1783063312,1,1,,1,2,0,666666666,,IlD9Hs2rMSdqJrgoa=HwRWj0,0*62\r\n",
		// 信息补充片段，DAC = 413, FI = 07
		//"$AIASM,1783849836,1,1,,2,2,0,666666666,,IlL0?fIj`04At081@09cH020@06e?wPOwwlmOv<B,0*61\r\n",
		// 描述性文本补充片段，DAC = 413, FI = 08
		"$ABASM,1784007214,1,1,,2,4,0,210210210,666666666,Ik09PDvlEQ>h9j>808`OAPh4g7lH<663r464040,2*28\r\n",
		//"$AIASM,1783852782,1,1,,1,2,0,666666666,,IlP0?fIjb1aDMk2kjMv0,0*16\r\n",
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

	if (!bridges.empty())
	{
		uint32_t targetID = bridges[0].dataID;
		std::cout << "Testing DeleteBridge with dataID (MRN): " << targetID << std::endl;
		vdesManager.DeleteBridge(targetID);
		auto bridgesAfter = vdesManager.GetBridges(0, 100);
		std::cout << "After DeleteBridge, parsed bridges count: " << bridgesAfter.size() << std::endl;
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
		std::cout << "  MRN: " << (ns.nets.empty() ? 0 : ns.nets[0].MRN)
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
				  << ", Fragment: " << (int)ew.fragment
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
		std::cout << "  Status: " << (int)atonDyn.status
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
		std::cout << "  MRN (First): " << (ns.nets.empty() ? 0 : ns.nets[0].MRN)
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
		bool matchMRN = false;
		for (const auto &net : n.nets) {
			if (net.MRN == 12345) {
				matchMRN = true;
				break;
			}
		}
		if (matchMRN) {
			found = true;
			std::cout << "Persisted Net Sounder found in DB. First MRN: 12345" 
					  << ", Type: " << (int)n.type 
					  << ", Continuous: " << n.isContinous 
					  << ", IsOwnShip: " << n.isOwn 
					  << ", Nets count: " << n.nets.size() 
					  << ", Desc: " << n.description << std::endl;
			for (size_t i = 0; i < n.nets.size(); ++i) {
				std::cout << "    Net " << i << ": MRN=" << n.nets[i].MRN 
						  << ", FragmentDesc=" << (int)n.nets[i].fragmentDesc 
						  << ", Lat=" << n.nets[i].latitude 
						  << ", Lon=" << n.nets[i].longitude 
						  << ", Desc=\"" << n.nets[i].description << "\"" << std::endl;
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
		bool matchMRN = false;
		for (const auto &net : n.nets) {
			if (net.MRN == 12345) {
				matchMRN = true;
				break;
			}
		}
		if (matchMRN) {
			found = true;
			std::cout << "Persisted Net Sounder after update: First MRN: 12345" 
					  << ", Net 12345 Desc: \"" << n.nets[0].description << "\"" << std::endl;
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
			std::cout << "  AtoNDynamic Record ID to delete: " << item.dataID << std::endl;
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

	// Verify Revocation of Specific Information by MRN (DAC 413, FI 9)
	std::cout << "\n=== Verification: Revocation of Specific Information by MRN (DAC 413, FI 9) ===" << std::endl;

	// Let's populate the DB with an AtoNDynamics message (MRN=67890)
	std::cout << "Injecting mock AtoNDynamics (FI=33, status=1, MRN=67890) message..." << std::endl;
	auto atonVDMs = GenerateDAC_412_FI_33(1);
	for (const auto &vdm : atonVDMs)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Let's populate the DB with a NetSounder message
	std::cout << "Injecting mock NetSounder (FI=45) message..." << std::endl;
	auto nsVDMs = GenerateDAC_412_FI_45();
	for (const auto &vdm : nsVDMs)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify NetSounder exists in DB before revocation
	auto beforeNS = vdesManager.GetNetSounders(0, 100);
	std::cout << "NetSounders count in DB before revocation: " << beforeNS.size() << std::endl;
	for (const auto &ns : beforeNS)
	{
		std::cout << "  NetSounder ID: " << ns.dataID << ", MRN: " << (ns.nets.empty() ? 0 : ns.nets[0].MRN) << std::endl;
	}

	// Verify AtoNDynamics exists in DB before revocation
	auto beforeAton = vdesManager.GetAtoNDynamics(0, 100);
	std::cout << "AtoNDynamics count in DB before revocation: " << beforeAton.size() << std::endl;
	for (const auto &n : beforeAton)
	{
		for (const auto &elem : n.elements)
		{
			std::cout << "  AtoNDynamic Element MRN: " << elem.MRN << std::endl;
		}
	}

	// Generate and parse a revocation message targeting:
	// 1. (DAC=412, FI=33, MRN=67890) -> AtoNDynamics
	// 2. (DAC=412, FI=45, MRN=100001) -> NetSounder
	std::cout << "Injecting revocation (FI=9) NMEA..." << std::endl;
	std::vector<MockRevocationElement> revElems = {
		{ 412, 33, 67890 },
		{ 412, 45, 100001 }
	};
	auto revVDMs = GenerateDAC_413_FI_9(revElems);
	for (const auto &vdm : revVDMs)
	{
		std::cout << "Revocation VDM: " << vdm;
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify NetSounder has been revoked/deleted
	auto afterNS = vdesManager.GetNetSounders(0, 100);
	std::cout << "NetSounders count in DB after revocation: " << afterNS.size() << std::endl;
	for (const auto &ns : afterNS)
	{
		std::cout << "  Remaining NetSounder ID: " << ns.dataID << ", MRN: " << (ns.nets.empty() ? 0 : ns.nets[0].MRN) << std::endl;
	}

	// Verify AtoNDynamics has been revoked/deleted
	auto afterAton = vdesManager.GetAtoNDynamics(0, 100);
	std::cout << "AtoNDynamics count in DB after revocation: " << afterAton.size() << std::endl;
	for (const auto &n : afterAton)
	{
		for (const auto &elem : n.elements)
		{
			std::cout << "  Remaining AtoNDynamic Element MRN: " << elem.MRN << std::endl;
		}
	}
	std::cout << "================================================================================" << std::endl;

	// Verify Revocation of Continuous MRN Range Information (DAC 413, FI 10)
	std::cout << "\n=== Verification: Revocation of Continuous MRN Range Information (DAC 413, FI 10) ===" << std::endl;

	// Populate database with NetSounder message
	std::cout << "Injecting mock NetSounder (FI=45) message..." << std::endl;
	auto rangeNsVDMs = GenerateDAC_412_FI_45();
	for (const auto &vdm : rangeNsVDMs)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Populate database with 3 separate AtoNDynamics messages
	std::cout << "Injecting mock AtoNDynamics elements with MRNs 60001, 60002, 60003..." << std::endl;
	for (uint32_t mrn : {60001, 60002, 60003})
	{
		auto atonVDMs = GenerateDAC_412_FI_33_WithMRN(1, 12300 + mrn % 100, mrn);
		for (const auto &vdm : atonVDMs)
		{
			vdesManager.Parse(vdm.c_str(), vdm.length());
		}
	}

	// Verify NetSounders count in DB before range revocation
	auto beforeRangeNS = vdesManager.GetNetSounders(0, 100);
	std::cout << "NetSounders count before range revocation: " << beforeRangeNS.size() << std::endl;
	for (const auto &ns : beforeRangeNS)
	{
		std::cout << "  NetSounder ID: " << ns.dataID << ", MRN: " << (ns.nets.empty() ? 0 : ns.nets[0].MRN) << std::endl;
	}

	// Verify AtoNDynamics count in DB before range revocation
	auto beforeRangeAton = vdesManager.GetAtoNDynamics(0, 100);
	std::cout << "AtoNDynamics count before range revocation: " << beforeRangeAton.size() << std::endl;
	for (const auto &n : beforeRangeAton)
	{
		for (const auto &elem : n.elements)
		{
			std::cout << "  AtoNDynamic Element MRN: " << elem.MRN << std::endl;
		}
	}

	// Generate and parse range revocation message targeting range [100001, 100002]
	std::cout << "Injecting range revocation (FI=10) NMEA for NetSounder targeting range [100001, 100002]..." << std::endl;
	auto rangeRevVDMs1 = GenerateDAC_413_FI_10(412, 45, 100001, 100002);
	for (const auto &vdm : rangeRevVDMs1)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Generate and parse range revocation message targeting range [60001, 60002]
	std::cout << "Injecting range revocation (FI=10) NMEA for AtoNDynamics targeting range [60001, 60002]..." << std::endl;
	auto rangeRevVDMs2 = GenerateDAC_413_FI_10(412, 33, 60001, 60002);
	for (const auto &vdm : rangeRevVDMs2)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify NetSounders count in DB after range revocation
	auto afterRangeNS = vdesManager.GetNetSounders(0, 100);
	std::cout << "NetSounders count after range revocation: " << afterRangeNS.size() << std::endl;
	for (const auto &ns : afterRangeNS)
	{
		std::cout << "  Remaining NetSounder ID: " << ns.dataID << ", MRN: " << (ns.nets.empty() ? 0 : ns.nets[0].MRN) << std::endl;
	}

	// Verify AtoNDynamics count in DB after range revocation
	auto afterRangeAton = vdesManager.GetAtoNDynamics(0, 100);
	std::cout << "AtoNDynamics count after range revocation: " << afterRangeAton.size() << std::endl;
	for (const auto &n : afterRangeAton)
	{
		for (const auto &elem : n.elements)
		{
			std::cout << "  Remaining AtoNDynamic Element MRN: " << elem.MRN << std::endl;
		}
	}
	std::cout << "================================================================================" << std::endl;

	// Verify Channel Centerlines and Boundaries Deletion APIs
	std::cout << "\n=== Verification: Channel Centerlines & Channel Boundaries Deletion ===" << std::endl;

	// Populate database with mock Channel Centerlines and Boundaries messages
	std::cout << "Injecting mock Channel Centerline (FI=42) and Channel Boundary (FI=43/44) messages..." << std::endl;
	auto centerlinesVDM = GenerateDAC_412_FI_42();
	for (const auto &vdm : centerlinesVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}
	auto boundariesLeftVDM = GenerateDAC_412_FI_43(0);
	for (const auto &vdm : boundariesLeftVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}
	auto boundariesRightVDM = GenerateDAC_412_FI_43(1);
	for (const auto &vdm : boundariesRightVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify they exist in DB before deletion
	auto beforeCenterlines = vdesManager.GetChannelCenterlines(0, 100);
	std::cout << "Channel Centerlines count before deletion: " << beforeCenterlines.size() << std::endl;
	for (const auto &c : beforeCenterlines)
	{
		std::cout << "  Centerline ID: " << c.dataID << ", MRN: " << c.MRN << ", Width: " << c.width << std::endl;
	}

	auto beforeBoundaries = vdesManager.GetChannelBoundaries(0, 100);
	std::cout << "Channel Boundaries count before deletion: " << beforeBoundaries.size() << std::endl;
	for (const auto &b : beforeBoundaries)
	{
		std::cout << "  Boundary ID: " << b.dataID << ", MRN: " << b.MRN << std::endl;
	}

	// Delete them using IDs
	if (!beforeCenterlines.empty())
	{
		std::vector<uint32_t> ids { beforeCenterlines.front().dataID };
		std::cout << "Deleting Centerline ID: " << ids[0] << std::endl;
		bool delSuccess = vdesManager.DeleteChannelCenterlines(ids);
		std::cout << "Delete result: " << (delSuccess ? "SUCCESS" : "FAILED") << std::endl;
	}

	if (!beforeBoundaries.empty())
	{
		std::vector<uint32_t> ids { beforeBoundaries.front().dataID };
		std::cout << "Deleting Boundary ID: " << ids[0] << std::endl;
		bool delSuccess = vdesManager.DeleteChannelBoundaries(ids);
		std::cout << "Delete result: " << (delSuccess ? "SUCCESS" : "FAILED") << std::endl;
	}

	// Verify post-deletion counts
	auto afterCenterlines = vdesManager.GetChannelCenterlines(0, 100);
	std::cout << "Channel Centerlines count after ID deletion: " << afterCenterlines.size() << std::endl;

	auto afterBoundaries = vdesManager.GetChannelBoundaries(0, 100);
	std::cout << "Channel Boundaries count after ID deletion: " << afterBoundaries.size() << std::endl;

	// Verify paginated range deletion
	std::cout << "Deleting remaining Channel Centerlines by index/number..." << std::endl;
	vdesManager.DeleteChannelCenterlines(0, 100);
	std::cout << "Deleting remaining Channel Boundaries by index/number..." << std::endl;
	vdesManager.DeleteChannelBoundaries(0, 100);

	auto finalCenterlines = vdesManager.GetChannelCenterlines(0, 100);
	std::cout << "Final Channel Centerlines count: " << finalCenterlines.size() << std::endl;

	auto finalBoundaries = vdesManager.GetChannelBoundaries(0, 100);
	std::cout << "Final Channel Boundaries count: " << finalBoundaries.size() << std::endl;
	std::cout << "================================================================================" << std::endl;

	// Verify Channel Centerlines and Boundaries Supplementary Descriptions (FI 8)
	std::cout << "\n=== Verification: Channel Centerlines & Channel Boundaries Supplementary Descriptions (FI 8) ===" << std::endl;

	// Populate database with mock Channel Centerlines and Boundaries messages
	std::cout << "Injecting mock Channel Centerline (MRN=301) and Channel Boundary (MRN=302) messages..." << std::endl;
	for (const auto &vdm : centerlinesVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}
	for (const auto &vdm : boundariesLeftVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify they exist in DB before supplementary descriptions
	auto testCenterlines = vdesManager.GetChannelCenterlines(0, 100);
	std::cout << "Centerlines before FI 8: count = " << testCenterlines.size() << std::endl;
	for (const auto &c : testCenterlines)
	{
		std::cout << "  Centerline MRN: " << c.MRN << ", Description: '" << c.description << "'" << std::endl;
	}

	auto testBoundaries = vdesManager.GetChannelBoundaries(0, 100);
	std::cout << "Boundaries before FI 8: count = " << testBoundaries.size() << std::endl;
	for (const auto &b : testBoundaries)
	{
		std::cout << "  Boundary MRN: " << b.MRN << ", Description: '" << b.description << "'" << std::endl;
	}

	// Inject supplementary description for Centerline (MRN=301, mainFI=42)
	std::cout << "Injecting FI 8 text description for Centerline MRN=301..." << std::endl;
	auto centerlineDescVDM = GenerateDAC_413_FI_8(301, 412, 42, "Channel Centerline Supplementary Text Description");
	for (const auto &vdm : centerlineDescVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Inject supplementary description for Boundary (MRN=302, mainFI=43)
	std::cout << "Injecting FI 8 text description for Boundary MRN=302..." << std::endl;
	auto boundaryDescVDM = GenerateDAC_413_FI_8(302, 412, 43, "Channel Boundary Supplementary Text Description");
	for (const auto &vdm : boundaryDescVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify they are updated in DB
	auto testCenterlinesAfter = vdesManager.GetChannelCenterlines(0, 100);
	std::cout << "Centerlines after FI 8: count = " << testCenterlinesAfter.size() << std::endl;
	for (const auto &c : testCenterlinesAfter)
	{
		std::cout << "  Centerline MRN: " << c.MRN << ", Description: '" << c.description << "'" << std::endl;
	}

	auto testBoundariesAfter = vdesManager.GetChannelBoundaries(0, 100);
	std::cout << "Boundaries after FI 8: count = " << testBoundariesAfter.size() << std::endl;
	for (const auto &b : testBoundariesAfter)
	{
		std::cout << "  Boundary MRN: " << b.MRN << ", Description: '" << b.description << "'" << std::endl;
	}

	// Verify Channel Centerlines Supplementary Coordinates (FI 7)
	std::cout << "\n=== Verification: Channel Centerlines Supplementary Coordinates (FI 7) ===" << std::endl;
	
	// Query current coordinates of centerline MRN=301
	auto centerlineBeforeFI7 = vdesManager.GetChannelCenterlines(0, 100);
	for (const auto &c : centerlineBeforeFI7)
	{
		if (c.MRN == 301)
		{
			std::cout << "Centerline 301 coordinate count before FI 7: " << c.coordinates.size() << std::endl;
			for (size_t i = 0; i < c.coordinates.size(); ++i)
			{
				std::cout << "  Pt " << i << ": Lat=" << c.coordinates[i].GetLatitude() << ", Lon=" << c.coordinates[i].GetLongitude() << std::endl;
			}
		}
	}

	// Prepare delta coordinates for the supplement
	std::vector<VDES::Coordinate> deltas;
	deltas.push_back(VDES::Coordinate(-0.02, 0.05)); // Lat, Lon
	deltas.push_back(VDES::Coordinate(0.01, 0.02));

	std::cout << "Injecting FI 7 supplementary coordinates for Centerline MRN=301..." << std::endl;
	auto centerlineFI7VDM = GenerateDAC_413_FI_7(301, 412, 42, deltas);
	for (const auto &vdm : centerlineFI7VDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Query again and verify that coordinates have been successfully appended and reconstructed
	auto centerlineAfterFI7 = vdesManager.GetChannelCenterlines(0, 100);
	for (const auto &c : centerlineAfterFI7)
	{
		if (c.MRN == 301)
		{
			std::cout << "Centerline 301 coordinate count after FI 7: " << c.coordinates.size() << std::endl;
			for (size_t i = 0; i < c.coordinates.size(); ++i)
			{
				std::cout << "  Pt " << i << ": Lat=" << c.coordinates[i].GetLatitude() << ", Lon=" << c.coordinates[i].GetLongitude() << std::endl;
			}
		}
	}
	std::cout << "================================================================================" << std::endl;

	// Verify Channel Boundaries Supplementary Coordinates (FI 7)
	std::cout << "\n=== Verification: Channel Boundaries Supplementary Coordinates (FI 7) ===" << std::endl;
	
	// Inject initial right side boundary message (FI=44) for MRN=302
	auto boundariesRightVDM2 = GenerateDAC_412_FI_43(1);
	for (const auto &vdm : boundariesRightVDM2)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Query current coordinates of boundary MRN=302
	auto boundaryBeforeFI7 = vdesManager.GetChannelBoundaries(0, 100);
	for (const auto &b : boundaryBeforeFI7)
	{
		if (b.MRN == 302)
		{
			std::cout << "Boundary 302 left coordinate count before FI 7: " << b.leftCoordinates.size() << std::endl;
			std::cout << "Boundary 302 right coordinate count before FI 7: " << b.rightCoordinates.size() << std::endl;
		}
	}

	// Prepare delta coordinates for left boundary (FI=43) supplement
	std::vector<VDES::Coordinate> leftDeltas;
	leftDeltas.push_back(VDES::Coordinate(-0.01, 0.03));
	leftDeltas.push_back(VDES::Coordinate(0.02, 0.04));

	std::cout << "Injecting FI 7 supplementary coordinates for Left Boundary MRN=302..." << std::endl;
	auto leftBoundaryFI7VDM = GenerateDAC_413_FI_7(302, 412, 43, leftDeltas);
	for (const auto &vdm : leftBoundaryFI7VDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Prepare delta coordinates for right boundary (FI=44) supplement
	std::vector<VDES::Coordinate> rightDeltas;
	rightDeltas.push_back(VDES::Coordinate(0.03, -0.01));
	rightDeltas.push_back(VDES::Coordinate(-0.02, 0.05));

	std::cout << "Injecting FI 7 supplementary coordinates for Right Boundary MRN=302..." << std::endl;
	auto rightBoundaryFI7VDM = GenerateDAC_413_FI_7(302, 412, 44, rightDeltas);
	for (const auto &vdm : rightBoundaryFI7VDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Query again and verify that left/right coordinates have been successfully appended and reconstructed
	auto boundaryAfterFI7 = vdesManager.GetChannelBoundaries(0, 100);
	for (const auto &b : boundaryAfterFI7)
	{
		if (b.MRN == 302)
		{
			std::cout << "Boundary 302 left coordinate count after FI 7: " << b.leftCoordinates.size() << std::endl;
			for (size_t i = 0; i < b.leftCoordinates.size(); ++i)
			{
				std::cout << "  Left Pt " << i << ": Lat=" << b.leftCoordinates[i].GetLatitude() << ", Lon=" << b.leftCoordinates[i].GetLongitude() << std::endl;
			}
			std::cout << "Boundary 302 right coordinate count after FI 7: " << b.rightCoordinates.size() << std::endl;
			for (size_t i = 0; i < b.rightCoordinates.size(); ++i)
			{
				std::cout << "  Right Pt " << i << ": Lat=" << b.rightCoordinates[i].GetLatitude() << ", Lon=" << b.rightCoordinates[i].GetLongitude() << std::endl;
			}
		}
	}
	std::cout << "================================================================================" << std::endl;

	// Verify Military Activity Supplementary Description (FI 8)
	std::cout << "\n=== Verification: Military Activity Supplementary Description (FI 8) ===" << std::endl;

	// Populate database with mock Military Activity message (MRN=12345, geometryType=0)
	std::cout << "Injecting mock Military Activity message (MRN=12345)..." << std::endl;
	auto militaryVDM = GenerateDAC_412_FI_38(0);
	for (const auto &vdm : militaryVDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify before supplement
	auto activitiesBefore = vdesManager.GetMilitaryActivitys(0, 100);
	for (const auto &a : activitiesBefore)
	{
		if (a.dataID == 12345)
		{
			std::cout << "Military Activity 12345 description before FI 8: '" << a.description << "'" << std::endl;
		}
	}

	// Inject FI 8 text description for Military Activity MRN=12345
	std::cout << "Injecting FI 8 text description for Military Activity MRN=12345..." << std::endl;
	auto militaryFI8VDM = GenerateDAC_413_FI_8(12345, 412, 38, "MILITARY ACTIVITY SUPPLEMENTARY TEXT DESCRIPTION");
	for (const auto &vdm : militaryFI8VDM)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Verify after supplement
	auto activitiesAfter = vdesManager.GetMilitaryActivitys(0, 100);
	for (const auto &a : activitiesAfter)
	{
		if (a.dataID == 12345)
		{
			std::cout << "Military Activity 12345 description after FI 8: '" << a.description << "'" << std::endl;
		}
	}
	std::cout << "================================================================================" << std::endl;

	// Verify Marine Meteorology and Environmental Warnings (FI 31)
	std::cout << "\n=== Verification: Marine Meteorology & Environmental Warnings (FI=31) ===" << std::endl;
	
	// Clean up tables first
	vdesManager.DeleteMewTropicalCyclones(0, 100);
	vdesManager.DeleteMewGales(0, 100);
	vdesManager.DeleteMewLargeWaves(0, 100);
	vdesManager.DeleteMewSeaFogs(0, 100);
	vdesManager.DeleteMewStormSurges(0, 100);
	vdesManager.DeleteMewSeaIces(0, 100);

	// Parse Type 1 (Cyclone)
	auto cycloneVDMs = GenerateDAC_412_FI_31(1);
	for (const auto &vdm : cycloneVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	// Parse Type 2 (Gale)
	auto galeVDMs = GenerateDAC_412_FI_31(2);
	for (const auto &vdm : galeVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	// Parse Type 3 (Large Wave)
	auto waveVDMs = GenerateDAC_412_FI_31(3);
	for (const auto &vdm : waveVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	// Parse Type 5 (Storm Surge)
	auto surgeVDMs = GenerateDAC_412_FI_31(5);
	for (const auto &vdm : surgeVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	// Parse Type 6 (Sea Ice)
	auto iceVDMs = GenerateDAC_412_FI_31(6);
	for (const auto &vdm : iceVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	// Parse Type 4 (Sea Fog)
	auto fogVDMs = GenerateDAC_412_FI_31(4);
	for (const auto &vdm : fogVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	// Retrieve and print from database to verify Fragment and other fields
	auto dbCyclones = vdesManager.GetMewTropicalCyclones(0, 100);
	std::cout << "MewTropicalCyclones from DB: " << dbCyclones.size() << std::endl;
	for (const auto &ew : dbCyclones)
	{
		std::cout << "  MRN: " << ew.MRN << ", Fragment: " << (int)ew.fragment << ", Path points: " << ew.pathPoints.size() << std::endl;
	}

	auto dbGales = vdesManager.GetMewGales(0, 100);
	std::cout << "MewGales from DB: " << dbGales.size() << std::endl;
	for (const auto &ew : dbGales)
	{
		std::cout << "  MRN: " << ew.MRN << ", Fragment: " << (int)ew.fragment << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << std::endl;
	}

	auto dbWaves = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "MewLargeWaves from DB: " << dbWaves.size() << std::endl;
	for (const auto &ew : dbWaves)
	{
		std::cout << "  MRN: " << ew.MRN << ", Fragment: " << (int)ew.fragment << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << std::endl;
	}

	auto dbFogs = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "MewSeaFogs from DB: " << dbFogs.size() << std::endl;
	for (const auto &ew : dbFogs)
	{
		std::cout << "  MRN: " << ew.MRN << ", Fragment: " << (int)ew.fragment << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << std::endl;
	}

	auto dbSurges = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "MewStormSurges from DB: " << dbSurges.size() << std::endl;
	for (const auto &ew : dbSurges)
	{
		std::cout << "  MRN: " << ew.MRN << ", Fragment: " << (int)ew.fragment << ", City: " << (int)ew.cityCode << ", Height: " << (int)ew.surgeHeight << ", Level: " << (int)ew.warningLevel << std::endl;
	}

	auto dbIces = vdesManager.GetMewSeaIces(0, 100);
	std::cout << "MewSeaIces from DB: " << dbIces.size() << std::endl;
	for (const auto &ew : dbIces)
	{
		std::cout << "  MRN: " << ew.MRN << ", Region: " << (int)ew.regionCode << ", Level: " << (int)ew.warningLevel << std::endl;
	}

	std::cout << "\n=== Verification: Tropical Cyclone Supplementary Points (FI 7) & Text Description (FI 8) ===" << std::endl;
	auto cyclonesBefore = vdesManager.GetMewTropicalCyclones(0, 100);
	std::cout << "Initial Tropical Cyclones count: " << cyclonesBefore.size() << std::endl;
	for (const auto &ew : cyclonesBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", Path points count: " << ew.pathPoints.size() << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary coordinates (FI 7) for Tropical Cyclone MRN=100..." << std::endl;
	std::vector<VDES::MewTropicalCyclone::PathPoint> suppPoints;
	{
		VDES::MewTropicalCyclone::PathPoint pt1;
		pt1.timestamp = VDES::UtilityInterface::GetCurrentTimeStamp();
		pt1.centerLongitude = 119.5;
		pt1.centerLatitude = 25.5;
		pt1.cycloneType = 2;
		pt1.radiusWindScale7 = 180;
		pt1.radiusWindScale10 = 95;
		pt1.radiusWindScale12 = 48;
		pt1.moveSpeed = 22;
		pt1.moveDirection = 315;
		pt1.maxWindScale = 14;
		pt1.centerPressure = 975;
		suppPoints.push_back(pt1);

		VDES::MewTropicalCyclone::PathPoint pt2;
		pt2.timestamp = pt1.timestamp + 3600;
		pt2.centerLongitude = 120.0;
		pt2.centerLatitude = 26.0;
		pt2.cycloneType = 1;
		pt2.radiusWindScale7 = 200;
		pt2.radiusWindScale10 = 100;
		pt2.radiusWindScale12 = 50;
		pt2.moveSpeed = 20;
		pt2.moveDirection = 320;
		pt2.maxWindScale = 15;
		pt2.centerPressure = 970;
		suppPoints.push_back(pt2);
	}

	auto cycloneSuppVDM = GenerateDAC_413_FI_7_Cyclone(100, suppPoints);
	for (const auto &vdm : cycloneSuppVDM) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto cyclonesAfterFI7 = vdesManager.GetMewTropicalCyclones(0, 100);
	std::cout << "Tropical Cyclones count after FI 7: " << cyclonesAfterFI7.size() << std::endl;
	for (const auto &ew : cyclonesAfterFI7)
	{
		std::cout << "  MRN: " << ew.MRN << ", Path points count: " << ew.pathPoints.size() << ", Description: '" << ew.description << "'" << std::endl;
		for (size_t i = 0; i < ew.pathPoints.size(); ++i)
		{
			std::cout << "    Pt " << i << ": Lat=" << ew.pathPoints[i].centerLatitude << ", Lon=" << ew.pathPoints[i].centerLongitude << std::endl;
		}
	}

	std::cout << "Injecting supplementary text description (FI 8) for Tropical Cyclone MRN=100..." << std::endl;
	auto cycloneDescVDM = GenerateDAC_413_FI_8(100, 412, 31, "TROPICAL CYCLONE TEXT DESCRIPTION SUPPLEMENT");
	for (const auto &vdm : cycloneDescVDM) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto cyclonesAfterFI8 = vdesManager.GetMewTropicalCyclones(0, 100);
	std::cout << "Tropical Cyclones count after FI 8: " << cyclonesAfterFI8.size() << std::endl;
	for (const auto &ew : cyclonesAfterFI8)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Re-injecting main Tropical Cyclone (FI 31) to verify description preservation..." << std::endl;
	for (const auto &vdm : cycloneVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto cyclonesAfterReinject = vdesManager.GetMewTropicalCyclones(0, 100);
	std::cout << "Tropical Cyclones count after Reinject: " << cyclonesAfterReinject.size() << std::endl;
	for (const auto &ew : cyclonesAfterReinject)
	{
		std::cout << "  MRN: " << ew.MRN << ", Path points count: " << ew.pathPoints.size() << ", Description: '" << ew.description << "'" << std::endl;
	}
	std::cout << "================================================================================\n" << std::endl;

	// Inject FI 8 text description for Gale Warning (MRN=201, mainFI=31)
	std::cout << "Injecting FI 8 text description for Gale Warning MRN=201..." << std::endl;
	auto galeDescVDM = GenerateDAC_413_FI_8(201, 412, 31, "Gale Supplementary Text Description");
	for (const auto &vdm : galeDescVDM) vdesManager.Parse(vdm.c_str(), vdm.length());

	// Retrieve again and verify description
	dbGales = vdesManager.GetMewGales(0, 100);
	std::cout << "MewGales from DB after FI 8 update:" << std::endl;
	for (const auto &ew : dbGales)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	// Verify Gale warning supplement (FI=7)
	std::cout << "\n=== Verification: Gale Warning Supplementary Elements (FI 7) & Text Description (FI 8) ===" << std::endl;
	auto galesBefore = vdesManager.GetMewGales(0, 100);
	std::cout << "Gales count before supplement: " << galesBefore.size() << std::endl;
	for (const auto &ew : galesBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary warnings (FI 7) for Gale warning group (parent MRN=201) containing child MRN=203..." << std::endl;
	std::vector<MockGeneralWarningElement> suppGales;
	{
		MockGeneralWarningElement elem;
		elem.MRN = 203;
		elem.fragment = 1;
		elem.seaAreaCode = 8;
		elem.warningLevel = 1;
		suppGales.push_back(elem);
	}

	auto galeSuppVDMs = GenerateDAC_413_FI_7_Gale(201, suppGales);
	for (const auto &vdm : galeSuppVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto galesAfterFI7 = vdesManager.GetMewGales(0, 100);
	std::cout << "Gales count after FI 7 supplement: " << galesAfterFI7.size() << std::endl;
	for (const auto &ew : galesAfterFI7)
	{
		std::cout << "  MRN: " << ew.MRN << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary text description (FI 8) for supplementary Gale warning MRN=203..." << std::endl;
	auto gale203DescVDMs = GenerateDAC_413_FI_8(203, 412, 31, "GALE 203 TEXT DESCRIPTION SUPPLEMENT");
	for (const auto &vdm : gale203DescVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto galesAfterFI8 = vdesManager.GetMewGales(0, 100);
	std::cout << "Gales count after FI 8: " << galesAfterFI8.size() << std::endl;
	for (const auto &ew : galesAfterFI8)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Re-injecting main Gale warnings (FI 31) to verify description preservation for MRN=201..." << std::endl;
	for (const auto &vdm : galeVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto galesAfterReinject = vdesManager.GetMewGales(0, 100);
	std::cout << "Gales count after Reinject: " << galesAfterReinject.size() << std::endl;
	for (const auto &ew : galesAfterReinject)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}
	std::cout << "================================================================================\n" << std::endl;

	// Inject FI 8 text description for Large Wave Warning (MRN=201, mainFI=31)
	// Inject FI 8 text description for Large Wave Warning (MRN=211, mainFI=31)
	std::cout << "Injecting FI 8 text description for Large Wave Warning MRN=211..." << std::endl;
	auto waveDescVDM = GenerateDAC_413_FI_8(211, 412, 31, "Large Wave Supplementary Text Description");
	for (const auto &vdm : waveDescVDM) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto dbWavesBefore = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "MewLargeWaves from DB after FI 8 update:" << std::endl;
	for (const auto &ew : dbWavesBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	// Verify Large Wave warning supplement (FI=7)
	std::cout << "\n=== Verification: Large Wave Warning Supplementary Elements (FI 7) & Text Description (FI 8) ===" << std::endl;
	auto wavesBefore = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "Waves count before supplement: " << wavesBefore.size() << std::endl;
	for (const auto &ew : wavesBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary warnings (FI 7) for Large Wave warning group (parent MRN=211) containing child MRN=213..." << std::endl;
	std::vector<MockGeneralWarningElement> suppWaves;
	{
		MockGeneralWarningElement elem;
		elem.MRN = 213;
		elem.fragment = 1;
		elem.seaAreaCode = 9;
		elem.warningLevel = 2;
		suppWaves.push_back(elem);
	}

	auto waveSuppVDMs = GenerateDAC_413_FI_7_Gale(211, suppWaves);
	for (const auto &vdm : waveSuppVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto wavesAfterFI7 = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "Waves count after FI 7 supplement: " << wavesAfterFI7.size() << std::endl;
	for (const auto &ew : wavesAfterFI7)
	{
		std::cout << "  MRN: " << ew.MRN << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary text description (FI 8) for supplementary Large Wave warning MRN=213..." << std::endl;
	auto wave203DescVDMs = GenerateDAC_413_FI_8(213, 412, 31, "WAVE 213 TEXT DESCRIPTION SUPPLEMENT");
	for (const auto &vdm : wave203DescVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto wavesAfterFI8 = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "Waves count after FI 8: " << wavesAfterFI8.size() << std::endl;
	for (const auto &ew : wavesAfterFI8)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Re-injecting main Large Wave warnings (FI 31) to verify description preservation for MRN=211..." << std::endl;
	for (const auto &vdm : waveVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto wavesAfterReinject = vdesManager.GetMewLargeWaves(0, 100);
	std::cout << "Waves count after Reinject: " << wavesAfterReinject.size() << std::endl;
	for (const auto &ew : wavesAfterReinject)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}
	std::cout << "================================================================================\n" << std::endl;

	// Inject FI 8 text description for Sea Fog Warning (MRN=221, mainFI=31)
	std::cout << "Injecting FI 8 text description for Sea Fog Warning MRN=221..." << std::endl;
	auto fogDescVDM = GenerateDAC_413_FI_8(221, 412, 31, "Sea Fog Supplementary Text Description");
	for (const auto &vdm : fogDescVDM) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto dbFogsBefore = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "MewSeaFogs from DB after FI 8 update:" << std::endl;
	for (const auto &ew : dbFogsBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	// Verify Sea Fog warning supplement (FI=7)
	std::cout << "\n=== Verification: Sea Fog Warning Supplementary Elements (FI 7) & Text Description (FI 8) ===" << std::endl;
	auto fogsBefore = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "Fogs count before supplement: " << fogsBefore.size() << std::endl;
	for (const auto &ew : fogsBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary warnings (FI 7) for Sea Fog warning group (parent MRN=221) containing child MRN=223..." << std::endl;
	std::vector<MockGeneralWarningElement> suppFogs;
	{
		MockGeneralWarningElement elem;
		elem.MRN = 223;
		elem.fragment = 1;
		elem.seaAreaCode = 10;
		elem.warningLevel = 3;
		suppFogs.push_back(elem);
	}

	auto fogSuppVDMs = GenerateDAC_413_FI_7_Gale(221, suppFogs);
	for (const auto &vdm : fogSuppVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto fogsAfterFI7 = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "Fogs count after FI 7 supplement: " << fogsAfterFI7.size() << std::endl;
	for (const auto &ew : fogsAfterFI7)
	{
		std::cout << "  MRN: " << ew.MRN << ", Area: " << (int)ew.areaCode << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary text description (FI 8) for supplementary Sea Fog warning MRN=223..." << std::endl;
	auto fog203DescVDMs = GenerateDAC_413_FI_8(223, 412, 31, "FOG 223 TEXT DESCRIPTION SUPPLEMENT");
	for (const auto &vdm : fog203DescVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto fogsAfterFI8 = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "Fogs count after FI 8: " << fogsAfterFI8.size() << std::endl;
	for (const auto &ew : fogsAfterFI8)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Re-injecting main Sea Fog warnings (FI 31) to verify description preservation for MRN=221..." << std::endl;
	for (const auto &vdm : fogVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto fogsAfterReinject = vdesManager.GetMewSeaFogs(0, 100);
	std::cout << "Fogs count after Reinject: " << fogsAfterReinject.size() << std::endl;
	for (const auto &ew : fogsAfterReinject)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}
	std::cout << "================================================================================\n" << std::endl;

	// Inject FI 8 text description for Storm Surge Warning (MRN=301, mainFI=31)
	std::cout << "Injecting FI 8 text description for Storm Surge Warning MRN=301..." << std::endl;
	auto surgeDescVDM = GenerateDAC_413_FI_8(301, 412, 31, "Storm Surge Supplementary Text Description");
	for (const auto &vdm : surgeDescVDM) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto dbSurgesBefore = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "MewStormSurges from DB after FI 8 update:" << std::endl;
	for (const auto &ew : dbSurgesBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	// Verify Storm Surge warning supplement (FI=7)
	std::cout << "\n=== Verification: Storm Surge Warning Supplementary Elements (FI 7) & Text Description (FI 8) ===" << std::endl;
	auto surgesBefore = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "Surges count before supplement: " << surgesBefore.size() << std::endl;
	for (const auto &ew : surgesBefore)
	{
		std::cout << "  MRN: " << ew.MRN << ", City: " << (int)ew.cityCode << ", Height: " << (int)ew.surgeHeight << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary warnings (FI 7) for Storm Surge warning group (parent MRN=301) containing child MRN=303..." << std::endl;
	std::vector<MockStormSurgeElement> suppSurges;
	{
		MockStormSurgeElement elem;
		elem.MRN = 303;
		elem.fragment = 1;
		elem.cityCode = 20;
		elem.surgeHeight = 15;
		elem.warningLevel = 2;
		suppSurges.push_back(elem);
	}

	auto surgeSuppVDMs = GenerateDAC_413_FI_7_Surge(301, suppSurges);
	for (const auto &vdm : surgeSuppVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto surgesAfterFI7 = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "Surges count after FI 7 supplement: " << surgesAfterFI7.size() << std::endl;
	for (const auto &ew : surgesAfterFI7)
	{
		std::cout << "  MRN: " << ew.MRN << ", City: " << (int)ew.cityCode << ", Height: " << (int)ew.surgeHeight << ", Level: " << (int)ew.warningLevel << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Injecting supplementary text description (FI 8) for supplementary Storm Surge warning MRN=303..." << std::endl;
	auto surge203DescVDMs = GenerateDAC_413_FI_8(303, 412, 31, "SURGE 303 TEXT DESCRIPTION SUPPLEMENT");
	for (const auto &vdm : surge203DescVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto surgesAfterFI8 = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "Surges count after FI 8: " << surgesAfterFI8.size() << std::endl;
	for (const auto &ew : surgesAfterFI8)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}

	std::cout << "Re-injecting main Storm Surge warnings (FI 31) to verify description preservation for MRN=301..." << std::endl;
	for (const auto &vdm : surgeVDMs) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto surgesAfterReinject = vdesManager.GetMewStormSurges(0, 100);
	std::cout << "Surges count after Reinject: " << surgesAfterReinject.size() << std::endl;
	for (const auto &ew : surgesAfterReinject)
	{
		std::cout << "  MRN: " << ew.MRN << ", Description: '" << ew.description << "'" << std::endl;
	}
	std::cout << "================================================================================\n" << std::endl;

	// Clean up FI 31 warnings
	vdesManager.DeleteMewTropicalCyclones(0, 100);
	vdesManager.DeleteMewGales(0, 100);
	vdesManager.DeleteMewLargeWaves(0, 100);
	vdesManager.DeleteMewSeaFogs(0, 100);
	vdesManager.DeleteMewStormSurges(0, 100);
	vdesManager.DeleteMewSeaIces(0, 100);
	std::cout << "================================================================================" << std::endl;

	vdesManager.DeleteChannelCenterlines(0, 100);
	vdesManager.DeleteChannelBoundaries(0, 100);

	// Verify Bridge deletion
	std::cout << "\n=== Verification: Bridge Deletion ===" << std::endl;
	auto testBridgesList = GenerateDAC_412_FI_41();
	for (const auto &vdm : testBridgesList) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto dbBridgesBefore = vdesManager.GetBridges(0, 100);
	std::cout << "Bridges count before delete: " << dbBridgesBefore.size() << std::endl;
	for (const auto &b : dbBridgesBefore)
	{
		std::cout << "  Bridge MRN: " << b.MRN << ", dataID: " << b.dataID << ", Spans: " << b.spans.size() << std::endl;
	}

	if (!dbBridgesBefore.empty())
	{
		uint32_t targetID = dbBridgesBefore[0].dataID;
		std::cout << "Calling DeleteBridge with dataID: " << targetID << std::endl;
		vdesManager.DeleteBridge(targetID);
		
		auto dbBridgesAfter = vdesManager.GetBridges(0, 100);
		std::cout << "Bridges count after delete: " << dbBridgesAfter.size() << std::endl;
	}
	
	// Clean up bridge table
	if (!vdesManager.GetBridges(0, 100).empty())
	{
		std::vector<uint32_t> ids;
		for (const auto &b : vdesManager.GetBridges(0, 100)) ids.push_back(b.dataID);
		vdesManager.DeleteBridges(ids);
	}
	std::cout << "Bridges count after batch delete: " << vdesManager.GetBridges(0, 100).size() << std::endl;
	std::cout << "========================================" << std::endl;

	// Verify HydrometeorologyResponse correlation & Timeout
	std::cout << "\n=== Verification: HydrometeorologyResponse Correlation & Timeout ===" << std::endl;
	
	// Register a dedicated event listener
	std::mutex mtxNotify;
	std::condition_variable cvNotify;
	bool gotTimeout = false;
	bool gotResponse = false;
	bool gotSuccess = false;
	
	auto callbackToken = vdesManager.notifyEvent.append([&](const VDES::VDESManager::EventType type, const int retCode) {
		if (type == VDES::VDESManager::EventType::MESSAGE_SEND) {
			std::unique_lock<std::mutex> lock(mtxNotify);
			if (retCode == 4) {
				gotTimeout = true;
				std::cout << "[EVENT] Got MESSAGE_SEND with timeout retCode: 4" << std::endl;
			} else if (retCode == 1) {
				gotSuccess = true;
				std::cout << "[EVENT] Got MESSAGE_SEND with success retCode: 1" << std::endl;
			}
			cvNotify.notify_all();
		}
		if (type == VDES::VDESManager::EventType::ASM_HYDROMETEOROLOGY_RESPONSE) {
			std::unique_lock<std::mutex> lock(mtxNotify);
			gotResponse = true;
			cvNotify.notify_all();
			std::cout << "[EVENT] Got ASM_HYDROMETEOROLOGY_RESPONSE" << std::endl;
		}
	});

	// Make sure own MMSI is set
	auto ownVesselInfo = VDES::ConfigureManager::GetInstance().GetOwnVesselInfo();
	std::cout << "Own vessel MMSI: " << ownVesselInfo.mmsi << std::endl;

	// 1. Send Request
	VDES::HydrometeorologyRequest testReq;

	testReq.windSpeed = true;
	testReq.windDirection = true;
	testReq.visibility = true;
	testReq.waveHeight = true;
	testReq.waveDirection = true;
	testReq.swellHeight = true;
	testReq.requestTime = VDES::UtilityInterface::GetCurrentTimeStamp();
	testReq.coordinates.push_back(VDES::Coordinate(38.5, 118.2));
	testReq.coordinates.push_back(VDES::Coordinate(39.1, 119.5));

	std::cout << "Sending HydrometeorologyRequest..." << std::endl;
	vdesManager.SendHydrometeorologyRequest(testReq);

	// Simulate AMK response (seqNo=1, ackType=0) from transceiver
	std::cout << "Simulating AMK response (seqNo=1, ackType=0) from transceiver..." << std::endl;
	std::string amkSentence = "$AIAMK,004129999,1,,1,0";
	VDES::UtilityInterface::AddChecksum(amkSentence);
	amkSentence += "\r\n";
	vdesManager.Parse(amkSentence.c_str(), amkSentence.length());

	// 2. Parse mock FI 50 Response matching own MMSI
	std::cout << "Simulating FI 50 response from shore..." << std::endl;
	auto responseVDMs = GenerateDAC_412_FI_50(ownVesselInfo.mmsi, 99999);
	for (const auto &vdm : responseVDMs)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Retrieve the response and check coordinates
	auto responses = vdesManager.GetHydrometeorologyResponses(0, 1);
	std::cout << "Hydrometeorology responses count: " << responses.size() << std::endl;
	if (!responses.empty())
	{
		auto &resp = responses.front();
		std::cout << "Response matched MRN: " << resp.MRN << " (Expected: 0)" << std::endl;
		std::cout << "Response responder MMSI: " << resp.mmsiResponser << " (Expected: 99999)" << std::endl;
		std::cout << "Response forecast points count: " << resp.points.size() << std::endl;
		for (size_t i = 0; i < resp.points.size(); ++i)
		{
			auto &pt = resp.points[i];
			std::cout << "  Point " << i << " -> Coord: (" << pt.latitude << ", " << pt.longitude 
					  << "), windSpeed: " << (int)pt.windSpeed 
					  << ", windDirection: " << pt.windDirection 
					  << ", visibility: " << (int)pt.visibility 
					  << ", waveHeight: " << (int)pt.waveHeight 
					  << ", waveDirection: " << pt.waveDirection 
					  << ", swellHeight: " << (int)pt.swellHeight << std::endl;
		}
	}

	// 3. Test Timeout (Send request and wait > 10 seconds)
	VDES::HydrometeorologyRequest timeoutReq = testReq;
	std::cout << "\nSending HydrometeorologyRequest for timeout test..." << std::endl;
	vdesManager.SendHydrometeorologyRequest(timeoutReq);

	std::cout << "Waiting 11 seconds for timeout..." << std::endl;
	{
		std::unique_lock<std::mutex> lockNotify(mtxNotify);
		cvNotify.wait_for(lockNotify, std::chrono::seconds(12), [&]() { return gotTimeout; });
	}

	std::cout << "Timeout check finished. gotTimeout = " << (gotTimeout ? "TRUE" : "FALSE") << std::endl;
 
	// Reset flags for Route Recommendation Request verification
	gotTimeout = false;
	gotSuccess = false;
 
	std::cout << "\n=== Verification: RouteRecommendationRequest Correlation & Timeout ===" << std::endl;
	VDES::RouteRecommendationRequest routeReq;
	routeReq.grossTonnage = 12000;
	routeReq.maxStaticDraft = 8.5;
	routeReq.cargoType = 3;
	routeReq.month = 7;
	routeReq.day = 13;
	routeReq.hour = 22;
	routeReq.minute = 10;
	routeReq.startCoordinate = VDES::Coordinate(38.5, 118.2);
	routeReq.destCoordinate = VDES::Coordinate(39.1, 119.5);
 
	std::cout << "Sending RouteRecommendationRequest..." << std::endl;
	vdesManager.SendRouteRecommendationRequest(routeReq);
 
	// Since Route Recommendation Request is sent, m_sequenceNoAAB is incremented.
	// As HydrometeorologyRequest was sent twice (seq 1, seq 2), routeReq is seq 3.
	std::cout << "Simulating AMK response (seqNo=3, ackType=0) from transceiver..." << std::endl;
	std::string routeAmk = "$AIAMK,004129999,1,,3,0";
	VDES::UtilityInterface::AddChecksum(routeAmk);
	routeAmk += "\r\n";
	vdesManager.Parse(routeAmk.c_str(), routeAmk.length());
 
	std::cout << "gotSuccess (Route Req) = " << (gotSuccess ? "TRUE" : "FALSE") << std::endl;

	// Simulate Route Recommendation Response (FI 47) matching source MMSI 88888
	std::cout << "Simulating Route Recommendation Response (FI 47) from shore..." << std::endl;
	auto routeResponseVDMs = GenerateDAC_412_FI_47(ownVesselInfo.mmsi, 88888);
	for (const auto &vdm : routeResponseVDMs)
	{
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	// Retrieve response and check
	auto routeResponses = vdesManager.GetRouteRecommendationResponses(0, 1);
	std::cout << "Route Recommendation responses count: " << routeResponses.size() << std::endl;
	if (!routeResponses.empty())
	{
		auto &resp = routeResponses.front();
		std::cout << "Response effective time: " << resp.effectiveTime << " (Expected: 31.5)" << std::endl;
		std::cout << "Response responder MMSI: " << resp.mmsiResponser << " (Expected: 88888)" << std::endl;
		std::cout << "Response coordinates count: " << resp.coordinates.size() << " (Expected: 2)" << std::endl;
	}

	// Reset gotTimeout flag for timeout test
	gotTimeout = false;
	std::cout << "\nSending RouteRecommendationRequest for timeout test..." << std::endl;
	vdesManager.SendRouteRecommendationRequest(routeReq); // seq 4
 
	std::cout << "Waiting 11 seconds for timeout..." << std::endl;
	{
		std::unique_lock<std::mutex> lockNotify(mtxNotify);
		cvNotify.wait_for(lockNotify, std::chrono::seconds(12), [&]() { return gotTimeout; });
	}
	std::cout << "Route Req Timeout check finished. gotTimeout = " << (gotTimeout ? "TRUE" : "FALSE") << std::endl;
	std::cout << "==========================================================" << std::endl;

	// 4. Verification: SendNetSounder Correlation & Timeout
	gotTimeout = false;
	gotSuccess = false;

	std::cout << "\n=== Verification: SendNetSounder Correlation & Timeout ===" << std::endl;
	VDES::NetSounder nsReq;
	nsReq.type = 3;
	nsReq.isContinous = true;

	VDES::NetSounder::NetInfo nsNet1;
	nsNet1.MRN = 77777;
	nsNet1.latitude = 38.5;
	nsNet1.longitude = 118.2;
	nsNet1.fragmentDesc = 0;
	nsNet1.description = "Net 1";
	nsReq.nets.push_back(nsNet1);

	VDES::NetSounder::NetInfo nsNet2;
	nsNet2.MRN = 77778;
	nsNet2.latitude = 38.6;
	nsNet2.longitude = 118.3;
	nsNet2.fragmentDesc = 0;
	nsNet2.description = "Net 2";
	nsReq.nets.push_back(nsNet2);

	std::cout << "Sending NetSounder request..." << std::endl;
	vdesManager.SendNetSounder(nsReq); // Should be seqNo incremented.
	// Since RouteRecommendationRequest sent twice (seq 3, seq 4), NetSounder request should be seq 5.

	// Retrieve the sequence number of the last inserted own NetSounder (highest dataID)
	uint32_t lastSeqNo = 0;
	auto dbNSList = vdesManager.GetNetSounders(0, 100);
	if (!dbNSList.empty())
	{
		uint32_t maxID = 0;
		for (const auto &ns : dbNSList)
		{
			if (ns.dataID > maxID)
			{
				maxID = ns.dataID;
				lastSeqNo = ns.sequenceNum;
			}
		}
	}
	std::cout << "Detected NetSounder seqNo: " << lastSeqNo << std::endl;

	// Simulate AMK response (seqNo=lastSeqNo, ackType=0) from transceiver
	std::cout << "Simulating AMK response (seqNo=" << lastSeqNo << ", ackType=0) for NetSounder..." << std::endl;
	std::string nsAmk = fmt::format("$AIAMK,004129999,1,,{0},0", lastSeqNo);
	VDES::UtilityInterface::AddChecksum(nsAmk);
	nsAmk += "\r\n";
	vdesManager.Parse(nsAmk.c_str(), nsAmk.length());

	std::cout << "gotSuccess (NetSounder Req) = " << (gotSuccess ? "TRUE" : "FALSE") << std::endl;

	// Verify database record has send status updated to 0 (success)
	dbNSList = vdesManager.GetNetSounders(0, 100);
	if (!dbNSList.empty())
	{
		for (const auto &ns : dbNSList)
		{
			std::cout << "  - DB NetSounder ID: " << ns.dataID << ", seq: " << ns.sequenceNum << ", sendStatus: " << ns.sendStatus << std::endl;
			if (ns.sequenceNum == lastSeqNo)
			{
				std::cout << "Database NetSounder seq " << lastSeqNo << " send status: " << ns.sendStatus << " (Expected: 0)" << std::endl;
			}
		}
	}

	// Timeout test
	gotTimeout = false;
	std::cout << "\nSending NetSounder request for timeout test..." << std::endl;
	vdesManager.SendNetSounder(nsReq); // seq 6

	uint32_t timeoutSeqNo = 0;
	dbNSList = vdesManager.GetNetSounders(0, 100);
	if (!dbNSList.empty())
	{
		uint32_t maxID = 0;
		for (const auto &ns : dbNSList)
		{
			if (ns.dataID > maxID)
			{
				maxID = ns.dataID;
				timeoutSeqNo = ns.sequenceNum;
			}
		}
	}
	std::cout << "Detected NetSounder timeout seqNo: " << timeoutSeqNo << std::endl;

	std::cout << "Waiting 11 seconds for NetSounder timeout..." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(11));
	std::cout << "NetSounder Req Timeout check finished." << std::endl;

	// Verify database record has send status updated to 4 (timeout)
	dbNSList = vdesManager.GetNetSounders(0, 100);
	if (!dbNSList.empty())
	{
		for (const auto &ns : dbNSList)
		{
			std::cout << "  - DB NetSounder ID: " << ns.dataID << ", seq: " << ns.sequenceNum << ", sendStatus: " << ns.sendStatus << std::endl;
			if (ns.sequenceNum == timeoutSeqNo)
			{
				std::cout << "Database NetSounder seq " << timeoutSeqNo << " send status: " << ns.sendStatus << " (Expected: 4)" << std::endl;
			}
		}
	}
	std::cout << "==========================================================" << std::endl;

	// Verify NetSounder deletion
	std::cout << "\n=== Verification: NetSounder Deletion ===" << std::endl;
	auto testNSList = GenerateDAC_412_FI_45();
	for (const auto &vdm : testNSList) vdesManager.Parse(vdm.c_str(), vdm.length());

	auto dbNSBefore = vdesManager.GetNetSounders(0, 100);
	std::cout << "NetSounders count before delete: " << dbNSBefore.size() << std::endl;
	for (const auto &ns : dbNSBefore)
	{
		std::cout << "  NetSounder ID: " << ns.dataID << ", Type: " << (int)ns.type << ", Nets count: " << ns.nets.size() << std::endl;
	}

	if (!dbNSBefore.empty())
	{
		uint32_t targetID = dbNSBefore[0].dataID;
		std::cout << "Calling DeleteNetSounder with dataID: " << targetID << std::endl;
		vdesManager.DeleteNetSounder(targetID);
		
		auto dbNSAfter = vdesManager.GetNetSounders(0, 100);
		std::cout << "NetSounders count after delete: " << dbNSAfter.size() << std::endl;
	}
	
	// Clean up NetSounders table using batch delete
	if (!vdesManager.GetNetSounders(0, 100).empty())
	{
		std::vector<uint32_t> ids;
		for (const auto &ns : vdesManager.GetNetSounders(0, 100)) ids.push_back(ns.dataID);
		vdesManager.DeleteNetSounders(ids);
	}
	std::cout << "NetSounders count after batch delete: " << vdesManager.GetNetSounders(0, 100).size() << std::endl;
	std::cout << "========================================" << std::endl;

	// Verify RouteExchange (FI=48)
	std::cout << "\n=== Verification: RouteExchange (FI=48) ===" << std::endl;
	bool gotRouteExchange = false;
	auto token = vdesManager.notifyEvent.append([&](const VDES::VDESManager::EventType type, const int retCode) {
		if (type == VDES::VDESManager::EventType::ASM_ROUTE_EXCHANGE) {
			std::cout << "[EVENT] Got ASM_ROUTE_EXCHANGE callback!" << std::endl;
			gotRouteExchange = true;
		}
	});

	auto routeVDMs = GenerateDAC_412_FI_48();
	for (const auto &vdm : routeVDMs) {
		vdesManager.Parse(vdm.c_str(), vdm.length());
	}

	auto dbRoutes = vdesManager.GetRouteExchanges(0, 100);
	std::cout << "RouteExchanges count in DB: " << dbRoutes.size() << std::endl;
	for (const auto &route : dbRoutes) {
		std::cout << "  - Route ID: " << route.dataID 
		          << ", Sender MMSI: " << route.mmsiSender 
		          << ", Version: " << (int)route.routeVersion 
		          << ", StartTime: " << route.startTime 
		          << ", Waypoints count: " << route.waypoints.size() << std::endl;

		for (size_t i = 0; i < route.waypoints.size(); ++i) {
			const auto &wp = route.waypoints[i];
			std::cout << "    * Waypoint #" << (i+1) 
			          << ": Lat: " << wp.coordinate.GetLatitude() 
			          << ", Lon: " << wp.coordinate.GetLongitude() 
			          << ", TimeUnit: " << (int)wp.timeUnit 
			          << ", Duration: " << wp.duration << std::endl;
		}
	}

	// Verify delete
	if (!dbRoutes.empty()) {
		std::cout << "Deleting RouteExchange ID: " << dbRoutes[0].dataID << std::endl;
		std::vector<uint32_t> ids = { dbRoutes[0].dataID };
		vdesManager.DeleteRouteExchanges(ids);
		auto dbRoutesAfter = vdesManager.GetRouteExchanges(0, 100);
		std::cout << "RouteExchanges count in DB after delete: " << dbRoutesAfter.size() << std::endl;
	}

	vdesManager.notifyEvent.remove(token);
	std::cout << "========================================" << std::endl;

	// Execute VDES ABB message test case
	TestABBTextMessages();

	std::cout << "================================================================================" << std::endl;

	return 0;
}
