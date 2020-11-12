#include <stdio.h>
#include <stdint.h> // allows use of uint32_t 
#include <SDL2/SDL.h>
#include "constants.h"
#include "randoms.h"
#include "display.h"
//#include "rectangle.h"

// globals
int game_is_running = FALSE;
int n_rectangles_large; // number of LARGE rectangles we want to draw
int n_rectangles_small; // number of SMALL rectangles we want to draw
int rectangle_width_large;
int rectangle_height_large;
int rectangle_width_small;
int rectangle_height_small;

rectangle_t listOfRectangles[175];
int last_frame_time;
int delta_time;


//function declarations
void setup(void);
void process_input(void);
void update(void);
void render(void);
uint32_t randomColorUINT32(void);
void addNewRectangleToList(int rowNumber, int size);
int rectanglesDoOverlap(rectangle_t *r1, rectangle_t *r2);
int validNewRectangle(rectangle_t *candidateRect, int rowNumber);


// function definitions
int main(int argc, char* args[]) {
    n_rectangles_large = DEFAULT_N_RECTANGLES_LARGE;
    n_rectangles_small = DEFAULT_N_RECTANGLES_SMALL;
    rectangle_width_large = DEFAULT_RECTANGLE_WIDTH_LARGE;
    rectangle_height_large = DEFAULT_RECTANGLE_HEIGHT_LARGE;
    rectangle_width_small = DEFAULT_RECTANGLE_WIDTH_SMALL;
    rectangle_height_small = DEFAULT_RECTANGLE_HEIGHT_SMALL;   

    game_is_running = initialize_window();
    setup();
    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    return FALSE;
} // end main function

void setup(void) {
    
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
        );

    // seed the random number generator
    srand(time(NULL)); // seed random number generator

    // fill screen with black
    clear_color_buffer(BLACK);

    // build the list or rectangles to be drawn
    int largeRectangleCount = 0;
    int smallRectangleCount = 0;


   while(largeRectangleCount < n_rectangles_large){
        
        addNewRectangleToList(largeRectangleCount, 1); // 1 = large, 2 = small
        largeRectangleCount++;
        printf("\n%d large rectangles added to list",largeRectangleCount);
    }

    while(smallRectangleCount < n_rectangles_small){
            addNewRectangleToList(n_rectangles_large + smallRectangleCount, 0); // 1 = large, 2 = small

            smallRectangleCount++;
            printf("\n%d small rectangles added to list",smallRectangleCount);
        }

    printf("\nsetup completed");
    return;
} // end setup function

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        printf("\nuser QUIT");
        game_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE){
            printf("\nuser ESCaped");
            game_is_running = FALSE;           
        }
        break;
    }
} // end process_input function

void update(void) {
	
    static int nextRow = 0;

    // sleep the execution until we reach the target frame time in milliseconds
	int time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - last_frame_time);
	
	// only call delay if we are too fast to process this frame
	if(time_to_wait > 0 && time_to_wait <= TARGET_FRAME_TIME){
		SDL_Delay(time_to_wait);
	}
	
	//float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    if(nextRow < n_rectangles_large + n_rectangles_small){
        // give rectangle correct color
        (listOfRectangles[nextRow]).color = (nextRow <n_rectangles_large)? COLOR_LARGE : COLOR_SMALL;
        
        addRectangleToColorBuffer(&listOfRectangles[nextRow]);
        nextRow++;
    }

	last_frame_time = SDL_GetTicks();
} // end update function

void render(void) {

    render_color_buffer();

    SDL_RenderPresent(renderer); // actually do the rendering
} // end render function

uint32_t randomColorUINT32(void){
    
    // returns uint32 between 0xFF000000 and 0xFFFFFFFF
    int randomInt = rand();                     //   0 <= randomInt <= RAND_MAX
    randomInt = randomInt % ( 0xFFFFFFFF - 0xFF000000 + 1);                         
    return (uint32_t) randomInt + 0xFF000000;
} // end randomColorUINT32 function

void addNewRectangleToList(int rowNumber, int size){ // size 1 = large, 0 = small

    // place valid new rectangle in row n of the list of rectangles
    int landscape = (rand()%2 == 0)? 1 : 0; // 50% landscape 50% portrait

    int x_upperLimit;
    int y_upperLimit;
    int w;
    int h;

    if(landscape && size==1){
        x_upperLimit = window_width - rectangle_width_large;
        y_upperLimit = window_height - rectangle_height_large;
        w = rectangle_width_large;
        h = rectangle_height_large;
    }
    else 
    {
            if(landscape && size==0)
            {
                x_upperLimit = window_width - rectangle_width_small;
                y_upperLimit = window_height - rectangle_height_small;
                w = rectangle_width_small;
                h = rectangle_height_small;
            }
            else
            {
                if(!landscape && size==1) // portrait and large
                {
                    x_upperLimit = window_height - rectangle_height_large;
                    y_upperLimit = window_width - rectangle_width_large;
                    w = rectangle_height_large;
                    h = rectangle_width_large;
                }
                else // portrait and small
                {
                    x_upperLimit = window_height - rectangle_height_small;
                    y_upperLimit = window_width - rectangle_width_small;
                    w = rectangle_height_small;
                    h = rectangle_width_small;                   
                }
            }
    }

    int ex = randomIntegerInRange(0,x_upperLimit);
    int wy = randomIntegerInRange(0,y_upperLimit);
    rectangle_t candidate = {ex,wy,w,h,RED};

    while (validNewRectangle(&candidate,rowNumber) == FALSE){

        // try again
        candidate.x = randomIntegerInRange(0,x_upperLimit);
        candidate.y = randomIntegerInRange(0,y_upperLimit);
        
    } // end while

    // reach here only when new rectangle is valid, so add it to the list
    listOfRectangles[rowNumber] = candidate;

    return;
} // end of addNewRectangleToList function

int rectanglesDoOverlap(rectangle_t *r1, rectangle_t *r2){

    // return 1 if they overlap, else return 0;
    if( (r2->y + r2->h + MARGIN < r1->y) || (r2->y > r1->y + r1->h + MARGIN) || (r2->x + r2->w + MARGIN < r1->x) || (r2->x > r1->x + r1->w + MARGIN) )
    {
        return FALSE; // one of the conditions for not overlapping is true
    }
    else
    {
        return TRUE;
    }
} // end rectanglesDoOverlap function

int validNewRectangle(rectangle_t *candidateRect, int rowNumber){
    
    // return TRUE iff this candidate rectangle does not overlap any rectangle already in the list of rectangles
    for(int i = 0; i<rowNumber; i++){
        if(rectanglesDoOverlap(candidateRect, &listOfRectangles[i]))
        {
            return FALSE;
        }
    }
    
    // reach here only if we found no rectangle which this one overlaps
    return TRUE;
} // end validNewRectangle function










