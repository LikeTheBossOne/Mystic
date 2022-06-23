#include "Renderer3D.h"

#include <array>

#include "Mesh.h"
#include "RenderCommand.h"
#include "Mystic/Core/Core.h"
#include "Mystic/Render/VertexArray.h"
#include "Mystic/Render/Texture.h"
#include "Mystic/Render/Shader.h"
#include "Mystic/Render/UniformBuffer.h"

namespace Mystic
{

	struct DefaultVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//float TexIndex;
		//float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct RenderBatchData
	{
		Ref<VertexArray> BatchVertexArray;
		Ref<VertexBuffer> BatchVertexBuffer;
		Ref<Shader> BatchTextureShader;

		uint32_t IndexCount = 0;
		float* VertexBufferBase = nullptr;
		float* VertexBufferPtr = nullptr;
	};

	struct Renderer3DData
	{
		// Maybe should make this per Batch
		static const uint32_t MaxMeshes = 10;
		static const uint32_t MaxVertices = MaxMeshes * 100000;
		static const uint32_t MaxIndices = MaxMeshes * 150000;

		Renderer3D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

		// Per-shader, then per-mesh
		std::unordered_map<std::string, std::unordered_map<std::string, Ref<RenderBatchData>>> BatchMap;
		Ref<ShaderLibrary> ShaderLib;
		Ref<MeshLibrary> MeshLib;

		Ref<Texture2D> WhiteTexture;
		std::array<Ref<Texture2D>, 32> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
		s_Data.ShaderLib = std::make_shared<ShaderLibrary>();
		s_Data.MeshLib = std::make_shared<MeshLibrary>();

		s_Data.WhiteTexture = Texture2D::Create("WhiteTexture", 1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer3D::Shutdown()
	{
		for (auto shaderGroup : s_Data.BatchMap)
		{
			for (auto batch : shaderGroup.second)
			{
				delete[] batch.second->VertexBufferBase;
			}
		}
	}

	/*
	 * Right now, this assumes that the bufferlayout starts with position, [normal], and UV.
	 */
	void Renderer3D::RegisterBatch(std::string shaderName, std::string shaderPath, std::string meshName, std::string meshPath, BufferLayout bufferLayout)
	{
		if (!s_Data.BatchMap.contains(shaderName))
		{
			// is this necessary?
			s_Data.BatchMap.emplace(shaderName, std::unordered_map<std::string, Ref<RenderBatchData>>());
		}

		if (s_Data.BatchMap[shaderName].contains(meshName))
		{
			assert(false, "Batch already exists for mesh");
			return;
		}


		Ref<RenderBatchData> batch = std::make_shared<RenderBatchData>();
		batch->BatchTextureShader = s_Data.ShaderLib->Load(shaderName, shaderPath, bufferLayout);

		batch->BatchVertexArray = VertexArray::Create();

		batch->BatchVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices);
		batch->BatchVertexBuffer->SetLayout(bufferLayout);
		batch->BatchVertexArray->AddVertexBuffer(batch->BatchVertexBuffer);

		batch->VertexBufferBase = new float[s_Data.MaxVertices];

		Ref<Mesh> mesh = s_Data.MeshLib->LoadFromMystAsset(meshName, meshPath);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(mesh->indices.data(), mesh->indices.size());
		batch->BatchVertexArray->SetIndexBuffer(indexBuffer);

		s_Data.BatchMap[shaderName].emplace(meshName, batch);
	}

	void Renderer3D::RegisterBatch(Ref<Shader> shader, Ref<Mesh> mesh)
	{
		if (!s_Data.BatchMap.contains(shader->GetName()))
		{
			// is this necessary?
			s_Data.BatchMap.emplace(shader->GetName(), std::unordered_map<std::string, Ref<RenderBatchData>>());
		}

		if (s_Data.BatchMap[shader->GetName()].contains(mesh->GetName()))
		{
			assert(false, "Batch already exists for mesh");
			return;
		}


		Ref<RenderBatchData> batch = std::make_shared<RenderBatchData>();
		batch->BatchTextureShader = shader;
		s_Data.ShaderLib->Add(shader->GetName(), shader);

		batch->BatchVertexArray = VertexArray::Create();

		batch->BatchVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices);
		batch->BatchVertexBuffer->SetLayout(shader->GetBufferLayout());
		batch->BatchVertexArray->AddVertexBuffer(batch->BatchVertexBuffer);

		batch->VertexBufferBase = new float[s_Data.MaxVertices];

		s_Data.MeshLib->Add(mesh->GetName(), mesh);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(mesh->indices.data(), mesh->indices.size());
		batch->BatchVertexArray->SetIndexBuffer(indexBuffer);

		s_Data.BatchMap[shader->GetName()].emplace(mesh->GetName(), batch);
	}

	bool Renderer3D::BatchExists(const std::string& shaderName, const std::string& meshName)
	{
		if (!s_Data.BatchMap.contains(shaderName))
		{
			return false;
		}

		return s_Data.BatchMap[shaderName].contains(meshName);
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatches();
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatches();
	}

	void Renderer3D::BeginScene(const PerspectiveCamera& camera)
	{
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatches();
	}

	void Renderer3D::EndScene()
	{
		Flush();
	}

	void Renderer3D::Flush()
	{
		bool first = true;

		for (auto shaderGroup: s_Data.BatchMap)
		{
			s_Data.ShaderLib->Get(shaderGroup.first)->Bind();
			for (auto batch : shaderGroup.second)
			{
				if (batch.second->IndexCount == 0)
					continue;
				
				if (first)
				{
					// Bind textures
					for (int i = 0; i < s_Data.TextureSlotIndex; i++)
						s_Data.TextureSlots[i]->Bind(i);
					first = false;
				}

				uint32_t dataSize = (uint32_t)((uint8_t*)batch.second->VertexBufferPtr - (uint8_t*)batch.second->VertexBufferBase);
				batch.second->BatchVertexBuffer->SetData(batch.second->VertexBufferBase, dataSize);

				RenderCommand::DrawIndexed(batch.second->BatchVertexArray, batch.second->IndexCount);
				s_Data.Stats.DrawCalls++;
			}
		}
	}

	void Renderer3D::DrawModel(const glm::mat4& transform, const std::string shaderName, const std::string meshName, int entityID)
	{
		DrawModel(transform, shaderName, meshName, nullptr, entityID);
	}

	// In the future, consolidate Shader and Texture into a single Material object
	void Renderer3D::DrawModel(const glm::mat4& transform, const std::string shaderName, const std::string meshName, const Ref<Texture2D>& texture, int entityID)
	{
		// Find batch and add to it
		bool foundBatch = s_Data.BatchMap.contains(shaderName);
		assert(foundBatch, "Tried to access shader: " + shaderName + " that was not loaded.");
		if (!foundBatch)
			return;

		foundBatch = s_Data.BatchMap[shaderName].contains(meshName);
		assert(foundBatch, "Tried to access mesh: " + mesh->GetName() + " that was not loaded.");
		if (!foundBatch)
			return;


		Ref<RenderBatchData> currentBatch = s_Data.BatchMap[shaderName][meshName];

		float texIndex = 0.0f;
		if (texture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *texture)
				{
					texIndex = (float)i;
					break;
				}
			}

			if (texIndex == 0.0f)
			{
				if (s_Data.TextureSlotIndex >= s_Data.TextureSlots.max_size())
					assert(false, "Too many textures registererd");

				texIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		Ref<Mesh> mesh = s_Data.MeshLib->Get(meshName);

		// Loop over buffer layout and add elements. Assumes all layouts begin with a_position -> [a_normal ->] a_UV
		uint32_t dataIdx = 0;
		while (dataIdx < mesh->RawData.size())
		{
			uint8_t rawLen = 0;
			if (mesh->HasUVs)
				rawLen += 2;
			if (mesh->HasNormals)
				rawLen += 3;
			
			glm::vec4 pos = { mesh->RawData[dataIdx++], mesh->RawData[dataIdx++], mesh->RawData[dataIdx++], 1.0f };
			glm::vec3 worldPos = transform * pos;
			for (uint8_t i = 0; i < 3; i++)
			{
				*currentBatch->VertexBufferPtr = worldPos[i];
				currentBatch->VertexBufferPtr++;
			}
			for (uint8_t i = 0; i < rawLen; i++)
			{
				*currentBatch->VertexBufferPtr = mesh->RawData[dataIdx++];
				currentBatch->VertexBufferPtr++;
			}

			uint8_t layoutIdx = 0;
			// extra elements
			for (BufferElement element : currentBatch->BatchVertexBuffer->GetLayout())
			{
				if (layoutIdx == 0 ||
					(layoutIdx == 1 && (mesh->HasNormals || mesh->HasUVs)) ||
					(layoutIdx == 2 && mesh->HasNormals && mesh->HasUVs))
				{
					layoutIdx++;
					continue;
				}


				if (element.Name == "a_EntityID")
				{
					*currentBatch->VertexBufferPtr = std::bit_cast<float>(entityID);
				}
				else if (element.Name == "a_TexIndex")
				{
					if (texIndex > 0)
					{
						*currentBatch->VertexBufferPtr = texIndex;
					}
					else
					{
						*currentBatch->VertexBufferPtr = 0;
					}
				}

				// For other elements, we will need to pass in a "Material" that will have all of the calculated values passed into the shader.
				// (Think Unity Shader Graph)

				currentBatch->VertexBufferPtr++;
				layoutIdx++;
			}
		}

		currentBatch->IndexCount += mesh->indices.size();

		s_Data.Stats.MeshCount++;
	}

	void Renderer3D::StartBatches()
	{
		for (auto shaderGroup : s_Data.BatchMap)
		{
			for (auto batch : shaderGroup.second)
			{
				batch.second->IndexCount = 0;
				batch.second->VertexBufferPtr = batch.second->VertexBufferBase;
			}
		}

		s_Data.TextureSlotIndex = 1;
	}
}
