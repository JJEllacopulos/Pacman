#include<allegro.h>
#include<cstdlib>


#define MAXFILAS 20
#define MAXCOLS  31

BITMAP *buffer;
BITMAP *roca;
BITMAP *pacbmp;
BITMAP *pacman;
BITMAP *comida;

BITMAP *muertebmp;

MIDI *musica_1;
SAMPLE *bolita;
SAMPLE *caminando;
SAMPLE *muerte;


///Pacman
int dir = 4;
int px = 30*14, py = 30*11;
int anterior_px, anterior_py;


char mapa[MAXFILAS][MAXCOLS]={
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "X  o |o o o XXXXX o o o| o  X",
  "X XXX XXXXX XXXXX XXXXX XXX X",
  "XoXXX XXXXX XXXXX XXXXX XXXoX",
  "X      o|o   o o   o|o      X",
  "XoXXXoXX XXXXXXXXXXX XXoXXXoX",
  "X    |XX    |XXX|    XX     X",
  "XoXXXoXXXXXX XXX XXXXXXoXXXoX",
  "X XXXoXX ooo|ooo|ooo XXoXXX X",
  " o   |XX XXXXXXXXXXX XX|   o ",
  "X XXXoXX XXXXXXXXXXX XXoXXX X",
  "XoXXXoXX oo |ooo|ooo XXoXXXoX",
  "X XXXoXXXXXX XXX XXXXXXoXXX X",
  "X     XX     XXX     XX     X",
  "X XXXoXX XXXXXXXXXXX XXoXXX X",
  "XoXXX| o| o o o o o |o |XXXoX",
  "X XXXoXXXX XXXXXXXX XXX XXX X",
  "XoXXXoXXXX          XXX XXXoX",
  "X  o |o o  XXXXXXXX o o| o  X",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};


void dibujar_mapa(){

    int row, col;

    for(row = 0 ; row < MAXFILAS ; row++){
        for(col = 0 ; col < MAXCOLS ; col++){
            if(mapa[row][col] == 'X'){
                draw_sprite(buffer, roca, col * 30, row * 30);
            }
            else if(mapa[row][col] == 'o'){
                draw_sprite(buffer, comida, col * 30, row * 30);
                if(py/30 == row && px/30 == col){

                    ///play_sample(bolita, 300, 150, 1000, 0);
                    ///(Variable de audio que se reproducira, volumen del audio, velocidad del audio, distribucion del sonido, "0" hara que solo se reproduciera una sola ves)

                    mapa[row][col] = ' ';
                }
            }
        }
    }
}

void pantalla(){
    blit(buffer, screen, 0, 0, 0, 0, 880, 600);
}

void dibujar_personaje(){
    blit(pacbmp, pacman, dir * 33, 0, 0, 0, 33, 33);
    draw_sprite(buffer, pacman, px, py);
}

bool game_over(){
    int row, col;

    for(row = 0 ; row < MAXFILAS ; row++){
        for(col = 0 ; col < MAXCOLS ; col++){
            if(mapa[row][col] == 'o') return true;
        }
    }
    return false;

}

class fantasma{
    private:
    int fdir;
    int _x, _y;
    BITMAP *enemigobmp;
    BITMAP *enemigo;

    public:
    fantasma(int x, int y);
    void dibujar_fantasma();
    void choque_pacman();
    void mover_fantasma();

};

fantasma::fantasma(int x, int y){
    _x = x;
    _y = y;
    fdir = rand() % 4;

    enemigobmp = load_bitmap("enemigo.bmp", NULL);
    enemigo = create_bitmap(30, 30);
}
void fantasma::dibujar_fantasma(){

    blit(enemigobmp, enemigo, 0, 0, 0, 0, 33, 33);
    draw_sprite(buffer, enemigo, _x, _y);
}

void fantasma::choque_pacman(){

    if(py == _y && px == _x || _y == anterior_py && _x == anterior_px){

        ///play_sample(muerte, 300, 150, 1000, 0);

        for(int j = 0 ; j <= 5 ; j++){

            clear(pacman);
            clear(buffer);
            dibujar_mapa();

            blit(muertebmp, pacman, j * 33, 0, 0, 0, 33, 33);
            draw_sprite(buffer, pacman, px, py);

            pantalla();
            rest(80);

        }

        px = 14 * 30;
        py = 11 * 30;
        dir = 4;

    }

}

void fantasma::mover_fantasma(){
    choque_pacman();
    dibujar_fantasma();

    if(fdir == 0){
        if(mapa[_y/30][(_x-30)/30] != 'X'){
            _x -= 30;
        }
        else{
            fdir = rand() % 4;
        }
    }

    if(fdir == 1){
        if(mapa[_y/30][(_x+30)/30] != 'X'){
            _x += 30;
        }
        else{
            fdir = rand() % 4;
        }
    }

    if(fdir == 2){
        if(mapa[(_y-30)/30][_x/30] != 'X'){
            _y -= 30;
        }
        else{
            fdir = rand() % 4;
        }
    }

    if(fdir == 3){
        if(mapa[(_y+30)/30][_x/30] != 'X'){
            _y += 30;
        }
        else{
            fdir = rand() % 4;
        }
    }

    ///Rutina de transportacion:

    if(_x <= -30) _x = 870;
        else if(_x >= 870) _x = -30;

}

int main(){
    allegro_init();
    install_keyboard();

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 600, 0, 0);

    ///Inicializar el audio en alegro:
    if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0){
        allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
        return 1;
    }

    ///Ajustamos el volumen:
    set_volume(70, 70);

    musica_1 = load_midi("13. Field of Hopes and Dreams.mp3.mid");
    bolita = load_wav("coin.wav");
    caminando = load_wav("jump.wav");
    muerte = load_wav("Muerte.wav");

    buffer = create_bitmap(880, 600);
    roca = load_bitmap("roca.bmp", NULL);
    comida = load_bitmap("Comida.bmp", NULL);

    ///Pacman
    pacbmp = load_bitmap("pacman.bmp", NULL);
    pacman = create_bitmap(33, 33);

    muertebmp = load_bitmap("muerte.bmp", NULL);

    fantasma A(30*2, 30*3);
    fantasma B(30*2, 30*3);


    play_midi(musica_1, 1);///El '1' es para que se reprodusca en bucle.

    while(!key[KEY_ESC] && game_over()){

        if(dir != 4){
            ///play_sample(caminando, 300, 150, 1000, 0);
        }

        anterior_px = px;
        anterior_py = py;

        ///Rutina de mover el pacman:
        if(key[KEY_RIGHT]){
                dir = 1;
        }
        else if(key[KEY_LEFT]){
                dir = 0;
        }
        else if(key[KEY_UP]){
                dir = 2;
        }
        else if(key[KEY_DOWN]){
                dir = 3;
        }

        if(dir == 0){
            if(mapa[py/30][(px-30)/30] != 'X'){
                px -= 30;
            }
            else dir = 4;

        }
        else if(dir == 1){
            if(mapa[py/30][(px+30)/30] != 'X'){
                px += 30;
            }
            else dir = 4;
        }
        else if(dir == 2){
            if(mapa[(py-30)/30][px/30] != 'X'){
                py -= 30;
            }
            else dir = 4;
        }
        else if(dir == 3){
            if(mapa[(py+30)/30][px/30] != 'X'){
                py += 30;
            }
            else dir = 4;
        }

        ///Rutina de transportacion:

        if(px <= -30) px = 870;
            else if(px >= 870) px = -30;

        clear(buffer);
        dibujar_mapa();
        dibujar_personaje();

        A.mover_fantasma();
        B.mover_fantasma();

        pantalla();
        rest(150);

        clear(pacman);
        blit(pacbmp, pacman, 4 * 33, 0, 0, 0, 33, 33);
        draw_sprite(buffer, pacman, px, py);
        pantalla();
        rest(75);
    }
}
END_OF_MAIN();
