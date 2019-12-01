#include "PhsyXPCH.h"
#include "PhysXWorld.h"

PhysXWorld::PhysXWorld()
{

}

PhysXWorld::~PhysXWorld()
{
	SAFE_RELEASE(m_Scene);
	SAFE_RELEASE(m_Dispatcher);
	SAFE_RELEASE(m_Physics);
	SAFE_RELEASE(m_Foundation);
}

bool PhysXWorld::Init(ContactReportCallback* contactReport)
{
	static PxDefaultAllocator allocator;
	static PxDefaultErrorCallback errorCallback;

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
	if (!m_Foundation)
	{
		//Seems like overkill, but that it was in Tilan's maybe just needs to be:
		//return false;
		abort();
	}

	//Unused because Pvd unused
	/*m_Pvd = PxCreatePvd(*m_Foundation);
	auto transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);*/

	bool recordMemoryAllocations = true;
	PxTolerancesScale scale;

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, scale, recordMemoryAllocations, nullptr);

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());

	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = contactReport;
	sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
	sceneDesc.ccdMaxPasses = 4;
	m_Scene = m_Physics->createScene(sceneDesc);

	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

	return true;
}

void PhysXWorld::Update()
{
	m_ContactPositions.clear();
	m_ContactImpulses.clear();

	m_Scene->simulate(1.0f / 60.0f);
	m_Scene->fetchResults(true);

	//Output contactpositions
}
