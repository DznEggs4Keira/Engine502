#include "AssimpModelClass.h"

AssimpModelClass::AssimpModelClass()
{
	m_device = 0;
}

AssimpModelClass::~AssimpModelClass()
{
}

bool AssimpModelClass::Initialize(ID3D11Device* device, const std::string & modelFilename)
{
	bool result;

	m_device = device;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}
}

bool AssimpModelClass::LoadModel(const std::string & filename)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, 
											aiProcess_Triangulate | 
											aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		return false;
	}

	this->ProcessNode(scene->mRootNode, scene);
	return true;
}

void AssimpModelClass::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

MeshClass* AssimpModelClass::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Get vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texture.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texture.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		vertices.push_back(vertex);
	}

	// Get indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	/*
	// Get textures
	aiMaterial *pMaterial = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<ID3D11ShaderResourceView*> textures = LoadMaterialTextures(pMaterial, aiTextureType::aiTextureType_DIFFUSE, pScene);
	*/

	// Create mesh
	MeshClass *m_Meshes = new MeshClass(m_device, vertices, indices); //Calls initialise in mesh class
	return m_Meshes;
}


