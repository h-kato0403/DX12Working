#pragma once
#include <vector>
#include <map>

template <typename EnumType>
class AssetLoadBase
{
public:
	virtual void Load() = 0;

protected:
	std::map<EnumType, const char*> assetFilePaths;
};

