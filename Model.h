#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
	public:
		Model(const char* file);

		void Draw(Shader& shader, Camera& camera);

	private:
		const char* file;
		std::vector<unsigned char> data;
		json JSON;

		vector<Mesh> meshes;
		vector<glm::vec3> translationMeshes;
		vector<glm::quat> rotationsMeshes;
		vector<glm::vec3> scalesMeshes;
		vector<glm::mat4> matricesMeshes;


		vector<string> loadedTexName;
		vector<Texture> loadedTex;

		void loadMesh(unsigned int indMesh);

		void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.f));

		vector<unsigned char> getData();
		vector<float> getFloats(json accessor);
		vector<GLuint> getIndices(json accessor);
		vector<Texture> getTextures();

		vector<Vertex> assembleVertices
		(
			vector<glm::vec3> positions,
			vector<glm::vec3> normals,
			vector<glm::vec2> texUVs
		);

		vector<glm::vec2> groupFloatsVec2(vector<float> floatVec);
		vector<glm::vec3> groupFloatsVec3(vector<float> floatVec);
		vector<glm::vec4> groupFloatsVec4(vector<float> floatVec);
};

#endif
