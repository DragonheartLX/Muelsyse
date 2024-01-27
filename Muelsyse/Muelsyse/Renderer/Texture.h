#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Asset/Asset.h"
#include "Muelsyse/Core/Buffer.h"
#include <string>

namespace mul
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};
	
	class MUL_API Texture: public Asset
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getRendererID() const = 0;

		virtual void setData(Buffer data) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;

		virtual bool isLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class MUL_API Texture2D: public Texture
	{
	public:
		static Ref<Texture2D> create(const TextureSpecification& specification, Buffer data = Buffer());

		static AssetType getStaticType() { return AssetType::Texture2D; }
		virtual AssetType getType() const { return getStaticType(); }
	};
}