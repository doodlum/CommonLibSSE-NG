#pragma once

#include "skse64/GameRTTI.h"  // RTTI_BarterMenu

#include "RE/BSString.h"  // BSString
#include "RE/BSTArray.h"  // BSTArray
#include "RE/GFxValue.h"  // GFxValue
#include "RE/IMenu.h"  // IMenu


namespace RE
{
	class ItemList;


	class BarterMenu : public IMenu
	{
	public:
		inline static const void* RTTI = RTTI_BarterMenu;


		struct Data
		{
			GFxValue			unk00;	// 00 - kObject
			BSString			unk18;	// 18
			UInt64				unk28;	// 28
			UInt32				unk30;	// 30
			UInt32				pad34;	// 34
			GPtr<GFxMovieView>	view;	// 38
		};
		STATIC_ASSERT(sizeof(Data) == 0x40);


		virtual ~BarterMenu();												// 00

		// override (IMenu)
		virtual void	Accept(CallbackProcessor* a_processor) override;	// 01
		virtual Result	ProcessMessage(UIMessage* a_message) override;		// 04
		virtual void	Render() override;									// 06


		// members
		ItemList*		list;				// 30
		Data*			unk38;				// 38
		GFxValue*		unk40;				// 40 - kObject
		GFxValue		root;				// 48 - "Menu_mc"
		UInt64			unk60;				// 60
		UInt64			unk68;				// 68
		UInt64			unk70;				// 70
		UInt8			unk78;				// 78
		UInt8			pad79;				// 79
		UInt16			pad7A;				// 7A
		UInt32			pad7C;				// 7C
		BSTArray<void*>	unk80;				// 80
		UInt64			unk98;				// 98
		UInt32			unkA0;				// A0
		bool			pcControlsReady;	// A4
		UInt8			padA5;				// A5
		UInt16			padA6;				// A6
	};
	STATIC_ASSERT(sizeof(BarterMenu) == 0xA8);
}