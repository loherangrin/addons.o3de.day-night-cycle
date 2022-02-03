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

#include "Components/DayNightCycleComponent.hpp"


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleModuleInterface
		: public AZ::Module
	{
	public:
		AZ_RTTI(DayNightCycleModuleInterface, "{FEE28DFC-4C55-42F7-A67D-63E3ABB378B6}", AZ::Module);
		AZ_CLASS_ALLOCATOR(DayNightCycleModuleInterface, AZ::SystemAllocator, 0);

		DayNightCycleModuleInterface()
		{
			m_descriptors.insert(m_descriptors.end(),
			{
				DayNightCycleComponent::CreateDescriptor(),
			});
		}

		AZ::ComponentTypeList GetRequiredSystemComponents() const override
		{
			return AZ::ComponentTypeList {};
		}
	};

} // Loherangrin::Gems::DayNightCycle
