#include "genuary202603App.h"
#include "cinder/gl/Vao.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

void genuary202603App::setup()
{
	setupCamera();
	updateTerrain(0.0f);
	setupFireworks();
	setup3DObjects();
	gl::enableAlphaBlending();
}

void genuary202603App::update()
{
	float t = getElapsedSeconds() * 2.2f;
	updateTerrain(t);
	updateFireworks();

	void* bufferPtr = mTerrainBuffer->mapReplace();
	memcpy(bufferPtr, mTerrainData.data(), mTerrainData.size() * sizeof(mPoint));
	mTerrainBuffer->unmap();

	bufferPtr = mFireworksBuffer->mapReplace();
	memcpy(bufferPtr, mFireworksData.data(), mFireworksData.size() * sizeof(mPoint));
	mFireworksBuffer->unmap();
}

void genuary202603App::draw()
{

	gl::clear(Color(0.0f, 0.0f, 0.0f));
	gl::pushMatrices();
	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::setMatricesWindow(getWindowSize());
	gl::draw(mTexBG, getWindowBounds());
	gl::popMatrices();

	gl::pushMatrices();
	gl::setMatrices(mCamera);
	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::pointSize(8.0f);
	mTerrainBatch->draw();

	//gl::pointSize(4.0f);
	//mFireworksBatch->draw();

	gl::popMatrices();
}

void genuary202603App::setupCamera()
{
	float verticalFOV = 45.0f;
	float aspectRatio = getWindowAspectRatio();
	float nearClip = 0.1f;
	float farClip = 10.0f;
	vec3 cameraPosition = vec3(-0.003f, 1.28f, -1.777f);
	vec3 cameraTarget = vec3(0, 0.25, 0.75);
	vec3 cameraUpAxis = vec3(0, 1, 0);

	mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
	mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
	mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void genuary202603App::setup3DObjects()
{
	mTerrainBuffer = gl::Vbo::create(GL_ARRAY_BUFFER, mTerrainData, GL_STREAM_DRAW);
	geom::BufferLayout layout;
	layout.append(geom::Attrib::POSITION, 3, sizeof(mPoint), offsetof(mPoint, position));
	layout.append(geom::Attrib::COLOR, 3, sizeof(mPoint), offsetof(mPoint, color));

	auto terrainMesh = gl::VboMesh::create(static_cast<uint32_t>(mTerrainData.size()), GL_POINTS, { { layout, mTerrainBuffer } });
	mTerrainBatch = gl::Batch::create(terrainMesh, gl::getStockShader(gl::ShaderDef().color()));

	mFireworksBuffer = gl::Vbo::create(GL_ARRAY_BUFFER, mFireworksData, GL_STREAM_DRAW);

	auto fireworksMesh = gl::VboMesh::create(static_cast<uint32_t>(mFireworksData.size()), GL_POINTS, { { layout, mFireworksBuffer } });
	mFireworksBatch = gl::Batch::create(fireworksMesh, gl::getStockShader(gl::ShaderDef().color()));

	mTexBG = gl::Texture::create(loadImage(loadAsset("TX_BG_Starry.png")));
}

void genuary202603App::updateTerrain( float gOffset )
{
	mTerrainData.clear();
	float yOffset = gOffset;
	for (int y = 0; y < kNumRows; ++y)
	{
		float xOffset = 0.0f;
		for (int x = 0; x < kNumCols; ++x)
		{
			float x0 = (x - kNumCols / 2) * kXScale;
			float y0 = lmap<float>(Perlin().noise(xOffset, yOffset), -0.5f, .5f, kNoiseMin, kNoiseMax);
			y0 = glm::clamp<float>(y0, 0.07f, kNoiseMax);
			float z0 = (y - kNumRows / 2) * kYScale;
			vec3 position = vec3(x0, y0, z0);
	
			vec4 color = kSnow;
			if (y0 < 0.20f && y0 > 0.125f)
				color = kBrown;
			else if (y0 <= 0.125f && y0 > 0.071f)
				color = kGreen;
			else if (y0 <= 0.07f)
				color = kBlue;

			mTerrainData.push_back({ position, color });
			xOffset += kXStep;
		}
		yOffset += kYStep;
	}
}

void genuary202603App::setupFireworks()
{
	mFireworksEmitters.clear();
	for (int i = 0; i < 9; i++)
	{
		float ffx = Rand::randFloat(-0.3f, 0.3f);
		float ffy = Rand::randFloat(0.4f, 1.2f);
		vec4 color = vec4(Rand::randFloat(0.5f, 1.0f), Rand::randFloat(0.5f, 1.0f), Rand::randFloat(0.5f, 1.0f), 1.0f);
		ffEmitter emitter = ffEmitter(6, color, vec3(ffx, ffy, 0.0f));
		mFireworksEmitters.push_back(emitter);
	}

	updateFireworks();
}

void genuary202603App::updateFireworks()
{
	mFireworksData.clear();
	for( int f=0;f < mFireworksEmitters.size(); f++ )
	{
		if(mFireworksEmitters[f].mAlive == false)
		{
			float ffx = Rand::randFloat(-0.3f, 0.3f);
			float ffy = Rand::randFloat(0.4f, 1.2f);
			vec4 color = vec4(Rand::randFloat(0.5f, 1.0f), Rand::randFloat(0.5f, 1.0f), Rand::randFloat(0.5f, 1.0f), 1.0f);
			ffEmitter emitter = ffEmitter(6, color, vec3(ffx, ffy, 0.0f));
			mFireworksEmitters[f] = emitter;
		}

		mFireworksEmitters[f].step();
		for( int p=0; p < mFireworksEmitters[f].mParticles.size(); p++ )
		{
			vec3 position = mFireworksEmitters[f].mParticles[p].mPos + mFireworksEmitters[f].mOrigin;
			vec4 color = mFireworksEmitters[f].mColor;
			mFireworksData.push_back({ position, color });
		}
	}	
}

void prepareSettings( App::Settings* settings )
{
	settings->setWindowSize( 540, 960 );
}

CINDER_APP( genuary202603App, RendererGl, *prepareSettings )
