#pragma once
#include <string>
#include "JREngine/Asset/Asset.h"

struct _TTF_Font;
namespace JRE
{
	//RAII wrapper for a _TTF_Font
	class Font final : public Asset
	{
	public:
		explicit Font(const std::filesystem::path& path, unsigned int size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;

		_TTF_Font* GetFont() const;

		static constexpr std::string_view GetStaticType() { return "Font"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };
	private:
		_TTF_Font* m_pFont{};
	};
}
