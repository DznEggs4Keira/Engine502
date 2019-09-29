#ifndef _ASSIMPMODELCLASS_H_
#define _ASSIMPMODELCLASS_H_

//My class includes
#include "MeshClass.h"

#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>

class AssimpModelClass
{
public:
	AssimpModelClass();
	~AssimpModelClass();

	bool Initialize(ID3D11Device*, const std::string &);
	std::vector<MeshClass> m_Meshes;

private:
	
	bool LoadModel(const std::string &);
	void ProcessNode(aiNode*, const aiScene*);
	MeshClass ProcessMesh(aiMesh*, const aiScene*);


	ID3D11Device* m_device;
};

#endif