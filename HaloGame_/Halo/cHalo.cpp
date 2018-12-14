// Includes
//=========
//12/13/2018
#include "cHalo.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include "Engine/Graphics/Graphics.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/cRenderState.h"
#include "Engine/Math/Functions.h"
#include "Engine/Math/cMatrix_transformation.h"
#include "Engine/Math/cQuaternion.h"
#include "Engine/Math/sVector.h"
#include "Engine/UserOutput/UserOutput.h"
#include "Engine/Physics/CollisionDetection.h"
#include "Engine/Physics/PhysicsSimulation.h"
#include "Custom Game Objects/HomingCube.h"
#include "Custom Game Objects/MoveableCube.h"
#include "Custom Game Objects/Player.h"
#include "Custom Game Objects/Boss.h"


namespace eae6320 {
	void UpdateScore(cHalo * const i_Halo);
}

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cHalo::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cHalo::Initialize()
{
	//initialize camera 
	mainCamera.Initialize(Math::sVector(0.0f, 0.0f, 10.0f), Math::cQuaternion(), Math::ConvertDegreesToRadians(45), 1.0f, 0.1f, 500.0f);
	
	//create two meshes 	
	eae6320::Assets::cHandle<Mesh> mesh_cube;
	eae6320::Assets::cHandle<Mesh> mesh_outline;
	eae6320::Assets::cHandle<Mesh> mesh_plane;
	eae6320::Assets::cHandle<Mesh> mesh_gun;
	eae6320::Assets::cHandle<Mesh> mesh_bullet;
	eae6320::Assets::cHandle<Mesh> mesh_ceiling;
	eae6320::Assets::cHandle<Mesh> mesh_front_back_wall;
	eae6320::Assets::cHandle<Mesh> mesh_boss;
	eae6320::Assets::cHandle<Mesh> mesh_missile;

	eae6320::Assets::cHandle<Mesh> mesh_numbers[10];

	auto result = eae6320::Results::Success;
	if (!(result = Mesh::s_manager.Load("data/meshes/cube.mesh", mesh_cube))) {
		EAE6320_ASSERT(false);
	}
	
	if (!(result = Mesh::s_manager.Load("data/meshes/outline.mesh", mesh_outline))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/plane.mesh", mesh_plane))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/gun.mesh", mesh_gun))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/bullet.mesh", mesh_bullet))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/ceiling.mesh", mesh_ceiling))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/front_back_wall.mesh", mesh_front_back_wall))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/boss.mesh", mesh_boss))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/missile.mesh", mesh_missile))) {
		EAE6320_ASSERT(false);
	}
	
	if (!(result = Mesh::s_manager.Load("data/meshes/0.mesh", mesh_numbers[0]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/1.mesh", mesh_numbers[1]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/2.mesh", mesh_numbers[2]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/3.mesh", mesh_numbers[3]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/4.mesh", mesh_numbers[4]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/5.mesh", mesh_numbers[5]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/6.mesh", mesh_numbers[6]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/7.mesh", mesh_numbers[7]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/8.mesh", mesh_numbers[8]))) {
		EAE6320_ASSERT(false);
	}
	if (!(result = Mesh::s_manager.Load("data/meshes/9.mesh", mesh_numbers[9]))) {
		EAE6320_ASSERT(false);
	}

	masterMeshArray.push_back(mesh_cube);
	masterMeshArray.push_back(mesh_outline);
	masterMeshArray.push_back(mesh_plane);
	masterMeshArray.push_back(mesh_gun);
	masterMeshArray.push_back(mesh_bullet);
	masterMeshArray.push_back(mesh_ceiling);
	masterMeshArray.push_back(mesh_front_back_wall);
	masterMeshArray.push_back(mesh_boss);
	masterMeshArray.push_back(mesh_missile);
	
	for (int i = 0; i < 10; i++) {
		masterMeshArray.push_back(mesh_numbers[i]);
	}

	//create two effect
	Effect* pEffect_white;
	Effect* pEffect_blue;
	Effect* pEffect_white_front_light;
	Effect* pEffect_white_back_light;
	Effect* pEffect_red;
	Effect* pEffect_text_r;
	Effect* pEffect_text_l;
	Effect::Load("data/effects/white.effect", pEffect_white);
	Effect::Load("data/effects/blue.effect", pEffect_blue);
	Effect::Load("data/effects/white_front_light.effect", pEffect_white_front_light);
	Effect::Load("data/effects/white_back_light.effect", pEffect_white_back_light);
	Effect::Load("data/effects/red.effect", pEffect_red);
	Effect::Load("data/effects/text_r.effect", pEffect_text_r);
	Effect::Load("data/effects/text_l.effect", pEffect_text_l);

	masterEffectArray.push_back(pEffect_white);
	masterEffectArray.push_back(pEffect_blue);
	masterEffectArray.push_back(pEffect_white_front_light);
	masterEffectArray.push_back(pEffect_white_back_light);
	masterEffectArray.push_back(pEffect_red);
	masterEffectArray.push_back(pEffect_text_r);
	masterEffectArray.push_back(pEffect_text_l);

	//create sound
	soundArray.push_back(new Engine::Sound("data/audio/neon.wav"));
	soundArray[0]->PlayInLoop();
	//player
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(0.0f, 3.0f, 15.0f);
		objState.boundingBox.center = Math::sVector(0.0f, -0.7f, 0.0f);
		objState.boundingBox.extends = Math::sVector(1.0f, 2.0f, 1.0f);
		
		Player * player = new Player(pEffect_white_front_light, mesh_outline, objState, this);
		player->m_Camera = &mainCamera;
		
		GameCommon::GameObject * pGun = new GameCommon::GameObject(masterEffectArray[0], mesh_gun, objState);
		gameOjbectsWithoutCollider.push_back(pGun);

		player->m_Gun = pGun;
		strcpy_s(player->objectType, "Player");
		masterGameObjectArr.push_back(player);
	}
	//add ground
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(0.0f, -2.0f, 0.0f);
		objState.boundingBox.center = Math::sVector(0.0f, 0.0f, 0.0f);
		objState.boundingBox.extends = Math::sVector(5.0f, 0.25f, 20.0f);
		GameCommon::GameObject * pGameObject = new GameCommon::GameObject(masterEffectArray[0], masterMeshArray[2], objState);
		strcpy_s(pGameObject->objectType, "Ground");
		masterGameObjectArr.push_back(pGameObject);
	}
	//add left wall
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(-6.6f, 2.5f, 0.0f);
		objState.boundingBox.center = Math::sVector(0.0f, 0.0f, 0.0f);
		objState.boundingBox.extends = Math::sVector(5.0f, 0.25f, 20.0f);
		objState.euler_z = 110;
		objState.UpdateOrientation(0);
		GameCommon::GameObject * pGameObject = new GameCommon::GameObject(masterEffectArray[0], masterMeshArray[2], objState);
		masterGameObjectArr.push_back(pGameObject);
	}
	//add right wall
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(6.6f, 2.5f, 0.0f);
		objState.boundingBox.center = Math::sVector(0.0f, 0.0f, 0.0f);
		objState.boundingBox.extends = Math::sVector(5.0f, 0.25f, 20.0f);
		objState.euler_z = -110;
		objState.UpdateOrientation(0);
		GameCommon::GameObject * pGameObject = new GameCommon::GameObject(masterEffectArray[0], masterMeshArray[2], objState);
		masterGameObjectArr.push_back(pGameObject);
	}
	//ceiling
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(0.0f, 7.4f, 0.0f);
		objState.boundingBox.center = Math::sVector(0.0f, 0.0f, 0.0f);
		objState.boundingBox.extends = Math::sVector(10.0f, 0.25f, 20.0f);
		GameCommon::GameObject * pGameObject = new GameCommon::GameObject(masterEffectArray[0], mesh_ceiling, objState);
		masterGameObjectArr.push_back(pGameObject);
	}
	//front wall
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(0.0f, 4.0f, -20.0f);
		objState.boundingBox.center = Math::sVector(0.0f, 0.0f, 0.0f);
		objState.boundingBox.extends = Math::sVector(9.0f, 6.0f, 0.5f);
		GameCommon::GameObject * pGameObject = new GameCommon::GameObject(pEffect_white_front_light, mesh_front_back_wall, objState);
		masterGameObjectArr.push_back(pGameObject);
	}
	//back wall
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(0.0f, 4.0f, 20.0f);
		objState.boundingBox.center = Math::sVector(0.0f, 0.0f, 0.0f);
		objState.boundingBox.extends = Math::sVector(9.0f, 6.0f, 0.5f);
		GameCommon::GameObject * pGameObject = new GameCommon::GameObject(pEffect_white_back_light, mesh_front_back_wall, objState);
		strcpy_s(pGameObject->objectType, "BackWall");
		masterGameObjectArr.push_back(pGameObject);
	}
	//boss
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(0.0f, 2.5f, -18.5f);
		Boss * pGameObject = new Boss(pEffect_white_front_light, mesh_boss, objState, this);
		gameOjbectsWithoutCollider.push_back(pGameObject);
	}

	//text
	{
		Physics::sRigidBodyState objState;
		scoreboard[0] = new GameCommon::GameObject(pEffect_text_r, mesh_numbers[0], objState);
		scoreboard[1] = new GameCommon::GameObject(pEffect_text_l, mesh_numbers[0], objState);
	}
	//test cube
	/*
	{
		Physics::sRigidBodyState objState;
		objState.position = Math::sVector(0.0f, 2.0f, -9.0f);
		objState.boundingBox.center = Math::sVector(0.0f, 0.0f, 0.0f);
		objState.boundingBox.extends = Math::sVector(0.05f, 0.05f, 0.05f);
		GameCommon::GameObject * pGameObject = new GameCommon::GameObject(masterEffectArray[1], masterMeshArray[4], objState);
		masterGameObjectArr.push_back(pGameObject);
	}
	*/
	return Results::Success;
}

void eae6320::cHalo::UpdateSimulationBasedOnInput() {
	if (isGameOver == false) {
		//mainCamera.UpdateCameraBasedOnInput();
		size_t numOfObjects = masterGameObjectArr.size();
		for (size_t i = 0; i < numOfObjects; i++) {
			masterGameObjectArr[i]->UpdateGameObjectBasedOnInput();
		}
		numOfObjects = gameOjbectsWithoutCollider.size();
		for (size_t i = 0; i < numOfObjects; i++) {
			gameOjbectsWithoutCollider[i]->UpdateGameObjectBasedOnInput();
		}
	}
}

void  eae6320::cHalo::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) {
	if (isGameOver == false) {
		size_t size_physicsObject = masterGameObjectArr.size();
		size_t size_nonPhyiscsObject = gameOjbectsWithoutCollider.size();
// ***********************run physics****************************************************	
		//update game objects with AABB
		Physics::PhysicsUpdate(masterGameObjectArr, i_elapsedSecondCount_sinceLastUpdate);
		//update non-phyiscs objects
		for (size_t i = 0; i < size_nonPhyiscsObject; i++) {
			gameOjbectsWithoutCollider[i]->m_State.Update(i_elapsedSecondCount_sinceLastUpdate);
		}
		//update camera
		mainCamera.UpdateState(i_elapsedSecondCount_sinceLastUpdate);
		
//run AI*********************************************************************************
		for (size_t i = 0; i < size_physicsObject; i++) {
			masterGameObjectArr[i]->EventTick(i_elapsedSecondCount_sinceLastUpdate);
		}
		for (size_t i = 0; i < size_nonPhyiscsObject; i++) {
			gameOjbectsWithoutCollider[i]->EventTick(i_elapsedSecondCount_sinceLastUpdate);
		}
	}
	else {
		GameCommon::ResetAllGameObjectsVelo(masterGameObjectArr, gameOjbectsWithoutCollider, mainCamera);
	}
	GameCommon::RemoveInactiveGameObjects(masterGameObjectArr);
	UpdateScore(this);
}


eae6320::cResult eae6320::cHalo::CleanUp()
{	//release all game objects first
	size_t numOfObjects = masterGameObjectArr.size();
	for (size_t i = 0; i < numOfObjects; i++) {
		delete masterGameObjectArr[i];
	}
	masterGameObjectArr.clear();
	numOfObjects = gameOjbectsWithoutCollider.size();
	for (size_t i = 0; i < numOfObjects; i++) {
		delete gameOjbectsWithoutCollider[i];
	}
	gameOjbectsWithoutCollider.clear();

	//delete scoreboard
	if (scoreboard[0] != nullptr)
		delete scoreboard[0];
	if (scoreboard[1] != nullptr)
		delete scoreboard[1];

	//release effect
	for (size_t i = 0; i < masterEffectArray.size(); i++) {
		masterEffectArray[i]->DecrementReferenceCount();
		masterEffectArray[i] = nullptr;
	}
	masterEffectArray.clear();

	//release mesh handle
	for (size_t i = 0; i < masterMeshArray.size(); i++) {
		Mesh::s_manager.Release(masterMeshArray[i]);
	}
	masterMeshArray.clear();

	//delete sound
	for (size_t i = 0; i < soundArray.size(); i++) {
		delete soundArray[i];
	}
	soundArray.clear();
	
	return Results::Success;
}

void eae6320::cHalo::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate) {	
	//submit background color
	float color[] = { 0, 1, 0, 1 };
	eae6320::Graphics::SubmitBGColor(color);
	
	//submit gameObject with colliders 
	for (size_t i = 0; i < masterGameObjectArr.size(); i++) {
		//smooth movement first
		Math::sVector predictedPosition = masterGameObjectArr[i]->m_State.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
		Math::cQuaternion predictedOrientation = masterGameObjectArr[i]->m_State.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate);
		//submit
		eae6320::Graphics::SubmitObject(Math::cMatrix_transformation(predictedOrientation, predictedPosition),
			masterGameObjectArr[i]->GetEffect(), Mesh::s_manager.Get(masterGameObjectArr[i]->GetMesh()));

	}
	//submit gameObject without colliders
	for (size_t i = 0; i < gameOjbectsWithoutCollider.size(); i++) {
		//smooth movement first
		Math::sVector predictedPosition = gameOjbectsWithoutCollider[i]->m_State.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
		Math::cQuaternion predictedOrientation = gameOjbectsWithoutCollider[i]->m_State.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate);
		//submit
		eae6320::Graphics::SubmitObject(Math::cMatrix_transformation(predictedOrientation, predictedPosition),
			gameOjbectsWithoutCollider[i]->GetEffect(), Mesh::s_manager.Get(gameOjbectsWithoutCollider[i]->GetMesh()));

	}
	
	//submit scoreboard
	for (int i = 0; i < 2; i++) {
		Math::sVector predictedPosition; 
		Math::cQuaternion predictedOrientation;
		//submit
		eae6320::Graphics::SubmitObject(Math::cMatrix_transformation(predictedOrientation, predictedPosition), scoreboard[i]->GetEffect(), Mesh::s_manager.Get(scoreboard[i]->GetMesh()));
	}
	
	//submit camera
	{
		//smooth camera movemnt first before it's submitted
		Math::sVector predictedPosition = mainCamera.m_State.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
		Math::cQuaternion predictedOrientation = mainCamera.m_State.PredictFutureOrientation(i_elapsedSecondCount_sinceLastSimulationUpdate);
		//submit
		eae6320::Graphics::SubmitCamera(Math::cMatrix_transformation::CreateWorldToCameraTransform(predictedOrientation, predictedPosition),
			mainCamera.GetCameraToProjectedMat());
	}	
}

namespace eae6320 {
	void UpdateScore(cHalo * const i_Halo) {
		int low = i_Halo->score % 10;
		int high = i_Halo->score / 10;

		if (low == 0) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[9]);
		}
		else if (low == 1) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[10]);
		}
		else if (low == 2) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[11]);
		}
		else if (low == 3) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[12]);
		}
		else if (low == 4) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[13]);
		}
		else if (low == 5) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[14]);
		}
		else if (low == 6) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[15]);
		}
		else if (low == 7) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[16]);
		}
		else if (low == 8) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[17]);
		}
		else if (low == 9) {
			i_Halo->scoreboard[0]->SetMesh(i_Halo->masterMeshArray[18]);
		}

		if (high == 0) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[9]);
		}
		else if (high == 1) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[10]);
		}
		else if (high == 2) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[11]);
		}
		else if (high == 3) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[12]);
		}
		else if (high == 4) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[13]);
		}
		else if (high == 5) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[14]);
		}
		else if (high == 6) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[15]);
		}
		else if (high == 7) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[16]);
		}
		else if (high == 8) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[17]);
		}
		else if (high == 9) {
			i_Halo->scoreboard[1]->SetMesh(i_Halo->masterMeshArray[18]);
		}
	}
}