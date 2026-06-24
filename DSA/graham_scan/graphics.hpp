#ifndef GRAPHICS_HPP 
#define GRAPHICS_HPP

#include "point.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string> 
#include <vector> 
#include <unordered_map> 

// degub 
#include <iostream> 

#define DEFAULT_FONT "arial"

#define WHITE 	SDL_Color{255,255,255,255}
#define BLUE 	SDL_Color{0,0,255,255}
#define GREEN	SDL_Color{0,255,0,255}
#define LIME	SDL_Color{50, 205, 50,255}
#define YELLOW	SDL_Color{255,255,0,255}
#define RED		SDL_Color{255,0,0,255}
#define ROSE	SDL_Color{255,0,127,255}
#define MAGENTA	SDL_Color{255,0,144,255}
#define GRAY	SDL_Color{155,155,155,255}
#define BLACK 	SDL_Color{0,0,0,255}

#define INIT() 	SDL_Init(SDL_INIT_VIDEO);\	
				TTF_Init();\
				IMG_Init(IMG_INIT_PNG);

#define QUIT() SDL_Quit()

class Renderable {
protected: 

	SDL_Renderer* renderer;
	Renderable(SDL_Renderer* r) 
	: renderer(r){}

public: 

	virtual void render() const =0;

};

class TextBox : public Renderable {

	SDL_Texture* text;
	SDL_Rect rect;
	SDL_Color color;
	bool visible_box;

public: 

	TextBox(int x, int y, int w, int h, const std::string& text, SDL_Color color, SDL_Renderer* render, int textsize, bool box_visible = true) 
	: rect{x,y,w,h}, color(color), Renderable(render), visible_box(box_visible) {
		SDL_Surface* tmp = TTF_RenderText_Solid(TTF_OpenFont(DEFAULT_FONT , textsize), text.c_str(), WHITE);
		this -> text = SDL_CreateTextureFromSurface(renderer, tmp); 
		SDL_FreeSurface(tmp); 
	}

	void render() const override {

		if (visible_box) {
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b,255);
			SDL_RenderFillRect(renderer, &rect); 
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
			SDL_RenderDrawRect(renderer, &rect); 
		}

		static const SDL_Rect RenderRect = SDL_Rect{rect.x + 20, rect.y + 10, rect.w - 20, rect.h - 10};
		SDL_RenderCopy(renderer, text, NULL, &RenderRect); 
		
	}

	// bool is_clicked(Uint32 x, Uint32 y) {

	// 	if (x < rect.x || x > (rect.x + rect.w))
	// 		return false;
	// 	if (y < rect.y || y > (rect.y + rect.h))
	// 		return false;
	// 	return true;

	// }

	// void on_click() const {
	// 	function(); 
	// }

	~TextBox() {
		SDL_DestroyTexture(text); 
	}

};

void DrawLine(Point* p1, Point* p2, SDL_Color color, SDL_Renderer* render) {
	int y1 = (70 - p1 -> y)*10;
	int y2 = (70 - p2 -> y)*10;
	SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 255); 
	SDL_RenderDrawLine(render, p1 -> x*10, y1, p2 -> x*10, y2); 
	SDL_SetRenderDrawColor(render, 0,0,0, 255); 
}

SDL_Texture* fromImage(const char* img, SDL_Renderer* render) {
	SDL_Surface* tmp = IMG_Load(img); 
	SDL_Texture* t = SDL_CreateTextureFromSurface(render, tmp); 
	SDL_FreeSurface(tmp); 
	return t;
}

void RenderPoint(Point* p, SDL_Renderer* render, SDL_Texture* point) {
	int x = p  -> x * 10;
	int y = (70 - p -> y) * 10; 
	SDL_Rect renderRect{x - 3, y - 3, 6, 6}; 
	int v = SDL_RenderCopy(render, point, NULL, &renderRect); 
	if (v != 0) {
		const char* err = SDL_GetError(); 
		std::cout << err << std::endl;
	}
}

class Points : public Renderable {

	SDL_Texture* point_img;

public: 

	std::vector<Point*> points; 

	Points(SDL_Renderer* render) : Renderable(render) {
		point_img = fromImage("../dot.png", render); 
	}

	void add(double x, double y) {
		if (x <= 0 || y <= 0 || x >= 120 || y >= 70) return;
		points.push_back(new Point(x, y)); 
	}

	void render() const override {
		for (Point* p : points)
			RenderPoint(p, renderer, point_img);
	}

	~Points() {
		for (auto ptr : points) delete ptr;
	}

};

#endif
