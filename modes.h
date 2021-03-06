#ifndef __MODES_H
#define __MODES_H

#include <Arduino.h>

#define PALETTE_SIZE 8
#define MEMORY_SIZE ((PALETTE_SIZE + 2) * 3)
#define MEMORY_START 520

#define A_SPEED 0
#define A_TILTX 1
#define A_TILTY 2
#define A_FLIPZ 3

void unpackHue(uint16_t hue, uint8_t *r, uint8_t *g, uint8_t *b);
void unpackColor(uint8_t color, uint8_t *r, uint8_t *g, uint8_t *b);

/* Primes are used for the current two modes to define the animation. There are
 * currently 2 primes: strobe and tracer.
 *
 * Strobe takes color_time and blank_time arguments. Requires num_colors set
 * to the highest palette idx to use. Some popular timings are:
 *   - Strobe: 5, 8
 *   - Hyperstrobe: 17, 17
 *   - DOPS: 2, 10 (real DOPS uses 1.5, 10, but until I support half-ms it's 2)
 *   - Strobie: 3, 23
 *   - Chroma: 11
 *
 * Tracer takes color_time, tracer_time arguments. Requires
 * num_colors set to the highest palette idx to use.
 *
 * BlinkE takes color_time and blank_time. Each color is shown for color_time
 * which is followed by blink_time.
 */
class Prime {
  public:
    Prime() {}

    void renderColor(uint8_t *r, uint8_t *g, uint8_t *b);

    virtual void render(uint8_t *r, uint8_t *g, uint8_t *b) {}
    virtual void reset() {}
    virtual void incTick() {}

    bool canConfig() { return true; }
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    void save(uint16_t addr);
    void load(uint16_t addr);

    uint16_t tick;
    uint8_t palette[PALETTE_SIZE];
    uint8_t num_colors;
    uint8_t cur_color, edit_color;
};

// Popular timings
// Strobe; 5, 8
// Hyperstrobe: 17, 17
// DOPS: 1.5 (use 2), 10
// Strobie: 3, 23
// Chroma: 11, 0
class StrobePrime : public Prime {
  public:
    StrobePrime(uint16_t color_time, uint16_t blank_time) : Prime(), color_time(color_time), blank_time(blank_time) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
};

class TracerPrime : public Prime {
  public:
    TracerPrime(uint16_t color_time, uint16_t tracer_time) : Prime(), color_time(color_time), tracer_time(tracer_time) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, tracer_time;
};

class BlinkEPrime : public Prime {
  public:
    BlinkEPrime(uint16_t color_time, uint16_t blank_time) : Prime(), color_time(color_time), blank_time(blank_time) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
};

class MorphPrime : public Prime {
  public:
    MorphPrime(uint16_t color_time, uint16_t blank_time, uint8_t strobes) :
      Prime(), color_time(color_time), blank_time(blank_time), strobes(strobes) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
    uint8_t strobes, cur_strobe;
};

class FadePrime : public Prime {
  public:
    FadePrime(uint16_t color_time, uint16_t blank_time, uint8_t dir) :
      Prime(), color_time(color_time), blank_time(blank_time), dir(dir) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
    uint8_t dir;
};

class CandyStrobePrime : public Prime {
  public:
    CandyStrobePrime(uint16_t color_time, uint16_t blank_time, uint8_t batch, uint8_t repeats) :
      Prime(), color_time(color_time), blank_time(blank_time), batch(batch), repeats(repeats) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
    uint8_t batch, cur_batch;
    uint8_t repeat, repeats;
};

class ChasePrime : public Prime {
  public:
    ChasePrime(uint16_t color_time, uint16_t blank_time, uint8_t steps) :
      Prime(), color_time(color_time), blank_time(blank_time), steps(steps) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
    uint8_t steps, cur_step;
};

class TwoTimePrime : public Prime {
  public:
    TwoTimePrime(uint16_t color_time_a, uint16_t blank_time_a, uint16_t color_time_b, uint16_t blank_time_b) :
      Prime(), color_time_a(color_time_a), blank_time_a(blank_time_a), color_time_b(color_time_b), blank_time_b(blank_time_b) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time_a, blank_time_a, color_time_b, blank_time_b;
};

class LegoPrime : public Prime { // Credit Andrew Suchan
  public:
    LegoPrime(uint16_t blank_time) : Prime(), blank_time(blank_time) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
};

class EmberPrime : public Prime { // Credit Andrew Suchan
  public:
    EmberPrime(uint16_t color_time, uint16_t blank_time, uint8_t speed) :
      Prime(), color_time(color_time), blank_time(blank_time), speed(speed) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();

    uint16_t color_time, blank_time;
    int16_t extra_blank, dir;
    uint8_t speed;
};

class RainbowPrime : public Prime {
  public:
    RainbowPrime(uint16_t color_time, uint16_t blank_time, uint16_t splits, uint16_t split_dist, uint16_t speed) :
      Prime(), color_time(color_time), blank_time(blank_time), splits(splits), split_dist(split_dist), speed(speed) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void incTick();
    bool canConfig() { return false; }

    uint16_t color_time, blank_time;
    uint16_t splits, split_dist, speed;
    uint16_t split, hue_tick;
    uint16_t hue;
};


// Modes are what we can switch between. Currently there are 2 modes, SingleMode
// and DualMode. SingleMode uses one prime and doesn't use the accelerometer.
// DualMode uses two primes and switches between the current variant based on
// the acc sensitivity.
class Mode {
  public:
    Mode(float alpha, bool use_button) : alpha(alpha), use_button(use_button) {}

    virtual void render(uint8_t *r, uint8_t *g, uint8_t *b) {}
    virtual void reset() {}
    virtual void updateAcc(float fxg, float fyg, float fzg);

    virtual void nextPalette() {}
    virtual int8_t incIdx(int8_t v) {}
    virtual void incColor(int8_t v) {}
    virtual void incShade() {}

    virtual void load(uint16_t addr) {}
    virtual void save(uint16_t addr) {}

    virtual int8_t handlePress(bool pressed) {}

    float alpha;
    uint32_t tick;
    uint8_t render_mode;
    bool use_button;
};

class SingleMode : public Mode {
  public:
    SingleMode() : Mode(0, false) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void updateAcc(float fxg, float fyg, float fzg);
    void load(uint16_t addr);
    void save(uint16_t addr);

    void nextPalette();
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    int8_t handlePress(bool pressed) {}

    Prime *prime;
};

class DualMode : public Mode {
  // acc_mode: 0 - tiltX, 1 - tiltY, 2 - tiltZ, 3 - shake
  public:
    DualMode(uint8_t acc_mode, float alpha) : Mode(alpha, false), acc_mode(acc_mode), cur_variant(0) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void updateAcc(float fxg, float fyg, float fzg);
    void load(uint16_t addr);
    void save(uint16_t addr);

    void nextPalette();
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    int8_t handlePress(bool pressed) {}

    uint8_t acc_mode;
    uint8_t cur_variant;
    int16_t acc_counter;
    Prime *prime[2];
    float maxg;
};


class TriTilt : public Mode {
  public:
    TriTilt(uint8_t tilt_axis, float alpha) : Mode(alpha, false), tilt_axis(tilt_axis) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void updateAcc(float fxg, float fyg, float fzg);
    void load(uint16_t addr);
    void save(uint16_t addr);

    void nextPalette();
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    int8_t handlePress(bool pressed) {}

    uint8_t tilt_axis;
    uint8_t cur_variant;
    Prime *prime[3];
};

class TriSpeed : public Mode {
  public:
    TriSpeed(float alpha) : Mode(alpha, false) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void updateAcc(float fxg, float fyg, float fzg);
    void load(uint16_t addr);
    void save(uint16_t addr);

    void nextPalette();
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    int8_t handlePress(bool pressed) {}

    uint8_t cur_variant;
    int16_t acc_counter;
    float maxg;
    Prime *prime[3];
};

class TiltMorph : public Mode {
  public:
    TiltMorph(float alpha) : Mode(alpha, false) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void updateAcc(float fxg, float fyg, float fzg);
    void load(uint16_t addr);
    void save(uint16_t addr);

    void nextPalette();
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    int8_t handlePress(bool pressed) {}

    uint16_t hue, hue_offset;
    uint8_t color_time;
};

#define GM_ONLYX 0
#define GM_ONLYY 1
#define GM_XANDY 2

class GeoMorph : public Mode {
  public:
    GeoMorph(uint8_t op_mode, uint16_t color_time, uint16_t blank_time, float alpha) :
      Mode(alpha, false), op_mode(op_mode), color_time(color_time), blank_time(blank_time) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void updateAcc(float fxg, float fyg, float fzg);
    void load(uint16_t addr);
    void save(uint16_t addr);

    void nextPalette();
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    int8_t handlePress(bool pressed) {}

    uint16_t color_time, blank_time;
    uint8_t op_mode;
    uint8_t palette[5];
    uint8_t edit_color;
    uint8_t geo_r, geo_g, geo_b;
};

#define INOVA_TIMEOUT 2600
#define INOVA_OFF 0
#define INOVA_ON 1
#define INOVA_DOPS 2
#define INOVA_SIGNAL 3
#define INOVA_ON_WAIT 4
#define INOVA_CONFIG 10

class iNova : public Mode {
  public:
    iNova() : Mode(0, true) {}

    void render(uint8_t *r, uint8_t *g, uint8_t *b);
    void reset();
    void updateAcc(float fxg, float fyg, float fzg);
    void load(uint16_t addr);
    void save(uint16_t addr);

    void nextPalette();
    int8_t incIdx(int8_t v);
    void incColor(int8_t v);
    void incShade();

    int8_t handlePress(bool pressed);

    uint8_t op_mode;
    uint8_t color;
    uint32_t counter;
    uint8_t button_state;
    bool go_off;
};

#endif
