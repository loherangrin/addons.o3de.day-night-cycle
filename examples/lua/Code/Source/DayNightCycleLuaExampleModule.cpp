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


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleLuaExampleModule
		: public AZ::Module
	{
	public:
		AZ_RTTI(DayNightCycleLuaExampleModule, "{B484B7E2-C520-4014-879D-B35C293EB325}", AZ::Module);
		AZ_CLASS_ALLOCATOR(DayNightCycleLuaExampleModule, AZ::SystemAllocator, 0);

		DayNightCycleLuaExampleModule()
			: AZ::Module()
		{
			m_descriptors.insert(m_descriptors.end(), {});
		}

		AZ::ComponentTypeList GetRequiredSystemComponents() const override
		{
			return AZ::ComponentTypeList {};
		}
	};

} // Loherangrin::Gems::DayNightCycle

AZ_DECLARE_MODULE_CLASS(Gem_Loherangrin_DayNightCycle_LuaExample, Loherangrin::Gems::DayNightCycle::DayNightCycleLuaExampleModule)
