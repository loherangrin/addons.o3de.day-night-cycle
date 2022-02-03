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

#include "EditorComponents/DayNightCycleEditorComponent.hpp"

#include "DayNightCycleModuleInterface.hpp"


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleEditorModule
		: public DayNightCycleModuleInterface
	{
	public:
		AZ_RTTI(DayNightCycleEditorModule, "{12BF23E7-3544-414F-8F6A-BBE8A926C62E}", DayNightCycleModuleInterface);
		AZ_CLASS_ALLOCATOR(DayNightCycleEditorModule, AZ::SystemAllocator, 0);

		DayNightCycleEditorModule()
		{
			m_descriptors.insert(m_descriptors.end(),
			{
				DayNightCycleEditorComponent::CreateDescriptor()			
			});
		}

		AZ::ComponentTypeList GetRequiredSystemComponents() const override
		{
			return AZ::ComponentTypeList {};
		}
	};

}// Loherangrin::Gems::DayNightCycle

AZ_DECLARE_MODULE_CLASS(Gem_Loherangrin_DayNightCycle, Loherangrin::Gems::DayNightCycle::DayNightCycleEditorModule)
