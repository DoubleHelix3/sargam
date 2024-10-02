#include "util.h"
#include "audio.h"

#define high 2.0*
#define low  0.5*

void play_constant(float frequency, float duration) {
  set_frequency(frequency);
  wait(duration);
}

void play_interpolate(float start, float end, float duration) {
  float start_time = timer();
  while (timer() - start_time <= duration) {
    float t = (timer() - start_time) / duration;
    float frequency = start * pow(end/start, t); 
    set_frequency(frequency);
  }
}


void play_swars(float* swars, int swars_count, float beat_duration, float transition) {
  for (int swar_index = 0; swar_index < swars_count; swar_index++) {
    play_constant(swars[swar_index], beat_duration - transition);

    bool is_last_swar = swar_index == (swars_count - 1);
    if (!is_last_swar) {
      play_interpolate(swars[swar_index], swars[swar_index+1], transition);
    }
  }
  
}

int main() {
  setup_audio();

  float A = 240;

  float sa = A;
  float pa  = sa * (3.0/2.0);
  float shudho_re  = sa * (9.0/8.0);
  float shudho_ga  = sa * (5.0/4.0);
  float shudho_ma  = sa * (4.0/3.0);
  float shudho_dha = sa * (5.0/3.0);
  float shudho_ni  = sa * (15.0/8.0);
  float komol_re   = sa * (256.0/243.0);
  float komol_ga   = sa * (32.0/27.0);
  float komol_dha  = sa * (128.0/81.0);
  float komol_ni   = sa * (16.0/9.0);
  float teevra_ma = sa * (45.0/32.0);

  float re  = shudho_re;
  float ga  = shudho_ga;
  float ma  = teevra_ma;
  float dha = shudho_dha;
  float ni  = shudho_ni;

  float swars[] = {low ni, re, ga, ma, dha, ni, high re, high sa, ni, dha, pa, ma, ga, re, sa};
  int swars_count = ARRAY_COUNT(swars);

  play_swars(swars, swars_count, 0.2, 0.05);

  cleanup_audio();
}


/*
// hack
// from https://cboard.cprogramming.com/faq-board/27714-faq-there-getch-conio-equivalent-linux-unix.html
#include <termios.h>
#include <unistd.h>
int getch() {
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}


int main() {
  setup_audio();

  float C = 261.63;
  float A = 240;

  float sa = C;
  sa *= (4.0f/3.0f);
  float re  = sa * (9.0f/8.0f);
  float ga  = sa * (5.0f/4.0f);
  float ma  = sa * (4.0f/3.0f);
  float pa  = sa * (3.0f/2.0f);
  float dha = sa * (5.0f/3.0f);
  float ni  = sa * (15.0f/8.0f);

  float komol_re  = sa * (256.0f/243.0f);
  float komol_ga  = sa * (32.0f/27.0f);
  float komol_dha = sa * (128.0f/81.0f);
  float komol_ni  = sa * (16.0f/9.0f);

  float teevra_ma  = sa * (45.0f/32.0f);

  float swars[] = {sa, komol_re, re, komol_ga, ga, ma, teevra_ma, pa, komol_dha, dha, komol_ni, ni};
  int swars_count = ARRAY_COUNT(swars);

  char letters[] = {'s','r', 'r', 'g', 'g', 'm', 'm', 'p', 'd', 'd', 'n', 'n'};

  float raag_swars[] = {sa, re, ga, ma, pa, dha, ni};
  int raag_swars_count = ARRAY_COUNT(raag_swars);

  // hack
  system("stty -echo");

  play_constant(sa, 1);

  float last_swar = sa;
  float last_letter = 's';
  while (true) {
    float swar = last_swar; 
    float letter;
    while (true) {
      int random_index = random_int(0,swars_count);
      swar = swars[random_index];
      letter = letters[random_index];
      
      bool is_in_raag = false;
      for (
        int raag_swar_index = 0; raag_swar_index < raag_swars_count; raag_swar_index++
      ) {
        if (swar == raag_swars[raag_swar_index]) {
          is_in_raag = true;
        }
      }

      if (is_in_raag && letter != last_letter) break;
    }

    play_interpolate(last_swar, swar, 0.2);
    play_constant(swar, 0.3);

    bool is_correct = false;
    while (!is_correct) {
      char guess = getch();
      is_correct = guess == letter;
    }

    last_swar = swar;
    last_letter = letter;
  }

  cleanup_audio();
  return 0;
}
*/
