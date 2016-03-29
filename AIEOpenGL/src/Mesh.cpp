#include "Mesh.h"


using glm::vec3;
using glm::vec4;
using glm::mat4;

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}


void Mesh::Init(FBXFile * _fbx)
{
	fbx = _fbx;
	Create();

	shaderProgram.Load("data/shaders/lighting.shader");

}


void Mesh::Load(string path)
{
	auto _fbx = new FBXFile();
	_fbx->load(path.c_str());

	Init(_fbx);
}


void Mesh::Create()
{
	vector<vec3> vertexPoints;

	// create the GL VAO/VBO/IBO data for each mesh
	for (uint index = 0; index < fbx->getMeshCount(); ++index)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(index);
		materials.push_back(Material());

		materials[materials.size() - 1].Load(mesh->m_material);

		renderer.GenerateBuffers(RenderData::Buffers::ALL);
		renderer.Bind();

		glBufferData(GL_ARRAY_BUFFER,
			mesh->m_vertices.size() * sizeof(FBXVertex),
			mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->m_indices.size() * sizeof(unsigned int),
			mesh->m_indices.data(), GL_STATIC_DRAW);


		uint i = vertexPoints.size();
		vertexPoints.resize(vertexPoints.size() + mesh->m_vertices.size());
		for (auto& vertex : mesh->m_vertices)
		{
			vertexPoints[i] = vec3(vertex.position.x, 
								   vertex.position.y, 
								   vertex.position.z);
		}


		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1); // color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::ColourOffset);

		glEnableVertexAttribArray(2); // normal
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::NormalOffset);


		glEnableVertexAttribArray(3); // tangent
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::TangentOffset);

		glEnableVertexAttribArray(4); // BiNormal
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::BiNormalOffset);

		glEnableVertexAttribArray(5); // Indices
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::IndicesOffset);

		glEnableVertexAttribArray(6); // Weights
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::WeightsOffset);

		glEnableVertexAttribArray(7); // TexCoord1
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::TexCoord1Offset);

		glEnableVertexAttribArray(8); // Texcoord2
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::TexCoord2Offset);

		renderer.Unbind();

		mesh->m_userData = renderer.CreateRaw();
	}

	boundingSphere.Fit(vertexPoints);
}


void Mesh::Draw(const Camera& camera, const vec3& lightDirection, const vec3& lightColor)
{
	shaderProgram.Use();

	shaderProgram.SetUniform("ProjectionView", camera.getProjectionView());
	shaderProgram.SetUniform("LightDir", lightDirection);
	//shaderProgram.SetUniform("LightColor", lightColor);
	shaderProgram.SetUniform("CameraPos", vec3(camera.getTransform()[3]));
	shaderProgram.SetUniform("Model", transform.GetTransform());
	//shaderProgram.SetUniform("SpecPow", 5.0f);

	// Render if on screen
	if (boundingSphere.IsTouchingFrustumPlanes(camera.getProjectionView(), vec3(transform.GetTransform()[3])))
	{
		if (!isVisible)
		{
			isVisible = true;
			printf("mesh is visible\n");
		}

		// bind our vertex array object and draw the mesh
		for (uint index = 0; index < fbx->getMeshCount(); index++)
		{
			FBXMeshNode* mesh = fbx->getMeshByIndex(index);

			materials[index].Init(shaderProgram);

			uint* glData = (uint*)mesh->m_userData;

			glBindVertexArray(glData[0]);
			glDrawElements(GL_TRIANGLES,
				(uint)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
	else if (isVisible)
	{
		isVisible = false;
		printf("mesh is hidden\n");
	}

	shaderProgram.Unbind();
}


void Mesh::GLDelete()
{
	// Clean up the vertices attached to each mesh
	for (uint index = 0; index < fbx->getMeshCount(); index++) 
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(index);
		uint* glData = (uint*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}

	shaderProgram.GLDelete();
}

