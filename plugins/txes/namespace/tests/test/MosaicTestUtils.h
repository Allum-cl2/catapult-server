/**
*** Copyright (c) 2016-present,
*** Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp. All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#pragma once
#include "src/state/MosaicEntry.h"

namespace catapult { namespace test {

	/// Creates mosaic properties with a custom \a duration.
	model::MosaicProperties CreateMosaicPropertiesWithDuration(BlockDuration duration);

	/// Creates a mosaic definition with \a height.
	state::MosaicDefinition CreateMosaicDefinition(Height height);

	/// Creates a mosaic entry with \a id and \a supply.
	state::MosaicEntry CreateMosaicEntry(MosaicId id, Amount supply);

	/// Creates a mosaic entry with \a id, \a height and \a supply.
	state::MosaicEntry CreateMosaicEntry(MosaicId id, Height height, Amount supply);

	/// Creates a mosaic entry around \a namespaceId, \a id, \a height, \a owner, \a supply and \a duration.
	std::shared_ptr<const state::MosaicEntry> CreateMosaicEntry(
			NamespaceId namespaceId,
			MosaicId id,
			Height height,
			const Key& owner,
			Amount supply,
			BlockDuration duration);

	/// Asserts that actual properties (\a actualProperties) exactly match expected properties (\a expectedProperties).
	void AssertMosaicDefinitionProperties(
			const model::MosaicProperties& expectedProperties,
			const model::MosaicProperties& actualProperties);
}}
