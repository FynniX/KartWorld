// Copyright @ 2023 Fynn Haupt


#include "Meshes/TrackMesh.h"

DEFINE_LOG_CATEGORY(LogTrackMesh);

// Sets default values
ATrackMesh::ATrackMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// LoadWorld
	MeshComponent = CreateDefaultSubobject<URealtimeMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void ATrackMesh::LoadMesh()
{
	// Check weather game instance is not nullptr
	if (!GameInstance) return;

	// Load model
	TrackModel = GameInstance->TrackLoader->Load(TrackName);

	// Check weather model was imported without errors
	if (!TrackModel.bSuccess) return;

	//LoadWorld
	Mesh = MeshComponent->InitializeRealtimeMesh<URealtimeMeshSimple>();
	for(int32 MeshIndex = 0; MeshIndex < TrackModel.Model.Meshes.Num(); MeshIndex++)
		CreateMesh(MeshIndex);

	// Create nodes with meshes
	//USceneComponent* Scene = CreateNode(TrackModel.Model.NodeHierarchy);
	//SetRootComponent(Scene);
}

// Called when the game starts or when spawned
void ATrackMesh::BeginPlay()
{
	Super::BeginPlay();
	
	// Get GameInstance
	GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (!GameInstance) UE_LOG(LogTrackMesh, Warning, TEXT("GameInstance can't be casted!"));
}

/*USceneComponent* ATrackMesh::CreateNode(FNodeData& NodeData)
{
	// Create Node
	USceneComponent* Node = NewObject<USceneComponent>(this, FName(*NodeData.Name));
	Node->SetMobility(EComponentMobility::Movable);
	Node->RegisterComponent();
	AddInstanceComponent(Node);

	// Set transform
	Node->SetRelativeTransform(NodeData.Transform);

	// Create children nodes
	for (int32 NodeIndex = 0; NodeIndex < NodeData.Nodes.Num(); NodeIndex++) {
		USceneComponent* ChildrenNode = CreateNode(NodeData.Nodes[NodeIndex]);
		ChildrenNode->AttachToComponent(Node, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Create meshes
	for (int32 MeshIndex = 0; MeshIndex < NodeData.Meshes.Num(); MeshIndex++) {
		URealtimeMeshComponent* Mesh = CreateMesh(NodeData.Meshes[MeshIndex]);
		if (Mesh == nullptr) continue;
		Mesh->AttachToComponent(Node, FAttachmentTransformRules::KeepRelativeTransform);
	}

	return Node;
}*/

/*URealtimeMeshComponent* ATrackMesh::CreateMesh(int32 MeshIndex)
{
	if (!TrackModel.Model.Meshes.IsValidIndex(MeshIndex)) return nullptr;
	FMeshData& MeshData = TrackModel.Model.Meshes[MeshIndex];

	// Create mesh component
	URealtimeMeshComponent* MeshComponent = NewObject<URealtimeMeshComponent>(this);
	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	MeshComponent->RegisterComponent();
	AddInstanceComponent(MeshComponent);

	// Create mesh
	URealtimeMeshSimple* Mesh = MeshComponent->InitializeRealtimeMesh<URealtimeMeshSimple>();

	FRealtimeMeshStreamSet StreamSet;
	TRealtimeMeshBuilderLocal<uint16, FPackedNormal, FVector2DHalf, 1> Builder(StreamSet);
	Builder.EnableTangents();
	Builder.EnableTexCoords();
	Builder.EnablePolyGroups();
	Builder.EnableColors();

	// Load mesh
	for (int32 VertexIndex = 0; VertexIndex < MeshData.Verticies.Num(); VertexIndex++)
	{
		Builder.AddVertex(MeshData.Verticies[VertexIndex].Position)
			.SetNormalAndTangent(MeshData.Verticies[VertexIndex].Normal, MeshData.Verticies[VertexIndex].Tangent)
			.SetTexCoords(MeshData.Verticies[VertexIndex].UV0);
	}

	for (int32 TriangleIndex = 0; TriangleIndex < MeshData.Triangles.Num(); TriangleIndex++)
	{
		FTriangleData &Triangle = MeshData.Triangles[TriangleIndex];
		Builder.AddTriangle(Triangle.UV0, Triangle.UV1, Triangle.UV2, Triangle.PolyGroupIndex);
	}

	// Setup material
	int32 MaterialId = MeshData.MaterialId;
	if (TrackModel.Model.Materials.IsValidIndex(MaterialId))
		Mesh->SetupMaterialSlot(0, EName::None, TrackModel.Model.Materials[MaterialId]);

	const FRealtimeMeshSectionGroupKey GroupKey = FRealtimeMeshSectionGroupKey::CreateUnique(0);
	Mesh->CreateSectionGroup(GroupKey, StreamSet);

	const FRealtimeMeshSectionKey PolyGroupKey = FRealtimeMeshSectionKey::CreateForPolyGroup(GroupKey, 0);
	Mesh->UpdateSectionConfig(PolyGroupKey, FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Static, 0), true);
	
	return MeshComponent;
}*/

URealtimeMeshComponent* ATrackMesh::CreateMesh(int32 MeshIndex)
{
	if (!TrackModel.Model.Meshes.IsValidIndex(MeshIndex)) return nullptr;
	FMeshData& MeshData = TrackModel.Model.Meshes[MeshIndex];

	FRealtimeMeshStreamSet StreamSet;
	TRealtimeMeshBuilderLocal<uint16, FPackedNormal, FVector2DHalf, 1> Builder(StreamSet);
	Builder.EnableTangents();
	Builder.EnableTexCoords();
	Builder.EnablePolyGroups();
	Builder.EnableColors();

	// Load mesh
	for (int32 VertexIndex = 0; VertexIndex < MeshData.Verticies.Num(); VertexIndex++)
	{
		Builder.AddVertex(MeshData.Verticies[VertexIndex].Position)
			.SetNormalAndTangent(MeshData.Verticies[VertexIndex].Normal, MeshData.Verticies[VertexIndex].Tangent)
			.SetTexCoords(MeshData.Verticies[VertexIndex].UV0);
	}

	int32 MaterialId = MeshData.MaterialId;
	for (int32 TriangleIndex = 0; TriangleIndex < MeshData.Triangles.Num(); TriangleIndex++)
	{
		FTriangleData &Triangle = MeshData.Triangles[TriangleIndex];
		Builder.AddTriangle(Triangle.UV0, Triangle.UV1, Triangle.UV2, 0);
	}

	// Setup material
	if (TrackModel.Model.Materials.IsValidIndex(MaterialId))
		Mesh->SetupMaterialSlot(MaterialId, EName::None, TrackModel.Model.Materials[MaterialId]);

	const FRealtimeMeshSectionGroupKey GroupKey = FRealtimeMeshSectionGroupKey::CreateUnique(MeshData.LodData.Lod);
	Mesh->CreateSectionGroup(GroupKey, StreamSet);

	const FRealtimeMeshSectionKey PolyGroupKey = FRealtimeMeshSectionKey::CreateForPolyGroup(GroupKey, 0);
	Mesh->UpdateSectionConfig(PolyGroupKey, FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Static, MaterialId), true);

	return MeshComponent;
}