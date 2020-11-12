#include "display.h"
#include "constants.h"

// globals
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL; // pointer to array of unit32_t
SDL_Texture *color_buffer_texture = NULL;
int window_width;
int window_height;

int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "\nError initializing SDL.\n");
        return FALSE;
    }

    // set window height to 95% of the maximum possible height
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0,&display_mode);
    window_height = 95*(display_mode.h/100);
    window_width = window_height; // make the window square

    window = SDL_CreateWindow(
        "P I X E L W R I N K L Y",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_OPENGL
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }
    return TRUE;
} // end initialize_window function

void render_color_buffer(void){
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
} // end render_color_buffer function

void clear_color_buffer(uint32_t color){
    for(int y = 0; y < window_height; y++){
        for(int x = 0; x < window_width; x++){
            color_buffer[y*window_width + x] = color;
        }
    }
} // end clear_color_buffer function

void addRectangleToColorBuffer(rectangle_t *rect){

    //printf("\nrequested a rectangle: \nx=%d\ny=%d\nwidth=%d\nheight=%d\ncolor=%u",x,y,width,height,culler);
    for(int row = 0; row < rect->h; row++){
        for(int col = 0; col < rect->w; col++){
            color_buffer[(rect->y+row)*window_width + rect->x + col] = rect->color;
        }
    }
} // end addRectangleToColorBuffer

void destroy_window(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("\ncolor buffer freed, renderer and window both destroyed; \nSDL has quit\n");
}













