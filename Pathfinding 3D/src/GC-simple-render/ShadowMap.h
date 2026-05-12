#pragma once
#include <Shader.h>

#include "Render/ShadowMap.h"
#include "Camera.h"

#define LIGHT_DIR gce::Vector3f32(0.0f, 0.0f, -1.0f)
#define LIGHT_POS gce::Vector3f32( 15.0f, 8.0f, 25.0f)

class Renderer;

class ShadowMap
{
public:
	ShadowMap(Renderer const& renderer);
	~ShadowMap();
private:
	gce::ShadowMap m_shadowMap = gce::ShadowMap(4096);
	Camera m_shadowCam = Camera(ORTHOGRAPHIC);

	gce::Shader m_vsShadow;
	gce::Shader m_psShadow;
	gce::Shader m_rootSigShadow;
	gce::D12PipelineObject* m_pPsoShadow;

	friend class Renderer;
};