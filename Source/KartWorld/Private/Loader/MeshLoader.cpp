// Copyright @ 2023 Fynn Haupt

#include "Loader/MeshLoader.h"

DEFINE_LOG_CATEGORY(LogMeshLoader);

EMeshLoadingResult UMeshLoader::LoadRelative(
	FString FilePath,
	FModelData &ModelData)
{
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogMeshLoader, Error, TEXT("FilePath is empty!"));
		return EMeshLoadingResult_NOFILE;
	}

	FString FolderPath;
	FString FileName;
	FString FileExtension;
	FPaths::Split(FilePath, FolderPath, FileName, FileExtension);

	FString LodFileName(FileName + ".lod");
	FString LodFilePath = FPaths::Combine(FolderPath, LodFileName);

	// Create scene
	Scene = Importer.ReadFile(
		TCHAR_TO_UTF8(*FilePath),
		aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_MakeLeftHanded);

	if (Scene == nullptr)
	{
		UE_LOG(LogMeshLoader, Error, TEXT("%s"), Importer.GetErrorString());
		return EMeshLoadingResult_NOSCENE;
	}

	if (!Scene->HasMeshes())
	{
		UE_LOG(LogMeshLoader, Warning, TEXT("File has no meshes!"));
		return EMeshLoadingResult_NOMESHES;
	}

	// Fix root node rotation
	aiMatrix4x4 FixRotateX;
	aiMatrix4x4::RotationX(FMath::DegreesToRadians(-90.0f), FixRotateX);
	aiMatrix4x4 FixRotateY;
	aiMatrix4x4::RotationY(FMath::DegreesToRadians(-90.0f), FixRotateY);
	aiMatrix4x4 FixRotateZ;
	aiMatrix4x4::RotationZ(FMath::DegreesToRadians(180.0f), FixRotateZ);
	Scene->mRootNode->mTransformation *= (FixRotateX * FixRotateY * FixRotateZ);

	// Setup array lengths
	ModelData.Meshes.SetNum(Scene->mNumMeshes);

	// Load Meshes
	for (int32 MeshIndex = 0; MeshIndex < ModelData.Meshes.Num(); MeshIndex++)
	{
		FMeshData &MeshData = ModelData.Meshes[MeshIndex];
		aiMesh *Mesh = Scene->mMeshes[MeshIndex];
		aiNode *Node = GetParentNode(Scene->mRootNode, MeshIndex);

		// Material id
		MeshData.MaterialId = Mesh->mMaterialIndex;

		// Lod Data
		GetLodData(LodFilePath, FString(Node->mName.C_Str()), MeshData.LodData);

		// Vertices
		for (uint32 VertexIndex = 0; VertexIndex < Mesh->mNumVertices; VertexIndex++)
		{
			// Position
			aiVector3D &aiVertex = Mesh->mVertices[VertexIndex];
			FVector3f Position(
				aiVertex.x,
				aiVertex.y,
				aiVertex.z);

			// Normal
			FVector3f Normal = FVector3f::ZeroVector;
			if (Mesh->HasNormals())
			{
				aiVector3D &aiNormal = Mesh->mNormals[VertexIndex];
				Normal = FVector3f(
					aiNormal.x,
					aiNormal.y,
					aiNormal.z);
			}

			// Tangent
			FVector3f Tangent = FVector3f::ZeroVector;
			if (Mesh->HasTangentsAndBitangents())
			{
				aiVector3D &aiTangent = Mesh->mTangents[VertexIndex];
				Tangent = FVector3f(
					aiTangent.x,
					aiTangent.y,
					aiTangent.z);
			}

			// Linear Color
			FLinearColor LinearColor = FLinearColor::White;
			if (Mesh->HasVertexColors(0))
			{
				aiColor4D &aiColor = Mesh->mColors[0][VertexIndex];
				LinearColor = FLinearColor(
					aiColor.r,
					aiColor.g,
					aiColor.b,
					aiColor.a);
			}

			// UVs
			FVector2f UV0 = FVector2f::ZeroVector;
			FVector2f UV1 = FVector2f::ZeroVector;
			FVector2f UV2 = FVector2f::ZeroVector;
			FVector2f UV3 = FVector2f::ZeroVector;
			for (uint32 ChannelIndex = 0; ChannelIndex <= 3; ChannelIndex++)
			{
				if (Mesh->HasTextureCoords(ChannelIndex))
				{
					aiVector3D &aiCoordinate = Mesh->mTextureCoords[ChannelIndex][VertexIndex];
					FVector2f Coordinate(aiCoordinate.x, -aiCoordinate.y);

					switch (ChannelIndex)
					{
					case 0:
						UV0 = Coordinate;
						break;
					case 1:
						UV1 = Coordinate;
						break;
					case 2:
						UV2 = Coordinate;
						break;
					case 3:
						UV3 = Coordinate;
						break;
					}
				}
			}

			// Create vertex
			FVertexData Vertex;
			Vertex.Position = Position;
			Vertex.Normal = Normal;
			Vertex.Tangent = Tangent;
			Vertex.Color = LinearColor;
			Vertex.UV0 = UV0;
			Vertex.UV1 = UV1;
			Vertex.UV2 = UV2;
			Vertex.UV3 = UV3;

			// Save vertex
			MeshData.Verticies.Push(Vertex);
		}

		// Triangles
		for (uint32 FaceIndex = 0; FaceIndex < Mesh->mNumFaces; FaceIndex++)
		{
			aiFace &Face = Mesh->mFaces[FaceIndex];
			if (Face.mNumIndices < 3)
				continue;

			// Create triangle
			FTriangleData Triangle;
			// FTriangleData Triangle2;

			Triangle.UV0 = Face.mIndices[0];
			// Triangle2.UV0 = Face.mIndices[2];
			Triangle.UV1 = Face.mIndices[1];
			// Triangle2.UV1 = Face.mIndices[1];
			Triangle.UV2 = Face.mIndices[2];
			// Triangle2.UV2 = Face.mIndices[0];

			// Save triangle
			MeshData.Triangles.Push(Triangle);
			// MeshData.Triangles.Push(Triangle2);
		}
	}

	// Load Materials
	UMaterialLoader *MaterialLoader = NewObject<UMaterialLoader>();
	ModelData.Materials = MaterialLoader->LoadMaterials(FolderPath, Scene);

	// Node Hierarchy
	ModelData.NodeHierarchy = GetNodeHierarchy(Scene->mRootNode);

	return EMeshLoadingResult_OK;
}

EMeshLoadingResult UMeshLoader::LoadWorld(
	FString FilePath,
	FModelData &ModelData)
{
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogMeshLoader, Error, TEXT("FilePath is empty!"));
		return EMeshLoadingResult_NOFILE;
	}

	FString FolderPath;
	FString FileName;
	FString FileExtension;
	FPaths::Split(FilePath, FolderPath, FileName, FileExtension);

	FString LodFileName(FileName + ".lod");
	FString LodFilePath = FPaths::Combine(FolderPath, LodFileName);

	// Create scene
	Scene = Importer.ReadFile(
		TCHAR_TO_UTF8(*FilePath),
		aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_MakeLeftHanded);

	if (Scene == nullptr)
	{
		UE_LOG(LogMeshLoader, Error, TEXT("%s"), Importer.GetErrorString());
		return EMeshLoadingResult_NOSCENE;
	}

	if (!Scene->HasMeshes())
	{
		UE_LOG(LogMeshLoader, Warning, TEXT("File has no meshes!"));
		return EMeshLoadingResult_NOMESHES;
	}

	// Fix root node rotation
	aiMatrix4x4 FixRotateX;
	aiMatrix4x4::RotationX(FMath::DegreesToRadians(-90.0f), FixRotateX);
	aiMatrix4x4 FixRotateY;
	aiMatrix4x4::RotationY(FMath::DegreesToRadians(-90.0f), FixRotateY);
	aiMatrix4x4 FixRotateZ;
	aiMatrix4x4::RotationZ(FMath::DegreesToRadians(180.0f), FixRotateZ);
	Scene->mRootNode->mTransformation *= (FixRotateX * FixRotateY * FixRotateZ);

	// Setup array lengths
	ModelData.Meshes.SetNum(Scene->mNumMeshes);

	// Load Meshes
	for (int32 MeshIndex = 0; MeshIndex < ModelData.Meshes.Num(); MeshIndex++)
	{
		FMeshData &MeshData = ModelData.Meshes[MeshIndex];
		aiMesh *Mesh = Scene->mMeshes[MeshIndex];
		aiNode *Node = GetParentNode(Scene->mRootNode, MeshIndex);

		// Get world transform
		FTransform Transform = GetWorldTransformOfNode(Node);

		// Material id
		MeshData.MaterialId = Mesh->mMaterialIndex;

		// Lod Data
		GetLodData(LodFilePath, FString(Mesh->mName.C_Str()), MeshData.LodData);

		// Vertices
		for (uint32 VertexIndex = 0; VertexIndex < Mesh->mNumVertices; VertexIndex++)
		{
			// Position
			aiVector3D &aiVertex = Mesh->mVertices[VertexIndex];
			FVector PositionVertex = Transform.TransformFVector4(FVector(
				aiVertex.x,
				aiVertex.y,
				aiVertex.z));

			FVector3f Position(
				PositionVertex.X,
				PositionVertex.Y,
				PositionVertex.Z);

			// Normal
			FVector3f Normal = FVector3f::ZeroVector;
			if (Mesh->HasNormals())
			{
				aiVector3D &aiNormal = Mesh->mNormals[VertexIndex];
				FVector NormalVector = Transform.GetRotation().RotateVector(FVector(
					aiNormal.x,
					aiNormal.y,
					aiNormal.z
				));

				Normal = FVector3f(
					NormalVector.X,
					NormalVector.Y,
					NormalVector.Z);
			}

			// Tangent
			FVector3f Tangent = FVector3f::ZeroVector;
			if (Mesh->HasTangentsAndBitangents())
			{
				aiVector3D &aiTangent = Mesh->mTangents[VertexIndex];
				FVector TangentVector = Transform.GetRotation().RotateVector(FVector(
					aiTangent.x,
					aiTangent.y,
					aiTangent.z
				));

				Tangent = FVector3f(
					TangentVector.X,
					TangentVector.Y,
					TangentVector.Z);
			}

			// Linear Color
			FLinearColor LinearColor = FLinearColor::White;
			if (Mesh->HasVertexColors(0))
			{
				aiColor4D &aiColor = Mesh->mColors[0][VertexIndex];
				LinearColor = FLinearColor(
					aiColor.r,
					aiColor.g,
					aiColor.b,
					aiColor.a);
			}

			// UVs
			FVector2f UV0 = FVector2f::ZeroVector;
			FVector2f UV1 = FVector2f::ZeroVector;
			FVector2f UV2 = FVector2f::ZeroVector;
			FVector2f UV3 = FVector2f::ZeroVector;
			for (uint32 ChannelIndex = 0; ChannelIndex <= 3; ChannelIndex++)
			{
				if (Mesh->HasTextureCoords(ChannelIndex))
				{
					aiVector3D &aiCoordinate = Mesh->mTextureCoords[ChannelIndex][VertexIndex];
					FVector2f Coordinate(aiCoordinate.x, -aiCoordinate.y);

					switch (ChannelIndex)
					{
					case 0:
						UV0 = Coordinate;
						break;
					case 1:
						UV1 = Coordinate;
						break;
					case 2:
						UV2 = Coordinate;
						break;
					case 3:
						UV3 = Coordinate;
						break;
					}
				}
			}

			// Create vertex
			FVertexData Vertex;
			Vertex.Position = Position;
			Vertex.Normal = Normal;
			Vertex.Tangent = Tangent;
			Vertex.Color = LinearColor;
			Vertex.UV0 = UV0;
			Vertex.UV1 = UV1;
			Vertex.UV2 = UV2;
			Vertex.UV3 = UV3;

			// Save vertex
			MeshData.Verticies.Push(Vertex);
		}

		// Triangles
		for (uint32 FaceIndex = 0; FaceIndex < Mesh->mNumFaces; FaceIndex++)
		{
			aiFace &Face = Mesh->mFaces[FaceIndex];
			if (Face.mNumIndices < 3)
				continue;

			// Create triangle
			FTriangleData Triangle;

			Triangle.UV0 = Face.mIndices[0];
			Triangle.UV1 = Face.mIndices[1];
			Triangle.UV2 = Face.mIndices[2];

			// Save triangle
			MeshData.Triangles.Push(Triangle);
		}
	}

	// Load Materials
	UMaterialLoader *MaterialLoader = NewObject<UMaterialLoader>();
	ModelData.Materials = MaterialLoader->LoadMaterials(FolderPath, Scene);

	return EMeshLoadingResult_OK;
}

FTransform UMeshLoader::GetTransformOfNode(aiNode *Node)
{
	// Convert Assimp Matrix to FMatrix
	aiMatrix4x4 aiTransform = Node->mTransformation;

	FMatrix Matrix;
	Matrix.M[0][0] = aiTransform.a1;
	Matrix.M[0][1] = aiTransform.b1;
	Matrix.M[0][2] = aiTransform.c1;
	Matrix.M[0][3] = aiTransform.d1;
	Matrix.M[1][0] = aiTransform.a2;
	Matrix.M[1][1] = aiTransform.b2;
	Matrix.M[1][2] = aiTransform.c2;
	Matrix.M[1][3] = aiTransform.d2;
	Matrix.M[2][0] = aiTransform.a3;
	Matrix.M[2][1] = aiTransform.b3;
	Matrix.M[2][2] = aiTransform.c3;
	Matrix.M[2][3] = aiTransform.d3;
	Matrix.M[3][0] = aiTransform.a4;
	Matrix.M[3][1] = aiTransform.b4;
	Matrix.M[3][2] = aiTransform.c4;
	Matrix.M[3][3] = aiTransform.d4;

	return FTransform(Matrix);
}

FTransform UMeshLoader::GetWorldTransformOfNode(aiNode *Node)
{
	// Convert Assimp Matrix to FMatrix
	aiMatrix4x4 aiTransform = Node->mTransformation;

	FMatrix Matrix;
	Matrix.M[0][0] = aiTransform.a1;
	Matrix.M[0][1] = aiTransform.b1;
	Matrix.M[0][2] = aiTransform.c1;
	Matrix.M[0][3] = aiTransform.d1;
	Matrix.M[1][0] = aiTransform.a2;
	Matrix.M[1][1] = aiTransform.b2;
	Matrix.M[1][2] = aiTransform.c2;
	Matrix.M[1][3] = aiTransform.d2;
	Matrix.M[2][0] = aiTransform.a3;
	Matrix.M[2][1] = aiTransform.b3;
	Matrix.M[2][2] = aiTransform.c3;
	Matrix.M[2][3] = aiTransform.d3;
	Matrix.M[3][0] = aiTransform.a4;
	Matrix.M[3][1] = aiTransform.b4;
	Matrix.M[3][2] = aiTransform.c4;
	Matrix.M[3][3] = aiTransform.d4;

	if (Node->mParent == nullptr)
		return FTransform(Matrix);

	Matrix = GetWorldTransformOfNode(Node->mParent, Matrix);
	return FTransform(Matrix);
}

FMatrix UMeshLoader::GetWorldTransformOfNode(aiNode *Node, FMatrix WorldMatrix)
{
	aiMatrix4x4 aiTransform = Node->mTransformation;

	FMatrix Matrix;
	Matrix.M[0][0] = aiTransform.a1;
	Matrix.M[0][1] = aiTransform.b1;
	Matrix.M[0][2] = aiTransform.c1;
	Matrix.M[0][3] = aiTransform.d1;
	Matrix.M[1][0] = aiTransform.a2;
	Matrix.M[1][1] = aiTransform.b2;
	Matrix.M[1][2] = aiTransform.c2;
	Matrix.M[1][3] = aiTransform.d2;
	Matrix.M[2][0] = aiTransform.a3;
	Matrix.M[2][1] = aiTransform.b3;
	Matrix.M[2][2] = aiTransform.c3;
	Matrix.M[2][3] = aiTransform.d3;
	Matrix.M[3][0] = aiTransform.a4;
	Matrix.M[3][1] = aiTransform.b4;
	Matrix.M[3][2] = aiTransform.c4;
	Matrix.M[3][3] = aiTransform.d4;

	if (Node->mParent == nullptr)
		return WorldMatrix * Matrix;

	Matrix = GetWorldTransformOfNode(Node->mParent, WorldMatrix * Matrix);
	return Matrix;
}

FNodeData UMeshLoader::GetNodeHierarchy(aiNode *Node)
{
	FNodeData NodeData;
	NodeData.Name = FString(Node->mName.C_Str());

	// Get transform of node
	NodeData.Transform = GetTransformOfNode(Node);

	// Save each mesh index
	for (uint32 MeshIndex = 0; MeshIndex < Node->mNumMeshes; MeshIndex++)
		NodeData.Meshes.Push(Node->mMeshes[MeshIndex]);

	// Save each children
	for (uint32 ChildIndex = 0; ChildIndex < Node->mNumChildren; ChildIndex++)
		NodeData.Nodes.Push(GetNodeHierarchy(Node->mChildren[ChildIndex]));

	return NodeData;
}

aiNode *UMeshLoader::GetParentNode(aiNode *Node, int32 SearchMeshIndex)
{
	for (uint32 MeshIndex = 0; MeshIndex < Node->mNumMeshes; MeshIndex++)
		if (Node->mMeshes[MeshIndex] == SearchMeshIndex)
			return Node;

	for (uint32 ChildIndex = 0; ChildIndex < Node->mNumChildren; ChildIndex++)
	{
		aiNode *FoundNode = GetParentNode(Node->mChildren[ChildIndex], SearchMeshIndex);
		if (FoundNode != nullptr)
			return FoundNode;
	}

	return nullptr;
}

bool UMeshLoader::GetLodData(FString FilePath, FString SearchMeshName, FLodData &LodData)
{
	// When Lod file doesn't exist then return
	if (!FileManager.FileExists(*FilePath))
	{
		// Is no issue so dont say anything
		// UE_LOG(LogMeshLoader, Warning, TEXT("%s - Lod file not found!"), *FilePath);
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	// Load Json
	FString JsonFileContent;
	FFileHelper::LoadFileToString(JsonFileContent, *FilePath);
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonFileContent);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (!JsonObject.IsValid())
	{
		UE_LOG(LogMeshLoader, Error, TEXT("%s - Lod file can't be read!"), *FilePath);
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>> *Meshes;
	if (!JsonObject->TryGetArrayField("Meshes", Meshes))
	{
		UE_LOG(LogMeshLoader, Error, TEXT("%s - Lod file can't be read!"), *FilePath);
		return false;
	}

	for (int32 MeshIndex = 0; MeshIndex < Meshes->Num(); MeshIndex++)
	{
		TSharedPtr<FJsonObject> *Mesh;
		if (!(*Meshes)[MeshIndex]->TryGetObject(Mesh))
		{
			UE_LOG(LogMeshLoader, Error, TEXT("%s - Lod file can't be read!"), *FilePath);
			return false;
		}

		if (Mesh->Get()->TryGetField("MeshName"))
		{
			FString MeshName = Mesh->Get()->GetStringField("MeshName");
			if (SearchMeshName.Compare(MeshName) != 0)
				continue;
		}
		else
		{
			UE_LOG(LogMeshLoader, Error, TEXT("%s - MeshName missing!"), *FilePath);
			return false;
		}

		if (Mesh->Get()->TryGetField("Lod"))
		{
			LodData.Lod = Mesh->Get()->GetIntegerField("Lod");
		}
		else
		{
			UE_LOG(LogMeshLoader, Error, TEXT("%s - Lod missing!"), *FilePath);
			return false;
		}

		if (Mesh->Get()->TryGetField("ScreenSize"))
		{
			LodData.ScreenSize = Mesh->Get()->GetNumberField("ScreenSize");
		}
		else
		{
			UE_LOG(LogMeshLoader, Error, TEXT("%s - ScreenSize missing!"), *FilePath);
			return false;
		}
	}

	return true;
}