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

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>

#include "../Components/DayNightCycleComponentConfig.hpp"


namespace Loherangrin::Gems::DayNightCycle
{
	class DayNightCycleEditorComponent
		: public AzToolsFramework::Components::EditorComponentBase
	{
	public:
		AZ_EDITOR_COMPONENT(DayNightCycleEditorComponent, "{C76E8E57-0E53-4166-960B-E997C133355C}", AzToolsFramework::Components::EditorComponentBase);
		static void Reflect(AZ::ReflectContext* io_context);

		static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& io_provided);
		static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& io_incompatible);
		static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& io_required);
		static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& io_dependent);

	protected:
		// AZ::Component
		void Init() override;
		void Activate() override;
		void Deactivate() override;

		// AzToolsFramework::Components::EditorComponentBase
		void BuildGameEntity(AZ::Entity* io_gameEntity) override;

	private:
		DayNightCycleComponentConfig m_config;
	};

} // Loherangrin::Gems::DayNightCycle
