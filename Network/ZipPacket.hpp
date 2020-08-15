#pragma once
#include "Packet.hpp"
#include "lz4.h"

namespace swl
{
	class ZipPacket : public Packet
	{
	protected:
		const void* onSend(uint32_t& size) override;
		void onReceive(const void* data, const uint32_t& size) override;
	};
}