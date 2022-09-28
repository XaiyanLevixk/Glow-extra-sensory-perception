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

struct Color
{
	constexpr Color(float r, float g, float b, float a = 1.f) noexcept :
		r(r), g(g), b(b), a(a) { }

	float r, g, b, a;
};

int main()
{
	auto mem = Memory("csgo.exe");

	const auto client = mem.GetModuleAddress("client.dll");

	const auto color = Color(1.f, 1.f, 0.f);

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

			mem.Write<Color>(glowObjectManager + (glowIndex * 0x38) + 0x8, color);

			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x27, true);
			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
		}
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}