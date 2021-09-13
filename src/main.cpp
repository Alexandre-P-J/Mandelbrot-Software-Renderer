#include <SDL2/SDL.h>
#include <complex>

int mandelbrot_iters(std::complex<double> c, int max_iters) {
    std::complex<double> z(0, 0);
    int it = 0;
    while (it < max_iters) {
        z = z * z + c;
        if (std::abs(z) > 2.0)
            break;
        ++it;
    }
    return it;
}

void draw(int width, int height, int max_iterations, SDL_Window *window,
          SDL_Surface *surface, double xpos, double ypos, double zoom) {

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            std::complex<double> C(zoom * (x - (width - height) / 2) / height -
                                       xpos,
                                   zoom * y / height - ypos);
            int m = mandelbrot_iters(C, max_iterations);

            int color_value = 255 - int(float(m * 255) / max_iterations);

            ((Uint32 *)surface->pixels)[(y * surface->w) + x] = SDL_MapRGB(
                surface->format, color_value, color_value, color_value);
        }
    }
    SDL_UpdateWindowSurface(window);
}

// This signature is required by SDL2
int main(int argc, char *args[]) {
    int width = 1280;
    int height = 720;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s",
                     SDL_GetError());
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    double xpos = 3;
    double ypos = 3;
    double zoom = 5;

    bool quit = false;
    while (!quit) {
        // experimental zoom-iters relation
        int max_iterations = std::pow(std::log10(10e5 / zoom), 2);

        draw(width, height, max_iterations, window, surface, xpos, ypos, zoom);

        // Handle Events
        bool resize = false;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    width = event.window.data1;
                    height = event.window.data2;
                    resize = true;
                }
            } else if (event.type == SDL_KEYDOWN) {
                switch (((event.key).keysym).sym) {
                case SDLK_q:
                    quit = true;
                    break;
                case SDLK_PLUS:
                    zoom -= 0.1 * zoom;
                    break;
                case SDLK_MINUS:
                    zoom += 0.1 * zoom;
                    break;
                case SDLK_UP:
                    ypos += 0.1 * zoom;
                    break;
                case SDLK_DOWN:
                    ypos -= 0.1 * zoom;
                    break;
                case SDLK_LEFT:
                    xpos += 0.1 * zoom;
                    break;
                case SDLK_RIGHT:
                    xpos -= 0.1 * zoom;
                    break;
                default:
                    break;
                }
            }
        }
        if (resize) {
            resize = false;
            surface = SDL_GetWindowSurface(window);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}
