#include "Material.h"



Material::Material()
{
}


Material::~Material()
{
}


string FBXTextureTypeToString(FBXMaterial::TextureTypes textureType)
{
	switch (textureType)
	{
	case FBXMaterial::DiffuseTexture:
		return "diffuseTexture";
	case FBXMaterial::AmbientTexture:
		return "ambientTexture";
	case FBXMaterial::GlowTexture:
		return "glowTexture";
	case FBXMaterial::SpecularTexture:
		return "specularTexture";
	case FBXMaterial::GlossTexture:
		return "glossTexture";
	case FBXMaterial::NormalTexture:
		return "normalTexture";
	case FBXMaterial::AlphaTexture:
		return "alphaTexture";
	case FBXMaterial::DisplacementTexture:
		return "displacementTexture";
	default:
		break;
	}
}


void Material::Load(FBXMaterial * material)
{
	for (uint index = 0; index < FBXMaterial::TextureTypes_Count; index++)
	{
		auto name = FBXTextureTypeToString((FBXMaterial::TextureTypes)index);
		
		if (material->textures[index])
		{
			//Add(name, 
			//	material->textures[index]->data, 
			//	material->textures[index]->width, 
			//	material->textures[index]->height);
			Add(name, material->textures[index]->path);
		}
		else
		{
			Add(name, nullptr);
		}
	}
}


void Material::Add(string name, Texture * texture)
{
	textures[name] = texture;
}


bool Material::CheckGLErrors()
{
	// TODO
	return true;
}


void Material::GLDelete()
{
	// Do nothing
}


void Material::Init(ShaderProgram& shaderProgram)
{
	shaderProgram.Use();

	Create();

	SetSlotUniforms(shaderProgram);
}


void Material::Create()
{
	CreateSlots();
}

void Material::CreateSlots()
{
	uint slot = 0;
	for (auto& kv : textures)
	{
		if (slot > MAX_TEXTURE_SLOT)
			throw std::exception("Too many textures!");
		
		if (kv.second)
		{
			glActiveTexture(ToGLSlot(slot));
			kv.second->Use();
		}

		slot++;
	}
}


void Material::SetSlotUniforms(ShaderProgram& shaderProgram)
{
	uint slot = 0;
	for (auto& kv : textures)
	{
		if (kv.second)
			shaderProgram.SetUniform(kv.first, (int)slot);

		slot++;
	}
}


void Material::Add(string name, string location)
{
	auto texture = new Texture();
	texture->Load(location);

	Add(name, texture);
}


void Material::Add(string name, uchar* data, int width, int height)
{
	auto texture = new Texture();
	texture->Load(data, width, height);

	Add(name, texture);
}