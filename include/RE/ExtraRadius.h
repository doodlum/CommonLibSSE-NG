#pragma once

#include "skse64/GameRTTI.h"  // RTTI_ExtraRadius

#include "RE/BSExtraData.h"  // BSExtraData
#include "RE/ExtraDataTypes.h"  // ExtraDataType


namespace RE
{
	class ExtraRadius : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraRadius;


		enum { kExtraTypeID = ExtraDataType::kRadius };


		virtual ~ExtraRadius();															// 00

		// override (BSExtraData)
		virtual ExtraDataType	GetType() const override;								// 01 - { return kRadius; }
		virtual bool			IsNotEqual(const BSExtraData* a_rhs) const override;	// 02 - { return unk10 != a_rhs->unk10; }


		// members
		float	unk10;	// 10
		UInt32	pad14;	// 14
	};
	STATIC_ASSERT(sizeof(ExtraRadius) == 0x18);
}
