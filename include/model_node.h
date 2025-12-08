#ifndef _TOY_MODEL_NODE_H_
#define _TOY_MODEL_NODE_H_

#include <glad/glad.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>
#include <toynode.h>

#include <vector>
using namespace std;

class ModelNode : public ToyNode {
public:
    Shader shader;

public:
    ModelNode(Shader& shader, vector<Mesh>& meshes);
    ModelNode(Shader& shader, string const& path, bool gamma = false);

public:
    void Draw(const RenderContext& context) override;

private:
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

private:
    vector<Texture> texturesLoaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
};

#endif