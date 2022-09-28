#include "memory.h"
#include<thread>

using namespace std;

namespace offsets
{
	// client.dll
	constexpr auto localPlayer = 0xDC14CC;
	constexpr auto entityList = 0x4DDD93C;
	constexpr auto glowObjectManager = 0x5326638;

	// entity
	constexpr auto teamNum = 0xF4;
	constexpr auto glowIndex = 0x10488;
}

int main()
{
	auto mem = Memory("csgo.exe");

	const auto client = mem.GetModuleAddress("client.dll");

	while (true)
	{
		const auto localPlayer = mem.Read<uintptr_t>(client + offsets::localPlayer);

		const auto glowObjectManager = mem.Read<uintptr_t>(client + offsets::glowObjectManager);

		for (auto i = 0; i < 64; i++)
		{
			const auto  entity = mem.Read<uintptr_t>(client + offsets::entityList + 0x10 * i);

			if (mem.Read<uintptr_t>(entity + offsets::teamNum) == mem.Read<uintptr_t>(localPlayer + offsets::teamNum))
				continue;

			const auto glowIndex = mem.Read<int32_t>(entity + offsets::glowIndex);

			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, 1.f);
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, 0.f);
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, 0.f);
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, 1.f);

			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x27, true);
			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
		}
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}