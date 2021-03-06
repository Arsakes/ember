//
// C++ Implementation: EmberTerrainPageBridge
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#include "OgrePagingLandScapePrecompiledHeaders.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberTerrainPageBridge.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "EmberPagingLandScapeData2D_HeightField.h"
#include "terrain/TerrainPageGeometry.h"

namespace Ember
{
namespace OgreView
{

EmberTerrainPageBridge::EmberTerrainPageBridge(Ogre::PagingLandScapeData2DManager& data2dManager, const boost::shared_array<Ogre::Real>& heightData, size_t heightDataSize, UnsignedIndexType index) :
	mData2dManager(data2dManager), mHeightData(heightData), mHeightDataSize(heightDataSize), mIndex(index), mMaxHeight(0)
{
}

EmberTerrainPageBridge::~EmberTerrainPageBridge()
{
}

void EmberTerrainPageBridge::updateTerrain(Terrain::TerrainPageGeometry& geometry)
{
#if OGRE_DOUBLE_PRECISION
	//Mercator works with floats, so if we've built Ogre in double precision mode we need to send a float bitmap to Mercator, and then convert to double.
	std::vector<float> floatHeightData(mHeightDataSize);
	float* floatData = &floatHeightData.front();
	geometry.updateOgreHeightData(floatData);

	Ogre::Real* doubleData = mHeightData.get();
	for (size_t i = 0; i < mHeightDataSize; ++i) {
		doubleData[i] = (double)floatData[i];
	}
#else
	geometry.updateOgreHeightData(mHeightData.get());
#endif
	mMaxHeight = geometry.getMaxHeight();
}

void EmberTerrainPageBridge::terrainPageReady()
{
	EmberPagingLandScapeData2D_HeightField* heightField = getData2D();
	if (heightField) {
		heightField->setMaxHeight(mMaxHeight);
		heightField->eventTerrainPageLoaded();
	}
}

EmberPagingLandScapeData2D_HeightField* EmberTerrainPageBridge::getData2D()
{
	return static_cast<EmberPagingLandScapeData2D_HeightField*> (mData2dManager.getData2D(mIndex.first, mIndex.second, false));
}

}
}
