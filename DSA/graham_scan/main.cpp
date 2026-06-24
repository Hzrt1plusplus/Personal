#include "./graphics.hpp"
#include "./hull.hpp"
#include <iostream> 

void renderHull(std::vector<Point*>& hull, SDL_Renderer* render) {

	Point* p = hull[0];
	for (int i = 1; i  < hull.size(); i++) {
		DrawLine(p, hull[i], YELLOW, render); 
		p = hull[i]; 
	}

	DrawLine(hull[hull.size() - 1], hull[0], YELLOW, render);

}

void mainloop(int np) {

	INIT();

	SDL_Window* window = SDL_CreateWindow("Graham Scan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 700, SDL_WINDOW_SHOWN);
	SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);	

	srand(time(NULL)); 
	Points figure(render); 
	for (int i = 0; i < np; i++)
	{
		int rx = rand()%120;
		int ry = rand()%70;
		figure.add(rx, ry); 
	}
	
	std::vector<Point*> hull;

	bool running = true;
	while (running) {

		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {

			if (ev.type == SDL_QUIT)
			{
				running = false;
				break;
			} 

			if (ev.type == SDL_KEYDOWN) 
			{
				if(ev.key.keysym.sym ==  SDLK_SPACE) {
						std::cout << "hull calculated!\n"; 
						hull = convex_hull(figure.points);
						std::cout << "hull size: " << hull.size() << std::endl; 
				}
			}
		}

		SDL_RenderClear(render);
		figure.render(); 

		if (!hull.empty()) {
			renderHull(hull, render);
		}

		SDL_RenderPresent(render);

	}

	SDL_DestroyWindow(window); 
	SDL_DestroyRenderer(render); 

	QUIT();

}

int main() 
{
	Points figure();
	while (true) {

		std::cout << "Enter number of points: "; 
		int np; std::cin >> np;

		mainloop(np); 
		
	}

	return 0;
}
