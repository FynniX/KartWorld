// Copyright @ 2023 Fynn Haupt


#include "Meshes/ChassiMesh.h"

DEFINE_LOG_CATEGORY(LogChassiMesh);

// Sets default values
AChassiMesh::AChassiMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AChassiMesh::LoadMesh()
{
	// Check weather game instance is not nullptr
	if (!GameInstance) return;

	// Load models
	ChassiModel = GameInstance->ChassiLoader->Load(ChassiName);
	TireModel = GameInstance->TireLoader->Load(TireName);

	// Check weather model was imported without errors
	if (!ChassiModel.bSuccess) return;

	// Create nodes with meshes for chassi
	USceneComponent* Scene = CreateNode(ChassiModel.Model.NodeHierarchy);
	SetRootComponent(Scene);

	// Create and attach tires
}

// Called when the game starts or when spawned
void AChassiMesh::BeginPlay()
{
	Super::BeginPlay();
	
	// Get GameInstance
	GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (!GameInstance) UE_LOG(LogChassiMesh, Warning, TEXT("GameInstance can't be casted!"));
}

USceneComponent* AChassiMesh::CreateNode(FNodeData& NodeData)
{
	// Create Node
	//USkeletalMeshComponent* Node = NewObject<USkeletalMeshComponent>(this, FName(*NodeData.Name));
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

	// Tire Front Left
	if(NodeData.Name.Compare(ChassiModel.Linking.StubAxleLeft.Wheel) == 0) {
		URealtimeMeshComponent* Mesh = CreateTireModel(TireModel.ModelFl);
		Mesh->SetUsingAbsoluteScale(true);
		Mesh->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));
		if (Mesh != nullptr) Mesh->AttachToComponent(Node, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Tire Front Right
	if(NodeData.Name.Compare(ChassiModel.Linking.StubAxleRight.Wheel) == 0) {
		URealtimeMeshComponent* Mesh = CreateTireModel(TireModel.ModelFr);
		Mesh->SetUsingAbsoluteScale(true);
		Mesh->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
		if (Mesh != nullptr) Mesh->AttachToComponent(Node, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Tire Rear Left
	if(NodeData.Name.Compare(ChassiModel.Linking.RearAxle.WheelLeft) == 0) {
		URealtimeMeshComponent* Mesh = CreateTireModel(TireModel.ModelRl);
		Mesh->SetUsingAbsoluteScale(true);
		Mesh->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
		if (Mesh != nullptr) Mesh->AttachToComponent(Node, FAttachmentTransformRules::KeepRelativeTransform);
	}

	// Tire Rear Right
	if(NodeData.Name.Compare(ChassiModel.Linking.RearAxle.WheelRight) == 0) {
		URealtimeMeshComponent* Mesh = CreateTireModel(TireModel.ModelRr);
		Mesh->SetUsingAbsoluteScale(true);
		Mesh->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
		if (Mesh != nullptr) Mesh->AttachToComponent(Node, FAttachmentTransformRules::KeepRelativeTransform);
	}

	return Node;
}

URealtimeMeshComponent* AChassiMesh::CreateMesh(int32 MeshIndex)
{
	if (!ChassiModel.Model.Meshes.IsValidIndex(MeshIndex)) return nullptr;
	FMeshData& MeshData = ChassiModel.Model.Meshes[MeshIndex];

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
		Builder.AddTriangle(Triangle.UV0, Triangle.UV1, Triangle.UV2, 0);
	}

	// Setup material
	int32 MaterialId = MeshData.MaterialId;
	if (ChassiModel.Model.Materials.IsValidIndex(MaterialId))
		Mesh->SetupMaterialSlot(0, EName::None, ChassiModel.Model.Materials[MaterialId]);

	const FRealtimeMeshSectionGroupKey GroupKey = FRealtimeMeshSectionGroupKey::CreateUnique(0);
	Mesh->CreateSectionGroup(GroupKey, StreamSet);
	
	return MeshComponent;
}

URealtimeMeshComponent* AChassiMesh::CreateTireModel(FModelData& ModelData) {
	// Create mesh component
	URealtimeMeshComponent* MeshComponent = NewObject<URealtimeMeshComponent>(this);
	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	MeshComponent->RegisterComponent();
	AddInstanceComponent(MeshComponent);

	// Create mesh
	URealtimeMeshSimple* Mesh = MeshComponent->InitializeRealtimeMesh<URealtimeMeshSimple>();

	for(int32 MeshIndex = 0; MeshIndex < ModelData.Meshes.Num(); MeshIndex++) {
		FMeshData& MeshData = ModelData.Meshes[MeshIndex];
		
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
			Builder.AddTriangle(Triangle.UV0, Triangle.UV1, Triangle.UV2, 0);
		}

		// Setup material
		if (ModelData.Materials.IsValidIndex(MeshData.MaterialId))
			Mesh->SetupMaterialSlot(MeshData.MaterialId, EName::None, ModelData.Materials[MeshData.MaterialId]);
	
		const FRealtimeMeshSectionGroupKey GroupKey = FRealtimeMeshSectionGroupKey::CreateUnique(MeshData.LodData.Lod);
		Mesh->CreateSectionGroup(GroupKey, StreamSet);
		Mesh->UpdateLODConfig(MeshData.LodData.Lod, FRealtimeMeshLODConfig(MeshData.LodData.ScreenSize));

		const FRealtimeMeshSectionKey PolyGroupKey = FRealtimeMeshSectionKey::CreateForPolyGroup(GroupKey, 0);
		Mesh->UpdateSectionConfig(PolyGroupKey, FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Dynamic, MeshData.MaterialId), true);
	}

	return MeshComponent;
}
