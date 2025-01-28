#pragma once
#define NOMINMAX
#include <d3d12.h>
#include <directxmath.h>
#include <vector>
#include <string>

struct Mesh;
struct Vertex;
struct aiMesh;
struct aiMaterial;

struct ImportSettings
{
	const wchar_t* fileName = nullptr;
	std::vector<Mesh>& meshes;
	bool inverseU = false;
	bool inverseV = false;
};

class AssimpLoader
{
public:
	bool Load(ImportSettings setting); // ƒ‚ƒfƒ‹‚ğƒ[ƒh‚·‚é

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};

