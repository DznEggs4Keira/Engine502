#ifndef _ASSIMPMODELCLASS_H_
#define _ASSIMPMODELCLASS_H_

//My class includes
#include "MeshClass.h"

#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>

#include "textureclass.h"

#include "Utils.h"

class AssimpModelClass
{
public:
	AssimpModelClass();
	~AssimpModelClass();

	bool Initialize(ID3D11Device*, const std::string &, WCHAR*);
	ID3D11ShaderResourceView* GetTexture();

public:
	std::vector<MeshClass*> m_Meshes;

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

private:
	bool LoadModel(const std::string &);
	void ProcessNode(aiNode*, const aiScene*);
	MeshClass* ProcessMesh(aiMesh*, const aiScene*);

	bool LoadTexture(ID3D11Device*, WCHAR*);

private:
	ID3D11Device* m_device;
	TextureClass* m_Texture;
};

#endif