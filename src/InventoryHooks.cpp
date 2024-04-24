#include "MiscHooks.h"
#include "InventoryHooks.h"
#include "Processor.h"

namespace Hook
{

	//HUGE Credits to Nightfallstorm for some of the hooks!!
	//https://github.com/Nightfallstorm/DescriptionFramework released under GPL-3.0
	//https://github.com/Nightfallstorm/Recipes released under GPL-3.0
	struct ItemCardPopulateHook //BOOK CNAM
	{
		static void thunk(RE::ItemCard* itemCard, RE::TESBoundObject** a_item, char a3)
		{
			if (!itemCard || !a_item || !*a_item)
			{
				return func(itemCard, a_item, a3);
			}

			func(itemCard, a_item, a3);
			handleDescriptionItems(itemCard, *a_item);
		};

		static void handleDescriptionItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordType::kCNAM:
				{
					// Replace the ItemCardDescription of books
					if (a_item->formID == Information.Form->formID)
					{
						auto descriptionValue = RE::GFxValue(Information.ReplacerText);
						itemCard->obj.SetMember("description", descriptionValue);

						SKSE::log::debug("Replaced BOOK CNAM {0:08X} with:", a_item->formID);
						SKSE::log::debug("{}", Information.ReplacerText);
					}
				}
				break;

				default: break;
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			std::array targets{
				std::make_pair(RELOCATION_ID(50005, 50949), 0x80),
				std::make_pair(RELOCATION_ID(50201, 51130), 0xB2),
				std::make_pair(RELOCATION_ID(50297, 51218), 0x35),
				std::make_pair(RELOCATION_ID(50674, 51569), REL::Relocate(0x80, 0x7A, 0x80)),
				std::make_pair(RELOCATION_ID(50973, 51852), REL::Relocate(0x80, 0x7A, 0x80)),
			};

			for (auto& [id, offset] : targets)
			{
				REL::Relocation<std::uintptr_t> target(id, offset);
				stl::write_thunk_call<ItemCardPopulateHook>(target.address());
				SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target.address(), target.offset());
			}

			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(0, 51458), REL::VariantOffset(0x0, 0x87, 0x0) };
				stl::write_thunk_call<ItemCardPopulateHook>(target.address());
				SKSE::log::info("ItemCardPopulateHook hooked at address: {:x} and offset: {:x}", target.address(), target.offset());
			}
		}
	};

	void InstallInventoryHooks()
	{
		ItemCardPopulateHook::Install();
	}

}