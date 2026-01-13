#include "genuary202603App.h"
#include "cinder/gl/Vao.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

const float kCellSize = 0.025f;
const unsigned int kNumWidth = 90;
const unsigned int kNumDepth = 160;

const float kNoiseMin = -0.8f;
const float kNoiseMax = 0.8f;
const float kXScale = 0.015f;
const float kYScale = 0.02f;
const float kXStep = 0.1f;
const float kYStep = 0.08f;

const vec4 kBlue = vec4(38.0f / 255.0f, 104.0f / 255.0f, 120.0f / 255.0f, 0.9f);
const vec4 kGreen = vec4(42.0f / 255.0f, 120.0f / 255.0f, 38.0f / 255.0f, 0.2f);
const vec4 kBrown = vec4(94.0f / 255.0f, 74.0f / 255.0f, 34.0f / 255.0f, 1.0f);
const vec4 kSky = vec4(64.0f / 255.0f, 107.0f / 255.0f, 210.0f / 255.0f, 1.0f);
const vec4 kSnow = vec4(0.9f, 0.9f, 1.0f, 0.85f);

void genuary202603App::setup()
{
	setupCamera();
	//updateTerrain(0.0f);
	setupInstancedMesh();
	gl::disableAlphaBlending();
}

void genuary202603App::update()
{
	float t = getElapsedSeconds() * 2.2f;
	updateTerrain(t);
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
	mTerrainBatch->drawInstanced( kNumWidth * kNumDepth);

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

	mTexBG = gl::Texture::create(loadImage(loadAsset("TX_BG_Starry.png")));
}

void genuary202603App::setupShaders()
{
	auto vert_prog = CI_GLSL(150,
		uniform mat4 ciModelViewProjection;
		uniform mat3 ciNormalMatrix;

		in vec4 ciPosition;
		in vec2 ciTexCoord0;
		in vec3 ciNormal;

		// per instance
		in vec3 instXYZ;
		in vec3 instRGB;

		out lowp vec4 Color;
		out highp vec3 Normal;

		void main(void)
		{
			gl_Position = ciModelViewProjection * (4.0 * ciPosition + vec4(instXYZ, 0));
			Color = vec4(instRGB, 1.0f);
			Normal = ciNormalMatrix * ciNormal;
		}
	);

	auto frag_prog = CI_GLSL(150,
		
		in vec4	Color;
		in vec3	Normal;
		in vec2	TexCoord;

		out vec4 oColor;

		void main(void)
		{
			vec3 normal = normalize(vec3(-Normal.x, Normal.y, -Normal.z));
			float diffuse = max(dot(normal, vec3(0.0, 1.0, 0.0)), 0);
			oColor = Color * diffuse;
		}
	);

	mTerrainShader = gl::GlslProg::create(vert_prog, frag_prog);
}

void genuary202603App::setupInstancedMesh()
{
	vector<vec3> rgb_data;
	vector<vec3> xyz_data;

	float halfx = (kNumWidth * kCellSize * 4.0f) * 0.5f;
	float halfz = (kNumDepth * kCellSize * 4.0f) * 0.5f;
	for (int z = 0; z < kNumDepth; z++)
	{
		float pz = -halfz + ((z+0.5f) * kCellSize * 4.0f);

		for (int x = 0; x < kNumWidth; x++)
		{
			float px = -halfx + ((x+0.5f) * kCellSize * 4.0f);
			xyz_data.push_back(vec3(px, 0.0f, pz));
			rgb_data.push_back(vec3(randFloat(), randFloat(), randFloat()));
		}
	}

	mTerrainXyzVbo = gl::Vbo::create(GL_ARRAY_BUFFER, xyz_data.size() * sizeof(vec3), xyz_data.data(), GL_DYNAMIC_DRAW);
	geom::BufferLayout xyzLayout;
	xyzLayout.append(geom::CUSTOM_0, 3, 0, 0, 1);
	
	mTerrainRgbVbo = gl::Vbo::create(GL_ARRAY_BUFFER, rgb_data.size() * sizeof(vec3), rgb_data.data(), GL_DYNAMIC_DRAW);
	geom::BufferLayout rgbLayout;
	rgbLayout.append(geom::CUSTOM_1, 3, 0, 0, 1);

	mTerrainShape = gl::VboMesh::create(geom::Cube().size(vec3(kCellSize)).subdivisions(2));
	mTerrainShape->appendVbo(xyzLayout, mTerrainXyzVbo);
	mTerrainShape->appendVbo(rgbLayout, mTerrainRgbVbo);

	setupShaders();

	mTerrainBatch = gl::Batch::create(mTerrainShape, mTerrainShader, { {geom::Attrib::CUSTOM_0, "instXYZ"},{geom::Attrib::CUSTOM_1, "instRGB"} });
}

void genuary202603App::updateTerrain( float gOffset )
{
	vec3 *pos = (vec3*)mTerrainXyzVbo->mapReplace();
	vec3* rgb = (vec3*)mTerrainRgbVbo->mapReplace();
	float halfx = (kNumWidth * kCellSize * 4.0f) * 0.5f;
	float halfz = (kNumDepth * kCellSize * 4.0f) * 0.5f;
	float yOffset = gOffset;

	for (int z = 0; z < kNumDepth; z++)
	{
		float xOffset = 0.0f;
		float pz = -halfz + ((z + 0.5f) * kCellSize * 4.0f);
		for (int x = 0; x < kNumWidth; x++)
		{
			float px = -halfx + ((x + 0.5f) * kCellSize * 4.0f);
			float y0 = lmap<float>(Perlin().noise(xOffset, yOffset), -0.5f, .5f, kNoiseMin, kNoiseMax);
			y0 = glm::clamp<float>(y0, 0.07f, kNoiseMax);
			*pos++ = vec3(px, y0, pz);
			xOffset += kXStep;

			vec3 newRgb = vec3(kBlue);
			if (y0 > 0.07f && y0 < 0.4f)
			{
				newRgb = kGreen;
			}
			else if (y0 > 0.4f && y0 < 0.6f)
			{
				newRgb = kBrown;
			}
			else if (y0 > 0.6f)
			{
				newRgb = kSnow;
			}

			*rgb++ = newRgb;

		}
		yOffset += kYStep;
	}
	mTerrainXyzVbo->unmap();
	mTerrainRgbVbo->unmap();

	/*mTerrainData.clear();
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
	}*/
}

void genuary202603App::setupFireworks()
{
	updateFireworks();
}

void genuary202603App::updateFireworks()
{
}

void prepareSettings( App::Settings* settings )
{
	settings->setWindowSize( 540, 960 );
}

CINDER_APP( genuary202603App, RendererGl, *prepareSettings )
