/* Copyright 2022 Matteo Grasso
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "Components/LampComponent.hpp"
#include "Components/MainComponent.hpp"


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleCppExampleModule
		: public AZ::Module
	{
	public:
		AZ_RTTI(DayNightCycleCppExampleModule, "{A12FF726-9809-4959-9AC8-8BED137586C7}", AZ::Module);
		AZ_CLASS_ALLOCATOR(DayNightCycleCppExampleModule, AZ::SystemAllocator, 0);

		DayNightCycleCppExampleModule()
			: AZ::Module()
		{
			m_descriptors.insert(m_descriptors.end(),
			{
				LampComponent::CreateDescriptor(),
				MainComponent::CreateDescriptor()
			});
		}

		AZ::ComponentTypeList GetRequiredSystemComponents() const override
		{
			return AZ::ComponentTypeList {};
		}
	};

}// Loherangrin::Gems::DayNightCycle

AZ_DECLARE_MODULE_CLASS(Gem_Loherangrin_DayNightCycle_CppExample, Loherangrin::Gems::DayNightCycle::DayNightCycleCppExampleModule)
