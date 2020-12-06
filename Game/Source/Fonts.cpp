#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include"Log.h"

#include<string.h>

Fonts::Fonts()
{
}

Fonts::~Fonts()
{
}

// Load new texture from file path
int Fonts::Load(const char* texturePath, const char* characters, uint rows)
{
	int id = -1;

	if (texturePath == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texturePath);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texturePath, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texturePath, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	app->tex->GetSize(font.texture, font.charW, font.charH);
	// totalLength ---	length of the lookup table
	font.totalLength = strlen(characters);
	// table ---------  All characters displayed in the same order as the texture
	strcpy_s(fonts[id].table, characters);
	// columns -------  Amount of chars per row of the texture
	font.columns = fonts[id].totalLength / rows;
	// char_w --------	Width of each character
	font.charW /= fonts[id].totalLength / rows;
	// char_h --------	Height of each character
	font.charH /= rows;

	++fontsCount;

	LOG("Successfully loaded BMP font from %s", texturePath);

	return id;
}

void Fonts::UnLoad(int fontId)
{
	if (fontId >= 0 && fontId < MAX_FONTS && fonts[fontId].texture != nullptr)
	{
		app->tex->UnLoad(fonts[fontId].texture);
		fonts[fontId].texture = nullptr;
		--fontsCount;

		LOG("Successfully Unloaded BMP font_id %d", fontId);
	}
}

void Fonts::BlitText(int x, int y, int fontIndex, const char* text) const
{
	if (text == nullptr || fontIndex < 0 || fontIndex >= MAX_FONTS || fonts[fontIndex].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", fontIndex);
		return;
	}

	//LOG("----------------------%d----------------------", font_index);

	const Font* font = &fonts[fontIndex];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->charW;
	spriteRect.h = font->charH;

	for (uint i = 0; i < len; ++i)
	{
		uint charIndex = 0;
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 0.0f, false);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}
