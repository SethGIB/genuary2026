#include "genuary202610App.h"

void genuary202610App::setup()
{
	mSphereJson = loadJson(loadAsset("sphere_points.json"));
	setupInstances();
	setupCamera();
}

void genuary202610App::mouseDown( MouseEvent event )
{
}

void genuary202610App::update()
{
}

void genuary202610App::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::enableDepthWrite();
	gl::enableDepthRead();
	gl::setMatrices(mCamera);
	mInstancedObjects->drawInstanced( 10 * 10 * 10 );
}

void genuary202610App::setupCamera()
{
	mCamera.setAspectRatio(getWindowAspectRatio());
	mCamera.lookAt(vec3(0, 0, -10), vec3(0), vec3(0, 1, 0));
	mNavCtrl = CameraUi(&mCamera, getWindow());
	
}

void genuary202610App::setupInstances()
{
	/*
		To draw instances, we need the following things (in Cinder parlance):
		1) data - this could be a list of custom structs or just something simple like a list of positions or colors as vec<n>
		2) gl::GlslProgRef - this is the shader. In this case, beyond material properties, it applies per-instance props (position, etc). Essentially our instance controller
		3) gl::VboRef - this is our actual per-instance data. We take this and apply it to our VboMesh
			3a) geom::BufferLayout - this determines "where" our attribute actually is, i.e. how how our data is mapped into the shader/instance controller
		4) gl::VboMeshRef - this is the container for our instance source. Think static mesh uAsset
		5) gl::BatchRef - this is the thing we actually draw. Holds instance mesh, shader, and attribute mapping
		Follow along:
	*/

	// 2) Shader/GlslProg Setup ///
	auto vert_prog = CI_GLSL(150,
		uniform mat4 ciModelViewProjection;
		uniform mat3 ciNormalMatrix;

		in vec4 ciPosition;
		in vec2 ciTexCoord0;
		in vec3 ciNormal;
		in vec4 ciColor;
		in vec3 vInstancePos; // per-instance position variable

		out lowp vec4 Color;
		out highp vec3 Normal;

		void main(void)
		{
			gl_Position = ciModelViewProjection * (4.0 * ciPosition + vec4(vInstancePos, 0));
			Color = ciColor;
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
			vec3 normal = normalize(-Normal);
			float diffuse = max(dot(normal, vec3(0, 0, -1)), 0);
			oColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * diffuse;
		}
	);

	gl::GlslProgRef shader_prog = gl::GlslProg::create(vert_prog, frag_prog);

	// 3) Vbo (vertex buffer object) setup
	vector<vec3> position_data;

	for (int y = 0; y < 10; y++)
	{
		float py = lmap((float)y+0.5f, 0.0f, 10.0f, -8.0f, 8.0f);
		for (int z = 0; z < 10; z++)
		{
			float pz = lmap((float)z+0.5f, 0.0f, 10.0f, -4.5f, 4.5f);
			for (int x = 0; x < 10; x++)
			{
				float px = lmap((float)x+0.5f, 0.0f, 10.0f, -4.5f, 4.5f);
				position_data.push_back(vec3(px, py, pz));
			}
		}
	}

	
	gl::VboRef instance_data = gl::Vbo::create(GL_ARRAY_BUFFER, position_data.size() * sizeof(vec3), position_data.data(), GL_DYNAMIC_DRAW);
	// 3a) Buffer Layout
	geom::BufferLayout attrib_layout;
	attrib_layout.append(geom::Attrib::CUSTOM_0, 3, 0, 0, 1);

	
	// 4) VboMesh setup
	gl::VboMeshRef instance_mesh = gl::VboMesh::create(geom::Sphere().radius(0.05f).subdivisions(16));
	instance_mesh->appendVbo(attrib_layout, instance_data);

	// 5) BatchRef setup
	mInstancedObjects = gl::Batch::create(instance_mesh, shader_prog, { {geom::Attrib::CUSTOM_0, "vInstancePos"}});
}

void prepareSettings(App::Settings* settings)
{
	settings->setWindowSize(540, 960);
}

CINDER_APP( genuary202610App, RendererGl, prepareSettings )
